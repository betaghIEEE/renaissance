/*
 *  calType.h
 *  math02
 *
 *  Created by Daniel Beatty on Sun Aug 04 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

//#include <Carbon/Carbon.h>
#include "matrixType.h"
#include <fstream.h>
#include <iostream>
//#include 

class calType 
{
    private:
        ifstream input;
        int position;
        int i;
        int buflen;
        char *buffer;
        double Xw;
        double Yw;
        double Zw;
        double Xi;
        double Yi;
        double *v;
        double ox, oy, fx, fy, f;
        bool open;
        matrixType *projection;
        matrixType VT;
        matrixType T;
        matrixType R;
        matrixType Mi;
        matrixType Mx;
            
    public:
        bool openFile (const char* filename);
        bool closeFile ();
        bool readLine ();
        bool parseLine();
        bool parseVLine ();
        bool getdouble (double &value);
        double getXw () { return Xw;};
        double getYw () { return Yw;};
        double getZw () { return Zw;};
        double getXi () { return Xi;};
        double getYi () { return Yi;};
        matrixType& getT () { return T;};
        matrixType& getR () { return R;};
        double getV (int i) { return v[i];};
        void getPointsFromFile (const char* filename);
        void setWorldImage (double X, double Y, double Z, double x, double y, int &i) ;
        void computeParameter ();
        void setUpIntrinsicMatrix ();
        void setUpExtrinsicMatrix ();
        double computeFocalLength ();
        void setBuffer (const char* test);
        void computeSVD (int argc, const char * argv[]);
        //void computeSVD ();
};

