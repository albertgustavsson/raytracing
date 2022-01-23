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

class checker_texture : public texture {
public:
    checker_texture(std::shared_ptr<texture> _even, std::shared_ptr<texture> _odd) : even(_even), odd(_odd) {}
    checker_texture(rgb_color c1, rgb_color c2) : even(std::make_shared<solid_color>(c1)), odd(std::make_shared<solid_color>(c2)) {}

    virtual rgb_color value(double u, double v, const vector3& p) const override;

public:
    std::shared_ptr<texture> odd, even;
};
