//
//  compress.cpp
//  7-PPM_load
//
//  Created by Matteo Ferrari on 19/07/21.
//

//#include "compress.h"
#include "mat.h"
#include <vector>

void PackBitsEncode(const mat<uint8_t>& img, std::vector<uint8_t>& encoded){
    if(img.size() == 0){
        encoded.push_back(128);
        return;
    }
    
    int cont_eq = 0;
    std::vector<uint8_t> literals;
    bool first = true;
    bool run = false;
    bool copy = true;
    
    for(auto it = img.begin(); it != img.end(); it++){
        
        if(first){
            first = false;
            literals.push_back(*it);
            continue;
        }
        
        if(literals.back() != *it){
            
            copy = false;
            if(run) {
                encoded.push_back(257-(cont_eq+1));
                encoded.push_back(literals.back());
                literals.clear();
                cont_eq = 0;
                run = false;
            }
            literals.push_back(*it);
        
        } else {
            
            if(!copy){
                encoded.push_back(literals.size()-1);
                for(auto x: literals) encoded.push_back(x);
                literals.clear();
                literals.push_back(*it);
                copy = true;
                continue;
            }
            
            cont_eq++;
            run = true;
            
        }
        
        
    }
    
    if(literals.size()>1){
        encoded.push_back(literals.size()-1);
        for(auto x: literals){
            encoded.push_back(x);
        }
        literals.clear();
    }
    
    if(cont_eq > 0){
        encoded.push_back(257-(cont_eq+1));
        encoded.push_back(literals.back());
    }
    
    
    encoded.push_back(128);
}

