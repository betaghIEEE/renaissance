/*
 *  calibrate.cpp
 *  HW04At01
 *
 *  Created by Daniel Beatty on Thu May 02 2002.
 *  Copyright (c) 2002 Texas Tech University. All rights reserved.
 *
 */

#include "calibrate.h"
#include <math.h>
#include "numReader.h"
#include <stdlib.h>
//#include "svdReceipe.h"
//using namespace std;

void calibrate::getParamsFromFile (const char* filename)
{
// Get camera parameters from file
// The pattern for this gathering requires the 
//	states of T vector, R matrix, fx, fy, ox, oy
//  Therefore a loop is used for gathering of data.  
//	the states identify where to put the input. 

    int state = 0;
    //int stateEnd = 8;
    ifstream infile(filename);
    double temp;
    bool test;
    int row = 0; 
    int col = 0;
    T = imageType(3,1);
    R = imageType(3,3);
    
    while (!infile.eof()) {
        infile >> temp;
        test = false;
    switch (state) {
        case 0:  // qT  State
            T.setValue(row++,col,temp);
            if (row > 2) {
                state++;
                row = 0;
                col = 0;
            }
            break;
        case 1: // qR state
            R.setValue(row, col++, temp);
            if (col > 2) {
                col = 0;
                row++;
            }
            if (row > 2) {
                state++;
                row = 0;
                col = 0;
            }
            break;
        case 2: // qfx state
            fx = temp;
            state++;
            break;
        case 3: // qfy state
            fy = temp;
            state++;
            break;
        case 4:
            ox = temp;
            state++;
            break;
        case 5:
            oy = temp;
            test = true;
            state++;
            break;
        default: 
            cout << "What the ...";
            break;
        } // end of switch statement 
    } // end of while loop

} // end getParamsFromFile



void calibrate::saveParamsFromFile (const char* filename)
{
    int state = 0;
    int stateEnd = 6;
    ofstream infile(filename);
    double temp;
    bool test;
    int row = 0; 
    int col = 0;
    
    while (!(state == stateEnd)) {
            test = false;
    switch (state) {
        case 0:  // qT  State
            temp = T.getValue(row++,col);
            infile << temp;
            infile << " ";
            if (row > 2) {
                state++;
                row = 0;
                col = 0;
                infile << "\n";
            }
            break;
        case 1: // qR state
            temp = R.getValue(row, col++);
            infile << temp;
            infile << " ";
            if (col > 2) {
                col = 0;
                row++;
                infile << "\n";
            }
            if (row > 2) {
                state++;
                row = 0;
                col = 0;
                infile << "\n";
            }
            break;
        case 2: // qfx state
            temp = fx;
            infile << temp;
            infile << " ";
            state++;
            break;
        case 3: // qfy state
            temp = fy;
            infile << temp;
            infile << "\n";
            state++;
            break;
        case 4:
            ox = temp;
            infile << temp;
            infile << " ";
            state++;
            break;
        case 5:
            temp = oy;
            infile << temp;
            //infile << " ";
            test = true;
            state++;
            break;
        default: 
            cout << "What are you doing...";
            exit (1);
            break;
        } // end of switch statement 
    } // end of while loop
}

void calibrate::getPointsFromFile (const char* filename)
{
    
    double X,Y,Z,x,y;
    //char ch[10];
    ifstream infile(filename);
    imageType temp (40,12);
    Amatrix = temp;
    //bool empty = true;
    int i = 0;
    cout << "Obtaining data from file " << filename << " " << endl;
    numReader *num = new numReader();
    num->openFile (filename);
    while (num->readLine() && (i < 40))
    {
        X = num->getXw();
        Y = num->getYw();
        Z = num->getZw();
        x = num->getXi ();
        y = num->getYi ();
        setWorldImage(X,Y,Z,x,y,i);
    }
}

void calibrate::setWorldImage (double X, double Y, double Z, double x, double y, int &i) 
{
        
    Amatrix.setValue(i,0,X);
    Amatrix.setValue(i,1,Y);
    Amatrix.setValue(i,2,Z);
    Amatrix.setValue(i,3,1);
    Amatrix.setValue(i,4,0);
    Amatrix.setValue(i,5,0);
    Amatrix.setValue(i,6,0);
    Amatrix.setValue(i,7,0);
    Amatrix.setValue(i,8,-x*X);
    Amatrix.setValue(i,9,-x*Y);
    Amatrix.setValue(i,10,-x*Z);
    Amatrix.setValue(i,11,-x);
    //Amatrix.addRow();
    i++;
    Amatrix.setValue(i,0,0);
    Amatrix.setValue(i,1,0);
    Amatrix.setValue(i,2,0);
    Amatrix.setValue(i,3,0);
    Amatrix.setValue(i,4,X);
    Amatrix.setValue(i,5,Y);
    Amatrix.setValue(i,6,Z);
    Amatrix.setValue(i,7,1);
    Amatrix.setValue(i,8,-y*X);
    Amatrix.setValue(i,9,-y*Y);
    Amatrix.setValue(i,10,-y*Z);
    Amatrix.setValue(i,11,-y);
    i++;
    
}

void calibrate::getProjectionMatrix ()
{
    int i;
    double value = 0;
    imageType temp (1,11);
    for (i=0; i < 11; i++){
        value = VT.getValue(11,i);
        temp.setValue (0,i, value);
    }
    m = temp;
}

void calibrate::computeParameter ()
{
    double gamma;
    double sigma = 1;
    double temp;
    double tz;
   // double *tempMatrix;
    //m.getRow(11,tempMatrix);
    double tempMatrix[12];
    int i;
    
    T = imageType (3,1);
    R = imageType(3,3);
    
    for (i =0; i< 12; i++)
        tempMatrix[i] = VT.getValue(11,i);
    temp = tempMatrix[8] * tempMatrix[8];
    temp += tempMatrix[9] * tempMatrix[9];
    temp += tempMatrix[10] * tempMatrix[10];
    gamma = sqrt (temp);
    temp = tempMatrix[11] / gamma;
    if (temp < 0 ) 
            {
                temp = -temp;
                gamma = -gamma;
                sigma = -1;
            }
    tz = temp;
    
    
    ox = tempMatrix[0] * tempMatrix[8] + tempMatrix[1] * tempMatrix[9] + tempMatrix[2] * tempMatrix[10];
    oy = tempMatrix[4] * tempMatrix[8]+ tempMatrix[5] * tempMatrix[9] + tempMatrix[6] * tempMatrix[10];
    
    temp = tempMatrix[0] * tempMatrix[0];
    temp += tempMatrix[1] * tempMatrix[1];
    temp += tempMatrix[2] * tempMatrix[2];
    temp += ox * ox;
    if (temp < 0) {
        temp = - temp;
    }
    fx = sqrt (temp);
    temp = tempMatrix[4] * tempMatrix[4];
    temp += tempMatrix[5] * tempMatrix[5];
    temp += tempMatrix[6] * tempMatrix[6];
    temp += oy * oy;
    if (temp < 0) {
        temp = - temp;
    }
    fy = sqrt (temp);
    for ( int i = 0; i < 3; i++)
        {
            temp = sigma * (ox*tempMatrix[8+i] - tempMatrix[i])/fx;
            R.setValue (0,i,temp);
            temp = sigma * (oy*tempMatrix[8+i] - tempMatrix[4+ i])/fy;
            R.setValue (1,i,temp);
            temp = tempMatrix[8+i] / gamma;
            R.setValue (2, i, temp);
        }
    temp = sigma * ( ox * tz - tempMatrix[3]); 
    
    T.setValue(0,0, temp);
    temp = sigma * (oy * tz - tempMatrix[7]);
    T.setValue(1,0, temp);
    T.setValue (2,0, tz);
    
    cout << "Parameters computed " << endl;
    
    
}

void calibrate::setUpIntrinsicMatrix ()
{
    mint = imageType(3,3);
    mint.setIdentity();
    mint.setValue (0,0, fx);
    mint.setValue (1,1, fy);
    mint.setValue (0,2, ox);
    mint.setValue (1,2, oy);
    cout << "Intrinsic Parameters set " << endl;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
            cout << mint.getValue(i,j) << " ";
        cout << endl;
    }
    
}

void calibrate::setUpExtrinsicMatrix ()
{
    imageType tempVector (1,3);
    mext = imageType(3,4);
    double temp;
    double temp2;
    
    int i,j;
    
    for (i=0; i < 3; i++)
    {
        temp2 = 0;
        temp = 0;
        
        for (j=0; j < 3; j++)
        {
            temp = R.getValue(i,j);
            mext.setValue (i,j, temp);
            temp2 += T.getValue(j,0) * temp;
            
        }
        mext.setValue (i,3,temp2);
    }
    cout << "Extrinsic Parameters set " << endl;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 4; j++)
            cout << mext.getValue(i,j) << " ";
        cout << endl;
    }
}

void calibrate::computeFocalLength ()
{
    int i;
    double Zc;
    double Zw;
    double TTrans, TTransR;
    double ten = 10.0;
    double tiny = pow (ten, -20);
    double tempT;
    double tempT2;
    imageType tempM;
    imageType tempM2;
    imageType worldOrigin(4,1);
    imageType cameraOrigin(1,3);
    imageType imageOrigin (1,3);
    for (i=0; i < 3; i++)
        worldOrigin.setValue(i,0,0.0);
    worldOrigin.setValue(3,0,1.0);
    
    tempM = mint * mext;
    cameraOrigin = tempM * worldOrigin;
    Zc= cameraOrigin.getValue (2,0);
    imageOrigin = cameraOrigin * (1/ Zc);
    Zw = -1 * T.getValue(0,2);
    tempM2 = T.transpose();
    //tempM = T * tempM2;
    tempT2 = 0;
        for ( i = 0; i < 3; i++)
        {
            tempT = T.getValue (i,0);
            tempT2 += tempT * tempT;
        }
    TTrans = tempT2;
    //TTrans = tempM.getValue(0,0);
    if (TTrans == 0.0) 
        TTrans = tiny;
    TTransR = 1.0 / TTrans;
    tempM2 = (Zw *imageOrigin) * (TTransR * tempM2);
    f = Zw * tempM2.getValue(0,0);
    cout << "Focal Length is " << f << endl;
    
}



calibrate::calibrate (const char* filename)
{
    getPointsFromFile (filename);
    cout << "File values obtained " << endl;
    computeSVD();
    computeParameter();
    setUpIntrinsicMatrix ();
    setUpExtrinsicMatrix ();
    computeFocalLength();
}


calibrate::calibrate ()
{
    T = imageType (3,1);
    R = imageType (3,3);
    Amatrix = imageType (12,12);
    mint = imageType (3,3);
    mext = imageType (3,4);
    m = imageType(1,12);
    U = imageType(12,12);
    VT =imageType(12,12);
    D = imageType(12,12);
    
    f =0;
    fx =0;
    fy = 0;
    ox = 0;
    oy = 0;
    
}

calibrate::calibrate (imageType A)
{
    
    Amatrix = A;
    computeSVD();
    computeParameter();
    setUpIntrinsicMatrix ();
    setUpExtrinsicMatrix ();
    computeFocalLength();
    
}
/*
void calibrate::computeSVD ()
{
    int m = Amatrix.getRowSize();
    int n = Amatrix.getColSize();
    int k = m * n;
    int l= 0;
    int i,j;
    double *A = new double [k];
    //double **v;
    double temp;
    svdReceipie *svd = new svdReceipie();
    //svd->allocate (m,n,A);
    
    for ( i = 0; i < m; i++)
        for ( j = 0; j < n; j++)
            A[l++] = Amatrix.getValue(i,j);
    svd->setSourceMatrix (m,n,A);
    svd->computeSVD();
        VT = imageType (n,n);
        for (i = 0; i < n; i++)
            for ( j = 0; j < n; j++)
            {	
                temp = svd->getNullSpace(i,j);
                VT.setValue(j,i, temp);
            }    
    
}
*/

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

int calibrate::min (int a, int b)
{
    if (a > b)
        return b;
        else
        return a;
}

int calibrate::max (int a, int b)
{
    if (a >= b)
        return a;
        else
        return b;
}

void calibrate::computeSVD ()
// This function computes the camera M matrix by 
//  least squares fit 
{
  int i;
    int m_rows = Amatrix.getRowSize();
    int m_cols = Amatrix.getColSize();
  double *A = new double[m_rows*m_cols];
  Amatrix.copyMatrixToLapack(m_rows, m_cols, A);

  double *S = new double[min(m_rows, m_cols)];
  double *U2 = new double[m_rows*min(m_rows, m_cols)];
  int ldvt = min(m_rows, m_cols);
  double *VT2 = new double[ldvt*m_cols];
  int lwork = max(3*min(m_rows, m_cols)+max(m_rows, m_cols),5*min(m_rows,m_cols));
  double * WORK = new double[lwork];
  int info;

  dgesvd_( 'S', 'S', m_rows, m_cols, A, m_rows, S, U2, m_rows, VT2, ldvt, WORK, lwork, info );

  U.resize(m_rows, min(m_rows, m_cols));
  U.copyLapackToMatrix(m_rows, min(m_rows, m_cols), U2);
  D.resize(min(m_rows, m_cols), min(m_rows, m_cols));
  D.setIdentity();
  for (i=0; i<min(m_rows, m_cols); i++) 
    D.setValue(i,i,S[i]);
  VT.resize(ldvt, m_cols);
  VT.copyLapackToMatrix(ldvt, m_cols, VT2);
  

  delete []A;
  delete []U2;
  delete []S;
  delete []VT2;
  delete []WORK;  
    
}

