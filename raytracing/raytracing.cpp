#include <thread>
#include "image.h"
#include "scene.h"
#include "renderer.h"
#include "scenes.h"

int main() {
	scene sc = random_scene();

	unsigned int image_width = 1200;
	unsigned int image_height = (unsigned int)((double)image_width / sc.cam.ar);
	
	render_config conf = {
		.image_width = image_width,
		.image_height = image_height,
		.block_width = 32,
		.block_height = 32,
		.samples_per_pixel = 100,
		.n_threads = std::thread::hardware_concurrency(),
		.max_depth = 50 };

	image img = render_image(sc, conf);

	img.save_to_file("image.ppm");
}
