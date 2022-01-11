#include <ostream>
#include <cmath>
#include "vector3.h"
#include "utils.h"

vector3::vector3() : x(0.0), y(0.0), z(0.0) {}
vector3::vector3(float x, float y, float z) : x(x), y(y), z(z) {}

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

vector3& vector3::operator*=(const float t) {
    x *= t;
    y *= t;
    z *= t;
    return *this;
}

vector3& vector3::operator/=(const float t) {
    return *this *= 1 / t;
}

void vector3::normalize() {
    *this /= length();
}

float vector3::length() const {
    return sqrt(length_squared());
}

float vector3::length_squared() const {
    return x * x + y * y + z * z;
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

vector3 operator*(const vector3& v, const float t) {
    return vector3(v.x * t, v.y * t, v.z * t);
}

vector3 operator*(const float t, const vector3& v) {
    return v * t;
}

vector3 operator/(const vector3& v, const float t) {
    return v * (1 / t);
}

vector3 normalize(const vector3& v) {
    return v / v.length();
}

float dot(const vector3& u, const vector3& v) {
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

vector3 cross(const vector3& u, const vector3& v) {
    return vector3(
        u.y * v.z - u.z * v.y,
        u.z * v.x - u.x * v.z,
        u.x * v.y - u.y * v.x);
}