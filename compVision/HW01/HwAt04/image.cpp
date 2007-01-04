/*  ----------------------------------------------------------------------
    image.cpp
    ----------

    Implementations for CImage class
    ---------------------------------------------------------------------- */
#include "image.hpp"
#include <math.h>

CImage::CImage(int rows, int cols)
{
  m_rows = rows;
  m_cols = cols;
  allocateStorage();
}
 
CImage::CImage(const CImage& source)
{
  m_rows = source.m_rows;
  m_cols = source.m_cols;
  allocateStorage();
  copyElements(source);
}

CImage::CImage(const char *filename)
{
  m_rows = -1;
  m_cols = -1;
  m_data = NULL;

  read(filename);
}

CImage::~CImage(void)
{
  deleteStorage();
}

bool CImage::read(const char *filename)
{
  cout << "Reading: " << filename << endl;
  
  //  try { 
    // Create an image object and read an image 
    Magick::Image image( filename ); 
    
    // Write to BLOB in GRAY format 
    Magick::Blob blob; 
    image.magick( "GRAY" ); 
    image.write( &blob ); 
    
    int rows = image.size().height();
    int cols = image.size().width();

    if (checkSize(rows, cols) == false) {

      deleteStorage();
      m_rows = rows;
      m_cols = cols;

      allocateStorage();
    }

    unsigned char *ptr = (unsigned char *) blob.data();
    for (int r=0; r<m_rows; r++)
      for (int c=0; c<m_cols; c++)
	m_data[r][c] = (*ptr++)/255.0; // convert to 0.0 - 1.0
    //  }
  //  catch( Exception &error_ ) {
  //    cout << "Caught exception: " << error_.what() << endl; 
  //    return false;
    //  } 
  return true; 
}

bool CImage::save(const char *filename)
{
  //create the block of data

  unsigned char *block = new unsigned char[m_rows*m_cols];
  int value;
  int count = 0;
  for (int r=0; r<m_rows; r++)
    for (int c=0; c<m_cols; c++) {
      value = int( m_data[r][c]*255.0+0.5 ); // +0.5 to round
      if (value < 0)
	value = 0;
      if (value > 255)
	value = 255;
      block[count++] = (unsigned char) value;
    }
  void * ptr = (void *) block;

  char imageSize[64];
  Magick::Blob blob(ptr, m_cols*m_rows);
  sprintf(imageSize,"%dx%d",m_cols,m_rows);
  Magick::Geometry geo = imageSize;

  Magick::Image image(blob, geo, 8, "GRAY");
  //  Image image(m_cols, m_rows, "RGB", 0, ptr);

  string name = filename;
  string suffix(name, name.rfind(".")+1, name.size()-(name.rfind(".")+1));

  if (suffix=="jpg"||suffix=="JPG"||suffix=="jpeg"||suffix=="JPEG")
    image.magick("JPEG");
  else if (suffix=="PGM"||suffix=="pgm")
    image.magick("PGM+");
  else if (suffix=="GIF"||suffix=="gif")
    image.magick("GIF");
  else if (suffix=="PNG"||suffix=="png")
    image.magick("PNG");
  else if (suffix=="BMP"||suffix=="bmp")
    image.magick("BMP24");

  image.write(filename);

  delete []block;
  return true;
}

CImage& CImage::operator= (const CImage& rhs)
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

CImage& CImage::operator+=(const CImage& rhs)
{
  checkSize(rhs.m_rows, rhs.m_cols);

  for (int i=0; i<m_rows; i++)
    for (int j=0; j<m_cols; j++)
      m_data[i][j] += rhs[i][j];

  return (*this);
}

CImage& CImage::operator-=(const CImage& rhs)
{
  checkSize(rhs.m_rows, rhs.m_cols);

  for (int i=0; i<m_rows; i++)
    for (int j=0; j<m_cols; j++)
      m_data[i][j] -= rhs[i][j];

  return (*this);
}

CImage& CImage::operator*=(const CImage& rhs)
{
  if (m_cols != rhs.m_rows)
    error("operator*=: matrices not same inner size");

  double total;

  CImage result(m_rows, rhs.m_cols);
  for (int i=0; i<m_rows; i++)
    for (int j=0; j<rhs.m_cols; j++) {
      total = 0;
      for (int k=0; k<m_cols; k++)
	total += m_data[i][k]*rhs.m_data[k][j];
      result[i][j] = total;
    }

  (*this) = result;
  return (*this);
}

CImage& CImage::operator*=(double rhs)
{
  for (int i=0; i<m_rows; i++)
    for (int j=0; j<m_cols; j++)
      m_data[i][j] *= rhs;

  return (*this);
}

CImage& CImage::operator/=(double rhs)
{
  for (int i=0; i<m_rows; i++)
    for (int j=0; j<m_cols; j++)
      m_data[i][j] /= rhs;

  return (*this);
}

CImage& CImage::operator= (double rhs)
{
  for (int i=0; i<m_rows; i++)
    for (int j=0; j<m_cols; j++)
      m_data[i][j] = rhs;
  return (*this);
}

CImage& CImage::operator+=(double rhs)
{
  for (int i=0; i<m_rows; i++)
    for (int j=0; j<m_cols; j++)
      m_data[i][j] += rhs;

  return (*this);
}

CImage& CImage::operator-=(double rhs)
{
  for (int i=0; i<m_rows; i++)
    for (int j=0; j<m_cols; j++)
      m_data[i][j] -= rhs;

  return (*this);
}

void CImage::resize(int rows, int cols)
{
  deleteStorage();
  m_rows = rows;
  m_cols = cols;
  allocateStorage();
}

bool CImage::checkSize(int rows, int cols)
{
  return ((rows>=0)&&(rows<m_rows))&&((cols>=0)&&(cols<m_cols)); 
}

//mathematical operations
double CImage::calcAverage(void)
{
  double total;
  total = 0;
  for (int r=0; r<m_rows; r++)
    for (int c=0; c<m_cols; c++)
      total += (double) m_data[r][c];
  return total/(m_rows*m_cols);
}

double CImage::calcSumSqrAverage(void)
{
  double total;
  total = 0;
  for (int r=0; r<m_rows; r++)
    for (int c=0; c<m_cols; c++)
      total += ((double) m_data[r][c]* (double) m_data[r][c]);
  return total/(m_rows*m_cols);
}

void CImage::setUniformRandom(double range)
{
  for (int r=0; r<m_rows; r++)
    for (int c= 0; c<m_cols; c++)
      m_data[r][c] = (range*rand())/RAND_MAX;
}

void CImage::setIdentity(void)
{
  for (int r=0; r<m_rows; r++)
    for (int c=0; c<m_cols; c++)
      m_data[r][c] = (r==c);
}

double CImage::calcSumSquared(void)
{
  double total = 0;
  for (int r=0; r<m_rows; r++)
    for (int c=0; c<m_cols; c++)
      total += (m_data[r][c]*m_data[r][c]);
  return total;
}

const CImage CImage::transpose()
{
  int r,c;

  CImage u(m_cols, m_rows);
  for (r=0; r<m_cols; r++)
    for (c=0; c<m_rows; c++)
      u[r][c] = m_data[c][r];

  return u;
}

/*
    Calculate Gradient:
    by Dan Beatty
    Input: none
    Output: A CImage class with the results of a gradient calculation
*/


//image processing operations
/* const CImage CImage::calcRowGradient(int filterLength) {
}*/

CImage CImage::calcRowGradientFast () {
    CImage temp = CImage (m_rows ,m_cols);
    double value = 0;
    int i = 0;
    int j = 0;
    int m = m_rows;
    int n = m_cols;
    
    // begin loop
    for ( ; i < n; i++) 
        for ( ; j < m; j++) {
            if ((j > 0) && (j < (n-1)))
                value = -m_data[j+1][i] + m_data[j-1][i];
            else if ( j == 0)
                value = -m_data[j+1][i];
            else 
                value =  m_data[j-1][i];
            value = value/2;
            temp.setValue(j,i,value);
        }
    
    return temp;    
}


//const CImage CImage::calcColGradient(int filterLength)
CImage CImage::calcColGradientFast(){
    CImage temp = CImage (m_rows ,m_cols);
    double value = 0;
    int i = 0;
    int j = 0;
    int m = m_rows;
    int n = m_cols;
    
    // begin loop
    for ( ; i < m; i++) 
        for ( ; j < n; j++) {
            if ((j > 0) && (j < (n-1))) 
                value = -m_data[i][j+1] + m_data[i][j-1];
            else if ( j == 0)
                value = -  m_data[i][j+1];
            else 
                value =  m_data[i][j-1];
            value = value/2;
            temp.setValue(i,j,value);
        }
    
    return temp;
}

void CImage::squareEachElement ()
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

void CImage::squareRootEachElement()
{
    double value = 0;
    int i = 0;
    int j = 0;
    int m = m_rows;
    int n = m_cols;
    for (i = 0; i < m; i++)
        for (j=0 ; j < n; j++) {
            value = sqrt(m_data[i][j]);
            m_data[i][j] = value;
        }

}


void CImage::display(void)
{
  for (int r=0; r<m_rows; r++) {
    for (int c=0; c<m_cols; c++)
      cout << m_data[r][c] << "\t";

    cout << endl;
  }
}

bool CImage::allocateStorage(void)
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

bool CImage::deleteStorage(void)
{
  if (m_data != NULL) {
    delete []m_data[0];
    delete []m_data;
    m_data = NULL;
  }
  return true;
}

bool CImage::copyElements(const CImage& source)
{
  for (int i=0; i<m_rows; i++)
    for (int j=0; j<m_cols; j++)
      m_data[i][j] = source.m_data[i][j];
  return true;
}

void CImage::error(const char *errorMsg)
{
  cout << "CImage: " << errorMsg << endl;
  exit(1);
}

ostream& operator<<(ostream& s, const CImage& source)
{
  for (int i=0; i<source.getRowSize(); i++) {
    for (int j=0; j<source.getColSize(); j++)
      s << source[i][j] << "\t";
    s << endl;
  }
  return s;
}

const CImage operator*(const CImage& lhs, const CImage& rhs)
{
  CImage u = lhs;
  u *= rhs;
  return u;
}

const CImage operator*(const CImage &lhs, double rhs)
{
  CImage M = lhs;
  M *= rhs;
  return M;
}

const CImage operator*(double lhs, const CImage &rhs)
{
  CImage M = rhs;
  M *= lhs;
  return M;
}

const CImage operator+(const CImage &lhs, const CImage &rhs)
{
  CImage M;
  M = lhs;
  M += rhs;
  return M;
}

const CImage operator-(const CImage &lhs, const CImage &rhs)
{
  CImage M;
  M = lhs;
  M -= rhs;
  return M;
}

