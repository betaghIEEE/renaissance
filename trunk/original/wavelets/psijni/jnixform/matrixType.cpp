#include "matrixType.h"
//using namespace std;
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream.h>
#include <math.h>

matrixType::matrixType () 
{
    m_rows = 0; 
    m_cols=0; 
    m_data = NULL;
    defineEpsilon ();
}

void matrixType::defineEpsilon ()
{
    epsilon = 1/pow(2.0, 32.0);
}

bool matrixType::copyElements (const matrixType& source)
    {
      for (int i=0; i<m_rows; i++)
            for (int j=0; j<m_cols; j++)
                m_data[i][j] = source.m_data[i][j];
        shiftX = source.shiftX;
        shiftY = source.shiftY;
        scaleX = source.scaleX;
        scaleY = source.scaleY;
        return true;
    }
    
    matrixType& matrixType::operator= (const matrixType& rhs)
    {
        if (this != &rhs) {
        if (m_rows != rhs.m_rows || m_cols != rhs.m_cols) {
                deleteStorage();
                m_rows = rhs.m_rows;
                m_cols = rhs.m_cols;
                allocateStorage();
            }
            copyElements(rhs);
            
        }
        return (*this);
    }
    
    
    bool matrixType::allocateStorage ()
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
    
    bool matrixType::allocateStorage (int rows, int cols)
    {
        m_rows = rows;
        m_cols = cols;
        return allocateStorage();
    }
    
    bool matrixType::deleteStorage ()
    {
        if (m_data != NULL) {
            delete []m_data[0];
            delete []m_data;
            m_data = NULL;
        }
        return true;
    }
    
bool matrixType::zeroize ()
{
    int i, j;
    for ( i = 0; i < m_rows; i++)
        for ( j = 0; j < m_cols; j++)
            m_data[i][j] = 0;
}
    
matrixType& matrixType::operator+=(const matrixType& rhs)
{
  checkSize(rhs.m_rows, rhs.m_cols);

  for (int i=0; i<m_rows; i++)
    for (int j=0; j<m_cols; j++)
      m_data[i][j] += rhs.m_data[i][j];
        

  return (*this);
}


/*
matrixType& matrixType::operator-=(const imageType& rhs)
{
  checkSize(rhs.m_rows, rhs.m_cols);

  for (int i=0; i<m_rows; i++)
    for (int j=0; j<m_cols; j++)
      m_data[i][j] -= rhs.m_data[i][j];

  return (*this);
}
*/

matrixType& matrixType::operator*=(const matrixType& rhs)
{
  if (m_cols != rhs.m_rows)
    error("operator*=: matrices not same inner size");

  double total;
  int rhsCol = rhs.m_rows;

  matrixType result(m_rows, rhsCol);
  for (int i=0; i< m_rows; i++)
    for (int j=0; j< rhsCol; j++) {
      total = 0;
      for (int k=0; k< m_cols; k++)
	total += m_data[i][k]*rhs.m_data[k][j];
      result.m_data[i][j] = total;
    }

  (*this) = result;
  return (*this);
}

matrixType& matrixType::sparsematrixmultiply (const matrixType &lhs, const matrixType &rhs)
{
    int k = lhs.m_cols;
    int i,j;
    int ival, jval;
    //matrixlist *sparsemat = new matrixlist[k];
    // Load sparse matrix 
    
    // Perform sparse multiply
    for ( i = 0; i < lhs.m_rows; i++)
        for ( j = 0; j < rhs.m_cols; j++)
        {
            
        }
}


matrixType& matrixType::operator*=(double rhs)
{
  for (int i=0; i<m_rows; i++)
    for (int j=0; j<m_cols; j++)
      m_data[i][j] *= rhs;

  return (*this);
}



matrixType& matrixType::operator/=(double rhs)
{
  for (int i=0; i<m_rows; i++)
    for (int j=0; j<m_cols; j++)
      m_data[i][j] /= rhs;

  return (*this);
}


matrixType& matrixType::operator+=(double rhs)
{
  for (int i=0; i<m_rows; i++)
    for (int j=0; j<m_cols; j++)
      m_data[i][j] += rhs;

  return (*this);
}



matrixType& matrixType::operator-=(double rhs)
{
  for (int i=0; i<m_rows; i++)
    for (int j=0; j<m_cols; j++)
      m_data[i][j] -= rhs;

  return (*this);
}


const matrixType operator*(const matrixType& lhs, const matrixType& rhs)
{
  matrixType u = lhs;
  u *= rhs;
  return u;
}


const matrixType operator*(const matrixType &lhs, double rhs)
{
  matrixType M = lhs;
  M *= rhs;
  return M;
}

const matrixType operator*(double lhs, const matrixType &rhs)
{
  matrixType M = rhs;
  M *= lhs;
  return M;
}


const matrixType operator+(const matrixType &lhs, const matrixType &rhs)
{
  matrixType M;
  M = lhs;
  M += rhs;
  return M;
}


const matrixType operator-(const matrixType &lhs, const matrixType &rhs)
{
  matrixType M;
  M = lhs;
  M += -1 * (rhs);
  return M;
}


bool matrixType::checkSize(int rows, int cols)
{
  return ((rows>=0)&&(rows<m_rows))&&((cols>=0)&&(cols<m_cols)); 
}

void matrixType::error(const char *errorMsg)
{
  cout << "ImageType: " << errorMsg << endl;
  exit(1);
}


void matrixType::GJ ()
{
    int i, j, l, k;
    double temp;
    for ( i = 0; i < m_rows; i++)
    {
        k = i;
        if  ((m_data[k][i] < epsilon ) && (m_data[k][i] > (-epsilon)))
        {
            while (( k < m_rows) && (m_data[k][i] < epsilon ) && (m_data[k][i] > (-epsilon)) )
        	k++;
            for ( j = i; j < m_cols; j++)
            {
                temp = m_data[k][j];
                m_data[k][j] = m_data[i][j];
                m_data[i][j] = m_data[k][j];
            }
        }
        temp = 1/ m_data[i][i];
        for ( k = i + 1; k < m_cols; k++)
            m_data[i][k] *= temp;
        m_data[i][i] = 1;
        for ( k = i + 1; k < m_rows; k++)
        {
            if ((m_data[k][i] < epsilon ) && (m_data[k][i] > (-epsilon)) )
            {
                temp = 1/m_data[k][i];
                for ( l = 0; l < m_cols; l++)
                {
                    m_data[k][l] *= -temp;
                    m_data[k][l] += m_data[i][l]; 
                }
                m_data[k][i] = 0;
                
            }
        }
        m_data[i][i] = 1.0;
    }
}

double matrixType::getEnergy ()
{
    int i, j;
    double sumcol = 0;
    double sumrow = 0;
    for ( i = 0; i < m_rows; i++) {
        sumcol = 0;
        for ( j = 0 ; j < m_cols; j++)
        {
            sumcol += m_data[i][j] * m_data[i][j] / m_cols;
        }
        sumrow += sumcol / m_rows;
    }
    return sumrow;
}

double matrixType::getL1Energy ()
{
    int i, j;
    double sumcol = 0;
    double sumrow = 0;
    for ( i = 0; i < m_rows; i++) {
        sumcol = 0;
        for ( j = 0 ; j < m_cols; j++)
        {
            sumcol += m_data[i][j];
        }
        sumrow += sumcol;
    }
    return sumrow;
}
