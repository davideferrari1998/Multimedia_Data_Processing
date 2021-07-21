#pragma once

#include "mat.h"
#include "ppm.h"

#include <string>

bool LoadPPM(const std::string& filename, mat<vec3b>& img);
void SplitRGB(const mat<vec3b>& img, mat<uint8_t>& img_r, mat<uint8_t>& img_g, mat<uint8_t>& img_b);
void PackBitsEncode(const mat<uint8_t>& img, std::vector<uint8_t>& encoded);
std::string Base64Encode(const std::vector<uint8_t>& v);
std::string JSON(const std::string& filename);