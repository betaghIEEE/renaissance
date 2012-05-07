/*	-----------------------------------------------------------------------
	grayImage.cpp

	Notes:
	1/31/05  Addes M_PI definition			 

	2/3/05   Sak 
	         Only loads/saves pgm format.  Use ColorImage for color formats
		 All format loading functionality is in Grid class.  

	----------------------------------------------------------------------- */

#include "grayImage.hpp"

#include <string.h>
#include <string>
#include <math.h>

using namespace std;

GrayImage::GrayImage() : Grid<unsigned char>()
{}

GrayImage::GrayImage(size_t rows, size_t cols) : Grid<unsigned char>(rows, cols)
{}

GrayImage::GrayImage(const GrayImage& image) : Grid<unsigned char>(image.getRowSize(), image.getColSize())
{
  for (int r=0; r<m_rows; r++)
    for (int c=0; c<m_cols; c++)
      (*this)[r][c] = image[r][c];
}

GrayImage::GrayImage(const ColorImage& image) : Grid<unsigned char>(image.getRowSize(), image.getColSize())
{
  double gray = 0;

  for (int r=0; r<m_rows; r++)
    for (int c=0; c<m_cols; c++) {
      gray = 0.30*double(image[r][c][RED]);
      gray += 0.59*double(image[r][c][GREEN]);
      gray += 0.11*double(image[r][c][BLUE]);
      if (gray < 0)
	gray = 0;
      if (gray > 255)
	gray = 255;
      (*this)[r][c] = (unsigned char)(gray + 0.5);
    }
}


GrayImage::GrayImage(char *filename)
{
  read(filename);
}

GrayImage::GrayImage(string filename)
{
  read((char *)filename.c_str());
}

GrayImage::GrayImage(const Matrix &rhs)
{
  m_rows = rhs.getRowSize();
  m_cols = rhs.getColSize();
  allocateStorage();
 
  for (int r=0; r<m_rows; r++)
    for (int c=0; c<m_cols; c++)
      (*this)[r][c] = (unsigned char) rhs(r,c);
}

GrayImage::GrayImage(size_t rows, size_t cols, unsigned char *p) : Grid<unsigned char>(rows, cols)
{
  for (int r=0; r<m_rows; r++)
    for (int c=0; c<m_cols; c++)
      (*this)[r][c] = p[(r*m_cols)+c];
}

void GrayImage::save(char *filename)
{
  saveImage(filename);
}

void GrayImage::save(string filename)
{
  saveImage((char *)filename.c_str());
}

void GrayImage::read(char *filename)
{
  readImage(filename);
}

void GrayImage::read(string filename)
{
  read((char *)filename.c_str());
}

//data conversion
const GrayImage& GrayImage::operator= (const GrayImage &rhs)
{
 
  if (checkSize(rhs.getRowSize(), rhs.getColSize()) == false) {
    m_rows = rhs.getRowSize();
    m_cols = rhs.getColSize();
    allocateStorage();
  }

  for (int r=0; r<m_rows; r++)
    for (int c=0; c<m_cols; c++)
      (*this)[r][c] = rhs[r][c];

  return (*this);
}

//data conversion
const GrayImage& GrayImage::operator= (const ColorImage &rhs)
{

  double gray = 0;

  if (checkSize(rhs.getRowSize(), rhs.getColSize()) == false) {
    m_rows = rhs.getRowSize();
    m_cols = rhs.getColSize();
    allocateStorage();
  }

  for (int r=0; r<m_rows; r++)
    for (int c=0; c<m_cols; c++)
      {
	gray = 0.30*double(rhs[r][c][RED]);
	gray += 0.59*double(rhs[r][c][GREEN]);
	gray += 0.11*double(rhs[r][c][BLUE]);
	(*this)[r][c] = (unsigned char)(gray + 0.5);
      }

  return (*this);
}

//data conversion
const GrayImage& GrayImage::operator= (const ColorImage *rhs)
{

  double gray = 0;

  if (checkSize(rhs->getRowSize(), rhs->getColSize()) == false) {
    m_rows = rhs->getRowSize();
    m_cols = rhs->getColSize();
    allocateStorage();
  }

  for (int r=0; r<m_rows; r++)
    for (int c=0; c<m_cols; c++)
      {
	gray = 0.30*double((*rhs)[r][c][RED]);
	gray += 0.59*double((*rhs)[r][c][GREEN]);
	gray += 0.11*double((*rhs)[r][c][BLUE]);
	(*this)[r][c] = (unsigned char)(gray + 0.5);
      }

  return (*this);
}

//data conversion
const GrayImage& GrayImage::operator= (const Matrix &rhs)
{
  if (checkSize(rhs.getRowSize(), rhs.getColSize()) == false) {
    m_rows = rhs.getRowSize();
    m_cols = rhs.getColSize();
    allocateStorage();
  }

  for (int r=0; r<m_rows; r++)
    for (int c=0; c<m_cols; c++)
      (*this)[r][c] = (unsigned char) rhs(r,c);

  return (*this);
}
const GrayImage& GrayImage::operator= (int rhs)
{
  for (int r=0; r<m_rows; r++)
    for (int c=0; c<m_cols; c++)
      (*this)[r][c] = rhs;

  return (*this);
}

const Matrix GrayImage::calcRowGradient() const
{
  int r, c;
  Matrix rowGrad(m_rows, m_cols);

  for (r=1; r<m_rows-1; r++)
    for (c=0; c<m_cols; c++)
      rowGrad(r,c) = ((*this)[r+1][c] - (*this)[r-1][c])/2.0;

  for (c=0; c<m_cols; c++) {
    rowGrad(0,c) = ((*this)[1][c] - (*this)[0][c])/1.5;
    rowGrad(m_rows-1,c) = ((*this)[m_rows-1][c] - (*this)[m_rows-2][c])/1.5;
  }

  return rowGrad;
}

const Matrix GrayImage::calcColGradient() const
{
  int r, c;
  Matrix colGrad(m_rows, m_cols);

  for (r=0; r<m_rows; r++)
    for (c=1; c<m_cols-1; c++)
      colGrad(r,c) = ((*this)[r][c+1] - (*this)[r][c-1])/2.0;

  for (r=0; r<m_rows; r++) {
    colGrad(r,0) = ((*this)[r][1] - (*this)[r][0])/1.5;
    colGrad(r,m_cols-1) = ((*this)[r][m_cols-1] - (*this)[r][m_cols-2])/1.5;
  }

  return colGrad;
}

double GrayImage::bilinear(double r2, double c2) const
{
  int r = int(r2);
  int c = int(c2);

  //first check boundary conditions
  if ((r<0) || (r>m_rows-2) || (c<0) || (c>m_cols-2))
    //invalid ... outside boundaries
    return -1;

  double t = (c2-c);
  double u = (r2-r);

  return ((1-t)*(1-u)*(*this)[r][c]+ 
	  (1-t)*u*(*this)[r+1][c]+ 
	  t*u*(*this)[r+1][c+1] + 
	  t*(1-u)*(*this)[r][c+1]);
}

GrayImage toImage(Grid<bool> &booleanImage)
{
  int rows = booleanImage.getRowSize();
  int cols = booleanImage.getColSize();

  GrayImage image(rows, cols);

  for (int r=0; r<rows; r++)
    for (int c=0; c<cols; c++) 
      image[r][c] = (unsigned char) (255*booleanImage[r][c]);
  
  return image;
}
GrayImage toImage(Matrix &matrix)
{
  int rows = matrix.getRowSize();
  int cols = matrix.getColSize();

  GrayImage image(rows, cols);

  for (int r=0; r<rows; r++)
    for (int c=0; c<cols; c++) 
      image[r][c] = (unsigned char) (255*matrix(r,c));
  
  return image;
}
Matrix toMatrix(const GrayImage &image) 
{
  int rows = image.getRowSize();
  int cols = image.getColSize();

  Matrix matrix(rows, cols);

  for (int r=0; r<rows; r++)
    for (int c=0; c<cols; c++) 
      matrix(r,c) = image[r][c];
  
  return matrix;
}

Matrix GrayImage::getMatrix() const
{
  return toMatrix(*this);
}

const GrayImage downsample(const GrayImage& image, int factor)
{
  int r, c;
  int i,j;

  double total;
  int area;

  int rows = image.getRowSize()/factor;
  int cols = image.getColSize()/factor;

  GrayImage X(rows, cols);

  for (r=0; r<rows; r++)
    for (c=0; c<cols; c++) {
      total = 0.0;
      int area = 0;
      for (i=0; i<factor; i++)
	for (j=0; j<factor; j++)
	  if (r*factor+i < image.getRowSize() && c*factor+j < image.getColSize()) {
	    total += image[r*factor+i][c*factor+j];
	    area++;
	  }
      if (area ==0)
	total = 0;
      else
	total /= area;
      X[r][c] = (unsigned char)total;
    }

  return X;
}

void GrayImage::smooth(int filterLength)
{
  int r, c, l;
  double total;
  double x;

  vector<double> filter(2*filterLength+1);

  total = 0.0;
  for (r=0; r<2*filterLength+1; r++) {
    x = r-filterLength;
    x /= filterLength;
    filter[r] = 1/sqrt(2*M_PI)*exp(-x*x);
    total += filter[r];
  }

  for (r=0; r<2*filterLength+1; r++) 
    filter[r] /= total;

  Matrix temp(m_rows, m_cols);

  for (r=filterLength; r<m_rows-filterLength; r++) 
    for (c=0; c<m_cols; c++) {
      total = 0.0;
      for (l=-filterLength; l<=filterLength; l++)
	total += (*this)[r+l][c]*filter[l+filterLength];
      temp(r,c) =  total;
    }
  for (r=filterLength-1; r>=0; r--) 
    for (c=0; c<m_cols; c++) {
      temp(r,c) = temp(r+1,c);
      temp(m_rows-1-r,c) = temp(m_rows-1-r-1,c);
    }

  for (c=filterLength; c<m_cols-filterLength; c++)
    for (r=0; r<m_rows; r++) {
      total = 0.0;
      for (l=-filterLength; l<=filterLength; l++)
	total += temp(r,c+l)*filter[l+filterLength];
      (*this)[r][c] = (unsigned char) total;
    }

  for (c=filterLength-1; c>=0; c--)
    for (r=0; r<m_rows; r++) {
      (*this)[r][c] = (*this)[r][c+1];
      (*this)[r][m_cols-1-c] = (*this)[r][m_cols-1-c-1];
    }
}

const GrayImage GrayImage::subImage(int row, int col, 
				      int numRows, int numCols) const
{
  int r, c, d;
  int row2 = __min(__max(row, 0), int(m_rows)-1);
  int col2 = __min(__max(col, 0), int(m_cols)-1);

  int numRows2 = numRows - (row2-row);
  numRows2 = __min(int(m_rows)-row2, numRows2);

  int numCols2 = numCols - (col2-col);
  numCols2 = __min(int(m_cols)-col2, numCols2);

  GrayImage X(numRows2, numCols2);
  for (r=0; r<numRows2; r++)
    for (c=0; c<numCols2; c++) 
      X[r][c] =  (*this)[row2+r][col2+c];

  return X;
}

void GrayImage::setSubImage(const GrayImage &sub, int row, int col, int numRows, int numCols)
{
  int r, c, d;
  int row2, col2, numRows2, numCols2;

  //first check sizes for memory overloads
  row2 = __max(row, 0);
  row2 = __min(row2, int(m_rows-1));
  col2 = __max(col, 0);
  col2 = __min(col2, int(m_cols-1));

  numRows2 = numRows - (row2-row);
  numRows2 = __min(int(m_rows)-row2, numRows2);

  numCols2 = numCols - (col2-col);
  numCols2 = __min(int(m_cols)-col2, numCols2);

  //now copy it
  for (r=0; r<numRows2; r++)
    for (c=0; c<numCols2; c++) 
      (*this)[r+row2][c+col2] =  sub[r][c];
}



int GrayImage::count(unsigned char value) const
{
  int count = 0;
  for (int r=0; r<m_rows; r++)
    for (int c=0; c<m_cols; c++)
      if ((*this)[r][c] == value)
	count++;

  return count;
}

const GrayImage GrayImage::erode() const
{
  GrayImage X(m_rows, m_cols);
  bool flag;
  for (int r=0; r<m_rows; r++)
    for (int c=0; c<m_cols; c++) {

      flag = (*this)[r][c] > 0;

      if (r>0)
	flag = flag && ((*this)[r-1][c]>0);
      if (r<m_rows-1)
	flag = flag && ((*this)[r+1][c]>0);
      if (c>0)
	flag = flag && ((*this)[r][c-1]>0);
      if (c<m_cols-1)
	flag = flag && ((*this)[r][c+1]>0);

      if (flag == true)
	X[r][c] = 255;
      else
	X[r][c] = 0;
    }

  return X;
}

const GrayImage GrayImage::dilate() const
{
  GrayImage X(m_rows, m_cols);
  bool flag;
  for (int r=0; r<m_rows; r++)
    for (int c=0; c<m_cols; c++) {

      flag = (*this)[r][c] > 0;

      if (r>0)
	flag = flag || ((*this)[r-1][c]>0);
      if (r<m_rows-1)
	flag = flag || ((*this)[r+1][c]>0);
      if (c>0)
	flag = flag || ((*this)[r][c-1]>0);
      if (c<m_cols-1)
	flag = flag || ((*this)[r][c+1]>0);

      if (flag == true)
	X[r][c] = 255;
      else
	X[r][c] = 0;
    }

  return X;
}

void GrayImage::scale(int n_rows , int n_cols)
{
    vector< vector<unsigned char> > temp_buffer;
    int ri, ci; //row and col index into old image when selecting pixel value for new image
    int r, c;

    temp_buffer.resize(n_rows);
    for(int i = 0; i < n_cols; i++)
	temp_buffer[i].resize(n_cols);

    //do upscaling
    for(r = 0; r < n_rows; r++)
    {
	for(c = 0; c < n_cols; c++)
	{
	    //this is really slow b/c we're doing so many castings from int to float then back to int...maybe get rid of it later if it
	    //causes huge speed problems
	    ri = int(floor(float(r) / float(n_rows) * float(m_rows)));
            ci = int(floor(float(c) / float(n_cols) * float(m_cols)));
	    temp_buffer[r][c] = (*this)[ri][ci];
	}
    }

    //copy temp_buffer to current buffer
    this->resize2D(n_rows, n_cols);
    for(r = 0; r < m_rows; r++)
    {
	for(c = 0; c < m_cols; c++)
	{
	    (*this)[r][c] = temp_buffer[r][c];
	}
    }
}
void saveMatrixImage(Matrix &Ec, char *filename)
{
  Matrix A = 255.*(Ec-Ec.min())/(Ec.max()-Ec.min());
  GrayImage outImage = A;
  outImage.save(filename);
}
