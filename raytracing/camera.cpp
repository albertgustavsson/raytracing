#include "camera.h"

camera::camera(vector3 lookfrom, vector3 lookat, vector3 vup, double vfov, double aspect_ratio) {
    double theta = degrees_to_radians(vfov);
    double h = tan(theta / 2);
    double viewport_height = 2.0 * h;
    double viewport_width = aspect_ratio * viewport_height;
    
    double focal_length = 1.0;

    vector3 w = (lookfrom - lookat).get_normalized();
    vector3 u = cross(vup, w).get_normalized();
    vector3 v = cross(w, u);

    origin = lookfrom;
    horizontal = viewport_width * u;
    vertical = viewport_height * v;
    lower_left_corner = origin - horizontal / 2 - vertical / 2 - w;
}

ray camera::get_ray(double s, double t) const {
    return ray(origin, lower_left_corner + s * horizontal + t * vertical - origin);
}