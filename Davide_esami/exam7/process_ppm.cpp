//#include "process_ppm.h"

#include <fstream>
#include <iostream>
#include <string>

#include "mat.h"
#include "ppm.h"

bool LoadPPM(const std::string& filename, mat<vec3b>& img) {

	std::ifstream is(filename, std::ios::binary);
	if (!is)
		return false;

	std::string magic_number;
	int width, height, maxval;
	char newline;

	getline(is,magic_number);

	if (magic_number != "P6")
		return false;

	if (is.peek() == '#') {
		std::string comment;
		getline(is, comment);
	}

	is >> width >> height >> maxval;

	newline = is.get();
	if (newline != '\n')
		return false;

	if (maxval < 0)
		return false;
	
	img.resize(height, width);

	is.read(img.rawdata(), img.rawsize());

	return true;
}

void SplitRGB(const mat<vec3b>& img, mat<uint8_t>& img_r, mat<uint8_t>& img_g, mat<uint8_t>& img_b) {

	img_r.resize(img.rows(), img.cols());
	img_g.resize(img.rows(), img.cols());
	img_b.resize(img.rows(), img.cols());

	auto it_r = img_r.begin();
	auto it_g = img_g.begin();
	auto it_b = img_b.begin();
	size_t i = 0;

	for (auto& x : img) {

		it_r[i] = x[0];
		it_g[i] = x[1];
		it_b[i] = x[2];
		++i;

	}
}