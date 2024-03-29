#include <thread>
#include <future>
#include <iomanip>
#include <fstream>
#include "renderer.h"
#include "hittable.h"
#include "utils.h"
#include "materials.h"
#include "scene.h"

rgb_color ray_color(
		const ray& r, const rgb_color& background, const hittable& h, unsigned int depth) {
	// If we've exceeded the ray bounce limit, no more light is gathered.
	if (depth <= 0)
		return rgb_color(0);

	hit_record rec = h.hit(r, 0.000001, infinity);
	if (!rec.did_hit)
		return background;
	
	ray scattered;
	rgb_color attenuation;
	rgb_color emitted = rec.mat_ptr->emitted(rec.hp.u, rec.hp.v, rec.hp.p);
	if (!rec.mat_ptr->scatter(r, rec.hp, attenuation, scattered))
		return emitted;
	return emitted + attenuation * ray_color(scattered, background, h, depth - 1);
}

void render_area(image& img, const scene& sc,
		const render_config& rc, const block_config& bc) {
	for (unsigned int y = bc.y_start; y < bc.y_end; y++) {
		for (unsigned int x = bc.x_start; x < bc.x_end; x++) {
			rgb_color pixel_color(0);
			for (unsigned int s = 0; s < rc.samples_per_pixel; s++) {
				double u = (x + random_double()) / (img.width - 1);
				double v = (y + random_double()) / (img.height - 1);
				ray r = sc.cam.get_ray(u, v);
				pixel_color += ray_color(r, sc.background, sc.objects, rc.max_depth);
			}
			pixel_color /= rc.samples_per_pixel;
			pixel_color.apply_gamma_correction(rc.gamma_correction);

			unsigned int row = img.height - 1 - y;
			unsigned long int index = img.width * row + x;
			img.pixels[index] = pixel_color;
		}
	}
}

image render_image(const scene& sc, const render_config& conf) {
	image img(conf.image_width, conf.image_height);

	std::ofstream statsFile;
	statsFile.open("stats.csv");
	statsFile << "block_id,block_x,block_y,render_time\n";
	std::mutex statsFileWriterMutex;

	timer t("Rendering image");
	std::cout << "Rendering " << img.width << "x" << img.height << " image" << std::endl;

	std::vector<std::future<void>> future_vector;
	volatile std::atomic<unsigned int> block_counter(0);
	const unsigned int n_x_blocks =
		img.width / conf.block_width + (img.width % conf.block_width != 0);
	const unsigned int n_y_blocks =
		img.height / conf.block_height + (img.height % conf.block_height != 0);
	const unsigned int n_blocks = n_x_blocks * n_y_blocks;

	for (unsigned int thread = 0; thread < conf.n_threads; thread++) {
		future_vector.emplace_back(std::async([=, &block_counter, &img, &sc, &conf, &statsFile, &statsFileWriterMutex]() {
			while (true) {
				unsigned int block = block_counter++;
				if (block >= n_blocks)
					break;
				unsigned int y_block = block / n_x_blocks;
				unsigned int x_block = block % n_x_blocks;
				unsigned int x_start = x_block * conf.block_width;
				unsigned int x_end = std::min(x_start + conf.block_width, img.width);
				unsigned int y_start = y_block * conf.block_height;
				unsigned int y_end = std::min(y_start + conf.block_height, img.height);
				block_config bc = {
					.x_start = x_start,
					.x_end = x_end,
					.y_start = y_start,
					.y_end = y_end};

				std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
				render_area(img, sc, conf, bc);
				std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
				std::chrono::microseconds duration =
					std::chrono::duration_cast<std::chrono::microseconds>(end - start);
				{
					std::lock_guard<std::mutex> lock(statsFileWriterMutex);
					statsFile << block << ',' << x_block << ',' << y_block << ',' << duration.count() << '\n';
				}
			}
		}));
	}

	std::future<void> progress_future = std::async([=, &block_counter]() {
		while (true) {
			unsigned int block = block_counter;
			unsigned int block_lim = std::min(block, n_blocks);
			short int percentage = (short int)((double)block_lim / n_blocks * 100);
			unsigned int block_digits = (unsigned int)log10((double)n_blocks) + 1;
			std::cout << '\r' << std::setw(3) << percentage << "% (block " <<
				std::setw(block_digits) << block_lim << '/' << n_blocks << ')';

			if (block >= n_blocks + conf.n_threads) {
				std::cout << std::endl;
				break;
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}
	});

	for (unsigned int thread = 0; thread < conf.n_threads; thread++) future_vector[thread].wait();
	progress_future.wait();
	statsFile.close();
	return img;
}
