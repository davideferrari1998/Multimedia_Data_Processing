//
//  process_ppm.cpp
//  7-PPM_load
//
//  Created by Matteo Ferrari on 19/07/21.
//

//#include "process_ppm.h"
#include <fstream>
#include <iostream>
#include "ppm.h"
#include <string>
#include "mat.h"

bool LoadPPM(const std::string& filename, mat<vec3b>& img){
    
    std::ifstream is(filename, std::ios::binary);
    if(!is){
        std::cerr << "Cannot open file.\n";
        return false;
    }
    
    std::string magic_number;
    is >> magic_number;
    if(magic_number.compare("P6") != 0) return false;
    
    is.get();
    
    std::string comment;
    if(is.peek() == '#'){
        while(is.peek() != '\n'){
            is >> comment;
        }
    }
    
    std::string widths;
    is >> widths;
    int width = std::stoi(widths);
    
    std::string heights;
    is >> heights;
    int height = std::stoi(heights);
    
    img.resize(height, width);
    
    std::string maxvals;
    is >> maxvals;
    int maxval = std::stoi(maxvals);
    if((maxval <= 0) || (maxval >= 65536)) return false;
    
    //single whitespace
    is.get();
    
    /*
    if(maxval < 256){
        for(int r = 0; r < img.rows(); r++){
            for(int c = 0; c < img.cols(); c++){
                is >> img(r,c);
            }
        }
    }
    */
    is.read(reinterpret_cast<char*>(img.data()), img.rawsize());
    
    return true;
}

void SplitRGB(const mat<vec3b>& img, mat<uint8_t>& img_r, mat<uint8_t>& img_g, mat<uint8_t>& img_b){
    if(img.size() == 0) return;
    
    img_r.resize(img.rows(), img.cols());
    img_g.resize(img.rows(), img.cols());
    img_b.resize(img.rows(), img.cols());
    
    for(int r = 0; r< img.rows(); r++){
        for(int c = 0; c< img.cols(); c++){
            img_r(r,c) = img(r,c)[0];
            img_g(r,c) = img(r,c)[1];
            img_b(r,c) = img(r,c)[2];
        }
    }
}
