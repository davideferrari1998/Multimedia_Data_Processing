#pragma once

#include <string>
#include <vector>

#include "mat.h"
#include "types.h"

bool y4m_extract_color(const std::string& filename, std::vector<mat<vec3b>>& frames);