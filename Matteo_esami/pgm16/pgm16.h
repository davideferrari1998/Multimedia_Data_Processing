//
//  pgm16.h
//  pgm16
//
//  Created by Matteo Ferrari on 08/07/21.
//

#ifndef pgm16_h
#define pgm16_h

#include <string>
#include "mat.h"
#include <cstdint>

bool load(const std::string& filename, mat<uint16_t>& img, uint16_t& maxvalue);

#endif /* pgm16_h */
