/*
    Wavelet Transform Library for The psi^n MRE full decomposition
    scheme.  Libraries added or changed from X-Form-Quad-Tree:
        Tree Generation
        Estimation-Redundancy filter
        Packet Sorter - Wavelet Packet Order
        Packet Sorter - Psi^n Packet Order
        
    
*/


#include "matrixType.h"

#include "myVector.h"

#ifndef __XFORM_PSI_N__
#define __XFORM_PSI_N__
#include "qtreearray.hpp"
#include "matrixType.h"
#include "stackarray.hpp"
#include "quadnode.hpp"
#include "queuearray.hpp"
#include "permeasure.hpp"


class xformpsin
{
private:
    performmeasure pm;
    matrixType S;
    matrixType T;
    quadnode *qtree;
    double *histogram;
    int current;
    //queuearray *queue;
    //stackarray *astack;
    myVector ha;
    myVector hd;
    myVector xTemp;
    myVector xTemp2;
    myVector yTemp;
    myVector yTemp2;
    double sqhalf;
    int resolution;
    int treesize;
    int histosize;
    int aval, vval, hval, dval;
    
    bool epsilon ( double x, int th);
    bool epsilon ( double x, double thresh);

    int findX2 (int i);
    void getSstats (quadnode &r);
    void patchEnergy ();
    void generateHistogram ();
    void getNode (quadnode &r);
    void zeroize (quadnode &r);
    /*
    void setAnode (quadnode &r, quadnode &a);
    void setVnode (quadnode &r, quadnode &a);
    void setHnode (quadnode &r, quadnode &a);
    void setDnode (quadnode &r, quadnode &a);
    */
    
    
    void selfRowWXform (quadnode &r);
    void selfColWXform (quadnode &r);
    //void selfWXform ();
    void selfWXform (quadnode &r);
    void selfRowWIXform (quadnode &r);
    void selfColWIXform (quadnode &r);
    bool selfWIXform (quadnode &r);
    void swap ( quadnode &l, quadnode &r);
    void swap ( double &l, double &r);
    

    
public:
        xformpsin ();
        xformpsin (xformpsin *axform);
        void setmatrix (matrixType &A);
        void getmatrix (matrixType &A);
        int gettotalinstructions () { return pm.totalinstructions;}
        int gettotaldatacompares () { return pm.totaldatacompares;}
        int gettotaldatatransfers () { return pm.totaldatatransfers;}
        
        
        

        
        void selfWXform (matrixType &source, matrixType &result);
        void selfWXform (matrixType &source, matrixType &result, int resol);
        
        
        void selfWIXform (matrixType &source, matrixType &result);
        void selfWIXform (matrixType &source, matrixType &result, int resol);
       
        void generateTree ();
        void generateTree (matrixType &A);
        void DarwinPatchFilter (double epsilon);
        void DarwinFilter (double epsilon );
        void DarwinPatchFilter (matrixType &A, double epsilon);
        void DarwinFilter (matrixType &A, double epsilon );
        void sorttreewp ();
        void sorttreepsin ();
        void sorttreeL2 ();
        void sorthistogram ();

        
};

#endif __XFORM_PSI_N__