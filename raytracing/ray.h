#pragma once
#include "vector3.h"

class ray {
public:
	ray(); // TODO: remove
	ray(const vector3& o, const vector3& d);
	vector3 at(double t) const;
public:
	vector3 origin;
	vector3 direction;
};
