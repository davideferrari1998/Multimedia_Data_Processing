//
//  process_ppm.h
//  7-PPM_load
//
//  Created by Matteo Ferrari on 19/07/21.
//

#ifndef process_ppm_h
#define process_ppm_h

#include <string>
#include "mat.h"
#include "ppm.h"

bool LoadPPM(const std::string& filename, mat<vec3b>& img);
void SplitRGB(const mat<vec3b>& img, mat<uint8_t>& img_r, mat<uint8_t>& img_g, mat<uint8_t>& img_b);

#endif /* process_ppm_h */
