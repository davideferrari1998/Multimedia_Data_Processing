//
//  main.cpp
//  Y4M
//
//  Created by Matteo Ferrari on 13/07/21.
//

#include <iostream>
#include "y4m_color.h"

int main(int argc, const char * argv[]) {
    
    std::vector<mat<vec3b>> frames;
    y4m_extract_color("test1.y4m", frames);
    std::cout << frames.size();
    
    return 0;
}
