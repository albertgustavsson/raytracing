#pragma once
#include "hittable.h"
#include "utils.h"

class sphere :
    public hittable
{
public:
    sphere(vector3 cen, double r) : center(cen), radius(r) {};

    virtual bool hit(
        const ray& r, double t_min, double t_max, hit_record& rec) const override;

public:
    vector3 center;
    double radius;
};