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
image::image(unsigned int w, unsigned int h) : width(w), height(h) {
	pixels = new rgb_color[w * h];
}

/* Copy constructor for Image */
image::image(const image& other) : width(other.width), height(other.height) {
	pixels = new rgb_color[width * height];
	std::copy(other.pixels, other.pixels + width * height, pixels);
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
	for (unsigned int row = 0; row < height; row++) {
		std::cout << "\rWriting image to file: " << (int)((float)row / (height - 1) * 100) << "%";
		for (unsigned int col = 0; col < width; col++) {
			unsigned int index = width * row + col;
			rgb_color& pixel = pixels[index];
			int ir = static_cast<int>(255.999 * pixel.r);
			int ig = static_cast<int>(255.999 * pixel.g);
			int ib = static_cast<int>(255.999 * pixel.b);
			outFile << ir << ' ' << ig << ' ' << ib << ' ';
		}
		outFile << '\n';
	}
	std::cout << std::endl;
	outFile.close();
}
