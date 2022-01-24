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
	unsigned int samples_per_pixel = 500;
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

	image img = render_image(sc, conf);

	img.save_to_file("image.ppm");
}
