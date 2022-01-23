#pragma once
#include "color.h"
#include "vector3.h"

class texture {
public:
    virtual rgb_color value(double u, double v, const vector3& p) const = 0;
};

class solid_color : public texture {
public:
    solid_color(rgb_color c) : color_value(c) {}
    solid_color(double red, double green, double blue) : solid_color(rgb_color(red, green, blue)) {}

    virtual rgb_color value(double u, double v, const vector3& p) const override;

private:
    rgb_color color_value;
};