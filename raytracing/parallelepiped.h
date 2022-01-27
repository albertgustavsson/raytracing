#pragma once
#include "hittable.h"
#include "quadrilateral.h"

class parallelepiped : public hittable {
public:
    parallelepiped(vector3 v0, vector3 d0, vector3 d1, vector3 d2, std::shared_ptr<material> m);

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
    virtual bool bounding_box(aabb& output_box) const override;

private:
    quadrilateral sides[6];
    aabb box;
};
