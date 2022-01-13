#pragma once
#include "utils.h"

class sphere :
    public hittable
{
public:
    sphere(vector3 cen, double r, std::shared_ptr<material> m) : center(cen), radius(r), mat_ptr(m) {}

    virtual bool hit(
        const ray& r, double t_min, double t_max, hit_record& rec) const override;

public:
    vector3 center;
    double radius;
    std::shared_ptr<material> mat_ptr;
};