#ifndef __WAVE2DCOMP_H__
#define __WAVE2DCOMP_H__
#include "matrixType.h"
#include "myVector.h"

class wave2comp
{
        private:
            myVector hA;
            myVector hD;
            myVector xA;
            myVector xD;
            myVector yA;
            myVector yD;
            myVector sX;
            myVector sY;
	public:
	void rowWaveletXform ( matrixType &source, matrixType &result);
	void colWaveletXform ( matrixType &source, matrixType &result);
	void waveletXform (matrixType &source, matrixType &result);
	void filterThresh ( matrixType &input, matrixType &output, int thresh);
	void filterThresh ( matrixType &input, matrixType &output, double thresh);
        
	void rowWaveletXformInverse (matrixType &source, matrixType &result);
	void colWaveletXformInverse (matrixType &source, matrixType &result);
	void waveletXformInverse (matrixType &source, matrixType &result);
        
	void selfRowWaveletXformInverse (matrixType &source, matrixType &result, int i);
	void selfColWaveletXformInverse (matrixType &source, matrixType &result, int j);
	void selfWaveletXformInverse (matrixType &source, matrixType &result);
        
        void selfRowWaveletXformInverse (matrixType &W, int col, int i);
	void selfColWaveletXformInverse (matrixType &W, int row, int j);
	void selfWaveletXformInverse (matrixType &source, matrixType &result, int res);
        
        
        void selfRowWXform (matrixType &source, matrixType &result, myVector &ha, myVector &hd);
        
        void selfRowWXform (int i, int col, matrixType &W);
        void selfColWXform (int j, int row, matrixType &W);
        void selfColWXform (matrixType &source, matrixType &result, myVector &ha, myVector &hd);
        void selfWXform (matrixType &source, matrixType &result);
        void selfWXform (matrixType &source, matrixType &result, int res);
	bool epsilon ( double x, int th);
        bool epsilon ( double x, double thresh);
};


#endif
