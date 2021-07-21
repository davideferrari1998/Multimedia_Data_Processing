#include "process_ppm.h"


int main() {
	
	/*mat<vec3b> img;
	mat<uint8_t> r;
	mat<uint8_t> g;
	mat<uint8_t> b;

	std::vector<uint8_t> r_enc;
	std::vector<uint8_t> g_enc;
	std::vector<uint8_t> b_enc;

	LoadPPM("test.ppm", img);
	SplitRGB(img, r, g, b);
	PackBitsEncode(r, r_enc);
	PackBitsEncode(g, g_enc);
	PackBitsEncode(b, b_enc);

	std::string b64 = Base64Encode(r_enc);*/
	
	std::string json = JSON("test.ppm");

	return 0;
}