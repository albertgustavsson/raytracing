#pragma once
#include <ostream>

class vector3
{
public:
    vector3();
    vector3(double x, double y, double z);

    inline static vector3 random();
    inline static vector3 random(double min, double max);
    static vector3 random_in_unit_sphere();
    static vector3 random_unit_vector();

    vector3 operator-() const;
    vector3& operator+=(const vector3& v);
    vector3& operator-=(const vector3& v);
    vector3& operator*=(const double t);
    vector3& operator/=(const double t);

    vector3 get_normalized() const;
    void normalize();
    double length() const;
    double length_squared() const;

public:
    double x, y, z;
};

std::ostream& operator<<(std::ostream& out, const vector3& v);

vector3 operator+(const vector3& u, const vector3& v);
vector3 operator-(const vector3& u, const vector3& v);
vector3 operator*(const vector3& u, const vector3& v);
vector3 operator*(const vector3& v, const double t);
vector3 operator*(const double t, const vector3& v);
vector3 operator/(const vector3& v, const double t);

double dot(const vector3& u, const vector3& v);
vector3 cross(const vector3& u, const vector3& v);