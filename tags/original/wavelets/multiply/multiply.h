/*
 *  multiply.h
 *  
 *
 *  Created by Daniel Beatty on Tues Jun 24 2003.
 *  Copyright (c) 2003 DRILL, Texas Tech University. All rights reserved.
 *
 */
 
 #include "chainlink.h"
 #include "../include/matrixType.h"
 
 #ifndef __MULTIPLY_H__
#define __MULTIPLY_H__
class multiply 
{
    private:
        chainlink Ar;
        chainlink Bc;
        double tol;
    public:
        void chainMultiply (matrixType &C);
        void chainMultiply (matrixType &A, matrixType &B, matrixType &C);
        double chainMultiplyElement (int i, int j);
        void chainRow ( matrixType &A);
        void chainCol ( matrixType &B);
        bool isEpsilon ( double val);
        void waveMultiply (matrixType &A, matrixType &B, matrixType &C);
        
};

#endif
