#include <iostream.h>
#include "xform.hpp"
#include "xform2D.hpp"
#include "xformqtree.hpp"
#include "haarGen.hpp"
//#include "plotResults.hpp"
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
	matrixType recover;
	imageIO picio;
	const char* picture= "swalla10.pgm";
	const char* out = "wavepic.ppm";
	const char* recovPic = "recoverPy.ppm";
	picio.getImagePGM2 ( picture, source);
        cout << "Begining X form" << endl ;
	test.selfWXform(source, result);   
	picio.saveImagePGM2(  out, result);
	test.selfWaveletXformInverse ( result, recover);
	picio.saveImagePGM2 (recovPic, recover);
    
    //test.hWave(x, f, 4);
    std::cout << "Hello, World!\n";

    
    return 0;
}
