/*! \grayImage.hpp

    GrayImage handles reading, writing and accessing grayscale images.

    Actual image manipulation can be found in:
    + imageProcessing folder, and
    + vision folder

    ----------------------------------------------------------------------  */

#ifndef __GRAY_IMAGE_HPP__
#define __GRAY_IMAGE_HPP__

#include "config.hpp"
#include "colorImage.hpp"
#include "grid.hpp"
#include "matrix.hpp"
#include <string>
#include <algorithm>
using namespace std;

class ColorImage;
class Matrix;

class GrayImage : public Grid<unsigned char> 
{
public:
  GrayImage();
  GrayImage(size_t rows, size_t cols);
  GrayImage(const GrayImage& image);
  GrayImage(const ColorImage& image);
  GrayImage(char *filename);
  GrayImage(string filename);
  GrayImage(const Matrix &rhs);
  GrayImage(size_t rows, size_t cols, unsigned char *);

  void save(char *filename);
  void save(string filename);
  void read(char *filename);
  void read(string filename);

  //assignment and data conversion
  const GrayImage& operator= (const GrayImage &rhs);
  const GrayImage& operator= (const Matrix &rhs);
  const GrayImage& operator= (const ColorImage& rhs);
  const GrayImage& operator= (const ColorImage *rhs);
  const GrayImage& operator= (int rhs);

  Matrix getMatrix() const;
  
  //image manipulations
  double bilinear(double r, double c) const;

  int count(unsigned char value) const;	// returns the number of time value occurs
  const GrayImage erode() const;
  const GrayImage dilate() const;

  //mathematical operations
  const Matrix calcRowGradient(void) const;
  const Matrix calcColGradient(void) const;

  void smooth(int filterLength);

  //return a subset
  const GrayImage subImage(int row, int col,
			    int numRows, int numCols) const;
  void setSubImage(const GrayImage &sub, int row, int col,
		   int numRows, int numCols);

    void scale(int n_rows, int n_cols);

private:
};

//typedef GrayImage CGrayImage;

const Matrix smooth(const GrayImage &, int);
const GrayImage downsample(const GrayImage &, int factor);

void saveMatrixImage(Matrix &Ec, char *filename);



#endif //__GRAY_IMAGE_HPP__
