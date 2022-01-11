#pragma once
#include <ostream>

class vector3
{
public:
    vector3();
    vector3(float x, float y, float z);
    
    vector3 operator-() const;
    vector3& operator+=(const vector3& v);
    vector3& operator-=(const vector3& v);
    vector3& operator*=(const float t);
    vector3& operator/=(const float t);

    void normalize();
    float length() const;
    float length_squared() const;

public:
    float x, y, z;
};

std::ostream& operator<<(std::ostream& out, const vector3& v);

vector3 operator+(const vector3& u, const vector3& v);
vector3 operator-(const vector3& u, const vector3& v);
vector3 operator*(const vector3& u, const vector3& v);
vector3 operator*(const vector3& v, const float t);
vector3 operator*(const float t, const vector3& v);
vector3 operator/(const vector3& v, const float t);

vector3 normalize(const vector3& v);
float dot(const vector3& u, const vector3& v);
vector3 cross(const vector3& u, const vector3& v);