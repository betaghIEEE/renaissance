/*
 *  matrixType.h
 *  math02
 *
 *  Created by Daniel Beatty on Sat Aug 03 2002.
 *  Copyright (c) 2002Computer Science, Texas Tech University. All rights reserved.
 *
 */

//#include <Carbon/Carbon.h>
#ifndef __MATRIXTYPE_HPP__
#define __MATRIXTYPE_HPP__

class matrixType 
{
    public:
    int m_rows;
    int m_cols;
    double epsilon;
    int NUMBER_OF_GREY_LEVELS;
    double scaleX;
    double scaleY;
    double shiftX;
    double shiftY;
    double **m_data;
    matrixType ();
    matrixType (int rows, int cols) {defineEpsilon (); allocateStorage (rows,cols);};
    //~matrixType () { deleteStorage(); };
    bool copyElements (const matrixType& source);
    bool allocateStorage ();
    bool allocateStorage (int rows, int cols);
    bool deleteStorage ();
    bool zeroize();
    void GJ ();
    void defineEpsilon ();
    bool checkSize(int rows, int cols);
    matrixType& operator= (const matrixType& rhs);
    matrixType& operator+=(const matrixType& rhs);
    matrixType& operator-=(const matrixType& rhs);
    matrixType& operator*=(const matrixType& rhs); //This is a matrix multiplication
    matrixType& sparsematrixmultiply (const matrixType &lhs, const matrixType &rhs);
    matrixType& operator*=(double rhs);
    matrixType& operator/=(double rhs);
    
    matrixType& operator+=(double rhs);
    matrixType& operator-=(double rhs);
    
     
    void error(const char *errorMsg);
    double* operator[](int i) {return m_data[i];};
    double getEnergy ();
    double getL1Energy ();
    
};
// external operators

const matrixType operator*(const matrixType &lhs, const matrixType &rhs);
const matrixType operator*(const matrixType &lhs, double rhs);
const matrixType operator*(double lhs, const matrixType &rhs);
const matrixType operator+(const matrixType &lhs, const matrixType &rhs);
const matrixType operator-(const matrixType &lhs, const matrixType &rhs);
#endif

