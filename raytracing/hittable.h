#pragma once
#include <memory>
#include "aabb.h"
#include "vector3.h"
#include "ray.h"
//#include "materials.h"

// TODO: Why is this still necessary?
class material;

struct hit_point {
	vector3 p;
	vector3 normal;
	double t = -1.0;
	double u = 0.0, v = 0.0;
	bool front_face = true;
	inline void set_face_normal(const ray& r, const vector3& outward_normal) {
		front_face = dot(r.direction, outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

struct hit_record {
	bool did_hit = false;
	hit_point hp;
	std::shared_ptr<material> mat_ptr;
};

class hittable {
public:
	virtual hit_record hit(const ray& r, double t_min, double t_max) const = 0;
	virtual bool bounding_box(aabb& output_box) const = 0;
};
