#include <iostream>
#include <iomanip>
#include <thread>
#include <future>
#include "utils.h"
#include "image.h"
#include "color.h"
#include "camera.h"
#include "scene.h"
#include "sphere.h"
#include "triangle.h"
#include "materials.h"
#include "scenes.h"
#include "renderer.h"

int main() {
	scene sc = random_scene_triangles();
	
	double aspect_ratio = 3.0 / 2.0;
	unsigned int image_width = 1200;
	unsigned int image_height = (unsigned int)((double)image_width / aspect_ratio);
	
	// Camera
	vector3 lookfrom(13, 2, 3);
	vector3 lookat(0, 0, 0);
	vector3 vup(0, 1, 0);
	double dist_to_focus = 10.0;
	double aperture = 0.01;
	
	render_config conf = {
		.image_width = image_width,
		.image_height = image_height,
		.block_width = 32,
		.block_height = 32,
		.samples_per_pixel = 5,
		.n_threads = std::thread::hardware_concurrency(),
		.max_depth = 50,
		.cam = camera(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus)};

	image img = render_image(sc, conf);

	img.save_to_file("image.ppm");
}
