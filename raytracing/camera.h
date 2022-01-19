#pragma once
#include "utils.h"

class camera
{
public:
    camera(vector3 lookfrom, vector3 lookat, vector3 vup, double vfov, double aspect_ratio);
    ray get_ray(double s, double t) const;

private:
    vector3 origin;
    vector3 lower_left_corner;
    vector3 horizontal;
    vector3 vertical;
};