#pragma once
#include "hittable_list.h"
#include "camera.h"
#include "color.h"
#include "bvh_node.h"

#define USE_BVH 1

struct scene {
public:
	scene(const hittable_list& h, const camera& c) : scene(h, c, rgb_color(0)) {}
#if USE_BVH == 1
	scene(const hittable_list& h, const camera& c, rgb_color bg) : scene(bvh_node(h), c, bg) {}
	scene(const bvh_node& r, const camera& c) : scene(r, c, rgb_color(0)) {}
	scene(const bvh_node& r, const camera& c, rgb_color bg) : objects(r), cam(c), background(bg) {}
#else
	scene(const hittable_list& h, const camera& c, rgb_color bg) : objects(h), cam(c), background(bg) {}
#endif

public:
#if USE_BVH == 1
	bvh_node objects;
#else
	hittable_list objects;
#endif
	camera cam;
	rgb_color background;
};
