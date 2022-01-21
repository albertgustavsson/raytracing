#include <iostream>
#include <thread>
#include <future>
#include "image.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "utils.h"
#include "camera.h"

rgb_color ray_color(const ray& r, const hittable& world, unsigned int depth) {
	// If we've exceeded the ray bounce limit, no more light is gathered.
	const rgb_color black(0.0, 0.0, 0.0);
	const rgb_color background1(1.0, 1.0, 1.0);
	const rgb_color background2(0.5, 0.7, 1.0);

	if (depth <= 0)
		return black;

	hit_record rec;
	if (world.hit(r, 0.000001, infinity, rec)) {
		ray scattered;
		rgb_color attenuation;
		if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
			return attenuation * ray_color(scattered, world, depth - 1);
		return black;
	}
	vector3 unit_direction = r.direction.get_normalized();
	double t = 0.5 * (unit_direction.y + 1.0);
	return (1.0 - t) * background1 + t * background2;
}

hittable_list random_scene() {
	hittable_list scene;

	auto ground_material = std::make_shared<lambertian>(rgb_color(0.5, 0.5, 0.5));
	scene.add(std::make_shared<sphere>(vector3(0, -1000, 0), 1000, ground_material));

	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			double choose_mat = random_double();
			vector3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

			if ((center - vector3(4, 0.2, 0)).length() > 0.9) {
				std::shared_ptr<material> sphere_material;

				if (choose_mat < 0.8) {
					// diffuse
					rgb_color albedo = rgb_color::random() * rgb_color::random();
					sphere_material = std::make_shared<lambertian>(albedo);
				}
				else if (choose_mat < 0.95) {
					// metal
					rgb_color albedo = rgb_color::random(0.5, 1);
					double fuzz = random_double(0, 0.5);
					sphere_material = std::make_shared<metal>(albedo, fuzz);
				}
				else {
					// glass
					sphere_material = std::make_shared<dielectric>(1.5);
				}
				scene.add(std::make_shared<sphere>(center, 0.2, sphere_material));
			}
		}
	}

	auto material1 = std::make_shared<dielectric>(1.5);
	scene.add(std::make_shared<sphere>(vector3(0, 1, 0), 1.0, material1));

	auto material2 = std::make_shared<lambertian>(rgb_color(0.4, 0.2, 0.1));
	scene.add(std::make_shared<sphere>(vector3(-4, 1, 0), 1.0, material2));

	auto material3 = std::make_shared<metal>(rgb_color(0.7, 0.6, 0.5), 0.0);
	scene.add(std::make_shared<sphere>(vector3(4, 1, 0), 1.0, material3));

	return scene;
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
			std::cout << '\r' << (short int)((double)block_lim / n_blocks * 100) << '%'
				<< "  (block " << block_lim << "/" << n_blocks << ')';

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

int main() {
	hittable_list scene = random_scene();

	double aspect_ratio = 3.0 / 2.0;
	unsigned int image_width = 1200;
	unsigned int image_height = (unsigned int)((double)image_width / aspect_ratio);
	unsigned int samples_per_pixel = 100;
	unsigned int n_threads = std::thread::hardware_concurrency();
	unsigned int block_width = 32;
	unsigned int block_height = 32;
	render_config conf = {
		.image_width = image_width,
		.image_height = image_height,
		.samples_per_pixel = samples_per_pixel,
		.n_threads = n_threads,
		.block_width = block_width,
		.block_height = block_height};

	image img = render_image(scene, conf);

	img.save_to_file("image.ppm");
}