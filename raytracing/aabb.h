#pragma once
#include <algorithm>
#include "vector3.h"
#include "ray.h"

class aabb {
public:
	aabb() {}
	aabb(const vector3& min, const vector3& max) { minimum = min; maximum = max; }

	bool hit(const ray& r, double t_min, double t_max) const;

public:
	vector3 minimum, maximum;
};

aabb surrounding_box(const aabb& box0, const aabb& box1);
