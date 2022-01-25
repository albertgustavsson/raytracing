#include "scenes.h"
#include "scene.h"
#include "sphere.h"
#include "triangle.h"
#include "materials.h"
#include "vector3.h"
#include "utils.h"

scene random_scene_balls_only() {
	scene sc;

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
				sc.add(std::make_shared<sphere>(center, 0.2, sphere_material));
			}
		}
	}

	std::shared_ptr<material> material1 = std::make_shared<dielectric>(1.5);
	sc.add(std::make_shared<sphere>(vector3(0, 1, 0), 1, material1));

	std::shared_ptr<material> material2 = std::make_shared<lambertian>(rgb_color(0.4, 0.2, 0.1));
	sc.add(std::make_shared<sphere>(vector3(-4, 1, 0), 1, material2));

	std::shared_ptr<material> material3 = std::make_shared<metal>(rgb_color(0.7, 0.6, 0.5), 0);
	sc.add(std::make_shared<sphere>(vector3(4, 1, 0), 1, material3));

	return sc;
}

scene random_scene() {
	scene sc = random_scene_balls_only();

	std::shared_ptr<lambertian> ground_material = std::make_shared<lambertian>(rgb_color(0.5));
	sc.add(std::make_shared<sphere>(vector3(0, -1000, 0), 1000, ground_material));

	return sc;
}

scene random_scene_triangles_only() {
	scene sc;

	std::shared_ptr<lambertian> ground_material = std::make_shared<lambertian>(rgb_color(0.5));
	const int half_side = 15;
	vector3 corners[4] = {
		{ -half_side, 0, -half_side },
		{ -half_side, 0,  half_side },
		{  half_side, 0, -half_side },
		{  half_side, 0,  half_side } };
	sc.add(std::make_shared<triangle>(corners[0], corners[1], corners[2], ground_material));
	sc.add(std::make_shared<triangle>(corners[1], corners[3], corners[2], ground_material));

	return sc;
}

scene random_scene_triangles() {
	scene sc = random_scene_triangles_only();
	sc.add(std::make_shared<scene>(random_scene_balls_only()));
	return sc;
}

scene random_scene_checker() {
	scene sc = random_scene_balls_only();

	std::shared_ptr<checker_texture> checker = std::make_shared<checker_texture>(rgb_color(0.2, 0.3, 0.1), rgb_color(0.9));
	sc.add(std::make_shared<sphere>(vector3(0, -1000, 0), 1000, std::make_shared<lambertian>(checker)));

	return sc;
}

scene random_scene_light() {
	scene sc = random_scene_triangles();

	std::shared_ptr<diffuse_light> light_material = std::make_shared<diffuse_light>(rgb_color(5));
	sc.add(std::make_shared<sphere>(vector3(20, 15, 25), 15, light_material));

	return sc;
}

scene cornell_box() {
	scene sc;

	std::shared_ptr<lambertian> red =   std::make_shared<lambertian>(rgb_color(0.65, 0.05, 0.05));
	std::shared_ptr<lambertian> white = std::make_shared<lambertian>(rgb_color(0.73, 0.73, 0.73));
	std::shared_ptr<lambertian> green = std::make_shared<lambertian>(rgb_color(0.12, 0.45, 0.15));
	std::shared_ptr<diffuse_light> light = std::make_shared<diffuse_light>(rgb_color(15));

	const double box_size = 555;
	vector3 box_corners[8] = {
		{0,        0,        0},
		{box_size, 0,        0},
		{0,        box_size, 0},
		{box_size, box_size, 0},
		{0,        0,        box_size},
		{box_size, 0,        box_size},
		{0,        box_size, box_size},
		{box_size, box_size, box_size},
	};

	vector3 light_corners[4] = {
		{213, box_size - 1, 227},
		{343, box_size - 1, 227},
		{213, box_size - 1, 332},
		{343, box_size - 1, 332},
	};

	// Back wall
	sc.add(std::make_shared<triangle>(box_corners[4], box_corners[5], box_corners[6], white));
	sc.add(std::make_shared<triangle>(box_corners[5], box_corners[6], box_corners[7], white));
	
	// Floor
	sc.add(std::make_shared<triangle>(box_corners[0], box_corners[1], box_corners[4], white));
	sc.add(std::make_shared<triangle>(box_corners[1], box_corners[4], box_corners[5], white));
	
	// Ceiling
	sc.add(std::make_shared<triangle>(box_corners[2], box_corners[3], box_corners[6], white));
	sc.add(std::make_shared<triangle>(box_corners[3], box_corners[6], box_corners[7], white));
	
	// Left wall
	sc.add(std::make_shared<triangle>(box_corners[1], box_corners[3], box_corners[5], green));
	sc.add(std::make_shared<triangle>(box_corners[3], box_corners[5], box_corners[7], green));

	// Right wall
	sc.add(std::make_shared<triangle>(box_corners[0], box_corners[2], box_corners[4], red));
	sc.add(std::make_shared<triangle>(box_corners[2], box_corners[4], box_corners[6], red));
	
	// Light
	sc.add(std::make_shared<triangle>(light_corners[0], light_corners[1], light_corners[2], light));
	sc.add(std::make_shared<triangle>(light_corners[1], light_corners[2], light_corners[3], light));

	return sc;
}
