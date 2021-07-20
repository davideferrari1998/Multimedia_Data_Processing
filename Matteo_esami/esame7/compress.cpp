//
//  compress.cpp
//  7-PPM_load
//
//  Created by Matteo Ferrari on 19/07/21.
//

//#include "compress.h"
#include "mat.h"
#include <vector>
#include <string>

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

void findCode(std::vector<uint8_t>& b, std::string& str_base64, std::vector<char>& table64){
    char c1 = (b.at(0) & 0xfc) >> 2;
    char c2 = ((b.at(0) & 0x03) << 4) | ((b.at(1) & 0xf0) >> 4);
    char c3 = ((b.at(1) & 0x0f) << 2) | ((b.at(2) & 0xc0) >> 6);
    char c4 = b.at(2) & 0x3f;
    str_base64 += table64.at(c1);
    str_base64 += table64.at(c2);
    str_base64 += table64.at(c3);
    str_base64 += table64.at(c4);
    b.clear();
}

std::string Base64Encode(const std::vector<uint8_t>& v){
    
    if(v.size() == 0){
        return "";
    }
    
    std::vector<char> table64;
    char c = 'A';
    for(int i = 0; i < 64; i++){
        if(c == 'Z'){
            table64.push_back(c);
            c = 'a';
            continue;
        }
        if(c == 'z'){
            table64.push_back(c);
            c = '0';
            continue;
        }
        if(c == '9'){
            table64.push_back(c);
            table64.push_back('+');
            table64.push_back('/');
            break;
        }
        table64.push_back(c);
        c++;
    }
    
    std::string str_base64 = "";
    
    std::vector<uint8_t> b;
    for(auto val: v){
        
        if(b.size() == 3){
            findCode(b, str_base64, table64);
        }
        b.push_back(val);
    }
    
    if(b.size() != 0){
        while(b.size() < 3){
            b.push_back(128);
        }
        findCode(b, str_base64, table64);
    }
    
    return str_base64;
}
