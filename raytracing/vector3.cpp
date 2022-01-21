#include <ostream>
#include <cmath>
#include "vector3.h"
#include "utils.h"

vector3::vector3() : vector3(0,0,0) {}
vector3::vector3(double x, double y, double z) : x(x), y(y), z(z) {}

inline vector3 vector3::random() {
    return vector3(random_double(), random_double(), random_double());
}

inline vector3 vector3::random(double min, double max) {
    return vector3(random_double(min, max), random_double(min, max), random_double(min, max));
}

vector3 vector3::random_in_unit_sphere() {
    while (true) {
        vector3 p = vector3::random(-1, 1);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

vector3 vector3::random_in_unit_disk()
{
    while (true) {
        auto p = vector3(random_double(-1, 1), random_double(-1, 1), 0);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

vector3 vector3::random_unit_vector() {
    return random_in_unit_sphere().get_normalized();
}

vector3 vector3::random_in_hemisphere(const vector3& normal) {
    vector3 in_unit_sphere = random_in_unit_sphere();
    if (dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}

vector3 vector3::operator-() const {
    return vector3(-x, -y, -z);
}

vector3& vector3::operator+=(const vector3& v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

vector3& vector3::operator-=(const vector3& v) {
    return *this += (-v);
}

vector3& vector3::operator*=(const double t) {
    x *= t;
    y *= t;
    z *= t;
    return *this;
}

vector3& vector3::operator/=(const double t) {
    return *this *= 1 / t;
}

vector3 vector3::get_normalized() const {
    vector3 v = *this;
    v.normalize();
    return v;
}

void vector3::normalize() {
    *this /= length();
}

double vector3::length() const {
    return sqrt(length_squared());
}

double vector3::length_squared() const {
    return x * x + y * y + z * z;
}

bool vector3::near_zero() const {
    // Return true if the vector is close to zero in all dimensions.
    const double s = 1e-8;
    return (fabs(x) < s) && (fabs(y) < s) && (fabs(z) < s);
}


std::ostream& operator<<(std::ostream& out, const vector3& v) {
    return out << v.x << ' ' << v.y << ' ' << v.z;
}

vector3 operator+(const vector3& u, const vector3& v) {
    return vector3(u.x + v.x, u.y + v.y, u.z + v.z);
}

vector3 operator-(const vector3& u, const vector3& v) {
    return u + (-v);
}

vector3 operator*(const vector3& u, const vector3& v) {
    return vector3(u.x * v.x, u.y * v.y, u.z * v.z);
}

vector3 operator*(const vector3& v, const double t) {
    return vector3(v.x * t, v.y * t, v.z * t);
}

vector3 operator*(const double t, const vector3& v) {
    return v * t;
}

vector3 operator/(const vector3& v, const double t) {
    return v * (1 / t);
}

double dot(const vector3& u, const vector3& v) {
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

vector3 cross(const vector3& u, const vector3& v) {
    return vector3(
        u.y * v.z - u.z * v.y,
        u.z * v.x - u.x * v.z,
        u.x * v.y - u.y * v.x);
}

vector3 reflect(const vector3& v, const vector3& n) {
    return v - 2 * dot(v, n) * n;
}

vector3 refract(const vector3& uv, const vector3& n, double etai_over_etat) {
    double cos_theta = fmin(dot(-uv, n), 1.0);
    vector3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
    vector3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}
