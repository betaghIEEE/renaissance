/*
 *  matrixType.h
 *  math02
 *
 *  Created by Daniel Beatty on Sat Aug 03 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

//#include <Carbon/Carbon.h>

class matrixType 
{
    public:
    int m_rows;
    int m_cols;
    int NUMBER_OF_GREY_LEVELS;
    double **m_data;
    matrixType () {allocateStorage ();};
    matrixType (int rows, int cols) {allocateStorage (rows,cols);};
    bool copyElements (const matrixType& source);
    bool allocateStorage ();
    bool allocateStorage (int rows, int cols);
    bool deleteStorage ();
    bool checkSize(int rows, int cols);
    matrixType& operator= (const matrixType& rhs);
    matrixType& operator+=(const matrixType& rhs);
    matrixType& operator-=(const matrixType& rhs);
    matrixType& operator*=(const matrixType& rhs); //This is a matrix multiplication
    matrixType& operator*=(double rhs);
    matrixType& operator/=(double rhs);
    
    matrixType& operator+=(double rhs);
    matrixType& operator-=(double rhs);
    
     
    void error(const char *errorMsg);
    
    
};
// external operators

const matrixType operator*(const matrixType &lhs, const matrixType &rhs);
const matrixType operator*(const matrixType &lhs, double rhs);
const matrixType operator*(double lhs, const matrixType &rhs);
const matrixType operator+(const matrixType &lhs, const matrixType &rhs);
const matrixType operator-(const matrixType &lhs, const matrixType &rhs);

