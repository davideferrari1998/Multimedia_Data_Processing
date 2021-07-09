
#include "mat.h"
#include "pgm16.h"
#include <iostream>

//The function should open the PGM file specified by filename, read its data and store it in the 16 bit per pixel matrix. Moreover it should set maxvalue to the value stored in the file.

int main(int argc, const char * argv[]) {
    
    using namespace std;
    
    mat<uint16_t> img = mat<uint16_t>();
    uint16_t maxvalue;
    if(!load(argv[1], img, maxvalue)){
        cout << "Function failed to return true.\n";
        return EXIT_FAILURE;
    }
    
    
    return EXIT_SUCCESS;
}
