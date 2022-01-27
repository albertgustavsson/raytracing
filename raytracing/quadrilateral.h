#pragma once
#include "hittable.h"
#include "triangle.h"

class quadrilateral : public hittable {
public:
	quadrilateral(vector3 v0, vector3 v1, vector3 v2, vector3 v3, std::shared_ptr<material> m);

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
	virtual bool bounding_box(aabb& output_box) const override;

private:
	triangle t1, t2;
	aabb box;
};
