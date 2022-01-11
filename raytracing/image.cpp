#include <iostream>
#include <string>
#include <fstream>
#include "image.h"
#include "color.h"

/* Image class
* Represents an Image. Functions for reading and writing image files.
*/

/* Constructor for Image */
image::image() : width(0), height(0), pixels(nullptr) {}

/* Constructor for Image */
image::image(unsigned long int w, unsigned long int h) : width(w), height(h) {
	pixels = new rgb_color[w * h];
}

/* Copy constructor for Image */
image::image(const image& other) : width(other.width), height(other.height) {
	pixels = new rgb_color[width * height];
	std::copy(other.pixels, other.pixels + (width * height), pixels);
}

/* Move constructor for Image */
image::image(image&& other) noexcept : width(other.width), height(other.height), pixels(other.pixels) {
	other.pixels = nullptr;
	other.width = 0;
	other.height = 0;
}

image::~image() {
	delete[] pixels;
}

void image::save_to_file(const std::string& filename) {
	std::string extension = filename.substr(filename.find_last_of(".") + 1);
	if (extension == "ppm") {
		this->save_to_ppm(filename);
	}
	else {
		std::cerr << "Unsupported file extension: " << extension << std::endl;
	}
}

void image::save_to_ppm(const std::string& filename) {
	std::ofstream outFile;
	outFile.open(filename);
	outFile << "P3\n" << width << ' ' << height << "\n255\n";
	for (unsigned long int row = 0; row < height; row++) {
		std::cout << "\rWriting image to file: " << (int)((double)row / (height - 1) * 100) << "%";
		for (unsigned long int col = 0; col < width; col++) {
			unsigned long int index = width * row + col;
			rgb_color& pixel = pixels[index];
			uint8_t r = pixel.get_r_u8();
			uint8_t g = pixel.get_g_u8();
			uint8_t b = pixel.get_b_u8();
			outFile << (unsigned int)r << ' ' << (unsigned int)g << ' ' << (unsigned int)b << ' ';
		}
		outFile << '\n';
	}
	std::cout << std::endl;
	outFile.close();
}
