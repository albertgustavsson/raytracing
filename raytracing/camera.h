#pragma once
#include "vector3.h"
#include "ray.h"

class camera {
public:
    camera(vector3 lookfrom, vector3 lookat, vector3 vup, double vfov, double aspect_ratio, double aperture, double focus_dist);
    ray get_ray(double s, double t) const;

public:
    double ar;
private:
    vector3 origin;
    vector3 lower_left_corner;
    vector3 horizontal, vertical;
    vector3 u, v, w;
    double lens_radius;
};