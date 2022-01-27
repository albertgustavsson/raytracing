#include "aabb.h"

bool aabb::hit(const ray& r, double t_min, double t_max) const {
	for (int a = 0; a < 3; a++) {
		double inv_dir = 1.0 / r.direction[a];
		double t0 = (minimum[a] - r.origin[a]) * inv_dir;
		double t1 = (maximum[a] - r.origin[a]) * inv_dir;
		if (inv_dir < 0.0)
			std::swap(t0, t1);
		t_min = std::max(t0, t_min);
		t_max = std::min(t1, t_max);
		// Note: Cannot use <= when using zero volume boxes (e.g. axis-aligned flat triangles)
		if (t_max < t_min)
			return false;
	}
	return true;
}

aabb surrounding_box(const aabb& box0, const aabb& box1) {
	vector3 minimum(
		std::min(box0.minimum.x, box1.minimum.x),
		std::min(box0.minimum.y, box1.minimum.y),
		std::min(box0.minimum.z, box1.minimum.z));

	vector3 maximum(
		std::max(box0.maximum.x, box1.maximum.x),
		std::max(box0.maximum.y, box1.maximum.y),
		std::max(box0.maximum.z, box1.maximum.z));

	return aabb(minimum, maximum);
}
