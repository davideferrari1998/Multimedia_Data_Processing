//
//  main.cpp
//  Y4M
//
//  Created by Matteo Ferrari on 13/07/21.
//

#include <iostream>
#include "y4m_gray.h"

int main(int argc, const char * argv[]) {
    
    std::vector<mat<uint8_t>> frames;
    y4m_extract_gray("test1.y4m", frames);
    
    return 0;
}
