/*
 *  correlate.h
 *  HW04At01
 *
 *  Created by Daniel Beatty on Mon Apr 15 2002.
 *  Copyright (c) 2002 Texas Tech University. All rights reserved.
 *
 */

//#include <Carbon/Carbon.h>

#include "disparity.h"
//#include "calibrate.h"
#include "calType.h"

struct worldPoint
{
    double x;
    double y;
    double z;
    double I;
    double color[3];
    int next;
    int prev;
    
};

class correlate 
{
    private:
        //corrPairType pairs;
         //matrixType Mlint, Mrint;
         matrixType Tleft, Tright;
         matrixType T;
         //matrixType Rleft, Rright;
         matrixType R;
         double fleft, fright;
         disparity ID;
         worldPoint **wp;
         int w_rows, w_cols;
        
    public:
        //void setIntParams (calibrate *cal, bool left);
        //void setExtParams (calibrate *cal, bool left);
        //void compExtParam ();
        disparity getCorrPairs ();
        //imageType compEpiPoles ();
        //imageType compEpiLines (double alpha, int i, int j);
        void compRawCorr (matrixType &left, matrixType &right);
        //void compEpiCorr (matrixType left, matrixType right);
        int round (double x);
        double getFocalLeft () { return fleft;}
        double getFocalRight () {return fright;}
        //void getLeftParameters ( const char* filename);
        //void getRightParameters ( const char* filename);
        //void initialize (const char *left, const char *right);
        void computeViaRaw ( const char *leftFile, const char *rightFile);
        correlate ( const char *leftFile, const char *rightFile);
        correlate ();
        void getImagePGM2 (const char* filename, matrixType &matType);
        bool saveImagePGM2 ( const char* filename, matrixType &matType);
        void computeWorldPoints();
        matrixType& computePoint (matrixType &leftPoint, matrixType &rightPoint);
        bool allocateWorldPoints (int rows, int cols);
        void produceVRML (const char* filename);
        matrixType& cross ( matrixType &left, matrixType &right);
        void setWorldPoint (int i, int j, worldPoint &aPoint);
        matrixType& computeInverseGJ (matrixType &a);
        //void computeViaEpi ( const char *leftFile, const char *rightFile);
};
