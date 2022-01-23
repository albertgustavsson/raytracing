#include "triangle.h"

bool triangle::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    const double EPSILON = 0.0000001;
    
    const vector3 edge1 = v1 - v0;
    const vector3 edge2 = v2 - v0;
    vector3 h = cross(r.direction, edge2);
    double a = dot(edge1, h);
    
    if (fabs(a) < EPSILON)
        return false; // The ray is parallel to the triangle
    
    double f = 1.0 / a;
    vector3 s = r.origin - v0;
    double u = f * dot(s, h);
    
    if (u < 0.0 || u > 1.0)
        return false;
    
    vector3 q = cross(s, edge1);
    double v = f * dot(r.direction, q);
    
    if (v < 0.0 || u + v > 1.0)
        return false;
    
    // At this stage we can compute t to find out where the intersection point is on the line.
    double t = f * dot(edge2, q);
    if (t < t_min || t > t_max)
        return false;
    
    if (t > EPSILON) {
        // ray intersection
        rec.p = r.origin + r.direction * t;
        rec.t = t;
        vector3 outward_normal = cross(edge1, edge2).get_normalized();
        rec.set_face_normal(r, outward_normal);
        rec.mat_ptr = mat_ptr;
        return true;
    }
    // This means that there is a line intersection but not a ray intersection.
    return false;
}
