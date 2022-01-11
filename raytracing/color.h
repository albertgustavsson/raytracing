#pragma once
#include <cstdint>
class rgb_color
{
public:
	rgb_color();
	rgb_color(float r, float g, float b);
	uint8_t get_r_u8();
	uint8_t get_g_u8();
	uint8_t get_b_u8();
	rgb_color& operator+=(const rgb_color& c);
public:
	float r;
	float g;
	float b;
};

rgb_color operator+(const rgb_color& c, const rgb_color& d);
rgb_color operator*(const rgb_color& c, const float t);
rgb_color operator*(const float t, const rgb_color& c);
rgb_color operator/(const rgb_color& c, const float t);
rgb_color operator/(const float t, const rgb_color& c);