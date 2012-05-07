/*  ----------------------------------------------------------------------

    image.hpp

    Redone: 01/04/02
    Redone Again: Mon Jan 14 12:57:40 CST 2002
    Author: Eric D. Sinzinger

    After thousands of years of evolutionary programming starting with the
    abacus, the ultimate representation for image operations is CImage.

    Define elements as:
    CImage

    It handles:
    reading in images that are automatically converted to grayscale
    saving out grayscale images

    Still need to add functionality:
    * x=b/A (maybe put in line
    * createTransformationImage
    * image assignments
    * file i/o possibly
    * complete a set of imageTest.cpp
    
    ----------------------------------------------------------------------  */

#ifndef __IMAGE_HPP__
#define __IMAGE_HPP__

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <Magick++.h>
using namespace std;

class CImage{

public:

  //constructors and destructors
  CImage(int rows = -1, int cols = -1);
  CImage(const CImage& source);
  CImage(const char *filename);
  ~CImage(void);

  //overloaded operators
  CImage& operator= (const CImage& rhs);
  CImage& operator+=(const CImage& rhs);
  CImage& operator-=(const CImage& rhs);
  CImage& operator*=(const CImage& rhs); //This is a matrix multiplication

  CImage& operator= (double rhs);
  CImage& operator+=(double rhs);
  CImage& operator-=(double rhs);
  CImage& operator*=(double rhs);
  CImage& operator/=(double rhs);

  //data conversion operators
  const double* operator [](int row) const { return m_data[row]; }
  double* operator [](int row) { return m_data[row]; }

  //size operations
  void resize(int rows, int cols);
  int getRowSize(void) const { return m_rows; }
  int getColSize(void) const { return m_cols; }
  bool checkSize(int rows, int cols); 

  // Insert and Delete
  void setValue (int row, int col, double value) { m_data[row][col] = value;}
  double getValue (int row, int col) { return m_data[row][col];}
  
  //mathematical operations
  double calcAverage(void);
  double calcSumSqrAverage(void);
  void setUniformRandom(double range);
  void setIdentity();
  const CImage inverse();
  double calcSumSquared(void);
  //  void svd(CImage& U, CVector& D, CImage& VT);
  const CImage transpose();


  //image processing operations
  //  const CImage calcRowGradient(int filterLength);
  //  const CImage calcColGradient(int filterLength);
  
CImage calcRowGradientFast();
CImage calcColGradientFast();
void squareEachElement ();
void squareRootEachElement();
  
  //  void smooth(int filterLength);

  //general functions
  void display(void);

  //file I/O operations
  bool read(const char *filename);
  bool save(const char *filename);
  
private:
  int m_rows;
  int m_cols;
  double **m_data;

  bool allocateStorage(void);
  bool deleteStorage(void);
  bool copyElements(const CImage& source);  

 void error(const char *errorMsg);
};

//external operators

const CImage operator*(const CImage &lhs, const CImage &rhs);
const CImage operator*(const CImage &lhs, double rhs);
const CImage operator*(double lhs, const CImage &rhs);
const CImage operator+(const CImage &lhs, const CImage &rhs);
const CImage operator-(const CImage &lhs, const CImage &rhs);

#endif // __IMAGE_HPP__
