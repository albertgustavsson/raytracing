#pragma once
#include <memory>
#include <vector>
#include "hittable.h"

class scene : public hittable {
public:
    scene() {}
    scene(std::shared_ptr<hittable> object) { add(object); }
    scene(std::vector<std::shared_ptr<hittable>> objects) { add(objects); }

    void clear() { objects.clear(); }
    void add(std::shared_ptr<hittable> object) { objects.push_back(object); }
    void add(std::vector<std::shared_ptr<hittable>> obj);

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
    virtual bool bounding_box(aabb& output_box) const override;

public:
    std::vector<std::shared_ptr<hittable>> objects;
};