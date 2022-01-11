#pragma once
#include <limits>
#include <random>
#include "vector3.h"
#include "ray.h"

const float infinity = std::numeric_limits<float>::infinity();
const float pi = 3.1415926535897932385;

inline float degrees_to_radians(float degrees) {
	return degrees * pi / 180.0;
}

inline float random_float() {
	static std::uniform_real_distribution<float> distribution(0.0, 1.0);
	static std::mt19937 generator;
	return distribution(generator);
}

inline float random_float(float min, float max) {
	// Returns a random real in [min,max).
	return min + (max - min) * random_float();
}