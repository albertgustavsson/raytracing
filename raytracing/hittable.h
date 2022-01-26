#pragma once
#include <memory>
#include "aabb.h"
#include "vector3.h"
#include "ray.h"
//#include "materials.h"

// TODO: fix circular dependency
class material;

struct hit_record {
    vector3 p;
    vector3 normal;
    std::shared_ptr<material> mat_ptr;
    double t = -1.0;
    double u = 0.0, v = 0.0;
    bool front_face = true;
    inline void set_face_normal(const ray& r, const vector3& outward_normal) {
        front_face = dot(r.direction, outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
public:
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
    virtual bool bounding_box(aabb& output_box) const = 0;
};