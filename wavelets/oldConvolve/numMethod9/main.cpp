#include <iostream>
#include "xform.hpp"
#include "haarGen.hpp"
#include "plotResults.hpp"
#include "myVector.h"

int main (int argc, const char * argv[]) {
    // insert code here...
    xform test;
    haarGen haar;
    plotResults out;
    myVector x;
    haar.genSample(128, x);
    
    myVector f;
    myVector X;
    cout << "Test to begin" << endl;
    test.hWave(x, f);
    cout << "Test haar wave complete " << endl;
    test.hWaveInverse (f, X);
    std::cout << "Hello, World!\n";
	out.plotVal ( "sample.out", x);
    out.plotVal ( "haar.out", f);
    out.plotVal ( "recovered.out" ,X);
    
    return 0;
}
