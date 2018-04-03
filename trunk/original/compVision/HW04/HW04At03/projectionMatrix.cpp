/*
 *  projectionMatrix.cpp
 *  HW03At01
 *
 *  Created by Daniel Beatty on Fri Mar 29 2002.
 *  Copyright (c) 2002 Texas Tech University. All rights reserved.
 *
 */

#include "projectionMatrix.h"
#include <iostream>
#include <fstream>
#include <math.h>
using namespace std;

void projectionMatrix::computeParameter ()
{
    double gamma;
    double sigma = 1;
    double temp;
    double tz;
    double *tempMatrix;
    m.getRow(11,tempMatrix);
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
    
}

/*
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


void projectionMatrix::computeSVD ()
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
	i = min(m_rows, m_cols);
  D.resize(i, i);
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
*/

void projectionMatrix::getMatrixCompVals (const char *filename)
{
//  This function acquires the file containing the world and image
//	coordinate pairs.  Upon receiving the data, the data is written
//  	image Matrix (imageType) A.
// 	Requires:  8 states, one for each coordinate component and one for
//		reject, write, and stop.  
    
    
    // 
    //   state = 0; 
    
    int state = 0;
    int i;
    int stateEnd = 8;
    ifstream infile(filename);
    double temp;
    double X,Y,Z,x,y;
    bool test;
    //bool reject;

        
    while (!(state == stateEnd)){
    
        if (test = (!infile.eof())) 
            infile >> temp;
            
        switch (state) {
            case 0: // qX state
                if (test) {
                    state++;
                        X = temp;
                    }
                    else {
                        state = stateEnd;
                    }
                break;
            case 1: // qY state
                if (test) {
                    state++;
                        Y = temp;
                    }
                    else {
                        state = stateEnd;
                    }
                break;
            case 2: // qZ state
                if (test) {
                    state++;
                        Z = temp;
                    }
                    else {
                        state = stateEnd;
                    }
                break;
            case 3: // qx State
                if (test) {
                    state++;
                        x = temp;
                    }
                    else {
                        state = stateEnd;
                    }
                break;
            case 4: // qy State
                if (test) {
                    state++;
                        y = temp;
                    }
                    else {
                        state = stateEnd;
                    }
                
            case 5: // write state
                    m.setValue(i,0,X);
                    m.setValue(i,1,Y);
                    m.setValue(i,2,Z);
                    m.setValue(i,3,1);
                    m.setValue(i,4,0);
                    m.setValue(i,5,0);
                    m.setValue(i,6,0);
                    m.setValue(i,7,0);
                    m.setValue(i,8,-X*x);
                    m.setValue(i,9,-Y*x);
                    m.setValue(i,10,-Z*x);
                    m.setValue(i,11,-x);
                    
                    m.setValue(i+1,0,0);
                    m.setValue(i+1,1,0);
                    m.setValue(i+1,2,0);
                    m.setValue(i+1,3,0);
                    m.setValue(i+1,4,X);
                    m.setValue(i+1,5,Y);
                    m.setValue(i+1,6,Z);
                    m.setValue(i+1,7,1);
                    m.setValue(i+1,8,-X*y);
                    m.setValue(i+1,9,-Y*y);
                    m.setValue(i+1,10,-Z*y);
                    m.setValue(i,11,-y);
                    state = 0;
                    i= i+2;
                break;
            default:
                cout << "What the ....";
                break;
        }
    }

} 



void projectionMatrix::getParamsFromFile (const char* filename)
{
// Get camera parameters from file
// The pattern for this gathering requires the 
//	states of T vector, R matrix, fx, fy, ox, oy
//  Therefore a loop is used for gathering of data.  
//	the states identify where to put the input. 

    int state = 0;
    int stateEnd = 8;
    ifstream infile(filename);
    double temp;
    bool test;
    int row = 0; 
    int col = 0;
   
    while (!infile.eof()) {
        infile >> temp;
    // while (state != stateEnd) 
    
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



void projectionMatrix::saveParamsFromFile (const char* filename)
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
            infile << " ";
            test = true;
            state++;
            break;
        default: 
            cout << "What are you doing...";
            break;
        } // end of switch statement 
    } // end of while loop
}
