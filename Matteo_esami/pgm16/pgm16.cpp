//
//  pgm16.cpp
//  pgm16
//
//  Created by Matteo Ferrari on 08/07/21.
//

#include "pgm16.h"
#include <iostream>
#include <fstream>

bool load(const std::string& filename, mat<uint16_t>& img, uint16_t& maxvalue){
    
    //open file
    std::ifstream is(filename, std::ios::binary);
    if(!is){
        std::cout << "can't open file.\n";
        return false;
    }
    
    std::string magic_number;
    is >> magic_number;
    if(!(magic_number[0] == 'P' && magic_number[1] == '5')){
        std::cout << "file not PGM.\n";
        return false;
    }
    
    //avoid '\n'
    is.get();
    
    std::string comment;
    if(is.peek() == '#'){
        std::getline(is, comment);
    }
    
    std::string width, height;
    is >> width >> height;
    is.get();
    
    is >> maxvalue;
    
    is.get();
    
    img.resize(std::stoi(height), std::stoi(width));
    
    if(maxvalue < 256){
        for(auto& x: img){
            uint8_t c;
            is >> c;
            x = c;
        }
    } else {
        is.read(img.rawdata(), img.rawsize());
        for(auto& x: img){
            uint8_t b1 = x;
            uint8_t b2 = x >> 8;
            uint16_t b3 = b2 | (b1 << 8);
            x = b3;
        }
        
    }
    
    return true;
}
