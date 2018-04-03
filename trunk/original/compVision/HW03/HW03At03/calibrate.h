/*
 *  calibrate.h
 *  HW04At01
 *
 *  Created by Daniel Beatty on Thu May 02 2002.
 *  Copyright (c) 2002 Texas Tech University. All rights reserved.
 *
 */

//#include <Carbon/Carbon.h>
 #include "imageType.h"
 //#include "slopeClass.h"
 
 
class calibrate {
    private:
        imageType T;
        imageType R;
        imageType Amatrix;
        imageType m;
        imageType mint;
        imageType mext;
        imageType U, VT, D;
        
        double fx, fy;
        double ox, oy;
        double f;
        
    public:
        calibrate (const char* filename);
        calibrate ();
        calibrate (imageType A);
        void getParamsFromFile (const char* filename);
        void saveParamsFromFile (const char* filename);
        void getPointsFromFile (const char* filename);
        void setWorldImage (double X, double Y, double Z, double x, double y, int &i);
        void getProjectionMatrix ();
        void computeParameter ();
        void computeSVD ();
        void computeFocalLength ();
        void setUpExtrinsicMatrix ();
        void setUpIntrinsicMatrix ();
        imageType getT () { return T;}
        imageType getR () { return R;}
        imageType getAmatrix () { return Amatrix;}
        imageType getM () { return m;}
        imageType getMint () { return mint;}
        imageType getMext () { return mext;}
        int max (int a, int b);
        int min (int a, int b);
        double getFx () { return fx;};
        double getFy () { return fy;}
        double getOx () { return ox;}
        double getOy () { return oy;}
        double getFocalLength () { return f;}
        
};