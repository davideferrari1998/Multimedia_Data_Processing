//
//  json.cpp
//  7-PPM_load
//
//  Created by Matteo Ferrari on 20/07/21.
//
//#include "json.h"
//#include "process_ppm.h"
//#include "compress.h"

#include <fstream>
#include <iostream>
#include "ppm.h"
#include <string>
#include "mat.h"


void PackBitsEncodeEE(const mat<uint8_t>& img, std::vector<uint8_t>& encoded){
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

std::string Base64EncodeEE(const std::vector<uint8_t>& v){
    
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


std::string JSON(const std::string& filename){
    
    mat<vec3b> img;
    std::ifstream is(filename, std::ios::binary);
    if(!is){
        std::cerr << "Cannot open file.\n";
        return "{}";
    }
    
    std::string magic_number;
    is >> magic_number;
    if(magic_number.compare("P6") != 0) return "";
    
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
    if((maxval <= 0) || (maxval >= 65536)) return "{}";
    
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
    
    mat<uint8_t> img_r;
    mat<uint8_t> img_g;
    mat<uint8_t> img_b;
    
    if(img.size() == 0) return "{}";
    
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
    
    std::vector<uint8_t> encode_r;
    std::vector<uint8_t> encode_g;
    std::vector<uint8_t> encode_b;
    PackBitsEncodeEE(img_r, encode_r);
    PackBitsEncodeEE(img_g, encode_g);
    PackBitsEncodeEE(img_b, encode_b);
    
    std::string str_r = Base64EncodeEE(encode_r);
    std::string str_g = Base64EncodeEE(encode_g);
    std::string str_b = Base64EncodeEE(encode_b);
    
    std::string output = "{\n\t\"width\": " + std::to_string(img.cols()) + ",\n";
    output += "\t\"height\": " + std::to_string(img.rows()) + ",\n";
    output += "\t\"red\": \"" + str_r + "\",\n";
    output += "\t\"green\": \"" + str_g + "\",\n";
    output += "\t\"blue\": \"" + str_b + "\"\n";
    output += "}";
    
    return output;
}
