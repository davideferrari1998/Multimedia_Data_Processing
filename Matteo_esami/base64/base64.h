//
//  base64.h
//  base64
//
//  Created by Matteo Ferrari on 09/07/21.
//

#ifndef base64_h
#define base64_h

#include <string>
#include <map>
#include <cstdint>

class base64 {
public:
    std::string input;
    std::string strd;
    std::map<char, uint8_t> code;
    
    base64(std::string input){
        this->input = input;
        this->strd = "";
        construct();
    }
    
    void construct(){
        uint8_t codice = 0;
        for(char i = 'A'; i <= 'Z'; i++){
            code.insert(std::pair<char, uint8_t>(i, codice));
            codice++;
        }
        for(char i = 'a'; i <= 'z'; i++){
            code.insert(std::pair<char, uint8_t>(i, codice));
            codice++;
        }
        for(char i = '0'; i <= '9'; i++){
            code.insert(std::pair<char, uint8_t>(i, codice));
            codice++;
        }
        code.insert(std::pair<char, uint8_t>('+', 62));
        code.insert(std::pair<char, uint8_t>('/', 63));
    }
    
    void decode(){
        for(size_t i = 0; i < input.size(); i+=4){
            char value1 = (code[input.at(i)] << 2) | (code[input.at((i)+1)] << 2 >> 6);
            char value2 = (code[input.at((i)+1)] << 4) | (code[input.at((i)+2)] << 2 >> 4);
            char value3 = (code[input.at((i)+2)] << 6) | (code[input.at((i)+3)]);
            if(value1 != '\0') strd += value1;
            if(value2 != '\0') strd += value2;
            if(value3 != '\0') strd += value3;
        }
    }
    
};

std::string base64_decode(const std::string& input);

#endif /* base64_h */
