#include "matrixType.h"
//using namespace std;
#include <stddef.h>
#include <iostream>
using namespace std;

bool matrixType::copyElements (const matrixType& source)
    {
      for (int i=0; i<m_rows; i++)
            for (int j=0; j<m_cols; j++)
                m_data[i][j] = source.m_data[i][j];
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
