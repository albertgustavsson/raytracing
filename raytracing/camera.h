#pragma once
#include "utils.h"

class camera
{
public:
    camera();
    ray get_ray(double u, double v) const;

private:
    vector3 origin;
    vector3 lower_left_corner;
    vector3 horizontal;
    vector3 vertical;
};