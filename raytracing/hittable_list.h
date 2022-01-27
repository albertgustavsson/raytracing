#pragma once
#include <memory>
#include <vector>
#include "hittable.h"

class hittable_list : public hittable {
public:
	hittable_list() {}
	hittable_list(std::shared_ptr<hittable> object) { add(object); }
	hittable_list(std::vector<std::shared_ptr<hittable>> objects) { add(objects); }

	void clear() { objects.clear(); }
	void add(std::shared_ptr<hittable> object) { objects.push_back(object); }
	void add(std::vector<std::shared_ptr<hittable>> obj);

	virtual hit_record hit(const ray& r, double t_min, double t_max) const override;
	virtual bool bounding_box(aabb& output_box) const override;

public:
	std::vector<std::shared_ptr<hittable>> objects;
};
