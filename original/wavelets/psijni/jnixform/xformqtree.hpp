
#include "matrixType.h"

#include "myVector.h"

#ifndef __XFORM_QUAD_TREE__
#define __XFORM_QUAD_TREE__
#include "qtreearray.hpp"
#include "matrixType.h"
#include "stackarray.hpp"
#include "quadnode.hpp"
#include "queuearray.hpp"
#include "permeasure.hpp"


class xformquadtree
{
private:
    performmeasure pm;
    matrixType S;
    matrixType T;
    qtreearray *qtree;
    queuearray *queue;
    stackarray *astack;
    myVector ha;
    myVector hd;
    myVector xTemp;
    myVector xTemp2;
    myVector yTemp;
    myVector yTemp2;
    double sqhalf;
    int resolution;
    
    bool epsilon ( double x, int th);
    bool epsilon ( double x, double thresh);

    int findX2 (int i);
    void getSstats (quadnode &r);
    void getNode (quadnode &r);
    void setAnode (quadnode &r, quadnode &a);
    void setVnode (quadnode &r, quadnode &a);
    void setHnode (quadnode &r, quadnode &a);
    void setDnode (quadnode &r, quadnode &a);
    
    
    
    void selfRowWXform (quadnode &r);
    void selfColWXform (quadnode &r);
    void haarRowWXform (quadnode &r);
    void haarColWXform (quadnode &r);
    void selfWXform ();
    void selfWXform (quadnode &r);
    void selfPWXform (quadnode &r);
    void haarWXform (quadnode &r);
    void selfRowWIXform (quadnode &r);
    void selfColWIXform (quadnode &r);
    bool selfWIXform (quadnode &r);
    bool selfPWIXform (quadnode &r);
    

    
public:
        xformquadtree ();
        xformquadtree (xformquadtree *axform);
        xformquadtree (qtreearray &sample);
        void setquadtree(qtreearray &start);
        void getquadtree(qtreearray &start);
        void setmatrix (matrixType &A);
        void getmatrix (matrixType &A);
        void setmatrix (quadnode &A);
        void getmatrix (quadnode &A);
        int gettotalinstructions () { return pm.totalinstructions;}
        int gettotaldatacompares () { return pm.totaldatacompares;}
        int gettotaldatatransfers () { return pm.totaldatatransfers;}
        
        
        

        
        void selfWXform (matrixType &source, matrixType &result);
        void selfWXform (matrixType &source, matrixType &result, int resol);
        void selfPWXform (matrixType &source, matrixType &result, int resol);
        void selfRepWXform (matrixType &source, matrixType &result, int resol);
        
        void selfWXform (matrixType &source, qtreearray &result);
        void haarWXform (matrixType &source, matrixType &result);
        
        void selfWIXform (matrixType &source, matrixType &result);
        void selfWIXform (matrixType &source, matrixType &result, int resol);
        void selfPWIXform (matrixType &source, matrixType &result, int resol);
        void selfRepWIXform (matrixType &source, matrixType &result, int resol);
        void selfWIXform (qtreearray &source, matrixType &result);
        

        
};

#endif __XFORM_QUAD_TREE__