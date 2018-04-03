/*
 *  multiply.cpp
 *  
 *
 *  Created by Daniel Beatty on Sat Jun 14 2003.
 *  Copyright (c) 2003 DRILL, Texas Tech University. All rights reserved.
 *
 *
 *	Description:  (in LaTeX notation)
 *	The purpose of this class is to provide basic functions for some
 *	exotic matrix multiplication methods.  As such it includes the 
 *	chain link structure to facilitate the multiplication of two matrices.
 *	Only square matrices are being considered for the wavelet case.  
 *
 *	The chain multiply function is rather simple.  For every m and n in 
 *	new matrix, calculate the value, and return the matrix.  
 
 */
 
 #include "multiply.h"
 #include <math.h>
 #include <iostream.h>
 #include "xform2D.hpp"
 
void multiply::chainMultiply (matrixType &C)
{
    int mlimit = Ar.getLength();
    int nlimit = Bc.getLength();
    int m, n;
    double result;

    C.allocateStorage(mlimit, nlimit);
    cout << "Space allocated " << endl;
    for ( m = 0; m < mlimit; m++)
      for ( n = 0; n < nlimit; n++) {
	result = chainMultiplyElement (m,n);
	C[m][n] = result;
	//	cout << result << endl;
      }

    //            C[m][n] = chainMultiplyElement (m,n);
    //cout << C.m_rows << " rows and "  << C.m_cols << " columns " << endl; 
    //cout << mlimit << " rows and "  << nlimit << " columns " << endl; 
}


void multiply::chainMultiply (matrixType &A, matrixType &B, matrixType &C)
{
    tol = pow(1.0 , -6.0);
    cout << "Allocating Chains" << endl;
    chainRow (A);

    cout << "Display\n";
    Ar.display();

    chainCol (B);
    chainMultiply (C);
}


double multiply::chainMultiplyElement (int i, int j)
{
    int rlimit;
    int climit;
    int k;
    int l;
    int jlow;
    int lmatch;
    double total;
    bool BnotExhausted = true;
    k = l = jlow = lmatch = 0;
    total = 0.0;
    rlimit = Ar.getLinkLength (i);
    climit = Bc.getLinkLength (j); 
    while ( k < rlimit && BnotExhausted) {

      //      cout << k << "\t" << Ar.getHookKey(i,k) << "\t" << Ar.getHookLink(i,k) << "\t" << total << endl;
      lmatch = -1;
      for ( l = jlow; l < climit; l++) {
	//	cout << Bc.getHookKey(j, l) << "\t" << Bc.getHookLink(j, lmatch) << "\t" << lmatch << endl;
	if ( Ar.getHookKey(i, k) == Bc.getHookKey(j, l) ) lmatch = l;
      }
        if (lmatch == climit)
            BnotExhausted = false;
        else 
        {
            total += Ar.getHookLink (i,k) * Bc.getHookLink (j, lmatch);
            jlow = lmatch;
        }
        k++;
    }
    //cout << rlimit << " \t" << climit << " \t" << k << " \t" << jlow << " \t" << lmatch << " \t" << total << endl;
    return total;
}

void multiply::chainRow ( matrixType &A)
{
    int i, j;
    int rows = A.m_rows;
    int cols = A.m_cols;
    double temp;
    Ar.allocateHooks(rows,cols);
    Ar.setLength (rows);
    for ( i = 0; i < rows; i++)
        for ( j = 0; j < cols; j++) {
            temp = A[i][j];
            if ( !isEpsilon(temp))    Ar.addHookLink (i, j, temp);
            //cout << " i, j" << i << " , " <<  j << " " << temp << endl;
        }
   // cout << "row chain keys" << endl;
   // for ( i = 0; i < rows; i++)
    //{
      //  for ( j = 0; j < cols; j++)
      //      cout << Ar.getHookKey (i,j) << "\t";
      //  cout << endl;
    //}
}

void multiply::chainCol ( matrixType &B)
{    
    int i, j;
    int rows = B.m_rows;
    int cols = B.m_cols;
    Bc.allocateHooks(cols, rows);
    Bc.setLength (cols);
    for ( j = 0; j < cols; j++)
        for ( i = 0; i < rows; i++)
           if ( !(isEpsilon(B[i][j])) ) 
                Bc.addHookLink (j, i, B[i][j]);
    //cout << "Column chain " << endl;
   // for ( i = 0; i < rows; i++)
    //{
     //   for ( j = 0; j < cols; j++)
      //      cout << Bc.getHookLink (i,j) << "\t";
      //  cout << endl;
    //}
    
//    cout << "Column chain Keys " << endl;
//    for ( i = 0; i < rows; i++)
//    {
//        for ( j = 0; j < cols; j++)
//            cout << Bc.getHookKey (i,j) << "\t";
//        cout << endl;
//    }
}

bool multiply::isEpsilon (double val)
{
    if (( val > (-tol) ) && ( val < tol ))
        return true;
    return false;
}


void multiply::waveMultiply (matrixType &A, matrixType &B, matrixType &C)
{
    wave2comp X;
    matrixType alpha;
    matrixType beta;
    matrixType charlie;
    X.selfWXform (A, alpha);
    X.selfWXform (B, beta);
    chainMultiply (alpha, beta, charlie);
    X.selfWaveletXformInverse (charlie, C);
}
