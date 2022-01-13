#include "materials.h"

bool lambertian::scatter(const ray& r_in, const hit_record& rec, rgb_color& attenuation, ray& scattered) const {
    vector3 scatter_direction = rec.normal + vector3::random_unit_vector();
    
    // Catch degenerate scatter direction
    if (scatter_direction.near_zero())
        scatter_direction = rec.normal;
    
    scattered = ray(rec.p, scatter_direction);
    attenuation = albedo;
    return true;
}

bool metal::scatter(const ray& r_in, const hit_record& rec, rgb_color& attenuation, ray& scattered) const {
    vector3 reflected = reflect(r_in.direction.get_normalized(), rec.normal);
    scattered = ray(rec.p, reflected);
    attenuation = albedo;
    return (dot(scattered.direction, rec.normal) > 0);
}