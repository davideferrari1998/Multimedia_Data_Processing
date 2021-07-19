//
//  compress.h
//  7-PPM_load
//
//  Created by Matteo Ferrari on 19/07/21.
//

#ifndef compress_h
#define compress_h

#include "mat.h"
#include <vector>

void PackBitsEncode(const mat<uint8_t>& img, std::vector<uint8_t>& encoded);

#endif /* compress_h */
