//#include "y4m_color.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "mat.h"
#include "types.h"

uint8_t check_y(float val) {

	if (val < 16)
		val = 16;
	if (val > 235)
		val = 235;

	return val;

}

uint8_t check_cb_cr(float val) {

	if (val < 16)
		val = 16;

	if (val > 240)
		val = 240;

	return val;
}

uint8_t check_RGB(float val) {

	if (val < 0)
		val = 0;
	if (val > 255)
		val = 255;

	return val;



}

bool y4m_extract_color(const std::string& filename, std::vector<mat<vec3b>>& frames) {

	std::ifstream is(filename, std::ios::binary);
	if (!is)
		return false;

	std::string stream_header;
	is >> stream_header;
	if (stream_header != "YUV4MPEG2")
		return false;

	std::string tagged_field = "";

	mat<uint8_t> y;
	mat<uint8_t> cb;
	mat<uint8_t> cr;

	mat<vec3b> frame;

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
	y.resize(rows, cols);
	cb.resize(rows/2, cols/2);
	cr.resize(rows/2, cols/2);
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

		//leggo le Y, cb e cr
		is.read(y.rawdata(), y.rawsize());
		is.read(cb.rawdata(), cb.rawsize());
		is.read(cr.rawdata(), cr.rawsize());

		//converto le 3 matrici nei range corretti

		for (auto& x : y) {
			x = check_y(x);
		}

		for (auto& x : cb) {
			x = check_cb_cr(x);
		}

		for (auto& x : cr) {
			x = check_cb_cr(x);
		}

		//Iteratori per accedere ai dati delle matrici
		auto y_it = y.begin();
		auto cb_it = cb.begin();
		auto cr_it = cr.begin();
		auto frame_it = frame.begin();

		//Index
		size_t i = 0;
		// Variabili per formule
		float Y, Cb, Cr;
		float R, G, B;

		for (int r = 0; r < rows; ++r) {

			for (int c = 0; c < cols; ++c) {

				//Assegno i valori delle matrici
				Y = y_it[r * cols + c] - 16;

				Cb = cb_it[(r/2) * (cols/2) + (c/2)] - 128;
				Cr = cr_it[(r/2) * (cols/2) + (c/2)] - 128;

				//Effettuo UPSAMPLING
				R = Y * 1.164 + Cr * 1.596;
				G = Y * 1.164 - Cb * 0.392 - Cr * 0.813;
				B = Y * 1.164 + Cb * 2.017;

				//Assegno i valori nella matrice frame e aggiusto i range
				frame_it[i][0] = check_RGB(R);
				frame_it[i][1] = check_RGB(G);
				frame_it[i][2] = check_RGB(B);

				++i;

			}
		}


		//for (auto it = frame.begin(); it != frame.end(); it++) {

		//	//Assegno i valori delle matrici
		//	Y = y_it[i_rows*cols + i_cols] - 16;
		//	Cb = cb_it[i_rows * cols + i_cols] - 128;
		//	Cr = cr_it[i_rows * cols + i_cols] - 128;

		//	//Effettuo UPSAMPLING
		//	R = (float)Y * 1, 164 + (float)Cr * 1, 596;
		//	G = (float)Y * 1, 164 - (float)Cb * 0, 392 - (float)Cr * 0, 813;
		//	B = (float)Y * 1, 164 + (float)Cb * 2, 017;

		//	//Aggiusto i range
		//	R = check_RGB(R);
		//	G = check_RGB(G);
		//	B = check_RGB(B);

		//	//Assegno i valori nella matrice frame
		//	(*it).r = R;
		//	(*it).g = G;
		//	(*it).b = B;

		//	++i;
		//}

		// aggiungo il frame al vec
		frames.push_back(frame);

	}

	return true;

}