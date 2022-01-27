#include "quadrilateral.h"

quadrilateral::quadrilateral(vector3 v0, vector3 v1, vector3 v2, vector3 v3,
		std::shared_ptr<material> m) : t1(v0, v1, v3, m), t2(v1, v2, v3, m) {
	aabb box1, box2;
	t1.bounding_box(box1);
	t2.bounding_box(box2);
	box = surrounding_box(box1, box2);
}

bool quadrilateral::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	if (t1.hit(r, t_min, t_max, rec))
		return true;
	else if (t2.hit(r, t_min, t_max, rec))
		return true;
	return false;
}

bool quadrilateral::bounding_box(aabb& output_box) const {
	output_box = box;
	return true;
}
