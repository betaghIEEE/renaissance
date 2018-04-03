#include "xformqtree.hpp"
#include <iostream.h>
#include <math.h>
#include "xform.hpp"
#include "xform2D.hpp"
#include "haarGen.hpp"
//#include "plotResults.hpp"
#include "myVector.h"
#include "matrixType.h"
#include "imageio.hpp"


int main ()
{
	matrixType pic, xpic, rpic;
	xformquadtree xf;
        double renergy, rcenergy, senergy, eenergy;
	matrixType sourceA;
	matrixType sourceB;
	matrixType sourceC; 
    	matrixType resultA; 
    	matrixType resultB; 
    	matrixType result;
	matrixType recover;
        matrixType error;
	imageIO picio;
	int row, col;
        int resolution = 7;
	cout << "\n At resolution " << resolution << "\n";
	const char* A= "pgmclassics/peppers.pgm";
	const char* B = "pgmclassics/peppers.pgm";
        const char* AX = "example2.pgm";
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
	result.allocateStorage (row,col);
	recover.allocateStorage (row,col);
	double sqhalf = pow (0.5, 0.5);
        
        cout << "Before self wx form \n";
        picio.saveImagePGM2 (AX, sourceA);
	xf.selfWXform (sourceA, resultA,resolution); 
	//picio.saveImagePGM2u(AWX,resultA);
        picio.saveImagePGM2(AWX,resultA);
	
	
	 xf.selfWXform (sourceB, resultB,resolution); 
	//picio.getImagePGM2 (AWX, resultA);
	//picio.getImagePGM2 (AWX, resultB); 
	result = resultA * resultB;
         //recover.allocateStorage (result.m_rows, result.m_cols);
         sourceC = sourceA * sourceB;
         senergy = sourceC.getEnergy();
         renergy = result.getEnergy();
         cout << "\n Energy of the standard " << senergy;
         cout << "\n Energy of the approximation " << renergy;
         cout << "\n Percent difference " << (senergy - renergy) / senergy;
	//picio.saveImagePGM2u(  out, sourceC);
	//picio.saveImagePGM2u(  out2, result);
	picio.saveImagePGM2(  out, sourceC);
	picio.saveImagePGM2(  out2, result);
        cout << " Saved source and result\n ";
	// pgm.saveImagePGM2 ("test", xpic);
        
        xf.selfWIXform (result, recover,resolution);
	//picio.saveImagePGM2u (recovPic, recover);
	picio.saveImagePGM2 (recovPic, recover);
        rcenergy = recover.getEnergy();
        error = recover - sourceC;
        eenergy = pow(error.getEnergy(), 0.5);
        cout << "\n Energy of the standard " << senergy;
        cout << "\n Energy of the approximation " << rcenergy;
        cout << "\n Percent difference " << (senergy - rcenergy) / senergy;
        cout << "\n Fidelity " << eenergy ;
        cout << "\n";
        //pgm.saveImagePGM2 ("recover", rpic); 
	return 0;
}
