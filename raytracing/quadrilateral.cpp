#include "quadrilateral.h"

quadrilateral::quadrilateral(vector3 v0, vector3 v1, vector3 v2, vector3 v3,
	std::shared_ptr<material> m) : tris{ {v0, v1, v3, m}, {v1, v2, v3, m} } {
	aabb box1, box2;
	tris[0].bounding_box(box1);
	tris[1].bounding_box(box2);
	box = surrounding_box(box1, box2);
}

hit_record quadrilateral::hit(const ray& r, double t_min, double t_max) const {
	hit_record rec;
	double closest_so_far = t_max;

	for (int i = 0; i < 2; i++) {
		hit_record hr = tris[i].hit(r, t_min, closest_so_far);
		if (hr.did_hit) {
			closest_so_far = hr.hp.t;
			rec = hr;
		}
	}

	return rec;
}

bool quadrilateral::bounding_box(aabb& output_box) const {
	output_box = box;
	return true;
}
