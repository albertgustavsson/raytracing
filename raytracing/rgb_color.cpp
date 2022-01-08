#include "rgb_color.h"

rgb_color::rgb_color() : red(0.0), green(0.0), blue(0.0) {}
rgb_color::rgb_color(float r, float g, float b) : red(r), green(g), blue(b) {}

rgb_color operator+(const rgb_color& c, const rgb_color& d) {
	return rgb_color(c.red + d.red, c.green + d.green, c.blue + d.blue);
}

rgb_color operator*(const rgb_color& c, const float t) {
	return rgb_color(c.red * t, c.green * t, c.blue * t);
}

rgb_color operator*(const float t, const rgb_color& c) {
	return c * t;
}