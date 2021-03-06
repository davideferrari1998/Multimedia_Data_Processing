#pragma once

#include <string>
#include <vector>

#include "mat.h"

bool y4m_extract_gray(const std::string& filename, std::vector<mat<uint8_t>>& frames);
