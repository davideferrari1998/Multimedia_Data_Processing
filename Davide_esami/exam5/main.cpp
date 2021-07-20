#include "y4m_gray.h"
#include "y4m_color.h"
#include "types.h"

int main() {

	//std::vector<mat<uint8_t>> frames;

	//y4m_extract_gray("test1.y4m", frames);

	std::vector<mat<vec3b>> frames;

	y4m_extract_color("test1.y4m", frames);

	return 0;
}