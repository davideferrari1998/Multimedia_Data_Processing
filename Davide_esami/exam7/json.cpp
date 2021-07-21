#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

#include "mat.h"
#include "ppm.h"
#include "process_ppm.h"


std::string JSON(const std::string& filename) {

	mat<vec3b> img;
	mat<uint8_t> r;
	mat<uint8_t> g;
	mat<uint8_t> b;

	std::vector<uint8_t> r_enc;
	std::vector<uint8_t> g_enc;
	std::vector<uint8_t> b_enc;

	std::string r_b64;
	std::string g_b64;
	std::string b_b64;

	LoadPPM("test.ppm", img);
	SplitRGB(img, r, g, b);

	PackBitsEncode(r, r_enc);
	PackBitsEncode(g, g_enc);
	PackBitsEncode(b, b_enc);

	r_b64 = Base64Encode(r_enc);
	g_b64 = Base64Encode(g_enc);
	b_b64 = Base64Encode(b_enc);

	std::string json = "{\n\t\"width\":" + std::to_string(img.cols()) + ",\n\t";
	json = json + "\"height\":" + std::to_string(img.rows()) + ",\n\t";
	json = json + "\"red\":" + "\"" + r_b64 + "\",\n\t";
	json = json + "\"gree\":" + "\"" + g_b64 + "\",\n\t";
	json = json + "\"blue\":" + "\"" + b_b64 + "\"\n}";

	return "";

}
