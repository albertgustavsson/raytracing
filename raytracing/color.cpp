#include <cmath>
#include "color.h"

rgb_color::rgb_color() : r(0.0), g(0.0), b(0.0) {}
rgb_color::rgb_color(float r, float g, float b) : r(r), g(g), b(b) {}


uint8_t rgb_color::get_r_u8() {
	return static_cast<uint8_t>(fmin(255.0, r * 256.0));
}
uint8_t rgb_color::get_g_u8() {
	return static_cast<uint8_t>(fmin(255.0, g * 256.0));
}
uint8_t rgb_color::get_b_u8() {
	return static_cast<uint8_t>(fmin(255.0, b * 256.0));
}

rgb_color operator+(const rgb_color& c, const rgb_color& d) {
	return rgb_color(c.r + d.r, c.g + d.g, c.b + d.b);
}

rgb_color operator*(const rgb_color& c, const float t) {
	return rgb_color(c.r * t, c.g * t, c.b * t);
}

rgb_color operator*(const float t, const rgb_color& c) {
	return c * t;
}