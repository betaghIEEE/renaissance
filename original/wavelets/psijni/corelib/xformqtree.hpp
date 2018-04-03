
#include "matrixType.h"
#include "quadtree.h"
#include "myVector.h"
#include "quadnode.hpp"

#ifndef __XFORM_QUAD_TREE__
#define __XFORM_QUAD_TREE__



class xformquadtree
{
private:
    matrixType S;
    matrixType T;
    quadtree *head;
    quadtree *current;
    qtreearray *qtree;
    aueuearray *queue;
    myVector ha;
    myVector hd;
public:
        xformquadtree ();
        xformquadtree (xformquadtree *axform);
        void setquadtree(quadnode *start);
        void getquadtree(quadnode *start);
        void setmatrix (matrixType &A);
        void getmatrix (matrixType &A);
        
        
        void selfRowWXform ();
        void selfColWXform ();
        
        void selfRowWXform (int i, int col);
        void selfColWXform (int j, int row);
        void selfColWXform (int i, int col, myVector &ha, myVector &hd);
        void selfWXform (matrixType &source, matrixType &result);
        
	void selfRowWaveletXformInverse (matrixType &source, matrixType &result, int i);
	void selfColWaveletXformInverse (matrixType &source, matrixType &result, int j);
	void selfWaveletXformInverse (matrixType &source, matrixType &result);
        
        void selfRowWaveletXformInverse (matrixType &W, int col, int i);
	void selfColWaveletXformInverse (matrixType &W, int row, int j);
	void selfWaveletXformInverse (matrixType &source, matrixType &result, int res);
        
	bool epsilon ( double x, int th);
        bool epsilon ( double x, double thresh);
        
};

#endif __XFORM_QUAD_TREE__