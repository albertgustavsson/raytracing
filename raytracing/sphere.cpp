#include "sphere.h"
#include "utils.h"

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    vector3 oc = r.origin - center;
    double a = r.direction.length_squared();
    double half_b = dot(oc, r.direction);
    double c = oc.length_squared() - radius * radius;
    
    double discriminant = half_b * half_b - a * c;
    if (discriminant < 0) return false;
    
    double sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    double root = (-half_b - sqrtd) / a;
    if (root < t_min || root > t_max) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || root > t_max)
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    vector3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;
    get_sphere_uv(outward_normal, rec.u, rec.v);

    return true;
}

void sphere::get_sphere_uv(const vector3& p, double& u, double& v) {
    // p: a given point on the sphere of radius one, centered at the origin.
    // u: returned value [0,1] of angle around the Y axis from X=-1.
    // v: returned value [0,1] of angle from Y=-1 to Y=+1.
    //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
    //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
    //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

    double theta = acos(-p.y);
    double phi = atan2(-p.z, p.x) + pi;

    u = phi / (2 * pi);
    v = theta / pi;
}
