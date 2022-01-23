#pragma once
#include <memory>
#include <vector>
#include "hittable.h"

// TODO: rename
class hittable_list : public hittable {
public:
    hittable_list() {}

    void clear() { objects.clear(); }
    void add(std::shared_ptr<hittable> object) { objects.push_back(object); }

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

public:
    std::vector<std::shared_ptr<hittable>> objects;
};