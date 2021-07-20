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
<<<<<<< HEAD
std::string Base64Encode(const std::vector<uint8_t>& v);
=======
>>>>>>> a412cb64e18484b05a01071793d7d6655b8a5c7e

#endif /* compress_h */
