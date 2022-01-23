#include "scenes.h"
#include "hittable_list.h"
#include "sphere.h"
#include "triangle.h"
#include "materials.h"
#include "vector3.h"
#include "utils.h"

hittable_list random_scene() {
	hittable_list scene;

	//std::shared_ptr<lambertian> ground_material = std::make_shared<lambertian>(rgb_color(0.5, 0.5, 0.5));
	//const int half_side = 15;
	//vector3 corner0(-half_side, 0, -half_side);
	//vector3 corner1(-half_side, 0, half_side);
	//vector3 corner2(half_side, 0, -half_side);
	//vector3 corner3(half_side, 0, half_side);
	//scene.add(std::make_shared<triangle>(corner0, corner1, corner2, ground_material));
	//scene.add(std::make_shared<triangle>(corner1, corner3, corner2, ground_material));

	std::shared_ptr<checker_texture> checker = std::make_shared<checker_texture>(rgb_color(0.2, 0.3, 0.1), rgb_color(0.9, 0.9, 0.9));
	scene.add(std::make_shared<sphere>(vector3(0, -1000, 0), 1000, std::make_shared<lambertian>(checker)));

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