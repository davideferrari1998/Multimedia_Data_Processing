#include <string>
#include <stdbool.h>

#include "pgm16.h"
#include "mat.h"

int main() {

	mat<uint16_t> img(256,256);
	std::string filename = "CR-MONO1-10-chest.pgm";
	uint16_t max;

	bool ret = load(filename, img, max);


	return 0;
}