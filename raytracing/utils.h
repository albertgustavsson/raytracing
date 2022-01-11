#pragma once
#include <limits>
#include <random>
#include "vector3.h"
#include "ray.h"

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees) {
	return degrees * pi / 180.0;
}

inline double random_float() {
	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
	static std::mt19937 generator;
	return distribution(generator);
}

inline double random_float(double min, double max) {
	// Returns a random real in [min,max).
	return min + (max - min) * random_float();
}