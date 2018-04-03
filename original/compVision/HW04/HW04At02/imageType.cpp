/*
 *  imageType.cpp
 *  HW02At05
 *
 *  Created by Daniel Beatty on Mon Feb 25 2002.
 *  Copyright (c) 2001 __MyCompanyName__. All rights reserved.
 *
 */



#include "imageType.h"
#include <iostream.h>
#include <fstream.h>
#include <math.h>
#include <stdlib.h>
#include <string>


int imageType::charToInt (char temp ) 
{
    int tempInt;
    switch (temp){
        case '1': tempInt = 1;
            break;
        case '2': tempInt = 2;
            break;
        case '3': tempInt = 3;
            break;
        case '4': tempInt = 4;
            break;
        case '5': tempInt = 5;
            break;
        case '6': tempInt = 6;
            break;
        case '7': tempInt = 7;
            break;
        case '8': tempInt = 8;
            break;
        case '9': tempInt = 9;
            break;
        case '0': 
        default:    
            tempInt = 0;
            
        
        
    };
    return tempInt;
        
}

void imageType::getImagePGM2 (const char* filename)
{

	ifstream infile(filename);
        char temp;
        int tempInt;
        int rows;
        int cols;
        //string buf;
        double greyLevel;
        double value;
        
        
	if (infile.bad()) {
		cout <<"readPGM: Unable to open file! " ;
                cout << filename;
                    exit (1);
                }
	//infile >> buf;
	/*if (buf != "P2"){
		cout <<"readPGM: magic number is not P5!";
                exit (1);
                }
	*/
        infile >> temp;
        cout << temp;
        tempInt = 0;
            infile >> tempInt;
            cout << tempInt;
            //}
        cols = tempInt;
        
	//infile >> temp;
        tempInt = 0;
            infile >> tempInt;
            //}
        rows = tempInt;
        
	//infile >> temp;
        tempInt = 0;
            infile >> tempInt;
            
            //}
        greyLevel = tempInt;
        NUMBER_OF_GREY_LEVELS = tempInt;
        m_rows=rows;
        m_cols= cols;
        allocateStorage();
        
        for ( int i = 0; i < rows; i++) 
            for (int j = 0; j < cols; j++) {
                infile >> tempInt;
                value = tempInt/greyLevel;
                setValue(i,j,value);
        }
	
	if (infile.bad()){
		cout << "Unable to read entire file!";
                exit (1);
                }
	infile.close();
        cout << "Read the file";
}

bool imageType::saveImagePGM2 (const char * filename)
{
	int i;
	ofstream outfile(filename);
	outfile << "P2" << endl;
	outfile << m_cols << " " << m_rows << endl;
	outfile << 255 << endl; //this is 255
	
	for (int r=0; r<m_rows; r++) {
		for (int c=0; c<m_cols; c++)
                {
                    i = ceil (m_data[r][c] * 255);
                    if ( i > 255) i = 255;
                    if (i < 0 ) i = 0;
                    outfile << i;
                    outfile << " ";
                }
                outfile << endl;
            }

	outfile.close();
        return 1;
}

imageType::imageType(int rows, int cols)
{
  m_rows = rows;
  m_cols = cols;
  allocateStorage();
}

imageType::imageType(const imageType& source)
{
  m_rows = source.m_rows;
  m_cols = source.m_cols;
  allocateStorage();
  copyElements(source);
}

imageType::~imageType(void)
{
  deleteStorage();
}

imageType& imageType::operator= (const imageType& rhs)
{
  if (this != &rhs) {
    if (m_rows != rhs.m_rows || m_cols != rhs.m_cols) {
      deleteStorage();
      m_rows = rhs.getRowSize();
      m_cols = rhs.getColSize();
      allocateStorage();
    }
    copyElements(rhs);
  }
  return (*this);
}

imageType& imageType::operator+=(const imageType& rhs)
{
  checkSize(rhs.m_rows, rhs.m_cols);

  for (int i=0; i<m_rows; i++)
    for (int j=0; j<m_cols; j++)
      m_data[i][j] += rhs[i][j];

  return (*this);
}

imageType& imageType::operator-=(const imageType& rhs)
{
  checkSize(rhs.m_rows, rhs.m_cols);

  for (int i=0; i<m_rows; i++)
    for (int j=0; j<m_cols; j++)
      m_data[i][j] -= rhs[i][j];

  return (*this);
}

imageType& imageType::operator*=(const imageType& rhs)
{
  if (m_cols != rhs.getRowSize())
    error("operator*=: matrices not same inner size");

  double total;
  int rhsCol = rhs.getColSize();

  imageType result(m_rows, rhsCol);
  for (int i=0; i< m_rows; i++)
    for (int j=0; j< rhsCol; j++) {
      total = 0;
      for (int k=0; k< m_cols; k++)
	total += m_data[i][k]*rhs[i][j];
      result[i][j] = total;
    }

  (*this) = result;
  return (*this);
}

imageType& imageType::operator*=(double rhs)
{
  for (int i=0; i<m_rows; i++)
    for (int j=0; j<m_cols; j++)
      m_data[i][j] *= rhs;

  return (*this);
}

imageType& imageType::operator/=(double rhs)
{
  for (int i=0; i<m_rows; i++)
    for (int j=0; j<m_cols; j++)
      m_data[i][j] /= rhs;

  return (*this);
}

imageType& imageType::operator= (double rhs)
{
  for (int i=0; i<m_rows; i++)
    for (int j=0; j<m_cols; j++)
      m_data[i][j] = rhs;
  return (*this);
}

imageType& imageType::operator+=(double rhs)
{
  for (int i=0; i<m_rows; i++)
    for (int j=0; j<m_cols; j++)
      m_data[i][j] += rhs;

  return (*this);
}

imageType& imageType::operator-=(double rhs)
{
  for (int i=0; i<m_rows; i++)
    for (int j=0; j<m_cols; j++)
      m_data[i][j] -= rhs;

  return (*this);
}

void imageType::resize(int rows, int cols)
{
  deleteStorage();
  m_rows = rows;
  m_cols = cols;
  allocateStorage();
}

bool imageType::checkSize(int rows, int cols)
{
  return ((rows>=0)&&(rows<m_rows))&&((cols>=0)&&(cols<m_cols)); 
}

//mathematical operations
double imageType::calcAverage(void)
{
  double total;
  total = 0;
  for (int r=0; r<m_rows; r++)
    for (int c=0; c<m_cols; c++)
      total += (double) m_data[r][c];
  return total/(m_rows*m_cols);
}

double imageType::calcSumSqrAverage(void)
{
  double total;
  total = 0;
  for (int r=0; r<m_rows; r++)
    for (int c=0; c<m_cols; c++)
      total += ((double) m_data[r][c]* (double) m_data[r][c]);
  return total/(m_rows*m_cols);
}


void imageType::setUniformRandom(double range)
{
  for (int r=0; r<m_rows; r++)
    for (int c= 0; c<m_cols; c++)
      m_data[r][c] = (range*rand())/RAND_MAX;
}

void imageType::setIdentity(void)
{
  for (int r=0; r<m_rows; r++)
    for (int c=0; c<m_cols; c++)
      m_data[r][c] = (r==c);
}

double imageType::calcSumSquared(void)
{
  double total = 0;
  for (int r=0; r<m_rows; r++)
    for (int c=0; c<m_cols; c++)
      total += (m_data[r][c]*m_data[r][c]);
  return total;
}

const imageType imageType::transpose()
{
  int r,c;

  imageType u(m_cols, m_rows);
  for (r=0; r<m_cols; r++)
    for (c=0; c<m_rows; c++)
      u.setValue(r,c,m_data[c][r]);

  return u;
}

imageType imageType::calcRowGradientFast () {
    imageType temp = imageType (m_rows ,m_cols);
    double value = 0;
    int i = 0;
    int j = 0;
    int m = m_rows;
    int n = m_cols;
    
    // begin loop
    for ( ; i < n; i++) 
        for (j=0 ; j < m; j++) {
            if ((j > 0) && (j < (n-1)))
                value = m_data[j+1][i] - m_data[j-1][i];
            else if ( j == 0)
                value = m_data[j+1][i];
            else 
                value = 0 - m_data[j-1][i];
            value = value/2;
            temp.setValue(j,i,value);
        }
    
    return temp;    
}

imageType imageType::calcColGradientFast(){
    imageType temp = imageType (m_rows ,m_cols);
    double value = 0;
    int i = 0;
    int j = 0;
    int m = m_rows;
    int n = m_cols;
    
    // begin loop
    for ( ; i < m; i++) 
        for (j=0 ; j < n; j++) {
            if ((j > 0) && (j < (n-1))) 
                value = m_data[i][j+1]-m_data[i][j-1];
            else if ( j == 0)
                value = m_data[i][j+1];
            else 
                value = - m_data[i][j-1];
            value = value/2;
            temp.setValue(i,j,value);
        }
    
    return temp;
}

void imageType::squareEachElement ()
{
    double value = 0;
    int i = 0;
    int j = 0;
    int m = m_rows;
    int n = m_cols;
    for (i = 0; i < m; i++)
        for (j=0 ; j < n; j++) {
            value = m_data[i][j];
            value *= value;
            m_data[i][j] = value;
        }

}

imageType imageType::convolve (imageType &mask) 
{
    int i;
    int j;
    int k;
    int l;
    int maxI = getRowSize();
    int maxJ = getColSize();
    int maxK = mask.getRowSize();
    int maxL = mask.getColSize();
    double tempMult;
    double tempValue;
    double tempMaskVal;
    imageType tempImage (maxI, maxJ);
    for (i = 0; i < maxI; i++)
        for (j = 0; j < maxJ; j++) tempImage.setValue(i,j,0);
    maxI -= 3;
    maxJ -= 3;
    
    for (i = 0; i < maxI; i++)
        for (j = 0; j < maxJ; j++) {
            
            
            for (k = 0; k < maxK; k++)
                for (l = 0; l < maxL; l++)
                    {
                        tempMult = getValue (i,j);
                        tempValue = tempImage.getValue(i+k,j+l);
                        tempMaskVal = mask.getValue(k,l);
                        tempValue += tempMult * tempMaskVal;
                        tempImage.setValue (i,j,tempValue);
                    }
            
        }
        return tempImage;
}

void imageType::display(void)
{
  for (int r=0; r<m_rows; r++) {
    for (int c=0; c<m_cols; c++)
      cout << m_data[r][c] << "\t";

    cout << endl;
  }
}

bool imageType::allocateStorage(void)
{
  if (m_rows <=0 || m_cols <=0) {
    m_data = NULL;
    return false;
  }

  m_data = new double*[m_rows];
  m_data[0] = new double[m_rows*m_cols];
  for (int i=1; i<m_rows; i++)
    m_data[i] = m_data[i-1]+m_cols;
  return true;
}

bool imageType::deleteStorage(void)
{
  if (m_data != NULL) {
    delete []m_data[0];
    delete []m_data;
    m_data = NULL;
  }
  return true;
}

bool imageType::addRow () {
    int i = m_rows -1;
    int j = i+1;
    m_data[j] = m_data[i]+m_cols;
    return true;
}

bool imageType::getRow (int row, double * output) 
{
    
    for ( int j = 0; j < m_cols; j++) {
        output[j] = getValue(row, j); 
    }
    return true;
}

void imageType::copyMatrixToLapack(int rows, int cols, double * output)
{
  int r, c;
  int count = 0;
  for (c=0; c<cols; c++)
    for (r=0; r<rows; r++)
      output[count++] = getValue(r,c);
}

void imageType::copyLapackToMatrix(int rows, int cols, double *input)
{
  int r, c;
  int count = 0;
  for (c=0; c<cols; c++)
    for (r=0; r<rows; r++)
      setValue (r,c, input[count++]);
}

bool imageType::copyElements(const imageType& source)
{
  for (int i=0; i<m_rows; i++)
    for (int j=0; j<m_cols; j++)
      m_data[i][j] = source[i][j];
  return true;
}

void imageType::error(const char *errorMsg)
{
  cout << "ImageType: " << errorMsg << endl;
  exit(1);
}


ostream& operator<<(ostream& s, const imageType& source)
{
  for (int i=0; i<source.getRowSize(); i++) {
    for (int j=0; j<source.getColSize(); j++)
      s << source[i][j] << "\t";
    s << endl;
  }
  return s;
}

const imageType operator*(const imageType& lhs, const imageType& rhs)
{
  imageType u = lhs;
  u *= rhs;
  return u;
}

const imageType operator*(const imageType &lhs, double rhs)
{
  imageType M = lhs;
  M *= rhs;
  return M;
}

const imageType operator*(double lhs, const imageType &rhs)
{
  imageType M = rhs;
  M *= lhs;
  return M;
}

const imageType operator+(const imageType &lhs, const imageType &rhs)
{
  imageType M;
  M = lhs;
  M += rhs;
  return M;
}

const imageType operator-(const imageType &lhs, const imageType &rhs)
{
  imageType M;
  M = lhs;
  M -= rhs;
  return M;
}
