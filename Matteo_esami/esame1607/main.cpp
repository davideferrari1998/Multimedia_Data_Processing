//
//  main.cpp
//  esame1607
//
//  Created by Matteo Ferrari on 23/07/21.
//

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

template<typename T>
std::istream& raw_read(std::istream& is, T& num, size_t size=sizeof(T)){
    return is.read(reinterpret_cast<char*>(&num), size);
}

void load_preamble(std::istream& is, int& preamble){
    
    preamble = 0;
    uint8_t data;
    uint8_t first_bit;
    int cont = 0;
    while(1){
        raw_read(is, data);
        first_bit = data & 0x80;
        data = data & 0x7F;
        preamble = preamble | (data << (7*cont));
        cont++;
        if(first_bit == 0x00) break;
    }
}

bool snappy_decomp(std::istream& is, std::ostream& os){
    
    int preamble;
    load_preamble(is, preamble);
    int subtotal = 0;
    
    uint8_t byte_tag = 0;
    uint8_t type = 0;
    uint8_t mosts_bits = 0;
    uint32_t length = 0;
    int offset = 0;
    std::vector<uint8_t> dict;
    
    //subtotal < preamble
    while(raw_read(is, byte_tag)){
        //raw_read(is, byte_tag);
        type = byte_tag & 0x03;
        mosts_bits = (byte_tag & 0xFC) >> 2;
        
        //literals
        if(type == 0x00){
            if(mosts_bits < 60){
                length = mosts_bits + 1;
            } else if(mosts_bits == 60){
                raw_read(is, length, 1);
                length++;
            } else if(mosts_bits == 61){
                raw_read(is, length, 2);
                length++;
            } else if(mosts_bits == 62){
                raw_read(is, length, 3);
                length++;
            } else if(mosts_bits == 63){
                raw_read(is, length, 4);
                length++;
            }
            
            for(size_t i = 0; i < length; i++){
                uint8_t c;
                raw_read(is, c);
                dict.push_back(c);
                os.write(reinterpret_cast<char*>(&c), 1);
                subtotal++;
            }
        //copy
        } else {
            if(type == 1){
                length = (mosts_bits & 0x07);
                length += 4;
                offset = (mosts_bits & 0x38) << 5;
                uint8_t next;
                raw_read(is, next);
                offset |= next;
            }
            if(type == 2){
                length = mosts_bits +1;
                raw_read(is, offset, 2);
                
            }
            if(type == 3){
                length = mosts_bits +1;
                raw_read(is, offset);
            }
            
            size_t size = dict.size();
            size_t index = dict.size() - offset;
            for(size_t i = 0; i < length; i++){
                if(index == size){
                    index = size - offset;
                }
                dict.push_back(dict.at(index));
                os.write(reinterpret_cast<char*>(&(dict.at(index))), 1);
                index++;
                subtotal++;
            }
        }
    }
    
    return true;
}

int main(int argc, const char * argv[]) {
    
    if(argc != 3){
        std::cerr << "snappy_decomp <input file> <output file>\n";
        return EXIT_FAILURE;
    }
    
    std::ifstream is(argv[1], std::ios::binary);
    if(!is){
        std::cerr << "cannot open input file.\n";
        return EXIT_FAILURE;
    }
    
    std::ofstream os(argv[2], std::ios::binary);
    if(!os){
        std::cerr << "cannot open output file.\n";
        return EXIT_FAILURE;
    }
    
    bool result = snappy_decomp(is, os);
    
    if(result == true) return EXIT_SUCCESS;
    else return EXIT_FAILURE;
}
