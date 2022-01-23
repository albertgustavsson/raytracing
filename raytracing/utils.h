#pragma once
#include <iostream>
#include <limits>
#include <random>
#include <chrono>

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees) {
	return degrees * pi / 180.0;
}

inline double random_double() {
	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
	static std::mt19937 generator;
	return distribution(generator);
}

inline double random_double(double min, double max) {
	// Returns a random real in [min,max).
	return min + (max - min) * random_double();
}

struct timer {
	std::string name;
	std::chrono::high_resolution_clock::time_point start, end;
	timer(std::string name) : name(name) {
		start = std::chrono::high_resolution_clock::now();
	}

	~timer() {
		end = std::chrono::high_resolution_clock::now();
		std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
		std::cout << name << " took " << duration.count() / 1000.0 << " seconds" << std::endl;
	}
};

struct render_config {
	unsigned int image_width;
	unsigned int image_height;
	unsigned int samples_per_pixel;
	unsigned int n_threads;
	unsigned int block_width;
	unsigned int block_height;
};