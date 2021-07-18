//
//  y4m_color.h
//  Y4M
//
//  Created by Matteo Ferrari on 18/07/21.
//

#ifndef y4m_color_h
#define y4m_color_h

#include "types.h"
#include <string>
#include <vector>
#include "mat.h"

bool y4m_extract_color(const std::string& filename, std::vector<mat<vec3b>>& frames);

#endif /* y4m_color_h */
