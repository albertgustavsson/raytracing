#include "scene.h"

void scene::add(std::vector<std::shared_ptr<hittable>> obj) {
    objects.insert(objects.end(), obj.begin(), obj.end());
}

bool scene::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    double closest_so_far = t_max;

    for (const std::shared_ptr<hittable>& object : objects) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

bool scene::bounding_box(aabb& output_box) const
{
    if (objects.empty()) return false;

    aabb temp_box;
    bool first_box = true;

    for (const std::shared_ptr<hittable>& object : objects) {
        if (!object->bounding_box(temp_box)) return false;
        output_box = first_box ? temp_box : surrounding_box(output_box, temp_box);
        first_box = false;
    }

    return true;
}
