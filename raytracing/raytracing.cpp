// raytracing.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "image.h"
#include "ray.h"

float hit_sphere(const vector3& center, float radius, const ray& r) {
	vector3 oc = r.origin - center;
	float a = r.direction.length_squared();
	float half_b = dot(oc, r.direction);
	float c = oc.length_squared() - radius * radius;
	float discriminant = half_b * half_b - a * c;
	if (discriminant < 0) {
		return -1.0;
	}
	else {
		return (-half_b - sqrt(discriminant)) / a;
	}
}

rgb_color ray_color(const ray& r) {
	float t = hit_sphere(vector3(0, 0, -1), 0.5, r);
	if (t > 0.0) {
		vector3 N = normalize(r.at(t) - vector3(0, 0, -1));
		return 0.5 * rgb_color(N.x + 1, N.y + 1, N.z + 1);
	}
	vector3 unit_direction = normalize(r.direction);
	t = 0.5 * (unit_direction.y + 1.0);
	return (1.0 - t) * rgb_color(1.0, 1.0, 1.0) + t * rgb_color(0.5, 0.7, 1.0);
}

int main()
{
	const float aspect_ratio = 16.0f / 9.0f;
	const unsigned int image_width = 400;
	const unsigned int image_height = (unsigned int)((float)image_width / aspect_ratio);

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
			rgb_color pixel_color = ray_color(r);
			unsigned int index = img.width * row + col;
			img.pixels[index] = pixel_color;

		}
	}
	std::cout << std::endl;
	img.save_to_file("image.ppm");
}