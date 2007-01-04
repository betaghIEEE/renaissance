/*!  ----------------------------------------------------------------------
    colorImage.cpp
    --------------

    CColorImage handles reading, writing and accessing color images.

    Actual image manipulation can be found in:
    + imageProcessing folder, and
    + vision folder
    

    Notes:

    Modifications:

	1/31/05  Sak Removed ability to save formats for now.  Reading uses the CImage class

	2/2/05   Sak Removing dependency on CImage class.  
	         Sak Added readBMP,readJPEG,readPPM,savePPM
		 Sak Only saves in PPM for now

	2/3/05   Sak All format loading functionality is in this file.
                     PPM saver now works.  Fixed bug where pixels pointer could
                     be empty.

        3/7/05   Sak SUCCESS. WE HAVE PNG!
	
	3/8/05   Sak JPEG reading/writing added

	3/9/05   Sak TIFF saving implemented

    ----------------------------------------------------------------------  */

#include "colorImage.hpp"

#include <string.h>
#include <string>

using namespace std;

ColorImage::ColorImage() : Volume<unsigned char>()
{
    pixels = NULL;
}

ColorImage::ColorImage(size_t rows, size_t cols) : Volume<unsigned char>(rows, cols, 3)
{
    pixels = NULL;
}

//ColorImage::ColorImage(const ColorImage& image) : CGrid<unsigned char>(image)
//{}

ColorImage::ColorImage(char *filename)
{
    pixels = NULL;
  read(filename);
}

ColorImage::ColorImage(unsigned char *buffer, size_t rows, size_t cols) : Volume<unsigned char>(rows,cols,3)
{
    pixels = NULL;
  int r = 0;
  int c = 0;

  for (r=0;r<rows;r++)
    for (c=0;c<cols;c++)
      {
	(*this)[r][c][RED] = buffer[(r*cols+c)*3+RED];
	(*this)[r][c][GREEN] = buffer[(r*cols+c)*3+GREEN];
	(*this)[r][c][BLUE] = buffer[(r*cols+c)*3+BLUE];

      }

}

ColorImage::ColorImage(tRGBA *buffer, size_t rows, size_t cols) : Volume<unsigned char>(rows,cols,4)
{
    pixels = NULL;
  int r = 0;
  int c = 0;

  for (r=0;r<rows;r++)
    for (c=0;c<cols;c++)
      {
	(*this)[r][c][RED] = buffer[(r*cols+c)].red;
	(*this)[r][c][GREEN] = buffer[(r*cols+c)].green;
	(*this)[r][c][BLUE] = buffer[(r*cols+c)].blue;
	(*this)[r][c][ALPHA] = buffer[(r*cols+c)].alpha;

      }

}

ColorImage::ColorImage(const ColorImage& src) : Volume<unsigned char>(src.getRowSize(), src.getColSize(), src.getDepthSize())
{
  pixels = NULL;
  int r, c, d;

  for (r=0;r<src.getRowSize();r++)
    for (c=0;c<src.getColSize();c++)
      for (d=0;d<src.getDepthSize();d++)
	(*this)[r][c][d] = src[r][c][d];
}

ColorImage::~ColorImage()
{
    delete[] pixels;
}

ColorImage& ColorImage::operator=(const ColorImage& src) 
{
  if (checkSize(src.getRowSize(), src.getColSize(), 3) == false) {
    m_rows = src.getRowSize();
    m_cols = src.getColSize();
    m_depth = src.getDepthSize();
    allocateStorage();
  }

  for (int r=0; r<m_rows; r++)
    for (int c=0; c<m_cols; c++)
      for (int d=0; d<m_depth; d++)
	(*this)[r][c][d] = src[r][c][d];

  return (*this);
}

ColorImage& ColorImage::operator=(const GrayImage& src) 
{
  if (checkSize(src.getRowSize(), src.getColSize(), 3) == false) {
    m_rows = src.getRowSize();
    m_cols = src.getColSize();
    m_depth = 3;
    allocateStorage();
  }

  for (int r=0; r<m_rows; r++)
    for (int c=0; c<m_cols; c++)
      for (int d=0; d<3; d++)
	(*this)[r][c][d] = src[r][c];

  return (*this);
}
ColorImage& ColorImage::operator=(const unsigned char src) 
{
    pixels = NULL;
  for (int r=0; r<m_rows; r++)
    for (int c=0; c<m_cols; c++)
      for (int d=0; d<3; d++)
	(*this)[r][c][d] = src;

  return (*this);
}

GrayImage& ColorImage::operator()(int plane) const
{

  // use a pointer because the return type is a reference
  // consider returning a GrayImage so that copy 
  // constructor is called?

  GrayImage *gray = new GrayImage(m_rows, m_cols);

  for (int r=0; r<m_rows; r++)
    for (int c=0; c<m_cols; c++)   
      (*gray)[r][c] = (*this)[r][c][plane];

  return (*gray);
}

ColorImage &ColorImage::FlipY()
{

  ColorImage *flip = new ColorImage(m_rows, m_cols);

  int invert = m_rows - 1;

  for (int r=0; r<m_rows; r++)
    for (int c=0; c<m_cols; c++)
      for (int d=0; d<m_depth; d++)
	(*flip)[r][c][d] = (*this)[invert-r][c][d];

  return (*flip);

}

void ColorImage::FillPixelBuffer(unsigned char *buffer)
{

  int r;
  int c;

  for (r=0;r<m_rows;r++)
    for (c=0;c<m_cols;c++)
      {
	buffer[(r*m_cols+c)*3+RED] = (*this)[r][c][RED];
	buffer[(r*m_cols+c)*3+GREEN] = (*this)[r][c][GREEN];
	buffer[(r*m_cols+c)*3+BLUE] = (*this)[r][c][BLUE];
	
      }  
	 
}

//fills in pixel buffer from left to right, bottom to top order(for opengl)
void ColorImage::FillPixelBufferReverse(unsigned char *buffer)
{

  int r;
  int c;

  for (r=0;r<m_rows;r++)
    for (c=0;c<m_cols;c++)
      {
	buffer[(r*m_cols+c)*m_depth] = pixels[((m_rows-r-1)*m_cols+c)*m_depth];
	buffer[(r*m_cols+c)*m_depth+1] = pixels[((m_rows-r-1)*m_cols+c)*m_depth+1];
	buffer[(r*m_cols+c)*m_depth+2] = pixels[((m_rows-r-1)*m_cols+c)*m_depth+2];	
      }  
	 
}

ColorImage::ColorImage(string filename)
{
  read((char *)filename.c_str());
}

void ColorImage::save(string filename, string comments)
{
  save((char *)filename.c_str(), (char *)comments.c_str());
}

void ColorImage::saveFormat(char *filename, char *format, char *comments)
{
    //make sure pixels points to the linear storage of image data
    //if(pixels == NULL)
    //{
	int r,c,d;
	pixels = new unsigned char[m_rows*m_cols*3];
	unsigned char *ptr = pixels;
	for(r=0;r<m_rows;r++)
	    for(c=0;c<m_cols;c++)
		for(d=0;d<3;d++)
		{
		    *ptr = (*this)[r][c][d];
		    ptr++;
		}
	//}

    string str = format;
    if(str == "PPM")
      savePPM(filename, comments);
    if(str == "PGM")
      savePGM(filename, comments);
    if(str == "TIFF")
	saveTIFF(filename);
//#ifdef __linux__
    if(str == "PNG")
	savePNG(filename);
    if(str == "JPEG")
	saveJPEG(filename);
//#endif
}

//saves only ppm for now
void ColorImage::save(char *filename, char *comments)
{
  string name, suffix, suffix2;

  name = filename;
  suffix = name.substr(name.rfind('.')+1, string::npos);


  if (suffix=="PPM"||suffix=="ppm")
    suffix2="PPM";
  else if (suffix=="PGM"||suffix=="pgm")
    suffix2="PGM";
  else if (suffix=="GIF"||suffix=="gif")
    suffix2="GIF";
//#ifdef __linux__
  else if (suffix=="PNG"||suffix=="png")
    suffix2="PNG";
  else if (suffix=="jpg"||suffix=="JPG"||suffix=="jpeg"||suffix=="JPEG")
    suffix2="JPEG";
//#endif
  else if (suffix=="BMP"||suffix=="bmp")
    suffix2="BMP24";
  else if (suffix=="TIFF"||suffix=="TIF"||suffix=="tiff"||suffix=="tif")
    suffix2="TIFF";
  else
    suffix2="JPEG";

  saveFormat(filename, (char*)suffix2.c_str(), comments);
}

void ColorImage::read(string filename)
{
  read((char *)filename.c_str());
}
void ColorImage::read(char *filename)
{
  //determine format here
  string name = filename;
  string suffix(name, name.rfind(".")+1, name.size()-(name.rfind(".")+1));

//#ifdef __linux__
  if (suffix=="jpg"||suffix=="JPG"||suffix=="jpeg"||suffix=="JPEG")
    readJPEG(filename);
  if (suffix=="PNG"||suffix=="png")
    readPNG(filename);
//#endif
  if (suffix=="PPM"||suffix=="ppm")
    readPPM(filename);
  if (suffix=="BMP"||suffix=="bmp")
    readBMP(filename);
  if (suffix=="TIFF"||suffix=="tiff"||suffix=="TIF"||suffix=="tif")
    readTIFF(filename);
  if (suffix=="PGM"||suffix=="pgm")
    readPGM(filename);
  
  allocateStorage();
  
  unsigned char *ptr = pixels;
  for (int r=0; r<m_rows; r++)
    for (int c=0; c<m_cols; c++)
      for (int d=0; d<m_depth; d++)
	(*this)[r][c][d] = *ptr++;
}

string ColorImage::getComments() const
{
  return m_comment;
}

const Vector3D ColorImage::bilinear(double r2, double c2) const
{
  Vector3D pixel(-1, -1, -1);

  int r = int(r2);
  int c = int(c2);

  //first check boundary conditions
  if ((r<0) || (r>m_rows-2) || (c<0) || (c>m_cols-2)) 
    //invalid ... outside boundaries
    return pixel;

  double t = (c2-c);
  double u = (r2-r);

  for (int d=0; d<3; d++) 
    pixel[d] = ((1-t)*(1-u)*(*this)[r][c][d]+ 
		(1-t)*u*(*this)[r+1][c][d]+ 
		t*u*(*this)[r+1][c+1][d] + 
		t*(1-u)*(*this)[r][c+1][d]);

  

  return pixel;
}


//data conversion
/*const ColorImage& ColorImage::operator= (CMatrix &rhs)
{
  if (checkSize(rhs.getRowSize(), rhs.getColSize(), rhs.getDepthSize()) == false) {
    m_rows = rhs.getRowSize();
    m_cols = rhs.getColSize();
    m_depth = rhs.getDepthSize();
    allocateStorage();
  }

  for (int r=0; r<m_rows; r++)
    for (int c=0; c<m_cols; c++)
      for (int d=0; d<m_depth; d++)
	(*this)[r][c][d] = (unsigned char) rhs[r][c][d];

  return (*this);
  }*/

const ColorImage downsample(ColorImage &image)//, int factor)
{
  int factor = 2;
  int r, c;
  int l,i,j;
  double total;
  int area;

  int rows = image.getRowSize()/factor;
  int cols = image.getColSize()/factor;

  ColorImage X(rows, cols);

  for (r=0; r<rows; r++)
    for (c=0; c<cols; c++) 
      for (l=0; l<3; l++) {
	total = 0.0;
	int area = 0;
	for (i=0; i<factor; i++)
	  for (j=0; j<factor; j++)
	    if (r*factor+i < image.getRowSize() && c*factor+j < image.getColSize()) {
	      total += image[r*factor+i][c*factor+j][l];
	      area++;
	    }
	if (area ==0)
	  total = 0;
	else
	  total /= area;
	X[r][c][l] = (unsigned char) total;
      }

  return X;
}

void ColorImage::smooth(int size)
{
  int r, c, l;
  double total;
  double x;

  vector<double> filter(2*size+1);

  total = 0.0;
  for (r=0; r<2*size+1; r++) {
    x = r-size;
    x /= size;
    filter[r] = 1/sqrt(2*M_PI)*exp(-x*x);
    total += filter[r];
  }

  for (r=0; r<2*size+1; r++) 
    filter[r] /= total;

  Matrix temp(m_rows, m_cols);

  for (int d=0; d<3; d++) {
    for (r=size; r<m_rows-size; r++) 
      for (c=0; c<m_cols; c++) {
	total = 0.0;
	for (l=-size; l<=size; l++)
	  total += (*this)[r+l][c][d]*filter[l+size];
	temp(r,c) =  total;
      }
    for (r=size-1; r>=0; r--) 
      for (c=0; c<m_cols; c++) {
	temp(r,c) = temp(r+1,c);
	temp(m_rows-1-r,c) = temp(m_rows-1-r-1,c);
      }

    for (c=size; c<m_cols-size; c++)
      for (r=0; r<m_rows; r++) {
	total = 0.0;
	for (l=-size; l<=size; l++)
	  total += temp(r,c+l)*filter[l+size];
	(*this)[r][c][d] =  (unsigned char) total;
      }

    for (c=size-1; c>=0; c--)
      for (r=0; r<m_rows; r++) {
	(*this)[r][c][d] = (*this)[r][c+1][d];
	(*this)[r][m_cols-1-c][d] = (*this)[r][m_cols-1-c-1][d];
      }
  }
}

const ColorImage RGBtoHSV(const ColorImage& I)//, unsigned char *rgbBytes)
{
  int ir, ic;
  double r, g, b;
  double maxc, minc;
  double h, s, v;
  double delta;
  ColorImage hsv(I.getRowSize(), I.getColSize());

  for (ir=0; ir<I.getRowSize(); ir++)
    for (ic=0; ic<I.getColSize(); ic++) {
      
      r = int(I[ir][ic][0])/255.0;
      g = int(I[ir][ic][1])/255.0;
      b = int(I[ir][ic][2])/255.0;


      //this two lines caused big problems when -O2
      //optimization was turned on ... ended up
      //with 0 erros and h getting to be e+10 power
      //caused green to become red
      //maxc = max(max(r,g),b);
      //minc = min(min(r,g),b);

      if (r > g)
	maxc = r;
      else 
	maxc = g;
      if (b > maxc)
	maxc = b;

      if (r < g)
	minc = r;
      else
	minc = g;
      if (b < minc)
	minc = b;
      
      v = maxc;
      s = (maxc != 0.0) ? ((maxc-minc)/maxc) : 0.0;
      
      if (s == 0.0)
	h = -1;
      else {
	delta = maxc-minc;
	if (r == maxc)
	  h = (g-b)/delta;
	else if (g == maxc)
	  h = 2.0+(b-r)/delta;
	else if (b == maxc)
	  h = 4.0+(r-g)/delta;
	h *= 60.0;
	if (h < 0.0)
	  h+= 360.0;
      }

      hsv[ir][ic][0] = int((h/360.0)*255.0);
      hsv[ir][ic][1] = int(s*255.0);
      hsv[ir][ic][2] = int(v*255.0);
    }

  return hsv;
}

const ColorImage HSVtoRGB(const ColorImage& I)
{
  int rows = I.getRowSize();
  int cols = I.getColSize();

  double hue;
  double sat;
  double val;

  ColorImage X(rows, cols);
  for (int r=0; r<rows; r++)
    for (int c=0; c<cols; c++) {
      hue = I[r][c][0]/255.0*360.0;
      sat = I[r][c][1]/255.0;
      val = I[r][c][2]/255.0;
      vector<int> pixel = convertHSVtoRGB(hue, sat, val);
      X[r][c][0] = pixel[0];
      X[r][c][1] = pixel[1];
      X[r][c][2] = pixel[2];
    }

  return X;
}

const ColorImage extremeHSV(const ColorImage &I)
{
  ColorImage E(I.getRowSize(), I.getColSize());

  int ir, ic;
  for (ir=0; ir<I.getRowSize(); ir++)
    for (ic=0; ic<I.getColSize(); ic++) {
      if (I[ir][ic][1] < 256/4) {
	E[ir][ic][0] = 255;
	E[ir][ic][1] = 255;
	E[ir][ic][2] = 255;
      }
      else if (I[ir][ic][0] < 256/6 || I[ir][ic][0] > 5*256/6) {
	E[ir][ic][0] = 255;
	E[ir][ic][1] = 0;
	E[ir][ic][2] = 0;
      }
      else if (I[ir][ic][0] < 3*256/6) {
	E[ir][ic][0] = 0;
	E[ir][ic][1] = 255;
	E[ir][ic][2] = 0;
      }
      else {
	E[ir][ic][0] = 0;
	E[ir][ic][1] = 0;
	E[ir][ic][2] = 255;
      }

    }
  return E;
}

const ColorImage ColorImage::subImage(int row, int col, int numRows, int numCols) const
{
  int r, c, d;
  int row2 = __min(__max(row, 0), int(m_rows)-1);
  int col2 = __min(__max(col, 0), int(m_cols)-1);

  int numRows2 = numRows - (row2-row);
  numRows2 = __min(int(m_rows)-row2, numRows2);

  int numCols2 = numCols - (col2-col);
  numCols2 = __min(int(m_cols)-col2, numCols2);

  ColorImage X(numRows2, numCols2);
  for (r=0; r<numRows2; r++)
    for (c=0; c<numCols2; c++) 
      for (d=0; d<3; d++)
	X[r][c][d] =  (*this)[row2+r][col2+c][d];

  /*  cout << "The image size is " << numRows2 << "x" << numCols2 << endl;
  for (r=0; r<numRows2; r++) {
    for (c=0; c<numCols2; c++) 
      for (d=0; d<3; d++)
	cout << int(X[r][c][d]) << "\t";
    cout << endl;
    }*/

  return X;
}

void ColorImage::setSubImage(const ColorImage &sub, int row, int col, int numRows, int numCols)
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
      for (d=0; d<3; d++)
	(*this)[r+row2][c+col2][d] =  sub[r][c][d];
}

//convert HSV to RGB
vector<int> convertHSVtoRGB(double hue, double sat, double v)
{
  double red, green, blue;
  int i;
  double f, p, q, t;
  vector<int> X(3);

  //convert HSV to RGB
  if (sat == 0.0) {
    red = v;
    green = v;
    blue = v;
  } else {
    hue /= 60.0;
    i = int(floor(hue));
    f = hue-i;
    p = v*(1.0-sat);
    q = v*(1.0-(sat*f));
    t = v*(1.0-(sat*(1.0-f)));
    switch(i) {
    case 0: red=v; green=t; blue=p; break;
    case 1: red=q; green=v; blue=p; break;
    case 2: red=p; green=v; blue=t; break;
    case 3: red=p; green=q; blue=v; break;
    case 4: red=t; green=p; blue=v; break;
    case 5: red=v; green=p; blue=q; break;
    }
  }
  X[0] = int(red*255);
  X[1] = int(green*255);
  X[2] = int(blue*255);

  return X;
}
 
const ColorImage resizeImage(ColorImage& src, int rows, int cols)
{
  int r, c, d;
  int oldr;
  int oldc;
  
  ColorImage X(rows, cols);
  for (r=0; r<rows; r++)
    for (c=0; c<cols; c++) {

      oldr = int(double(r)/rows*src.getRowSize()+0.5);
      if (oldr >= src.getRowSize())
	oldr = src.getRowSize()-1;
      oldc = int(double(c)/cols*src.getColSize()+0.5);
      if (oldc >= src.getColSize())
	oldc = src.getColSize()-1;

      for (d=0; d<3; d++)
	X[r][c][d] = src[oldr][oldc][d];
    }
  
  return X;
}

const ColorImage RGBAtoRGB(const ColorImage& I)
{
  int r, c, d;

  ColorImage rgb(I.getRowSize(), I.getColSize());

  for (r=0; r<I.getRowSize(); r++)
    for (c=0; c<I.getColSize(); c++) 
      for (d=0;d<3;d++)
	rgb[r][c][d] = I[r][c][d];
   
  return rgb;
}

//#ifdef __linux__
//image loading functionality
void ColorImage::readJPEG(char *filename)
{ 
  delete []pixels; pixels = NULL;
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  FILE * infile;		// source file 
  JSAMPARRAY buffer;		// Output row buffer 
  int row_stride;		// physical row width in output buffer 

  if ((infile = fopen(filename, "rb")) == NULL) 
    return;

  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, infile);
  jpeg_read_header(&cinfo, TRUE);
  jpeg_start_decompress(&cinfo);

  m_rows = cinfo.output_height;
  m_cols = cinfo.output_width;
  m_depth = cinfo.output_components;
  row_stride = m_cols * m_depth;

  buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

  pixels = new unsigned char[m_rows*m_cols*m_depth];
  unsigned char*buffer2;
  int rowCount = 0;
  while (cinfo.output_scanline < m_rows) {
    jpeg_read_scanlines(&cinfo, buffer, 1);
    buffer2 = (unsigned char*) buffer[0];
    for (int i=0; i<row_stride; i++)
      pixels[rowCount*row_stride+i]=buffer2[i];
    rowCount++;
  }

  //assuming everything is 3 bytes

  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
  fclose(infile);  
}

void ColorImage::saveJPEG(char * filename) const
{
  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;

  FILE * outfile;		/* target file */
  JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
  int row_stride;		/* physical row width in image buffer */

  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_compress(&cinfo);

  if ((outfile = fopen(filename, "wb")) == NULL) return;

  jpeg_stdio_dest(&cinfo, outfile);

  cinfo.image_width = m_cols; 	/* image width and height, in pixels */
  cinfo.image_height = m_rows;
  cinfo.input_components = m_depth;		/* # of color components per pixel */
  cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */

  jpeg_set_defaults(&cinfo);
  jpeg_set_quality(&cinfo, 75, TRUE);
  jpeg_start_compress(&cinfo, TRUE);

  row_stride = m_cols * m_depth;	/* JSAMPLEs per row in image_buffer */

  while (cinfo.next_scanline < cinfo.image_height) 
  {
      row_pointer[0] = &pixels[cinfo.next_scanline * row_stride];
      jpeg_write_scanlines(&cinfo, row_pointer, 1);
  }

  jpeg_finish_compress(&cinfo);
  fclose(outfile);
  jpeg_destroy_compress(&cinfo);
}
//#endif //ifdef __linux__


/* ppmreader taken from Robin's maiden
 
   ppmRead: read a PPM raw (type P6) file.  The PPM file has a header
   that should look something like:

     P6
     # comment
     width height max_value
     rgbrgbrgb...

   where "P6" is the magic cookie which identifies the file type and
   should be the only characters on the first line followed by a
   carriage return.  Any line starting with a # mark will be treated
   as a comment and discarded.  After the magic cookie, three integer
   values are expected: width, height of the image and the maximum
   value for a pixel (max_value must be < 256 for PPM raw files).  The
   data section consists of width*height rgb triplets (one byte each)
   in binary format (i.e., such as that written with fwrite() or
   equivalent).

   The rgb data is returned as an array of unsigned chars (packed
   rgb).  The malloc()'d memory should be free()'d by the caller.  If
   an error occurs, an error message is sent to stderr and NULL is
   returned.

*/
void ColorImage::readPPM(char *filename)
{
  const int MAXIMUM_LINE_LENGTH = 120;

  delete []pixels; 
  pixels = NULL;
  FILE* fp;
  int i, w, h, d;
  char head[MAXIMUM_LINE_LENGTH];			// max line <= MAXIMUM_LINE_LENGTH in PPM (per spec). 

  fp = fopen(filename, "rb");
  if (!fp) {
    perror(filename);
    fprintf(stderr, "%s: Could not open\n", filename);
    return;
  }

  // grab first two chars of the file and make sure that it has the
  // correct magic cookie for a raw PPM file. 
  fgets(head, MAXIMUM_LINE_LENGTH, fp);
  if (strncmp(head, "P6", 2)) {
    fprintf(stderr, "%s: Not a raw PPM file\n", filename);
    return;
  }

  // grab the three elements in the header (width, height, maxval). 
  i = 0;
  while(i < 3) {
    fgets(head, MAXIMUM_LINE_LENGTH, fp);
    if (head[0] == '#' || head[0] == '\n') // skip comments and blank lines
      continue;
    else if (i == 0) {
      i += sscanf(head, "%d %d", &w, &h);
    }
    else if (i == 2)
      i += sscanf(head, "%d", &d);
  }

  // grab all the image data in one fell swoop. 
  pixels = new unsigned char[w*h*3];
  fread(pixels, sizeof(unsigned char), w*h*3, fp);
  fclose(fp);

  m_cols = w;
  m_rows = h;
  m_depth = 3;
}


// quick and dirty bitmap loader...for 24 bit bitmaps with 1 plane only.  
// See http://www.dcs.ed.ac.uk/~mxr/gfx/2d/BMP.txt for more info.
// taken from lesson6 of NeHe
void ColorImage::readBMP(char *filename)
{
  delete []pixels; pixels = NULL;
  unsigned char *pixels_reverse;

  FILE *file;
  unsigned long size;                 // size of the image in bytes.
  unsigned long i;                    // standard counter.
  unsigned short int planes;          // number of planes in image (must be 1) 
  unsigned short int bpp;             // number of bits per pixel (must be 24)
  char temp;                          // temporary color storage for bgr-rgb conversion.

  // make sure the file is there.
  if ((file = fopen(filename, "rb"))==NULL)
    {
      printf("File Not Found : %s\n",filename);
      return;
    }
    
  // seek through the bmp header, up to the width/height:
  fseek(file, 18, SEEK_CUR);

  unsigned long value;

  // read the width
  if ((i = fread(&value, 4, 1, file)) != 1) {
    printf("Error reading width from %s.\n", filename);
    return;
  }
  m_cols = value;
    
  // read the height 
  if ((i = fread(&value, 4, 1, file)) != 1) {
    printf("Error reading height from %s.\n", filename);
    return;
  }
  m_rows = value;
     
  // calculate the size (assuming 24 bits or 3 bytes per pixel).
  size = m_rows*m_cols*3;
  m_depth = 3;

  // read the planes
  if ((fread(&planes, 2, 1, file)) != 1) {
    printf("Error reading planes from %s.\n", filename);
    return;
  }
  if (planes != 1) {
    printf("Planes from %s is not 1: %u\n", filename, planes);
    return;
  }

  // read the bpp
  if ((i = fread(&bpp, 2, 1, file)) != 1) {
    printf("Error reading bpp from %s.\n", filename);
    return;
  }
  if (bpp != 24) {
    printf("Bpp from %s is not 24: %u\n", filename, bpp);
    return;
  }
	
  // seek past the rest of the bitmap header.
  fseek(file, 24, SEEK_CUR);

  // read the data. 
  pixels = new unsigned char[size];
  pixels_reverse = new unsigned char[size];
  if (pixels == NULL) {
    printf("Error allocating memory for color-corrected image data");
    return;	
  }

  if ((i = fread(pixels_reverse, size, 1, file)) != 1) {
    printf("Error reading image data from %s.\n", filename);
    return;
  }

  for (i=0;i<size;i+=3) { // reverse all of the colors. (bgr -> rgb)
    temp = pixels_reverse[i];
    pixels_reverse[i] = pixels_reverse[i+2];
    pixels_reverse[i+2] = temp;
  }

  //reverse order into pixels
  //fprintf(stderr, "%d %d\n", m_rows, m_cols);
  unsigned int r, c;
  for(r = 0; r < m_rows; r++)
  {
      for(c = 0; c < m_cols*m_depth; c++)
      {
	  pixels[(r*m_cols*m_depth)+c] = pixels_reverse[((m_rows-r-1)*m_cols*m_depth)+c];
      }
  }

  delete[] pixels_reverse;
}

void ColorImage::readTIFF(char *filename)
{
  delete []pixels; pixels = NULL;
/*  TIFF* tif = TIFFOpen(filename, "r");
  if (tif) {
    uint32 c, r;
    uint16 d;
    int i,j;

    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &c);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &r);
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &d);

    m_rows = r;
    m_cols = c;
    m_depth = d;

    size_t npixels = m_rows*m_cols;
    uint32 *raster = (uint32*) _TIFFmalloc(npixels * sizeof (uint32));
    unsigned char* data;
    if (raster != NULL) {
      if (TIFFReadRGBAImage(tif, m_cols, m_rows, raster, 0)) {
	pixels = new unsigned char[npixels*d];
	int count = 0;
	for (i=0; i<m_rows; i++)
	  for (j=0; j<m_cols; j++) {
	    data = (unsigned char*) &(raster[(m_rows-1-i)*m_cols+j]);
	    if (m_depth == 3) {
	      pixels[count++] = data[0];
	      pixels[count++] = data[1];
	      pixels[count++] = data[2];
	    }
	    else {
	      pixels[count++] = data[0];
	      pixels[count++] = data[1];
	      pixels[count++] = data[2];
	      pixels[count++] = data[3];
	    }
	  }
      }
      _TIFFfree(raster);
    }
    TIFFClose(tif);
  }
  */
}

void ColorImage::saveTIFF(char *filename) const
{
/*  TIFF *tif;

  tif = TIFFOpen(filename, "w");
  
  TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, m_cols);
  TIFFSetField(tif, TIFFTAG_IMAGELENGTH, m_rows);
  TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_DEFLATE);
  TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
  TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
  TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
  TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, m_depth);  

  TIFFWriteEncodedStrip(tif, 0, pixels, m_rows * m_cols * m_depth);

  TIFFClose(tif);
  */
}

void ColorImage::savePPM(char *filename, char *comments) const
{
	FILE *fp = fopen(filename, "wb");

	//write the magic number
	fputs("P6\n", fp);
	if (comments != NULL)
	  fprintf(fp, "# %s\n", comments);

	//write out width,height,max val
	fprintf(fp, "%d %d\n", m_cols, m_rows);
	fprintf(fp, "%d\n", 255);

	//write out pixel data in one call
	fwrite(pixels, sizeof(unsigned char), m_cols*m_rows*3, fp);

	fclose(fp);
}

//#ifdef __linux__
void ColorImage::readPNG(char *filename)
{
    delete[] pixels;
    pixels = NULL;

   FILE *fp = fopen(filename, "rb");
   if( !fp ) 
       return;

   // The last three arguments can be used to set up error handling callbacks.
   png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
   if( !png_ptr ) 
   { 
       fclose(fp); 
       return; 
   }

   // Allocate required structure to hold memory information.
   png_infop info_ptr = png_create_info_struct(png_ptr);
   if( !info_ptr )
   {
      fclose(fp);
      png_destroy_read_struct(&png_ptr, NULL, NULL);
      return;
   }

   // Because we didn't set up any error handlers, we need to be
   // prepared to handle longjmps out of the library on error
   // conditions.
   if( setjmp(png_ptr->jmpbuf) )
   {
      png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
      fclose(fp);
      return;
   }

   png_init_io(png_ptr, fp);

   // Read in all the image information
   png_read_info(png_ptr, info_ptr);

   // Get the header for the first image chunk
   png_uint_32 width, height;
   int bit_depth, color_type, interlace_type;

   png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
       &interlace_type, NULL, NULL);

   if( bit_depth == 16 )
       // truncate 16 bits/pixel to 8 bits/pixel
       png_set_strip_16(png_ptr);

   if( color_type == PNG_COLOR_TYPE_PALETTE )
       // expand paletted colors into RGB color values
       png_set_expand(png_ptr);
   else if( color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8 )
       // expand grayscale images to full 8 bits/pixel
       png_set_expand(png_ptr);

   // Expand paletted or RGB images with transparency to full alpha
   // channels so the data will be available as RGBA quartets.
   if( png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS) )
      png_set_expand(png_ptr);

   // update the palette and info structure
   png_read_update_info(png_ptr, info_ptr);


   // read the image data
   std::vector<png_bytep> row_pointers(height);
   int row;
   int num_chan = png_get_channels(png_ptr, info_ptr);
   int row_size = png_get_rowbytes(png_ptr, info_ptr);

   m_rows = height;
   m_cols = width;
   m_depth = num_chan;

   for (row = 0; row < height; row++)
      row_pointers[row] = (png_bytep)malloc(row_size);

   png_read_image(png_ptr, &row_pointers.front());
   png_read_end(png_ptr, info_ptr);

   //copy image data into linear storage
   pixels = new unsigned char[height * row_size];

   unsigned char *ptr = pixels;
   for(row=0; row<height; row++)
   {
       memcpy(ptr, row_pointers[row], row_size);
       ptr += row_size;
   }

   png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
   for(row=0; row<height; row++) 
       free(row_pointers[row]);
   
   fclose(fp);
}

void ColorImage::savePNG(char *filename) const
{
   FILE *fp = fopen(filename, "wb");
   if( !fp ) 
       return;

   png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
   if( !png_ptr ) 
   { 
       fclose(fp); 
       return; 
   }

   png_infop info_ptr = png_create_info_struct(png_ptr);
   if( !info_ptr )
   {
      fclose(fp);
      png_destroy_write_struct(&png_ptr,  (png_infopp)NULL);
      return;
   }

   if( setjmp(png_ptr->jmpbuf) )
   {
      fclose(fp);
      png_destroy_write_struct(&png_ptr,  (png_infopp)NULL);
      return;
   }

   png_init_io(png_ptr, fp);

   int img_type = PNG_COLOR_TYPE_RGB;

   png_set_IHDR(png_ptr, info_ptr,
		m_cols, m_rows, 8, img_type, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

   png_write_info(png_ptr, info_ptr);

   std::vector<png_bytep> row_pointers(m_rows);
   unsigned char *ptr = pixels;
   for(int row=0; row<m_rows; row++)
     row_pointers[row] = (png_bytep)ptr + row * m_cols * m_depth;

   png_write_image(png_ptr, &row_pointers.front());
   png_write_end(png_ptr, info_ptr);

   png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
   fclose(fp);
}
//#endif //ifdef __linux__

void ColorImage::readPGM(char *filename)
{
  const int MAXIMUM_LINE_LENGTH = 120;
	unsigned int i;
  delete []pixels; 
  pixels = NULL;
  FILE* fp;
  int w, h, d;
  char head[MAXIMUM_LINE_LENGTH];			// max line <= MAXIMUM_LINE_LENGTH in PPM (per spec). 

  fp = fopen(filename, "rb");
  if (!fp) {
    perror(filename);
    fprintf(stderr, "%s: Could not open\n", filename);
    return;
  }

  // grab first two chars of the file and make sure that it has the
  // correct magic cookie for a raw PGM file. 
  fgets(head, MAXIMUM_LINE_LENGTH, fp);
  if (strncmp(head, "P5", 2)) {
    fprintf(stderr, "%s: Not a raw PGM file\n", filename);
    return;
  }

  // grab the three elements in the header (width, height, maxval). 
  i = 0;
  while(i < 3) {
    fgets(head, MAXIMUM_LINE_LENGTH, fp);
    if (head[0] == '#' || head[0] == '\n') // skip comments and blank lines
      continue;
    else if (i == 0) {
      i += sscanf(head, "%d %d", &w, &h);
    }
    else if (i == 2)
      i += sscanf(head, "%d", &d);
  }

  // grab all the image data in one fell swoop. 
  pixels = new unsigned char[w*h*3];
  unsigned char tmp[1];
  //fread(pixels, sizeof(unsigned char), w*h, fp);
  for(i = 0; i < w*h*3; i+=3)
  {
      fread(tmp, sizeof(unsigned char), 1, fp);
      pixels[i] = tmp[0];
      pixels[i+1] = tmp[0];
      pixels[i+2] = tmp[0];
  }
  fclose(fp);

  m_cols = w;
  m_rows = h;
  m_depth = 3;
}

void ColorImage::savePGM(char *filename, char *comments) const
{
	FILE *fp = fopen(filename, "wb");

	//write the magic number
	fputs("P5\n", fp);

	if (comments != NULL)
	  fprintf(fp, "# %s\n", comments);

	//write out width,height,max val
	fprintf(fp, "%d %d\n", m_cols, m_rows);
	fprintf(fp, "%d\n", 255);

	//write out pixel data in one call
	//fwrite(pixels, sizeof(unsigned char), m_cols*m_rows, fp);
	unsigned char *ptr = pixels;
	unsigned char tmp[1];
	for(unsigned int r = 0; r < m_rows; r++)
	{
	    for(unsigned int c = 0; c < m_cols; c++)
	    {
		tmp[0] = (unsigned char)(float((*this)[r][c][0])*0.30 + 
				      float((*this)[r][c][1])*0.59 + 
				      float((*this)[r][c][2])*0.11);
                fwrite(tmp, sizeof(unsigned char), 1, fp);
	    }
	}

	fclose(fp);
}

void ColorImage::scale(int n_rows , int n_cols)
{
    //vector< vector<unsigned char> > temp_buffer;
    ColorImage temp_buffer;
    int ri, ci; //row and col index into old image when selecting pixel value for new image
    int r, c;

    temp_buffer.resize3D(n_rows,n_cols,3);

    //do scaling
    for(r = 0; r < n_rows; r++)
    {
	for(c = 0; c < n_cols; c++)
	{
	    //this is really slow b/c we're doing so many castings from int to float then back to int...maybe get rid of it later if it
	    //causes huge speed problems
	    ri = int(floor(float(r) / float(n_rows) * float(m_rows)));
            ci = int(floor(float(c) / float(n_cols) * float(m_cols)));
	    temp_buffer[r][c][0] = (*this)[ri][ci][0];
	    temp_buffer[r][c][1] = (*this)[ri][ci][1];
	    temp_buffer[r][c][2] = (*this)[ri][ci][2];
	}
    }

    //copy temp_buffer to current buffer
    this->resize3D(n_rows, n_cols,3);
    for(r = 0; r < m_rows; r++)
    {
	for(c = 0; c < m_cols; c++)
	{
	    (*this)[r][c][0] = temp_buffer[r][c][0];
	    (*this)[r][c][1] = temp_buffer[r][c][1];
	    (*this)[r][c][2] = temp_buffer[r][c][2];
	}
    }
}
