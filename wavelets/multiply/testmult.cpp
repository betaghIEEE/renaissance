/*
 *  testmult.cpp
 *  
 *
 *  Created by Daniel Beatty on Tues Jun 24, 2003.
 *  Copyright (c) 1970 2003 DRILL, Texas Tech University. All rights reserved.
 *
 */

#include "multiply.h"
#include <iostream.h>

int main ()
{
    matrixType A;
    matrixType B;
    int lim =8;
    int i, j, k;
    A.allocateStorage(lim, lim);
    B.allocateStorage(lim, lim);
    
    matrixType Ctrad;
    matrixType Cchain;
    matrixType Cwave;
    Ctrad.allocateStorage (lim, lim);
    //Cchain.allocateStorage (lim, lim);
    
    multiply mm;
    
    for ( i = 0; i < lim; i ++)
        for ( j = 0; j < lim; j++)
        {
            Ctrad[i][j] = 0;
            //Cchain[i][j] = 0;
            A[i][j] = i*3 + j;
            B[i][j] = i*3 + j;
            
            
        }
    cout << " Ctrad and CChain allocated : " << endl;
    for ( i = 0; i < lim; i++) {
        for ( j = 0; j < lim; j++){ 
            for ( k = 0; k < lim; k++)
                Ctrad[i][j] += A[i][k] * B[k][j];
            cout << A[i][j] << "\t";
        }
        cout << endl;
    }
    
    cout << endl;
    cout << " Traditional " << endl;
    for ( i = 0; i < lim; i++)
    {
        cout << endl;
        for ( j = 0; j < lim; j++)
        {
            cout << Ctrad[i][j] << "\t" ;
        }
    
    }
    cout << endl;
    
    mm.chainMultiply (A,B, Cchain);

    cout << endl;
    cout << " Chain Link version ";
    for ( i = 0; i < lim; i++)
    {
        cout << endl;
        for ( j = 0; j < lim; j++)
        {
            cout << Cchain[i][j] << "\t" ;
        }
    
    }    
    cout << endl;
    
 mm.waveMultiply (A,B, Cwave);

    cout << endl;
    cout << " Wavelet Chain Link version ";
    for ( i = 0; i < lim; i++)
    {
        cout << endl;
        for ( j = 0; j < lim; j++)
        {
            cout << Cwave[i][j] << "\t" ;
        }
    
    }    
    cout << endl;    
    return 0;
}
