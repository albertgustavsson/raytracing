#pragma once

#include <memory>
#include <vector>
#include "hittable.h"

class hittable_list : public hittable {
public:
    hittable_list() {}
    hittable_list(std::shared_ptr<hittable> object) { add(object); }

    void clear() { objects.clear(); }
    void add(std::shared_ptr<hittable> object) { objects.push_back(object); }

    virtual bool hit(
        const ray& r, float t_min, float t_max, hit_record& rec) const override;

public:
    std::vector<std::shared_ptr<hittable>> objects;
};