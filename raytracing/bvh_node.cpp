#include <iostream>
#include "bvh_node.h"
#include "utils.h"

bvh_node::bvh_node(
		const std::vector<std::shared_ptr<hittable>>& src_objects, size_t start, size_t end) {
	// Create a modifiable array of the source scene objects
	std::vector<std::shared_ptr<hittable>> objects = src_objects;

	int axis = random_int(0, 2);
	
	auto comparator = (axis == 0) ? box_x_compare
		: (axis == 1) ? box_y_compare
		: box_z_compare;

	size_t object_span = end - start;

	if (object_span == 1) {
		left = right = objects[start];
	}
	else if (object_span == 2) {
		if (comparator(objects[start], objects[start + 1])) {
			left = objects[start];
			right = objects[start + 1];
		}
		else {
			left = objects[start + 1];
			right = objects[start];
		}
	}
	else {
		std::sort(objects.begin() + start, objects.begin() + end, comparator);

		size_t mid = start + object_span / 2;
		left = std::make_shared<bvh_node>(objects, start, mid);
		right = std::make_shared<bvh_node>(objects, mid, end);
	}

	aabb box_left, box_right;

	if (!left->bounding_box(box_left) || !right->bounding_box(box_right))
		std::cerr << "No bounding box in bvh_node constructor.\n";

	box = surrounding_box(box_left, box_right);
}


hit_record bvh_node::hit(const ray& r, double t_min, double t_max) const {
	hit_record hr;
	if (!box.hit(r, t_min, t_max))
		return hr;

	hit_record hr_left = left->hit(r, t_min, t_max);
	hit_record hr_right = right->hit(r, t_min, hr_left.did_hit ? hr_left.hp.t : t_max);
	return hr_right.did_hit ? hr_right : hr_left;
}

bool bvh_node::bounding_box(aabb& output_box) const {
	output_box = box;
	return true;
}

inline bool box_compare(
		const std::shared_ptr<hittable>& a, const std::shared_ptr<hittable>& b, int axis) {
	aabb box_a;
	aabb box_b;

	if (!a->bounding_box(box_a) || !b->bounding_box(box_b))
		std::cerr << "No bounding box in bvh_node constructor.\n";

	return box_a.minimum[axis] < box_b.minimum[axis];
}

bool box_x_compare(const std::shared_ptr<hittable>& a, const std::shared_ptr<hittable>& b) {
	return box_compare(a, b, 0);
}

bool box_y_compare(const std::shared_ptr<hittable>& a, const std::shared_ptr<hittable>& b) {
	return box_compare(a, b, 1);
}

bool box_z_compare(const std::shared_ptr<hittable>& a, const std::shared_ptr<hittable>& b) {
	return box_compare(a, b, 2);
}
