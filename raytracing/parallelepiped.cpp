#include "parallelepiped.h"

parallelepiped::parallelepiped(vector3 v0, vector3 d0, vector3 d1, vector3 d2,
	std::shared_ptr<material> m) : sides{
		{v0     , v0 + d0     , v0 + d0 + d1     , v0 + d1     , m},
		{v0     , v0 + d0     , v0 + d0 + d2     , v0 + d2     , m},
		{v0     , v0 + d1     , v0 + d1 + d2     , v0 + d2     , m},
		{v0 + d2, v0 + d0 + d2, v0 + d0 + d1 + d2, v0 + d1 + d2, m},
		{v0 + d1, v0 + d0 + d1, v0 + d0 + d1 + d2, v0 + d1 + d2, m},
		{v0 + d0, v0 + d0 + d1, v0 + d0 + d1 + d2, v0 + d0 + d2, m}} {
	sides[0].bounding_box(box);
	for (int i = 1; i < 6; i++) {
		aabb box_temp;
		sides[i].bounding_box(box_temp);
		box = surrounding_box(box, box_temp);
	}
}

hit_record parallelepiped::hit(const ray& r, double t_min, double t_max) const {
	hit_record rec;
	double closest_so_far = t_max;

	for (int i = 0; i < 6; i++) {
		hit_record hr = sides[i].hit(r, t_min, closest_so_far);
		if (hr.did_hit) {
			closest_so_far = hr.hp.t;
			rec = hr;
		}
	}

	return rec;
}

bool parallelepiped::bounding_box(aabb& output_box) const {
	output_box = box;
	return true;
}
