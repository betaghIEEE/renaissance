/*! \file colorImage.hpp

    CColorImage handles reading, writing and accessing color images.

    Actual image manipulation can be found in:
    + imageProcessing folder, and
    + vision folder

    It handles the following images:
    \li JPEG
    \li PPM

*/

/*

    Modifications:

    06/21/03 EDS Renamed all to ColorImage.
                 But a typedef CCColorImage
		 for backwards compatability.

    2/2/05   Sak Added m_rows, m_cols, m_depth, pixels
             Sak Added readJPEG, readPPM, readBMP, savePPM
	     Sak only saves as PPM for now

    ----------------------------------------------------------------------  */

#ifndef __COLOR_IMAGE_HPP__
#define __COLOR_IMAGE_HPP__

#include "config.hpp"
#include "grayImage.hpp"
#include "volume.hpp"
#include "matrix.hpp"
#include "vector3D.hpp"
#include <valarray>
#include <string>
#include <algorithm>
using namespace std;


//#ifdef __linux__
#include <tiffio.h>
#include <png.h>
extern "C"
{
#include <jpeglib.h>
}
//#endif

#define RED   0
#define GREEN 1
#define BLUE  2
#define ALPHA 3

typedef struct 
{
  unsigned char red;
  unsigned char green;
  unsigned char blue;
  unsigned char alpha;
} tRGBA;

typedef struct
{
  unsigned char red;
  unsigned char green;
  unsigned char blue;
} tRGB;

class GrayImage;

class ColorImage : public Volume<unsigned char> 
{
public:
  ColorImage();
  ColorImage(size_t rows, size_t cols);
  ColorImage(unsigned char *buffer, size_t rows, size_t cols);
  ColorImage(tRGBA *buffer, size_t rows, size_t cols);
  ColorImage(char *filename);
  ColorImage(const ColorImage&);
  ColorImage(string filename);

  virtual ~ColorImage();

  void save(string filename, string comments = "");
  void save(char *filename, char * comments ="");
  void saveFormat(char *filename, char *format, char * comments="");
  void read(string filename);
  void read(char *filename);
  void readBMP(char *filename);
  void readPPM(char *filename);
  void readTIFF(char *filename);
  void readPGM(char *filename);
  void savePPM(char *filename, char *comments = NULL) const;
  void savePGM(char *filename, char *comments = NULL) const;
  void saveTIFF(char *filename) const;
  void FillPixelBuffer(unsigned char *);
  void FillPixelBufferReverse(unsigned char *);
    void scale(int n_rows , int n_cols);

//#ifdef __linux__
  void readJPEG(char *filename);
  void saveJPEG(char *filename) const;
  void savePNG(char *filename) const;
  void readPNG(char *filename);
//#endif

  //data conversion
  //const ColorImage& operator= (CMatrix &rhs);
  ColorImage& operator= (const ColorImage&);
  ColorImage& operator= (const GrayImage&);
  ColorImage& operator= (const unsigned char);
  
  GrayImage& operator()(int) const;

  ColorImage &ColorImage::FlipY();

  //operations
  void smooth(int size);

  string getComments(void) const;

  //numerical operations
  const Vector3D bilinear(double r, double c) const;

  //return a subset
  const ColorImage subImage(int row, int col, int numRows, int numCols) const;
  void setSubImage(const ColorImage &sub, int row, int col, int numRows, int numCols);
  
private:
  string m_comment;
  unsigned char *pixels; //linear storage of pixel data
};

//for old dependence on CColorImage
typedef ColorImage CColorImage;

//data converters

//other operations
const ColorImage downsample(ColorImage &image);//, int factor);
const ColorImage RGBtoHSV(const ColorImage& I);//, unsigned char *rgbBytes);
const ColorImage RGBAtoRGB(const ColorImage &I);
const ColorImage HSVtoRGB(const ColorImage& I);
const ColorImage extremeHSV(const ColorImage &I);
vector<int> convertHSVtoRGB(double hue, double sat, double v);

const ColorImage resizeImage(ColorImage&, int, int);

#endif //__COLOR_IMAGE_HPP__
