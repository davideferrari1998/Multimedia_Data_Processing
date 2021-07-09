//
//  main.cpp
//  base64
//
//  Created by Matteo Ferrari on 09/07/21.
//

#include <iostream>
#include "base64.h"

int main(int argc, const char * argv[]) {
    
    std::string ciao = "YWJj";
    std:: string str = base64_decode(ciao);
    
    return 0;
}
