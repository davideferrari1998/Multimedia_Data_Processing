//
//  base64.cpp
//  base64
//
//  Created by Matteo Ferrari on 09/07/21.
//

#include "base64.h"

std::string base64_decode(const std::string& input){
    
    if(input.empty()){
        return "";
    }
    
    base64 base64_map(input);
    base64_map.decode();
    
    
    return base64_map.strd;
}
