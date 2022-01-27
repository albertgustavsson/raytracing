#include "hittable_list.h"

void hittable_list::add(std::vector<std::shared_ptr<hittable>> obj) {
	objects.insert(objects.end(), obj.begin(), obj.end());
}

hit_record hittable_list::hit(const ray& r, double t_min, double t_max) const {
	hit_record rec;
	double closest_so_far = t_max;

	for (const std::shared_ptr<hittable>& object : objects) {
		hit_record hr = object->hit(r, t_min, closest_so_far);
		if (hr.did_hit) {
			closest_so_far = hr.hp.t;
			rec = hr;
		}
	}

	return rec;
}

bool hittable_list::bounding_box(aabb& output_box) const {
	if (objects.empty()) return false;

	aabb temp_box;
	bool first_box = true;

	for (const std::shared_ptr<hittable>& object : objects) {
		if (!object->bounding_box(temp_box)) return false;
		output_box = first_box ? temp_box : surrounding_box(output_box, temp_box);
		first_box = false;
	}

	return true;
}
