// raytracing.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "image.h"

int main()
{
	const unsigned int imageWidth = 256;
	const unsigned int imageHeight = 256;

	std::cout << "Creating image (" << imageWidth << "x" << imageHeight << ")" << std::endl;
	image img(imageWidth, imageHeight);

	for (unsigned int row = 0; row < img.height; row++) {
		std::cout << "\rGenerating image: " << (int)((float)row / (img.height - 1) * 100) << "%";
		for (unsigned int col = 0; col < img.width; col++) {
			unsigned int index = img.width * row + col;
			img.pixels[index].red = float(col) / (img.width-1);
			img.pixels[index].green = float(img.height - 1 - row) / (img.height - 1);
			img.pixels[index].blue = 0.25f;
		}
	}
	std::cout << std::endl;
	img.save_to_file("image.ppm");
}