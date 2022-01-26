#pragma once
#include <iostream>
#include "hittable.h"
#include "hittable_list.h"

class bvh_node : public hittable {
public:
    bvh_node(const hittable_list& sc) : bvh_node(sc.objects, 0, sc.objects.size()) {}
    bvh_node(const std::vector<std::shared_ptr<hittable>>& src_objects, size_t start, size_t end);

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
    virtual bool bounding_box(aabb& output_box) const override;

public:
    std::shared_ptr<hittable> left;
    std::shared_ptr<hittable> right;
    aabb box;
};

inline bool box_compare(const std::shared_ptr<hittable>& a, const std::shared_ptr<hittable>& b, int axis);

bool box_x_compare(const std::shared_ptr<hittable>& a, const std::shared_ptr<hittable>& b);
bool box_y_compare(const std::shared_ptr<hittable>& a, const std::shared_ptr<hittable>& b);
bool box_z_compare(const std::shared_ptr<hittable>& a, const std::shared_ptr<hittable>& b);