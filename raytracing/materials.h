#pragma once
#include "utils.h"

struct hit_record; // Because c++ is stupid

class material {
public:
    virtual bool scatter(const ray& r_in, const hit_record& rec, rgb_color& attenuation, ray& scattered) const = 0;
};

class lambertian : public material {
public:
    lambertian(const rgb_color& a) : albedo(a) {}
    virtual bool scatter(const ray& r_in, const hit_record& rec, rgb_color& attenuation, ray& scattered) const override;

public:
    rgb_color albedo;
};

class metal : public material {
public:
    metal(const rgb_color& a, double f) : albedo(a), fuzz(std::clamp(f, 0.0, 1.0)) {}
    virtual bool scatter(const ray& r_in, const hit_record& rec, rgb_color& attenuation, ray& scattered) const override;

public:
    rgb_color albedo;
    double fuzz;
};