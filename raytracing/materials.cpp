#include "materials.h"
#include "utils.h"

rgb_color material::emitted(double u, double v, const vector3& p) const {
    return rgb_color(0);
}

bool lambertian::scatter(const ray& r_in, const hit_record& rec, rgb_color& attenuation, ray& scattered) const {
    vector3 scatter_direction = rec.normal + vector3::random_unit_vector();
    
    // Catch degenerate scatter direction
    if (scatter_direction.near_zero())
        scatter_direction = rec.normal;
    
    scattered = ray(rec.p, scatter_direction);
    attenuation = albedo->value(rec.u, rec.v, rec.p);
    return true;
}

bool metal::scatter(const ray& r_in, const hit_record& rec, rgb_color& attenuation, ray& scattered) const {
    vector3 reflected = reflect(r_in.direction.get_normalized(), rec.normal);
    scattered = ray(rec.p, reflected + fuzz * vector3::random_in_unit_sphere());
    attenuation = albedo;
    return (dot(scattered.direction, rec.normal) > 0);
}

bool dielectric::scatter(const ray& r_in, const hit_record& rec, rgb_color& attenuation, ray& scattered) const {
    attenuation = rgb_color(1);
    double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

    vector3 unit_direction = r_in.direction.get_normalized();
    double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

    bool cannot_refract = refraction_ratio * sin_theta > 1.0;
    vector3 direction;

    if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
        direction = reflect(unit_direction, rec.normal);
    else
        direction = refract(unit_direction, rec.normal, refraction_ratio);

    scattered = ray(rec.p, direction);
    return true;
}

double dielectric::reflectance(double cosine, double ref_idx) {
    // Use Schlick's approximation for reflectance.
    double r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

rgb_color diffuse_light::emitted(double u, double v, const vector3& p) const {
    return emit->value(u, v, p);
}