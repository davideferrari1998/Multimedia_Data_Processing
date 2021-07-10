//
//  main.cpp
//  lzs
//
//  Created by Matteo Ferrari on 09/07/21.
//

#include "lzs.h"

int main(int argc, const char * argv[]) {
    
    using namespace std;
    
    ifstream is(argv[1], ios::binary);
    ofstream os(argv[2]);
    
    lzs_decompress(is, os);
    
    return 0;
}
