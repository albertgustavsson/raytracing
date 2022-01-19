#pragma once
#include"color.h"
#include <vector>

class image
{
public:
	image(unsigned long int width, unsigned long int height);
	void save_to_file(const std::string& filename);
	void save_to_ppm(const std::string& filename);
public:
	unsigned long int width;
	unsigned long int height;
	std::vector<rgb_color> pixels;
};