#pragma once
#include <memory>
#include "hittable.h"
#include "materials.h"
#include "vector3.h"
#include "ray.h"

class triangle : public hittable {
public:
	triangle(vector3 v0, vector3 v1, vector3 v2, std::shared_ptr<material> m)
		: v0(v0), v1(v1), v2(v2), mat_ptr(m) {}

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
	virtual bool bounding_box(aabb& output_box) const override;

public:
	vector3 v0, v1, v2;
	std::shared_ptr<material> mat_ptr;
};
