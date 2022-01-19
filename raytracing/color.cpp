#include <cmath>
#include "color.h"
#include <algorithm>
#include "utils.h"

rgb_color::rgb_color() : r(0.0), g(0.0), b(0.0) {}
rgb_color::rgb_color(double r, double g, double b) : r(r), g(g), b(b) {}

rgb_color rgb_color::random() {
	return rgb_color(random_double(), random_double(), random_double());
}

rgb_color rgb_color::random(double min, double max) {
	return rgb_color(random_double(min, max), random_double(min, max), random_double(min, max));
}

uint8_t rgb_color::get_r_u8() {
	return static_cast<uint8_t>(std::clamp(r * 256.0, 0.0, 255.0));
}
uint8_t rgb_color::get_g_u8() {
	return static_cast<uint8_t>(std::clamp(g * 256.0, 0.0, 255.0));
}
uint8_t rgb_color::get_b_u8() {
	return static_cast<uint8_t>(std::clamp(b * 256.0, 0.0, 255.0));
}

void rgb_color::apply_gamma_correction(const double gamma) {
	r = pow(r, gamma);
	g = pow(g, gamma);
	b = pow(b, gamma);
}

rgb_color& rgb_color::operator+=(const rgb_color& c) {
	return *this = *this + c;
}

rgb_color& rgb_color::operator/=(const double t) {
	return *this = *this / t;
}

rgb_color operator+(const rgb_color& c, const rgb_color& d) {
	return rgb_color(c.r + d.r, c.g + d.g, c.b + d.b);
}

rgb_color operator*(const rgb_color& c, const rgb_color& d) {
	return rgb_color(c.r * d.r, c.g * d.g, c.b * d.b);
}

rgb_color operator*(const rgb_color& c, const double t) {
	return rgb_color(c.r * t, c.g * t, c.b * t);
}

rgb_color operator*(const double t, const rgb_color& c) {
	return c * t;
}

rgb_color operator/(const rgb_color& c, const double t) {
	return rgb_color(c.r / t, c.g / t, c.b / t);
}

rgb_color operator/(const double t, const rgb_color& c) {
	return c / t;
}