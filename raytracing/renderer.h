#pragma once
#include "image.h"
#include "color.h"
#include "ray.h"
#include "camera.h"
#include "hittable.h"
#include "scene.h"

struct render_config {
	unsigned int image_width, image_height;
	unsigned int block_width, block_height;
	unsigned int samples_per_pixel;
	unsigned int n_threads;
	unsigned int max_depth;
};

struct block_config {
	unsigned int x_start, x_end, y_start, y_end;
};

rgb_color ray_color(const ray& r, const rgb_color& background, const hittable& h, unsigned int depth);

void render_area(image& img, const scene& sc,
	const render_config& rc, const block_config& bc);

image render_image(const scene& sc, const render_config& conf);