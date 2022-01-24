#pragma once
#include "image.h"
#include "color.h"
#include "ray.h"
#include "camera.h"
#include "hittable.h"
#include "scene.h"

struct render_config {
	unsigned int image_width;
	unsigned int image_height;
	unsigned int samples_per_pixel;
	unsigned int n_threads;
	unsigned int block_width;
	unsigned int block_height;
};

rgb_color ray_color(const ray& r, const scene& sc, unsigned int depth);

void render_area(image& img, const scene& sc, const camera& cam,
	const unsigned int samples_per_pixel, const unsigned int max_depth,
	const unsigned int x_start, const unsigned int x_end,
	const unsigned int y_start, const unsigned int y_end);

image render_image(const scene& sc, const render_config& conf);