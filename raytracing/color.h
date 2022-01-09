#pragma once
class rgb_color
{
public:
	rgb_color();
	rgb_color(float r, float g, float b);
public:
	float r;
	float g;
	float b;
};

rgb_color operator+(const rgb_color& c, const rgb_color& d);
rgb_color operator*(const rgb_color& c, const float t);
rgb_color operator*(const float t, const rgb_color& c);