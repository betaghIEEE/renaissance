/*
 *  correlate.h
 *  HW04At01
 *
 *  Created by Daniel Beatty on Mon Apr 15 2002.
 *  Copyright (c) 2002 Texas Tech University. All rights reserved.
 *
 */

//#include <Carbon/Carbon.h>
#include "calibrate.h"
#include "disparity.h"

class correlate 
{
    private:
        //corrPairType pairs;
         imageType Mlint, Mrint;
         imageType Tleft, Tright, Tlr;
         imageType Rleft, Rright, R;
         double fleft, fright;
         disparity ID;
        
    public:
        void setIntParams (calibrate *cal, bool left);
        void setExtParams (calibrate *cal, bool left);
        void compExtParam ();
        disparity getCorrPairs ();
        imageType compEpiPoles ();
        imageType compEpiLines (double alpha, int i, int j);
        void compRawCorr (imageType &left, imageType &right);
        void compEpiCorr (imageType left, imageType right);
        int round (double x);
        double getFocalLeft () { return fleft;}
        double getFocalRight () {return fright;}
        

};