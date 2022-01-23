#pragma once
#include <algorithm>
#include <memory>
#include "textures.h"
#include "ray.h"
#include "hittable.h"

class material {
public:
    virtual bool scatter(const ray& r_in, const hit_record& rec, rgb_color& attenuation, ray& scattered) const = 0;
};

class lambertian : public material {
public:
    lambertian(const rgb_color& a) : albedo(std::make_shared<solid_color>(a)) {}
    lambertian(std::shared_ptr<texture> a) : albedo(a) {}
    virtual bool scatter(const ray& r_in, const hit_record& rec, rgb_color& attenuation, ray& scattered) const override;

public:
    std::shared_ptr<texture> albedo;
};

class metal : public material {
public:
    metal(const rgb_color& a, double f) : albedo(a), fuzz(std::clamp(f, 0.0, 1.0)) {}
    virtual bool scatter(const ray& r_in, const hit_record& rec, rgb_color& attenuation, ray& scattered) const override;
public:
    rgb_color albedo;
    double fuzz;
};

class dielectric : public material {
public:
    dielectric(double index_of_refraction) : ir(index_of_refraction) {}
    virtual bool scatter(const ray& r_in, const hit_record& rec, rgb_color& attenuation, ray& scattered) const override;
private:
    static double reflectance(double cosine, double ref_idx);
public:
    double ir; // Index of Refraction
};
