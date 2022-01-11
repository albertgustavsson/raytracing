#pragma once
#include <cstdint>
class rgb_color
{
public:
	rgb_color();
	rgb_color(double r, double g, double b);
	uint8_t get_r_u8();
	uint8_t get_g_u8();
	uint8_t get_b_u8();
	rgb_color& operator+=(const rgb_color& c);
public:
	double r, g, b;
};

rgb_color operator+(const rgb_color& c, const rgb_color& d);
rgb_color operator*(const rgb_color& c, const double t);
rgb_color operator*(const double t, const rgb_color& c);
rgb_color operator/(const rgb_color& c, const double t);
rgb_color operator/(const double t, const rgb_color& c);