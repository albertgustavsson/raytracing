#pragma once
#include "hittable.h"
#include "triangle.h"

class quadrilateral : public hittable {
public:
	quadrilateral(vector3 v0, vector3 v1, vector3 v2, vector3 v3, std::shared_ptr<material> m);

	virtual hit_record hit(const ray& r, double t_min, double t_max) const override;
	virtual bool bounding_box(aabb& output_box) const override;

private:
	triangle tris[2];
	aabb box;
};
