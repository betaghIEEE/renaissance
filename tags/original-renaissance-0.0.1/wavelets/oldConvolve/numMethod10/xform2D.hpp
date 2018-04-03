#ifndef __WAVE2DCOMP_H__
#define __WAVE2DCOMP_H__
#include "matrixType.h"
class wave2comp
{
	public:
	void rowWaveletXform ( matrixType &source, matrixType &result);
	void colWaveletXform ( matrixType &source, matrixType &result);
	void waveletXform (matrixType &source, matrixType &result);
	void filterThresh ( matrixType &input, matrixType &output, int thresh);
	void rowWaveletXformInverse (matrixType &source, matrixType &result);
	void colWaveletXformInverse (matrixType &source, matrixType &result);
	void waveletXformInverse (matrixType &source, matrixType &result);
	bool epsilon ( double x);
};


#endif
