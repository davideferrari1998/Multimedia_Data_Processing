#include "pbm.h"

#include <fstream>
#include <iostream>

bool BinaryImage::ReadFromPBM(const std::string& filename) {

	std::ifstream is(filename, std::ios::binary);
	if (!is) {
		return false;
	}

	std::string magic_number;
	std::getline(is, magic_number);

	if (is.peek() == '#') {
		std::string comment;
		std::getline(is, comment);
	}

	int w, h;
	char newline;

	is >> w >> h;

	this->W = w;
	this->H = h;

	is.get(newline);
	if (newline != '\n')
		return false;

	uint8_t byte;

	is.unsetf(std::ios_base::skipws);

	while(is >> byte) {
		this->ImageData.push_back(byte);
	}

	return true;
	
}

Image BinaryImageToImage(const BinaryImage& bimg) {

	Image img;

	img.H = bimg.H;
	img.W = bimg.W;

	uint8_t padding;
	int cnt = 0;
	uint8_t buffer;

	for (auto& x : bimg) {

		if (((cnt + 1) % (img.W / 8 + 1)) == 0)
			padding = 8 - (img.W % 8);
		else
			padding = 0;

		for (size_t i = 0; i < (8-padding); ++i) {

			if (((x << i) >> 7) & 1)
				buffer = 0;
			else
				buffer = 255;

			img.ImageData.push_back(buffer);

		}

		++cnt;
	}	

	return img;
}