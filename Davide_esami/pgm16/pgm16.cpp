#include <iostream>
#include <fstream>

#include "pgm16.h"
#include "mat.h"

bool load(const std::string& filename, mat<uint16_t>& img, uint16_t& maxvalue) {

	std::ifstream is(filename, std::ios::binary);

	if (!is) {
		return false;
	}

	std::string magic_number;
	std::getline(is, magic_number);

	if (magic_number != "P5")
		return false;

	if (is.peek() == '#') {

		std::string comment;
		std::getline(is, comment);

	}

	int width, height, max;
	char newline;
	is >> width >> height >> max;

	maxvalue = max;

	is.get(newline);

	if (newline != '\n')
		return false;

	img.resize(height, width);

	if (maxvalue < 256) {
		char num;
		for (size_t i = 0; i < (img.cols() * img.rows()); ++i) {
			is.get(num);
			img.data()[i] = (uint8_t) num;
		}
	}
	else {
		is.read(img.rawdata(), img.rawsize());
		for (auto& x : img) {

			uint8_t b1 = x;
			uint8_t b2 = x >> 8;
			uint16_t b3 = b2 | (b1 << 8);
			x = b3;
			

		}
	}

	


	return true;
}