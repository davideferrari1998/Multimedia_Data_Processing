//
//  pcx.cpp
//  pcx
//
//  Created by Matteo Ferrari on 21/07/21.
//

#include "pcx.h"
#include <fstream>
#include <iostream>

bool load_pcx(const std::string& filename, mat<uint8_t>& img){
    
    std::ifstream is(filename, std::ios::binary);
    if(!is){
        std::cerr << "Cannot open file.\n";
        return false;
    }
    
    int cont_header = 0;
    
    uint8_t Manufacturer;
    is.read(reinterpret_cast<char*>(&Manufacturer), 1);
    if(Manufacturer != 0x0A){
        return false;
    }
    cont_header++;
    
    uint8_t Version;
    is.read(reinterpret_cast<char*>(&Version), 1);
    if(Version != 0x05){
        return false;
    }
    cont_header++;
    
    uint8_t Encoding;
    is.read(reinterpret_cast<char*>(&Encoding), 1);
    if(Encoding != 0x01){
        return false;
    }
    cont_header++;
    
    uint8_t BitsPerPlane;
    is.read(reinterpret_cast<char*>(&BitsPerPlane), 1);
    cont_header++;
    
    uint16_t WindowXmin, WindowYmin, WindowXmax, WindowYmax;
    is.read(reinterpret_cast<char*>(&WindowXmin), 2);
    is.read(reinterpret_cast<char*>(&WindowYmin), 2);
    is.read(reinterpret_cast<char*>(&WindowXmax), 2);
    is.read(reinterpret_cast<char*>(&WindowYmax), 2);
    cont_header += 8;
    
    for(size_t i = 0; i< 53; i++){
        is.get();
        cont_header++;
    }
    
    uint8_t ColorPlanes;
    is.read(reinterpret_cast<char*>(&ColorPlanes), 1);
    cont_header++;
    
    uint16_t BytesPerPlaneLine;
    is.read(reinterpret_cast<char*>(&BytesPerPlaneLine), 2);
    cont_header+=2;
    
    for(size_t i = cont_header; i< 128; i++){
        is.get();
    }

    int Xsize = WindowXmax - WindowXmin + 1;
    int Ysize = WindowYmax - WindowYmin + 1;
    img.resize(Ysize, Xsize);
    
    int TotalBytes = ColorPlanes * BytesPerPlaneLine;
    int count = 0;
    int subtotal = 0;
    int byte_scritti = 0;
    
    auto it = img.begin();
    uint8_t buffer1;
    uint8_t buffer2;
    std::vector<uint8_t> line;
    
    bool line_fully = false;
    
    while(Ysize-- > 0){
        while(1){
            if(line_fully) break;
            is.read(reinterpret_cast<char*>(&buffer1), 1);
            if(((buffer1 & 192) >> 6) == 3){
                count = buffer1 & 63;
                //subtotal += buffer1 & 63;
                is.read(reinterpret_cast<char*>(&buffer2), 1);
            } else {
                count = 1;
                //subtotal += 1;
                buffer2 = buffer1;
            }
            for(int i = 0; i < count; i++){
                line.push_back(buffer2);
                subtotal++;
                if(subtotal == TotalBytes){
                    line_fully = true;
                    break;
                }
            }
        }
        subtotal = 0;
        line_fully = false;
            
        for(auto x: line){
            if(byte_scritti >= Xsize) break;
            for(int j = 128, z = 7; j > 0 ; j /= 2, z--){
                if(byte_scritti >= Xsize) break;
                uint8_t px = (x & j) >> z;
                if(px == 0){
                    (*it) = 0;
                } else {
                    (*it) = 255;
                }
                it++;
                byte_scritti++;
            }
        }
        byte_scritti = 0;
        line.clear();
    }
    
    
    
    return true;
}

bool load_pcx(const std::string& filename, mat<vec3b>& img){
    std::ifstream is(filename, std::ios::binary);
    if(!is){
        std::cerr << "Cannot open file.\n";
        return false;
    }
    
    int cont_header = 0;
    
    uint8_t Manufacturer;
    is.read(reinterpret_cast<char*>(&Manufacturer), 1);
    if(Manufacturer != 0x0A){
        return false;
    }
    cont_header++;
    
    uint8_t Version;
    is.read(reinterpret_cast<char*>(&Version), 1);
    if(Version != 0x05){
        return false;
    }
    cont_header++;
    
    uint8_t Encoding;
    is.read(reinterpret_cast<char*>(&Encoding), 1);
    if(Encoding != 0x01){
        return false;
    }
    cont_header++;
    
    uint8_t BitsPerPlane;
    is.read(reinterpret_cast<char*>(&BitsPerPlane), 1);
    cont_header++;
    
    uint16_t WindowXmin, WindowYmin, WindowXmax, WindowYmax;
    is.read(reinterpret_cast<char*>(&WindowXmin), 2);
    is.read(reinterpret_cast<char*>(&WindowYmin), 2);
    is.read(reinterpret_cast<char*>(&WindowXmax), 2);
    is.read(reinterpret_cast<char*>(&WindowYmax), 2);
    cont_header += 8;
    
    for(size_t i = 0; i< 53; i++){
        is.get();
        cont_header++;
    }
    
    uint8_t ColorPlanes;
    is.read(reinterpret_cast<char*>(&ColorPlanes), 1);
    cont_header++;
    
    uint16_t BytesPerPlaneLine;
    is.read(reinterpret_cast<char*>(&BytesPerPlaneLine), 2);
    cont_header+=2;
    
    for(size_t i = cont_header; i< 128; i++){
        is.get();
    }

    int Xsize = WindowXmax - WindowXmin + 1;
    int Ysize = WindowYmax - WindowYmin + 1;
    img.resize(Ysize, Xsize);
    
    int TotalBytes = ColorPlanes * BytesPerPlaneLine;
    int count = 0;
    int subtotal = 0;
    int byte_scritti = 0;
    
    auto it = img.begin();
    uint8_t buffer1;
    uint8_t buffer2;
    std::vector<uint8_t> lineR;
    std::vector<uint8_t> lineG;
    std::vector<uint8_t> lineB;
    
    bool line_fully = false;
    
    while(Ysize-- > 0){
        while(1){
            if(line_fully) break;
            is.read(reinterpret_cast<char*>(&buffer1), 1);
            if(((buffer1 & 192) >> 6) == 3){
                count = buffer1 & 63;
                //subtotal += buffer1 & 63;
                is.read(reinterpret_cast<char*>(&buffer2), 1);
            } else {
                count = 1;
                //subtotal += 1;
                buffer2 = buffer1;
            }
            for(int i = 0; i < count; i++){
                if(subtotal < BytesPerPlaneLine){
                    lineR.push_back(buffer2);
                    subtotal++;
                    if(subtotal == TotalBytes){
                        line_fully = true;
                        break;
                    }
                }
                if((subtotal < BytesPerPlaneLine*2) && (subtotal >= BytesPerPlaneLine)){
                    lineG.push_back(buffer2);
                    subtotal++;
                    if(subtotal == TotalBytes){
                        line_fully = true;
                        break;
                    }
                }
                if((subtotal < BytesPerPlaneLine*3) && (subtotal >= BytesPerPlaneLine*2)){
                    lineB.push_back(buffer2);
                    subtotal++;
                    if(subtotal == TotalBytes){
                        line_fully = true;
                        break;
                    }
                }
            }
        }
        subtotal = 0;
        line_fully = false;
        
        auto g = lineG.begin();
        auto b = lineB.begin();
        for(auto r: lineR){
            if(byte_scritti >= Xsize) break;
            
            (*it)[0] = r;
            (*it)[1] = *g;
            (*it)[2] = *b;
            
            it++;
            g++;
            b++;
            byte_scritti++;
    
        }
        byte_scritti = 0;
        lineR.clear();
        lineG.clear();
        lineB.clear();
    }
    
    
    
    return true;
}
