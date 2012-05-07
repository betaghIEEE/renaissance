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
	out.plotVal ( "sample.out", x);
    test.hWave(x, f);
    cout << "Test haar wave complete " << endl;
    test.hWaveInverseO (f, X);
    //std::cout << "Hello, World!\n";
	
	
	
    out.plotVal ( "haar.out", f);
	cout << "Haar out";
    out.plotVal ( "recovered.out" ,X);
    
    return 0;
}
