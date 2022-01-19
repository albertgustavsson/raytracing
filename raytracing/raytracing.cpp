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

int main() {
	// Image
	const double aspect_ratio = 16.0 / 9.0;
	const unsigned int image_width = 500;
	const unsigned int image_height = (unsigned int)((double)image_width / aspect_ratio);
	const unsigned int samples_per_pixel = 200;
	const unsigned int max_depth = 10;

	// World
	hittable_list world;

	auto material_ground = std::make_shared<lambertian>(rgb_color(0.8, 0.8, 0.0));
	auto material_center = std::make_shared<lambertian>(rgb_color(0.1, 0.2, 0.5));
	auto material_left   = std::make_shared<dielectric>(1.5);
	auto material_right  = std::make_shared<metal>(rgb_color(0.8, 0.6, 0.2), 0.0);

	world.add(std::make_shared<sphere>(vector3( 0.0, -100.5, -1.0), 100.0 , material_ground));
	world.add(std::make_shared<sphere>(vector3( 0.0,    0.0, -1.0),   0.5 , material_center));
	world.add(std::make_shared<sphere>(vector3(-1.0,    0.0, -1.0),   0.5 , material_left));
	world.add(std::make_shared<sphere>(vector3(-1.0,    0.0, -1.0),  -0.45, material_left));
	world.add(std::make_shared<sphere>(vector3( 1.0,    0.0, -1.0),   0.5 , material_right));

	// Camera
	//camera cam(vector3(0,0,0), vector3(0, 0, -1), vector3(0, 1, 0), 90.0, aspect_ratio);
	vector3 lookfrom(3, 3, 2);
	vector3 lookat(0, 0, -1);
	vector3 vup(0, 1, 0);
	auto dist_to_focus = (lookfrom - lookat).length();
	auto aperture = 2.0;
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