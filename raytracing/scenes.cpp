#include "scenes.h"
#include "hittable_list.h"
#include "sphere.h"
#include "triangle.h"
#include "materials.h"
#include "vector3.h"
#include "utils.h"
#include "camera.h"
#include "scene.h"
#include "quadrilateral.h"
#include "parallelepiped.h"

camera random_scene_camera() {
	vector3 lookfrom(13, 2, 3);
	vector3 lookat(0, 0, 0);
	vector3 vup(0, 1, 0);
	double vfov = 20;
	double aspect_ratio = 3.0 / 2.0;
	double aperture = 0.01;
	double dist_to_focus = 128.0 / sqrt(182.0); // Distance to edge of closest big sphere
	return camera(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus);
}

hittable_list random_spheres() {
	hittable_list spheres;

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
				spheres.add(std::make_shared<sphere>(center, 0.2, sphere_material));
			}
		}
	}

	std::shared_ptr<material> material1 = std::make_shared<dielectric>(1.5);
	spheres.add(std::make_shared<sphere>(vector3(0, 1, 0), 1, material1));

	std::shared_ptr<material> material2 = std::make_shared<lambertian>(rgb_color(0.4, 0.2, 0.1));
	spheres.add(std::make_shared<sphere>(vector3(-4, 1, 0), 1, material2));

	std::shared_ptr<material> material3 = std::make_shared<metal>(rgb_color(0.7, 0.6, 0.5), 0);
	spheres.add(std::make_shared<sphere>(vector3(4, 1, 0), 1, material3));

	return spheres;
}

scene random_scene() {
	hittable_list hittables = random_spheres();

	std::shared_ptr<lambertian> ground_material = std::make_shared<lambertian>(rgb_color(0.5));
	hittables.add(std::make_shared<sphere>(vector3(0, -1000, 0), 1000, ground_material));

	return scene(hittables, random_scene_camera(), rgb_color(0.75, 0.85, 1.00));
}

hittable_list floor_triangles() {
	hittable_list triangles;

	std::shared_ptr<lambertian> ground_material = std::make_shared<lambertian>(rgb_color(0.5));
	const int half_side = 15;
	vector3 corners[4] = {
		{ -half_side, 0, -half_side },
		{ -half_side, 0,  half_side },
		{  half_side, 0, -half_side },
		{  half_side, 0,  half_side } };
	triangles.add(std::make_shared<triangle>(corners[0], corners[1], corners[2], ground_material));
	triangles.add(std::make_shared<triangle>(corners[1], corners[3], corners[2], ground_material));

	return triangles;
}

scene random_scene_triangles() {
	hittable_list hittables = random_spheres();

	hittables.add(floor_triangles().objects);
	
	return scene(hittables, random_scene_camera(), rgb_color(0.75, 0.85, 1.00));
}

scene random_scene_checker() {
	hittable_list hittables = random_spheres();

	std::shared_ptr<checker_texture> checker = std::make_shared<checker_texture>(rgb_color(0.2, 0.3, 0.1), rgb_color(0.9));
	hittables.add(std::make_shared<sphere>(vector3(0, -1000, 0), 1000, std::make_shared<lambertian>(checker)));

	return scene(hittables, random_scene_camera(), rgb_color(0.75, 0.85, 1.00));
}

scene random_scene_light() {
	hittable_list hittables = random_spheres();
	hittables.add(floor_triangles().objects);

	std::shared_ptr<diffuse_light> light_material = std::make_shared<diffuse_light>(rgb_color(5), rgb_color(0));
	hittables.add(std::make_shared<sphere>(vector3(20, 15, 25), 15, light_material));

	return scene(hittables, random_scene_camera());
}

hittable_list cornell_box() {
	// Wavelengths: R=650nm, G=510nm, B=475nm	
	rgb_color red_color(0.611, 0.0555, 0.06175);
	rgb_color green_color(0.117, 0.4125, 0.11425);
	rgb_color white_color(0.7295, 0.7355, 0.733);
	rgb_color light_emissive_color = rgb_color(0, 255.0/255, 146.0/255) * 8.0 + rgb_color(255.0/255, 190.0/255, 0) * 15.6 + rgb_color(255.0/255, 0, 0) * 18.4;
	rgb_color light_diffuse_color(0.78);

	std::shared_ptr<lambertian> red = std::make_shared<lambertian>(red_color);
	std::shared_ptr<lambertian> green = std::make_shared<lambertian>(green_color);
	std::shared_ptr<lambertian> white = std::make_shared<lambertian>(white_color);
	std::shared_ptr<diffuse_light> light = std::make_shared<diffuse_light>(light_emissive_color, light_diffuse_color);

	vector3 box_corners[8] = {
		{  0,     0,     0  },
		{552.8,   0,     0  },
		{  0,   548.8,   0  },
		{556.0, 548.8,   0  },
		{  0,     0,   559.2},
		{549.6,   0,   559.2},
		{  0,   548.8, 559.2},
		{556.0, 548.8, 559.2},
	};

	vector3 light_corners[4] = {
		{213, 548.8 - 0.1, 227},
		{343, 548.8 - 0.1, 227},
		{213, 548.8 - 0.1, 332},
		{343, 548.8 - 0.1, 332},
	};

	hittable_list hittables;
	
	// Back wall
	hittables.add(std::make_shared<quadrilateral>(box_corners[4], box_corners[5], box_corners[7], box_corners[6], white));
	// Floor
	hittables.add(std::make_shared<quadrilateral>(box_corners[0], box_corners[1], box_corners[5], box_corners[4], white));
	// Ceiling
	hittables.add(std::make_shared<quadrilateral>(box_corners[2], box_corners[3], box_corners[7], box_corners[6], white));
	// Left wall
	hittables.add(std::make_shared<quadrilateral>(box_corners[1], box_corners[3], box_corners[7], box_corners[5], red));
	// Right wall
	hittables.add(std::make_shared<quadrilateral>(box_corners[0], box_corners[2], box_corners[6], box_corners[4], green));
	// Light
	hittables.add(std::make_shared<quadrilateral>(light_corners[0], light_corners[1], light_corners[3], light_corners[2], light));

	return hittables;
}

hittable_list cornell_blocks() {
	rgb_color white_color(0.7295, 0.7355, 0.733);
	std::shared_ptr<lambertian> white = std::make_shared<lambertian>(white_color);

	hittable_list hittables;

	// Corners for the short block
	//	{130,   0,  65}
	//	{290,   0, 114}
	//	{130, 165,  65}
	//	{290, 165, 114}
	//	{ 82,   0, 225}
	//	{240,   0, 272}
	//	{ 82, 165, 225}
	//	{240, 165, 272}

	// Corners for the tall block
	//	{265,   0, 296}
	//	{423,   0, 247}
	//	{265, 330, 296}
	//	{423, 330, 247}
	//	{314,   0, 456}
	//	{472,   0, 406}
	//	{314, 330, 456}
	//	{472, 330, 406}

	// Doesn't match the blocks defined by Cornell perfectly, since they do not seem to have parallel sides
	hittables.add(std::make_shared<parallelepiped>(vector3(130, 0, 65), vector3(160, 0, 49), vector3(0, 165, 0), vector3(-48, 0, 160), white));
	hittables.add(std::make_shared<parallelepiped>(vector3(265, 0, 296), vector3(158, 0, -49), vector3(0, 330, 0), vector3(49, 0, 160), white));
	
	return hittables;
}

scene cornell_box_scene() {
	// Made to match the Cornell box, as described here:
	// https://www.graphics.cornell.edu/online/box/data.html
	hittable_list hittables = cornell_box();
	hittables.add(cornell_blocks().objects);

	vector3 lookfrom(278, 273, -800);
	vector3 lookat(278, 273, 0);
	vector3 vup(0, 1, 0);
	double vfov = 39.3;
	double aspect_ratio = 1.0;
	double aperture = 0.01;
	double dist_to_focus = (lookat - lookfrom).length();
	camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus);
	
	return scene(hittables, cam);
}
