#include "pbm.h"

int main() {

	std::string filename = "im2.pbm";

	BinaryImage img;

	img.ReadFromPBM(filename);

	Image i = BinaryImageToImage(img);

	return 0;
}