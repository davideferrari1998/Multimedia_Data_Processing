//
//  y4m_color.cpp
//  Y4M_color
//
//  Created by Matteo Ferrari on 18/07/21.
//

#include <stdio.h>
//
//  y4m_color.cpp
//  Y4M
//
//  Created by Matteo Ferrari on 18/07/21.
//

#include <iostream>
#include <fstream>
#include "y4m_color.h"
#include "types.h"
#include <string>
#include <vector>
#include "mat.h"

uint8_t squeeze(float value){
    if(value < 0) return 0;
    else if(value > 255) return 255;
    else return (uint8_t)value;
}

int computeChrominancePxSize(int sizeY){
    //4:2:0
    return sizeY/4*2;
}

bool y4m_extract_color(const std::string& filename, std::vector<mat<vec3b>>& frames){
    
    std::ifstream is(filename, std::ios::binary);
    if(!is) {
        std::cout << "cannot open input file\n.";
        return false;
    }
    
    std::string steam_header;
    is >> steam_header;
    if(steam_header.compare("YUV4MPEG2") != 0){
        return false;
    }
    
    std::string tagged_field = "";
    std::string frame_header = "FRAME";
    
    mat<vec3b> frame;
    int cols = 0;
    int rows = 0;
    
    while(1){
        while(1){
            
            if(is.peek() == EOF){
                return true;
            }
            
            is >> tagged_field;
            if(tagged_field.compare(frame_header) == 0) break;
            
            
            if(tagged_field[0] == 'W'){
                cols = std::stoi(&tagged_field[1]);
            }
            
            
            if(tagged_field[0] == 'H'){
                rows = std::stoi(&tagged_field[1]);
            }
            
            if(tagged_field[0] == 'C'){
                if(tagged_field.compare("C420jpeg") != 0){
                    return false;
                }
            }
        }
        
        frame.resize(rows, cols);
        
        //0x0A non lo voglio nel vettore dati
        is.get();
        
        std::vector<vec3b> pxs;
        for(size_t i = 0; i < frame.rows()*frame.cols(); i++){
            char pxY;
            is.get(pxY);
            uint8_t y = pxY;
            if(y < 16) y = 16;
            if(y > 235) y = 235;
            vec3b ycbcr = {y, 0, 0};
            pxs.push_back(ycbcr);
        }
        
        //read Cb e Cr
        //rawsize potrebbe dare problemi se si cambia il tipo di dato
        size_t chromaSize = computeChrominancePxSize(frame.rows()*frame.cols());
        
        //Cb
        for(size_t i = 0; i < chromaSize/2; i++){
            char chroma;
            is.get(chroma);
            uint8_t cb = chroma;
            if(cb < 16) cb = 16;
            if(cb > 240) cb = 240;
            for(size_t j = 0; j < 4; j++){
                pxs[i*4+j][1] = cb;
            }
        }
        //Cr
        for(size_t i = 0; i < chromaSize/2; i++){
            char chroma;
            is.get(chroma);
            uint8_t cr = chroma;
            if(cr < 16) cr = 16;
            if(cr > 240) cr = 240;
            for(size_t j = 0; j < 4; j++){
                pxs[i*4+j][2] = cr;
            }
        }
        
        //RGB conversion
        for(auto it = pxs.begin(); it != pxs.end(); it++){
            short Y = (*it)[0];
            Y+=-16;
            short cb2 = ((*it)[1]);
            cb2+=-128;
            short cr2 = (*it)[2]-128;
            float R = (float)Y*1.164 + (float)cr2*1.596 ;
            float G = (float)Y*1.164 - (float)cb2*0.392 - (float)cr2*0.813 ;
            float B = (float)Y*1.164 + (float)cb2*2.017 ;
            (*it)[0] = squeeze(R);
            (*it)[1] = squeeze(G);
            (*it)[2] = squeeze(B);
            
        }
        
        char* ptr = frame.rawdata();
        for(size_t i = 0, j = 0; i < pxs.size(); i++, j=j+3){
            ptr[j] = pxs[i][0];
            ptr[j+1] = pxs[i][1];
            ptr[j+2] = pxs[i][2];
        }
        
        
        
        pxs.clear();
        
        frames.push_back(frame);
    }
    
    return true;
}

