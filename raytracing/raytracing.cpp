#include <iostream>
#include "image.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "utils.h"
#include "camera.h"

rgb_color ray_color(const ray& r, const hittable& world) {
	hit_record rec;
	if (world.hit(r, 0, infinity, rec)) {
		return 0.5 * (rgb_color(rec.normal.x, rec.normal.y, rec.normal.z) + rgb_color(1, 1, 1));
	}
	vector3 unit_direction = normalize(r.direction);
	double t = 0.5 * (unit_direction.y + 1.0);
	return (1.0 - t) * rgb_color(1.0, 1.0, 1.0) + t * rgb_color(0.5, 0.7, 1.0);
}

int main()
{
	// Image
	const double aspect_ratio = 16.0 / 9.0;
	const unsigned int image_width = 100;
	const unsigned int image_height = (unsigned int)((double)image_width / aspect_ratio);
	const unsigned int samples_per_pixel = 50;

	// World
	hittable_list world;
	world.add(std::make_shared<sphere>(vector3(0, 0, -1), 0.5));
	world.add(std::make_shared<sphere>(vector3(0, -100.5, -1), 100));

	// Camera
	camera cam;

	std::cout << "Creating image (" << image_width << "x" << image_height << ")" << std::endl;
	image img(image_width, image_height);

	for (unsigned long int row = 0; row < img.height; row++) {
		std::cout << "\rGenerating image: " << (int)((double)row / (img.height - 1) * 100) << "%";
		for (unsigned long int col = 0; col < img.width; col++) {
			rgb_color pixel_color = rgb_color(0, 0, 0);
			for (unsigned int s = 0; s < samples_per_pixel; ++s) {
				double u = (col + random_float()) / (image_width - 1);
				double v = (image_height - 1 - row + random_float()) / (image_height - 1);
				ray r = cam.get_ray(u, v);
				pixel_color += ray_color(r, world);
			}

			unsigned long int index = img.width * row + col;
			img.pixels[index] = pixel_color / samples_per_pixel;

		}
	}
	std::cout << std::endl;
	img.save_to_file("image.ppm");
}