#include "camera.h"

camera::camera(vector3 lookfrom, vector3 lookat, vector3 vup, double vfov, double aspect_ratio, double aperture, double focus_dist) {
    double theta = degrees_to_radians(vfov);
    double h = tan(theta / 2);
    double viewport_height = 2.0 * h;
    double viewport_width = aspect_ratio * viewport_height;

    w = (lookfrom - lookat).get_normalized();
    u = cross(vup, w).get_normalized();
    v = cross(w, u);

    origin = lookfrom;
    horizontal = focus_dist * viewport_width * u;
    vertical = focus_dist * viewport_height * v;
    lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_dist * w;
    
    lens_radius = aperture / 2;
}

ray camera::get_ray(double s, double t) const {
    vector3 rd = lens_radius * vector3::random_in_unit_disk();
    vector3 offset = u * rd.x + v * rd.y;

    return ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset);
}