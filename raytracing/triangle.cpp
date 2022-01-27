#include "triangle.h"

hit_record triangle::hit(const ray& r, double t_min, double t_max) const {
	hit_record hr;
	const double EPSILON = 0.0000001;
	
	const vector3 edge1 = v1 - v0;
	const vector3 edge2 = v2 - v0;
	vector3 h = cross(r.direction, edge2);
	double a = dot(edge1, h);
	
	if (fabs(a) < EPSILON)
		return hr; // The ray is parallel to the triangle
	
	double f = 1.0 / a;
	vector3 s = r.origin - v0;
	double u = f * dot(s, h);
	
	if (u < 0.0 || u > 1.0)
		return hr;
	
	vector3 q = cross(s, edge1);
	double v = f * dot(r.direction, q);
	
	if (v < 0.0 || u + v > 1.0)
		return hr;
	
	// At this stage we can compute t to find out where the intersection point is on the line.
	double t = f * dot(edge2, q);
	if (t < t_min || t > t_max)
		return hr;
	
	if (t > EPSILON) {
		// ray intersection
		hr.hp.p = r.origin + r.direction * t;
		hr.hp.t = t;
		vector3 outward_normal = cross(edge1, edge2).get_normalized();
		hr.hp.set_face_normal(r, outward_normal);
		hr.mat_ptr = mat_ptr;
		hr.did_hit = true;
		return hr;
	}
	// This means that there is a line intersection but not a ray intersection.
	return hr;
}

bool triangle::bounding_box(aabb& output_box) const {
	vector3 minimum(
		std::min(v0.x, std::min(v1.x, v2.x)),
		std::min(v0.y, std::min(v1.y, v2.y)),
		std::min(v0.z, std::min(v1.z, v2.z)));

	vector3 maximum(
		std::max(v0.x, std::max(v1.x, v2.x)),
		std::max(v0.y, std::max(v1.y, v2.y)),
		std::max(v0.z, std::max(v1.z, v2.z)));

	output_box = aabb(minimum, maximum);
	return true;
}
