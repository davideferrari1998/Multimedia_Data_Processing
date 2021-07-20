//
//  y4m_gray.h
//  Y4M
//
//  Created by Matteo Ferrari on 13/07/21.
//

#ifndef y4m_gray_h
#define y4m_gray_h

#include <string>
#include <vector>
#include "mat.h"

bool y4m_extract_gray(const std::string& filename, std::vector<mat<uint8_t>>& frames);

#endif /* y4m_gray_h */
