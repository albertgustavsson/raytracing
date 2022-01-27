#pragma once
#include <memory>
#include "hittable.h"
#include "vector3.h"
#include "ray.h"

class sphere : public hittable {
public:
	sphere(vector3 c, double r, std::shared_ptr<material> m) : center(c), radius(r), mat_ptr(m) {}

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
	virtual bool bounding_box(aabb& output_box) const override;

private:
	static void get_sphere_uv(const vector3& p, double& u, double& v);

public:
	vector3 center;
	double radius;
	std::shared_ptr<material> mat_ptr;
};
