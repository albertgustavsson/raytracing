#pragma once
#include"color.h"

class image
{
public:
	image();
	image(unsigned long int width, unsigned long int height);
	image(const image& other);
	image(image&& other) noexcept;
	~image();
	void save_to_file(const std::string& filename);
	void save_to_ppm(const std::string& filename);
public:
	unsigned long int width;
	unsigned long int height;
	rgb_color* pixels;
};