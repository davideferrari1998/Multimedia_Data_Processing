//
//  main.cpp
//  7-PPM_load
//
//  Created by Matteo Ferrari on 19/07/21.
//

#include <iostream>
#include "process_ppm.h"
#include "compress.h"

int main(int argc, const char * argv[]) {
    
    mat<vec3b> img;
    mat<uint8_t> img_r;
    mat<uint8_t> img_g;
    mat<uint8_t> img_b;
    LoadPPM("facolta.ppm", img);
    SplitRGB(img, img_r, img_g, img_b);
    std::vector<uint8_t> encode;
    PackBitsEncode(img_g, encode);
    
    return 0;
}
