#include <thread>
#include <future>
#include <iomanip>
#include "renderer.h"
#include "utils.h"
#include "materials.h"

rgb_color ray_color(const ray& r, const hittable& scene, unsigned int depth) {
	// If we've exceeded the ray bounce limit, no more light is gathered.
	const rgb_color black(0.0, 0.0, 0.0);
	const rgb_color background1(1.0, 1.0, 1.0);
	const rgb_color background2(0.5, 0.7, 1.0);

	if (depth <= 0)
		return black;

	hit_record rec;
	if (scene.hit(r, 0.000001, infinity, rec)) {
		ray scattered;
		rgb_color attenuation;
		if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
			return attenuation * ray_color(scattered, scene, depth - 1);
		return black;
	}
	vector3 unit_direction = r.direction.get_normalized();
	double t = 0.5 * (unit_direction.y + 1.0);
	return (1.0 - t) * background1 + t * background2;
}

void render_area(image& img, const hittable_list& scene, const camera& cam,
	const unsigned int samples_per_pixel, const unsigned int max_depth,
	const unsigned int x_start, const unsigned int x_end,
	const unsigned int y_start, const unsigned int y_end) {

	for (unsigned int y = y_start; y < y_end; y++) {
		for (unsigned int x = x_start; x < x_end; x++) {
			rgb_color pixel_color = rgb_color(0, 0, 0);
			for (unsigned int s = 0; s < samples_per_pixel; s++) {
				double u = (x + random_double()) / (img.width - 1);
				double v = (y + random_double()) / (img.height - 1);
				ray r = cam.get_ray(u, v);
				pixel_color += ray_color(r, scene, max_depth);
			}
			pixel_color /= samples_per_pixel;
			pixel_color.apply_gamma_correction(1.0 / 2.2);

			unsigned int row = img.height - 1 - y;
			unsigned long int index = img.width * row + x;
			img.pixels[index] = pixel_color;
		}
	}
}

image render_image(const hittable_list& scene, const render_config& conf) {
	// Image
	image img(conf.image_width, conf.image_height);

	// Camera
	vector3 lookfrom(13, 2, 3);
	vector3 lookat(0, 0, 0);
	vector3 vup(0, 1, 0);
	double dist_to_focus = 10.0;
	double aperture = 0.01;
	const double aspect_ratio = (double)conf.image_width / conf.image_height;

	camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

	const unsigned int max_depth = 50;

	timer t("Rendering image");
	std::cout << "Rendering " << img.width << "x" << img.height << " image" << std::endl;

	std::vector<std::future<void>> future_vector;
	volatile std::atomic<unsigned int> block_counter(0);
	const unsigned int n_x_blocks = img.width / conf.block_width + (img.width % conf.block_width != 0);
	const unsigned int n_y_blocks = img.height / conf.block_height + (img.height % conf.block_height != 0);
	const unsigned int n_blocks = n_x_blocks * n_y_blocks;

	for (unsigned int thread = 0; thread < conf.n_threads; thread++) {
		future_vector.emplace_back(std::async([=, &block_counter, &img, &scene, &cam]() {
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

				render_area(img, scene, cam, conf.samples_per_pixel, max_depth, x_start, x_end, y_start, y_end);
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

	return img;
}