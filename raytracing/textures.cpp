#include "textures.h"

rgb_color solid_color::value(double u, double v, const vector3& p) const {
    return color_value;
}