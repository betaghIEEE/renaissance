/*
 *  projectionMatrix.h
 *  HW03At01
 *
 *  Created by Daniel Beatty on Fri Mar 29 2002.
 *  Copyright (c) 2002 Texas Tech University. All rights reserved.
 *
 */

//#include <Carbon/Carbon.h>
#include "imageType.h"

class projectionMatrix {
    private:
       /* int m_rows;
        int m_cols;
        double **m_data;
        bool allocateStorage(void);
        bool deleteStorage(void);
        bool copyElements(const imageType& source);  
    */
        
            imageType T;
            imageType R;
            imageType Amatrix;
            imageType m;
            imageType U, VT, D;
        
            double fx, fy;
            double ox, oy;
        
        //void error(const char *errorMsg);

    public: 
        void computeParameter ();
        void computeSVD (); 
        void getMatrixCompVals (const char *filename);
        void saveParamsFromFile (const char* filename);
        void getParamsFromFile (const char* filename);
        
};
