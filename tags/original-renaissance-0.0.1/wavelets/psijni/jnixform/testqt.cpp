#include "xformqtree.hpp"
#include "imageio.hpp"
#include "matrixType.h"
#include <iostream.h>
#include <math.H>


int main ()
{
	matrixType pic, xpic, rpic, epic;
	xformquadtree xf;
	imageIO pgm;
        int resolution = 2;
	pgm.getImagePGM2 ("sandra_512.pgm" , pic);
        pgm.saveImagePGM2 ("sandra", pic);
        cout << "Before self wx form \n";
	xf.selfPWXform (pic, xpic, resolution); 
	pgm.saveImagePGM2 ("test", xpic);
        
        xf.selfPWIXform (xpic, rpic, resolution);
        pgm.saveImagePGM2 ("recover", rpic);
        epic = rpic - pic;
        pgm.saveImagePGM2 ("error", epic);
        cout << "\n Fidelity " << pow (epic.getEnergy(), 0.5);
        cout << "\n";
	return 0;
}
