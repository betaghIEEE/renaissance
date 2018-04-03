/*
	Class Image I/O by Dan Beatty
	Purpose: To read and write operations on various image types.
		The basic type data stucture is the matrixType which 
		is generic.  
*/

#ifndef __IMAGEIO_HPP__
#define __IMAGEIO_HPP__
#include "matrixType.h"

class imageIO 
{
	public:
	//void getImagePGM (const char* filename, matrixType &output);
	//void saveImagePGM (const char* filename, matrixType &output);
	void getImagePGM2 (const char* filename, matrixType &output);
    bool saveImagePGM2 (const char * filename, matrixType &input );
    bool saveImagePGM2u (const char * filename, matrixType &input );
	int charToInt (char temp );
};

#endif
