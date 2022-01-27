#pragma once
#include <vector>
#include"color.h"

class image {
public:
	image(unsigned int width, unsigned int height);
	void save_to_file(const std::string& filename);
	void save_to_ppm(const std::string& filename);
public:
	unsigned int width;
	unsigned int height;
	std::vector<rgb_color> pixels;
};
