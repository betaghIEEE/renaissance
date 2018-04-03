/*
 *  imageType.h
 *  HW02At05
 *
 *  Created by Daniel Beatty on Mon Feb 25 2002.
 *  Copyright (c) 2001 __MyCompanyName__. All rights reserved.
 *
    The image type here is a generic matrix class with operations
    to go along with it.  These basic operations are geared for 
    image transformations.  
    
    Included are modified versions of functions originally 
    written by Eric D. Sinzinger.  The modifications were 
    clarity in C++ so that it could also easily ported to 
    Objective C.  
 */

class imageType {

private:
  int m_rows;
  int m_cols;
  int NUMBER_OF_GREY_LEVELS;
  double **m_data;

  bool allocateStorage(void);
  bool deleteStorage(void);
  bool copyElements(const imageType& source);  

 void error(const char *errorMsg);
 
 public:
  //constructors and destructors
  imageType(int rows = -1, int cols = -1);
  imageType(const imageType& source);
  ~imageType (void); 
 
  //overloaded operators
  imageType& operator= (const imageType& rhs);
  imageType& operator+=(const imageType& rhs);
  imageType& operator-=(const imageType& rhs);
  imageType& operator*=(const imageType& rhs); //This is a matrix multiplication

  imageType& operator= (double rhs);
  imageType& operator+=(double rhs);
  imageType& operator-=(double rhs);
  imageType& operator*=(double rhs);
  imageType& operator/=(double rhs); 
 
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
    void getImagePGM2 (const char* filename);
    bool saveImagePGM2 (const char * filename);
    int charToInt(char temp);
  //general functions
  void display(void);
  
  
  //mathematical operations
  double calcAverage(void);
  double calcSumSqrAverage(void);
  void setUniformRandom(double range);
  void setIdentity();
  
  const imageType inverse();
  double calcSumSquared(void);
  //  void svd(CImage& U, CVector& D, CImage& VT);
  const imageType transpose();
  
    //image processing operations
    
    imageType calcRowGradientFast();
    imageType calcColGradientFast();
    void squareEachElement ();
    void squareRootEachElement();
    imageType convolve (imageType &mask);
 
};

//external operators

const imageType operator*(const imageType &lhs, const imageType &rhs);
const imageType operator*(const imageType &lhs, double rhs);
const imageType operator*(double lhs, const imageType &rhs);
const imageType operator+(const imageType &lhs, const imageType &rhs);
const imageType operator-(const imageType &lhs, const imageType &rhs);


