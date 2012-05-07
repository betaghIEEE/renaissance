/*! \volume.hpp

    A generic three-dimensional storage array. Utilizes templates so can
    be utilized for any base type, T.

    To be incorporated into:
    \li ColorImage
    \li ...
    \li ...

    ----------------------------------------------------------------------  */

#ifndef __VOLUME_HPP__
#define __VOLUME_HPP__

#include <vector>
#include <iostream>  // blm
#include <fstream>   // blm

using namespace std;

template <class T>
class Volume : public vector<vector<vector<T> > >
{
public:
  Volume();
  Volume(size_t rows, size_t cols, size_t depth);

  Volume& operator= (const Volume &rhs);
  Volume& operator= (double rhs);

  const size_t getRowSize() const;
  const size_t getColSize() const;
  const size_t getDepthSize() const;
  
  void display() const;
  void save(char *filename) const;

  void resize3D(size_t rows, size_t cols, size_t depth);

protected:
  size_t m_rows;
  size_t m_cols;
  size_t m_depth;

  bool checkSize( size_t rows, size_t cols, size_t depth );
  void allocateStorage();

private:

};

//template code here
template <class T> 
Volume<T>::Volume() : vector<vector<vector<T> > >()
{ 
  m_rows = 0;
  m_cols = 0; 
  m_depth = 0;
}

template <class T> 
Volume<T>::Volume(size_t rows, size_t cols, size_t depth)// : vector<vector<vector<T> > >(rows, cols, depth)
{
  m_rows = rows;
  m_cols = cols;
  m_depth = depth;
  allocateStorage();
}

template <class T>
Volume<T>& Volume<T>::operator= (const Volume& rhs)
{
  if (checkSize(rhs.getRowSize(), rhs.getColSize()) == false) {
    m_rows = rhs.getRowSize();
    m_cols = rhs.getColSize();
    m_depth = rhs.getDepthSize();
    allocateStorage();
  }

  for (int r=0; r<m_rows; r++)
    for (int c=0; c<m_cols; c++)
      for (int d=0; d<m_depth; d++)
	(*this)[r][c][d] = rhs[r][c][d];

  return (*this);
}

template <class T>
Volume<T>& Volume<T>::operator= (double rhs)
{
  for (int r=0; r<m_rows; r++)
    for (int c=0; c<m_cols; c++)
      for (int d=0; d<m_depth; d++)
	(*this)[r][c][d] = rhs;
 
  return (*this);
}

template <class T>
void Volume<T>::display() const
{
  for (int r=0; r<m_rows; r++) {
    for (int c=0; c<m_cols; c++) {
      for (int d=0; d<m_depth; d++)
	cout << (*this)[r][c][d] << "\t";
      cout << endl;
    }
    cout << endl << endl;
  }
}

template <class T>
void Volume<T>::save(char *filename) const
{
  ofstream out_file(filename);
  out_file << m_rows << endl;
  out_file << m_cols << endl;
  out_file << m_depth << endl;

  for (int r=0; r<m_rows; r++) {
    for (int c=0; c<m_cols; c++) 
      for (int d=0; d<m_depth; d++)
	out_file << (*this)[r][c][d] << "\t";
    out_file << endl;
  }
}


template <class T> 
const size_t Volume<T>::getRowSize() const
{
  return m_rows;
}

template <class T> 
const size_t Volume<T>::getColSize() const
{
  return m_cols;
}

template <class T> 
const size_t Volume<T>::getDepthSize() const
{
  return m_depth;
}

template <class T> 
bool Volume<T>::checkSize( size_t rows, size_t cols, size_t depth )
{
  return (rows==m_rows && cols == m_cols && depth == m_depth);
}

template <class T>
void Volume<T>::allocateStorage()
{
  size_t r, c;
  this->resize(m_rows);
  for (r=0; r<m_rows; r++)
    (*this)[r].resize(m_cols);

  for (r=0; r<m_rows; r++)
    for (c=0; c<m_cols; c++)
      (*this)[r][c].resize(m_depth);
}

template <class T>
void Volume<T>::resize3D(size_t rows, size_t cols, size_t depth)
{
  m_rows = rows;
  m_cols = cols;
  m_depth = depth;
  allocateStorage();
}

#endif //__VOLUME_HPP__
