#include "y4m_gray.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "mat.h"

bool y4m_extract_gray(const std::string& filename, std::vector<mat<uint8_t>>& frames) {

	std::ifstream is(filename, std::ios::binary);
	if (!is)
		return false;

	std::string stream_header;
	is >> stream_header;
	if (stream_header != "YUV4MPEG2")
		return false;

	std::string tagged_field = "";

	mat<uint8_t> frame;
	int rows = 0;
	int cols = 0;

	//leggo i tag

	while (is.peek() != '\n') {

		is >> tagged_field;

		if (tagged_field[0] == 'H') {
			rows = std::stoi(&tagged_field[1]);
		}

		if (tagged_field[0] == 'W') {
			cols = std::stoi(&tagged_field[1]);
		}

		if (tagged_field[0] == 'C') {
			if (tagged_field != "C420jpeg")
				return false;
		}

	}

	//Errore nello stream header
	if (rows == 0 || cols == 0)
		return false;

	frame.resize(rows, cols);
	// leggo lo \n
	is.get();

	while (1) {

		if (is.peek() == EOF)
			break;

		is >> tagged_field;
		if (tagged_field != "FRAME")
			return false;

		// leggo lo \n
		is.get();
		//leggo le Y
		is.read(frame.rawdata(), frame.rawsize());
		
		// aggiungo il frame al vec
		frames.push_back(frame);

		//ora leggo le Cb e le Cr e li scarto
		for (size_t i = 0; i < (frame.rawsize() / 4 * 2); ++i) {
			is.get();
		}

	}
	
	return true;
}