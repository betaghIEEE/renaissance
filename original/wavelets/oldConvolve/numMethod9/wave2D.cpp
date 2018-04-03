#include <iostream>
#include "xform.hpp"
#include "xform2D.hpp"
#include "haarGen.hpp"
#include "plotResults.hpp"
#include "myVector.h"
#include "matrixType.h"
#include "imageio.hpp"

int main (int argc, const char * argv[]) {
// insert code here...
    	wave2comp test;
//haarGen haar;
	//plotResults out;
	matrixType source; 
    	matrixType result;
	imageIO picio;
	const char* picture= "rightDan.pgm";
	const char* out = "wavepic.ppm";
	picio.getImagePGM2 ( picture, source);
	test.waveletXform(source, result);   
	picio.saveImagePGM2(  out, result);
    
    //test.hWave(x, f, 4);
    std::cout << "Hello, World!\n";

    
    return 0;
}
