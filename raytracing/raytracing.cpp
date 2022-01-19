#include <iostream>
#include "image.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "utils.h"
#include "camera.h"

rgb_color ray_color(const ray& r, const hittable& world, unsigned int depth) {
	// If we've exceeded the ray bounce limit, no more light is gathered.
	const rgb_color black(0.0, 0.0, 0.0);
	const rgb_color white(1.0, 1.0, 1.0);
	const rgb_color background(0.5, 0.7, 1.0);

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
	return (1.0 - t) * white + t * background;
}

hittable_list random_scene() {
	hittable_list world;

	auto ground_material = std::make_shared<lambertian>(rgb_color(0.5, 0.5, 0.5));
	world.add(std::make_shared<sphere>(vector3(0, -1000, 0), 1000, ground_material));

	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			auto choose_mat = random_double();
			vector3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

			if ((center - vector3(4, 0.2, 0)).length() > 0.9) {
				std::shared_ptr<material> sphere_material;

				if (choose_mat < 0.8) {
					// diffuse
					auto albedo = rgb_color::random() * rgb_color::random();
					sphere_material = std::make_shared<lambertian>(albedo);
					world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
				}
				else if (choose_mat < 0.95) {
					// metal
					auto albedo = rgb_color::random(0.5, 1);
					auto fuzz = random_double(0, 0.5);
					sphere_material = std::make_shared<metal>(albedo, fuzz);
					world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
				}
				else {
					// glass
					sphere_material = std::make_shared<dielectric>(1.5);
					world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
				}
			}
		}
	}

	auto material1 = std::make_shared<dielectric>(1.5);
	world.add(std::make_shared<sphere>(vector3(0, 1, 0), 1.0, material1));

	auto material2 = std::make_shared<lambertian>(rgb_color(0.4, 0.2, 0.1));
	world.add(std::make_shared<sphere>(vector3(-4, 1, 0), 1.0, material2));

	auto material3 = std::make_shared<metal>(rgb_color(0.7, 0.6, 0.5), 0.0);
	world.add(std::make_shared<sphere>(vector3(4, 1, 0), 1.0, material3));

	return world;
}

int main() {
	// Image
	const double aspect_ratio = 3.0 / 2.0;
	const unsigned int image_width = 1200;
	const unsigned int image_height = (unsigned int)((double)image_width / aspect_ratio);
	const unsigned int samples_per_pixel = 500;
	const unsigned int max_depth = 50;

	// World
	hittable_list world = random_scene();

	// Camera
	vector3 lookfrom(13, 2, 3);
	vector3 lookat(0, 0, 0);
	vector3 vup(0, 1, 0);
	auto dist_to_focus = 10.0;//(lookfrom - lookat).length();
	auto aperture = 0.1;
	
	camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

	std::cout << "Creating image (" << image_width << "x" << image_height << ")" << std::endl;
	image img(image_width, image_height);

	for (long int j = 0; j < img.height; j++) {
		std::cout << "\rGenerating image: " << (int)((double)(j) / (img.height - 1) * 100) << "%";
		for (unsigned long int i = 0; i < img.width; i++) {
			rgb_color pixel_color = rgb_color(0, 0, 0);
			for (unsigned int s = 0; s < samples_per_pixel; ++s) {
				double u = (i + random_double()) / (img.width - 1);
				double v = (j + random_double()) / (img.height - 1);
				ray r = cam.get_ray(u, v);
				pixel_color += ray_color(r, world, max_depth);
			}
			pixel_color /= samples_per_pixel;
			pixel_color.apply_gamma_correction(1.0/2.2);

			unsigned long int row = img.height - 1 - j;
			unsigned long int index = img.width * row + i;
			img.pixels[index] = pixel_color;
		}
	}
	std::cout << std::endl;
	img.save_to_file("image.ppm");
}