#include <iostream>
#include "image.h"
#include "color.h"
#include "ray.h"
#include "hittable_list.h"
#include "sphere.h"

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

rgb_color ray_color(const ray& r, const hittable& world) {
	hit_record rec;
	if (world.hit(r, 0, infinity, rec)) {
		return 0.5 * (rgb_color(rec.normal.x, rec.normal.y, rec.normal.z) + rgb_color(1, 1, 1));
	}
	vector3 unit_direction = normalize(r.direction);
	float t = 0.5 * (unit_direction.y + 1.0);
	return (1.0 - t) * rgb_color(1.0, 1.0, 1.0) + t * rgb_color(0.5, 0.7, 1.0);
}

int main()
{
	// Image
	const float aspect_ratio = 16.0f / 9.0f;
	const unsigned int image_width = 400;
	const unsigned int image_height = (unsigned int)((float)image_width / aspect_ratio);

	// World
	hittable_list world;
	world.add(std::make_shared<sphere>(vector3(0, 0, -1), 0.5));
	world.add(std::make_shared<sphere>(vector3(0, -100.5, -1), 100));

	// Camera
	float viewport_height = 2.0;
	float viewport_width = aspect_ratio * viewport_height;
	float focal_length = 1.0;

	vector3 origin = vector3(0, 0, 0);
	vector3 horizontal = vector3(viewport_width, 0, 0);
	vector3 vertical = vector3(0, viewport_height, 0);
	vector3 lower_left_corner = origin - horizontal / 2 - vertical / 2 - vector3(0, 0, focal_length);

	std::cout << "Creating image (" << image_width << "x" << image_height << ")" << std::endl;
	image img(image_width, image_height);

	for (unsigned int row = 0; row < img.height; row++) {
		std::cout << "\rGenerating image: " << (int)((float)row / (img.height - 1) * 100) << "%";
		for (unsigned int col = 0; col < img.width; col++) {
			float u = (float)col / (image_width - 1);
			float v = (float)(image_height - 1 - row) / (image_height - 1);
			ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
			rgb_color pixel_color = ray_color(r, world);
			unsigned int index = img.width * row + col;
			img.pixels[index] = pixel_color;

		}
	}
	std::cout << std::endl;
	img.save_to_file("image.ppm");
}