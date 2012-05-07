/*! \grid.hpp

    A generic two-dimensional storage array. Utilizes templates so can
    be utilized for any base type, T.

    To be incorporated into:
    * CGrayImage
    * CMatrix
    * ...

	1/28/05 Sak
	Removing dependency on ImageMagick and using CImage class
        
	2/2/05  Sak
        Removing dependency on CImage class and integrating image loading into Grid itself

    ----------------------------------------------------------------------  */

#ifndef __GRID_HPP__
#define __GRID_HPP__


#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

template <class T>
class Grid : public vector<vector<T> >
{
public:
  Grid();
  Grid(size_t rows, size_t cols);
  Grid(char *filename);
  //  Grid(const Grid& grid);
  ~Grid();

  const Grid& operator= (const Grid &rhs);
  const Grid& operator= (double rhs);

  //file I/O
  void readImage(const char *filename);
  void readImage(char *filename);
  void saveImage(const char *filename);
  void saveImage(char *filename);
  void saveData(char *filename) const;
  void readData(char *filename);

  //general operations
  size_t getRowSize() const;
  size_t getColSize() const;
  void resize2D(size_t rows, size_t cols);

  void display() const;

  T getMinimum() const;
  T getMaximum() const;

  const Grid<T> subGrid(int row, int col,
			 int numRows, int numCols) const;

    void savePGM(char *filename) const;
    void readPGM(char *filename);

protected:
  size_t m_rows;
  size_t m_cols;
  unsigned char *pixels; //linear storage of image data
  bool checkSize( size_t rows, size_t cols );
  void allocateStorage();

private:

};

//template code here
template <class T> 
Grid<T>::Grid() : vector<vector<T> >()
{ 
    pixels = NULL;
  m_rows = 0;
  m_cols = 0; 
}

template <class T> 
Grid<T>::Grid(size_t rows, size_t cols) //: vector<vector<T> >(rows, cols)
{
    pixels = NULL;
  m_rows = rows;
  m_cols = cols;
  allocateStorage();
}

template <class T> 
Grid<T>::Grid(char *filename) 
{
    pixels = NULL;
  m_rows = 0;
  m_cols = 0;
  readData(filename);
}
/*template <class T> 
Grid<T>::Grid(const Grid& grid) : vector<vector<T> >(grid)
{
  m_rows = grid.getRowSize();
  m_cols = grid.getColSize();
  }*/

template<class T>
Grid<T>::~Grid()
{
  delete[] pixels;
}

template <class T>
const Grid<T>& Grid<T>::operator= (const Grid &rhs)
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

template <class T>
const Grid<T>& Grid<T>::operator= (double rhs)
{
  for (int r=0; r<m_rows; r++)
    for (int c=0; c<m_cols; c++)
      (*this)[r][c] = rhs;
 
  return (*this);
}

template <class T>
void Grid<T>::display() const
{
  for (int r=0; r<m_rows; r++) {
    for (int c=0; c<m_cols; c++)
      cout << (*this)[r][c] << "\t";
    cout << endl;
  }
}

template <class T> 
size_t Grid<T>::getRowSize() const
{
  return m_rows;
}

template <class T> 
size_t Grid<T>::getColSize() const
{
  return m_cols;
}

template <class T>
T Grid<T>::getMaximum() const
{
  T max = (*this)[0][0];
  for (int r=0; r<m_rows; r++)
    for (int c=0; c<m_cols; c++)
      if ((*this)[r][c] > max)
	max = (*this)[r][c];

  return max;
}

template <class T>
T Grid<T>::getMinimum() const
{
  T min = (*this)[0][0];
  for (int r=0; r<m_rows; r++)
    for (int c=0; c<m_cols; c++)
      if ((*this)[r][c] < min)
	min = (*this)[r][c];

  return min;
}



template <class T> 
bool Grid<T>::checkSize( size_t rows, size_t cols )
{
  return (rows==m_rows && cols == m_cols);
}

template <class T>
void Grid<T>::allocateStorage()
{
  this->resize(m_rows);

  typename vector<vector<T> >::iterator p;
  for (p=this->begin(); p!=this->end(); p++)
    p->resize(m_cols);
}

template <class T>
void Grid<T>::resize2D(size_t rows, size_t cols)
{
  m_rows = rows;
  m_cols = cols;
  allocateStorage();
}

//file I/O
template <class T>
void Grid<T>::readData(char *filename)
{
  ifstream infile(filename);
  if (infile.fail() || infile.bad()) {
    cerr << "ERROR: Could not read " << filename << "." << endl;
    exit(1);
  }
  infile >> m_rows;
  infile >> m_cols;
  string junk;
  getline(infile, junk);	// clears to end of line to begin anew

  allocateStorage();

  for (int r=0; r<m_rows; r++)
    for (int c=0; c<m_cols; c++) 
      infile >> (*this)[r][c];
}

template <class T>
void Grid<T>::saveData(char *filename) const
{
  ofstream out_file(filename);
  out_file << m_rows << endl;
  out_file << m_cols << endl;

  for (int r=0; r<m_rows; r++)
  {
    for (int c=0; c<m_cols; c++) 
    {
      out_file << (*this)[r][c];
    }
    out_file << endl;
  }
}

template <class T>
void Grid<T>::readImage(const char *filename)
{
  readImage((char*)filename);
}
template <class T>
void Grid<T>::readImage(char *filename)
{
    //only supports PGM for now.  Use CColorImage for color formats
    readPGM(filename);

    allocateStorage();

  unsigned char *ptr = pixels;  
  for (int r=0; r<m_rows; r++)
    for (int c=0; c<m_cols; c++)
      (*this)[r][c] = *ptr++;
}

template <class T>
void Grid<T>::saveImage(const char *filename)
{
  saveImage((char *)filename);

}
template <class T>
void Grid<T>::saveImage(char *filename)
{
    //make sure pixel points to the valid linear storage of image data
    if(pixels == NULL)
    {
	int r,c;
	pixels = new unsigned char[m_rows*m_cols];
	unsigned char *ptr = pixels;
	for(r=0;r<m_rows;r++)
	    for(c=0;c<m_cols;c++)
	    {
		*ptr = (*this)[r][c];
		ptr++;
	    }
    }

    //only one format for now
  savePGM(filename);
}

template <class T>
const Grid<T> Grid<T>::subGrid(int row, int col, 
				 int numRows, int numCols) const
{
  int r, c, d;
  int row2 = min(max(row, 0), int(m_rows)-1);
  int col2 = min(max(col, 0), int(m_cols)-1);

  int numRows2 = numRows - (row2-row);
  numRows2 = min(int(m_rows)-row2, numRows2);

  int numCols2 = numCols - (col2-col);
  numCols2 = min(int(m_cols)-col2, numCols2);

  Grid<T> X(numRows2, numCols2);
  for (r=0; r<numRows2; r++)
    for (c=0; c<numCols2; c++) 
      X[r][c] =  (*this)[row2+r][col2+c];

  return X;
}

//image loading functionality
template<class T>
void Grid<T>::readPGM(char *filename)
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
  pixels = new unsigned char[w*h];
  fread(pixels, sizeof(unsigned char), w*h, fp);
  fclose(fp);

  m_cols = w;
  m_rows = h;
}

template<class T>
void Grid<T>::savePGM(char *filename) const
{
	FILE *fp = fopen(filename, "wb");

	//write the magic number
	fputs("P5\n", fp);

	//write out width,height,max val
	fprintf(fp, "%d %d\n", m_cols, m_rows);
	fprintf(fp, "%d\n", 255);

	//write out pixel data in one call
	fwrite(pixels, sizeof(unsigned char), m_cols*m_rows, fp);

	fclose(fp);
}


#endif //__GRID_HPP__
