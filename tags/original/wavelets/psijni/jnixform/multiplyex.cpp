#include <iostream.h>
#include <math.h>
#include "xform.hpp"
#include "xform2D.hpp"
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
	matrixType sourceA;
	matrixType sourceB;
	matrixType sourceC; 
    	matrixType resultAr; 
    	matrixType resultBr; 
    	matrixType resultA; 
    	matrixType resultB; 
    	matrixType result;
	matrixType recover;
	imageIO picio;
	int row, col;
        int i ,j;
	const char* A= "example.pgm";;
	const char* B = "example.pgm";
	const char* AWX = "exWX.pgm";
	const char* out = "mulxf.ppm";
	const char* out2 = "mulxfWX.ppm";
	const char* recovPic = "result.ppm";
	picio.getImagePGM2 ( A, sourceA);
	picio.getImagePGM2 ( B, sourceB);
	row = sourceA.m_rows;
	col = sourceA.m_cols;
	resultA.allocateStorage (row,col);
	resultB.allocateStorage (row,col);
	resultAr.allocateStorage (row,col);
	resultBr.allocateStorage (row,col);
	recover.allocateStorage (row,col);
	sourceC = sourceA * sourceB;
	double sqhalf = pow (0.5, 0.5);
	
        cout << "Begining X form" << endl ;
	test.hA.allocate(2);
	test.hD.allocate(2);
	test.hA[0] = test.hD[0] = test.hA[1] = sqhalf;
	test.hD[1] = -sqhalf;
	test.selfRowWXform(sourceA, resultAr, test.hA, test.hD); 
	test.selfColWXform(resultAr, resultA, test.hA, test.hD); 
	picio.saveImagePGM2(AWX,resultA);
	test.selfRowWXform(sourceB, resultBr, test.hA, test.hD); 
	test.selfColWXform(resultBr, resultBr, test.hA, test.hD); 
	result = resultA * resultB;   
	picio.saveImagePGM2(  out, sourceC);
	picio.saveImagePGM2(  out2, result);
        row = result.m_rows;
        col = result.m_cols;
        for ( i = 0; i < row; i++) 
            test.selfColWaveletXformInverse ( result, recover, i);
        for ( i = 0; i < col; i++) 
            test.selfRowWaveletXformInverse ( result, recover, i);
	picio.saveImagePGM2 (recovPic, recover);
    
    //test.hWave(x, f, 4);
    std::cout << "Hello, World!\n";

    
    return 0;
}
