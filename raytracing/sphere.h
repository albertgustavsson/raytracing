#pragma once
#include "hittable.h"
#include "vector3.h"

class sphere :
    public hittable
{
public:
    sphere(vector3 cen, float r) : center(cen), radius(r) {};

    virtual bool hit(
        const ray& r, float t_min, float t_max, hit_record& rec) const override;

public:
    vector3 center;
    float radius;
};