//
//  y4m_gray.cpp
//  Y4M
//
//  Created by Matteo Ferrari on 13/07/21.
//

//#include "y4m_gray.h"
#include <iostream>
#include <fstream>
#include "mat.h"

int computeChrominancePxSize(int sizeY){
    //4:2:0
    return sizeY/4*2;
}

bool y4m_extract_gray(const std::string& filename, std::vector<mat<uint8_t>>& frames){
    
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
    
    mat<uint8_t> frame;
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
        
        is.read(frame.rawdata(), frame.rawsize());
        
        //read Cb e Cr
        char c = 'c';
        //rawsize potrebbe dare problemi se si cambia il tipo di dato
        for(size_t i = 0; i< computeChrominancePxSize(frame.rawsize()); i++){
            is.get(c);
        }
        
        frames.push_back(frame);
    }
    
    return true;
}
