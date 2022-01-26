#pragma once
#include "hittable_list.h"
#include "camera.h"
#include "color.h"
#include "bvh_node.h"

struct scene {
public:
	scene(const hittable_list& h, const camera& c) : scene(h, c, rgb_color(0)) {}
	scene(const hittable_list& h, const camera& c, rgb_color bg) : scene(bvh_node(h), c, bg) {}
	scene(const bvh_node& r, const camera& c, rgb_color bg) : bvh_root(r), cam(c), background(bg) {}

public:
	bvh_node bvh_root;
	camera cam;
	rgb_color background;
};
