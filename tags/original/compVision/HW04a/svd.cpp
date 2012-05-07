/*
 *  svd.cpp
 *  math02
 *
 *  Created by Daniel Beatty on Wed Aug 21 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#include "svd.h"
#include <stddef.h>


extern "C" void dgetrf_(
        const int &m,                   // (input)
        const int &n,                   // (input)
        double *a,                      // a[n][lda] (input/output)
        const int &lda,                 // (input)
        int *ipiv,                      // ipiv[min(m,n)] (output)
        int &info                       // (output)
        );

extern "C" void dgetri_(
        const int &n,                   // (input)
        double *a,                      // a[n][lda] (input/output)
        const int &lda,                 // (input)
        const int *ipiv,                // ipiv[n] (input)
        double *work,                   // work[lwork] (workspace/output)
        const int &lwork,               // (input)
        int &info                       // (output)
        );

extern "C" void dgesvd_(
        const char &jobu,               // (input)
        const char &jobvt,              // (input)
        const int &m,                   // (input)
        const int &n,                   // (input)
        double *a,                      // a[n][lda] (input/output)
        const int &lda,                 // (input)
        double *s,                      // s[min(m,n)] (output)
        double *u,                      // u[ucol][ldu] (output)
        const int &ldu,                 // (input)
        double *vt,                     // vt[n][ldvt] (output)
        const int &ldvt,                // (input)
        double *work,                   // work[lwork] (workspace/output)
        const int &lwork,               // (input)
        int &info                       // (output)
        );

void svdThis::computeSVD (int row, int col, double *source, double *result, int &i, int &j)
{
    //int rows = projection.m_rows;
    //int cols = projection.m_cols;
    int mirc, mxrc, lwork, ldvt,lworka, lworkb ;
    if ( row > col) 
        {
            mirc = col;
            mxrc = row;
        }
        else
        {
            mirc = row;
            mxrc = col;
        }
    
    lworka = 3*mirc+mxrc;
    lworkb = 5 * mirc;
    if ( lworka > lworkb) 
                lwork = lworka;
                else
                lwork = lworkb;
    int info;
    int k = 0;
    int l;
    double *A = new double [row * col];
    double *S = new double [row * col];
    double *U2 = new double [row * mirc];
    double *VT2 = new double [mirc * col];
    double *WORK = new double [lwork];
    //for (i = 0; i< rows; i++)
        //for (j=0; i<cols; j++)
            //A[k++] = projection.m_data[i][j];
    dgesvd_( 'S', 'S', row, col, source, row, S, U2, row, VT2, ldvt, WORK, lwork, info );
    
    //VT = matrixType (ldvt, cols);
    //k = 0;
    l = ldvt * col;
    if (result != NULL)
        delete result;
    result = new double [l];
    for ( k = 0; k < l; k++)
        result[k] = VT2[k];
    
    
    delete []A;
    delete []U2;
    delete []S;
    delete []VT2;
    delete []WORK;  
    
}



