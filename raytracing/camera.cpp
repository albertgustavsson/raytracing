#include "camera.h"

camera::camera() {
    float aspect_ratio = 16.0 / 9.0;
    float viewport_height = 2.0;
    float viewport_width = aspect_ratio * viewport_height;
    float focal_length = 1.0;

    origin = vector3(0, 0, 0);
    horizontal = vector3(viewport_width, 0.0, 0.0);
    vertical = vector3(0.0, viewport_height, 0.0);
    lower_left_corner = origin - horizontal / 2 - vertical / 2 - vector3(0, 0, focal_length);
}

ray camera::get_ray(float u, float v) const {
    return ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
}