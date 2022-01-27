#include "sphere.h"
#include "utils.h"

hit_record sphere::hit(const ray& r, double t_min, double t_max) const {
	hit_record hr;
	vector3 oc = r.origin - center;
	double a = r.direction.length_squared();
	double half_b = dot(oc, r.direction);
	double c = oc.length_squared() - radius * radius;
	
	double discriminant = half_b * half_b - a * c;
	if (discriminant < 0) return hr;
	
	double sqrtd = sqrt(discriminant);

	// Find the nearest root that lies in the acceptable range.
	double root = (-half_b - sqrtd) / a;
	if (root < t_min || root > t_max) {
		root = (-half_b + sqrtd) / a;
		if (root < t_min || root > t_max)
			return hr;
	}
	hr.did_hit = true;

	hr.hp.t = root;
	hr.hp.p = r.at(hr.hp.t);
	vector3 outward_normal = (hr.hp.p - center) / radius;
	hr.hp.set_face_normal(r, outward_normal);
	hr.mat_ptr = mat_ptr;
	get_sphere_uv(outward_normal, hr.hp.u, hr.hp.v);
	return hr;
}

bool sphere::bounding_box(aabb& output_box) const {
	vector3 offset(radius, radius, radius);
	output_box = aabb(center - offset, center + offset);
	return true;
}

void sphere::get_sphere_uv(const vector3& p, double& u, double& v) {
	// p: a given point on the sphere of radius one, centered at the origin.
	// u: returned value [0,1] of angle around the Y axis from X=-1.
	// v: returned value [0,1] of angle from Y=-1 to Y=+1.
	//     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
	//     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
	//     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

	double theta = acos(-p.y);
	double phi = atan2(-p.z, p.x) + pi;

	u = phi / (2 * pi);
	v = theta / pi;
}
