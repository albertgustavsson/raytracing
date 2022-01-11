#include "vector3.h"
#include "ray.h"
ray::ray() : origin{ 0,0,0 }, direction{ 0,0,0 } {}

ray::ray(const vector3& o, const vector3& d) : origin(o), direction(d) {}

vector3 ray::at(double t) const {
	return origin + direction * t;
}
