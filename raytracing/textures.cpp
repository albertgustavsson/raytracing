#include "textures.h"

rgb_color solid_color::value(double u, double v, const vector3& p) const {
    return color_value;
}

rgb_color checker_texture::value(double u, double v, const vector3& p) const {
    double sines = sin(10 * p.x) * sin(10 * p.y) * sin(10 * p.z);
    if (sines < 0)
        return odd->value(u, v, p);
    else
        return even->value(u, v, p);
}
