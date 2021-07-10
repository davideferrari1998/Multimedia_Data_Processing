//
//  lzs.cpp
//  lzs
//
//  Created by Matteo Ferrari on 09/07/21.
//

#include "lzs.h"


template<typename T> std::istream& raw_read(std::istream& is, T& num, size_t size){
    return is.read(reinterpret_cast<char*>(&num), size);
}

void write_code(uint32_t offset, uint32_t length, std::ostream& os, std::string& dict){
    if(length <= offset){
        os.write(&dict[dict.size()-offset], length);
        size_t len = dict.size()-offset+length;
        for(size_t i = dict.size()-offset; i < len; i++){
            dict.push_back(dict[i]);
        }
    } else {
        int cont = 0;
        size_t pos = dict.size()-offset;
        size_t len = dict.size();
        while(cont < length){
            while(pos < len){
                os.write(&dict[pos], 1);
                dict.push_back(dict[pos]);
                pos++;
                cont++;
                if(cont == length) return;
            }
            pos = len-offset;
        }
    }
}

void read_length(bitreader& br, uint32_t& length, std::ostream& os, std::string& dict, uint32_t& offset) {
    br.read(length, 2);
    if(length == 0){
        write_code(offset, 2, os, dict);
    }
    else if(length == 1){
        write_code(offset, 3, os, dict);
    }
    else if(length == 2){
        write_code(offset, 4, os, dict);
    }
    else if(length == 3){
        br.read(length, 2);
        if(length == 0){
            write_code(offset, 5, os, dict);
        }
        else if(length == 1){
            write_code(offset, 6, os, dict);
        }
        else if(length == 2){
            write_code(offset, 7, os, dict); //create func
        } else {
            int cont = 1;
            while(1){
                br.read(length, 4);
                cont++;
                if(length != 15){
                    cont--;
                    length = length + (cont*15-7);
                    write_code(offset, length, os, dict);
                    break;
                }
            }
        }
    }
}

void lzs_decompress(std::istream& is, std::ostream& os){
    
    bitreader br(is);
    uint32_t u, offset, length;
    std::string dict = "";
    while(1){
        br.read(u, 1);
        if(u == 1){ //offset length
            br.read(u, 1);
            if(u == 1){//offeset < 128
                br.read(offset, 7);
                if(offset == 0) return; //end marker
                else {
                    read_length(br, length, os, dict, offset);
                }
            } else { //offset > 128
                br.read(offset, 11);
                read_length(br, length, os, dict, offset);
            }
        } else { //literal
            br.read(u, 8);
            char c = u;
            dict.push_back(c);
            os << c;
        }
    }
    
    return;
}
