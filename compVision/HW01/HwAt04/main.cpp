#include <iostream>
#include "image.hpp"

int main (int argc, const char * argv[]) {
    // insert code here...
    cout << "Hello, World!\n";
    CImage image ("noGood.tif");
    image.save ("output.pgm");
    return 0;
}
