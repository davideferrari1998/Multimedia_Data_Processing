//
//  pcx.h
//  pcx
//
//  Created by Matteo Ferrari on 21/07/21.
//

#ifndef pcx_h
#define pcx_h

#include "types.h"
#include "mat.h"
#include <string>


bool load_pcx(const std::string& filename, mat<uint8_t>& img);
bool load_pcx(const std::string& filename, mat<vec3b>& img);
#endif /* pcx_h */
