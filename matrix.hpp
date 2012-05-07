/*! \file matrix.hpp

    \brief Basic matrix handling routines.

    Adapted from Stroustrap's book. Chapter on numerics.

    Should include:
    overloaded operators with doubles
    overloaded operators with valarrays
    overloaded operators with matrices

    external functions include:
    all cases of overloade operators
    4D transformation matrices:
    * rotation
    * scale
    * translate

    Note:
    X(row, col) is the correct order of indexing


    Needs to be massively redone to improve speed.
    For example, make everything stored as a double
    Have data be ordered one column at a time to
    save data transfer cost between SVD routines.

    ------------------------------------------------------------
    Notes: 03/04/05
    Spent too much time thinking about speed it up by dropping the
    valarray<double> base structure for double* base. The results of
    timing runs are below. Although there is some gain (maybe 3%),
    it is not that significant. Instead looking for something
    that doubles it. 

    valarray<double> Matrix routines

    optimization: none
    Multiplication of two 200 x 200 matrices: 0.828087 seconds
    Addition of two 200 x 200 matrices: 0.002279 seconds
    Subtraction of two 200 x 200 matrices: 0.002414 seconds

    optimization: -O2
    Multiplication of two 200 x 200 matrices: 0.32952 seconds
    Addition of two 200 x 200 matrices: 0.002362 seconds
    Subtraction of two 200 x 200 matrices: 0.002317 seconds

    optimization: -O4
    Multiplication of two 200 x 200 matrices: 0.133593 seconds
    Addition of two 200 x 200 matrices: 0.002313 seconds
    Subtraction of two 200 x 200 matrices: 0.002411 seconds


    ------------------------------------------------------------
    
    double* Matrix routines:
    
    
    optimization: none
    Multiplication of two 200 x 200 matrices: 0.658741 seconds
    Addition of two 200 x 200 matrices: 0.002347 seconds
    Subtraction of two 200 x 200 matrices: 0.002418 seconds
 
    optimization: -O2
    Multiplication of two 200 x 200 matrices: 0.315643 seconds
    Addition of two 200 x 200 matrices: 0.002234 seconds
    Subtraction of two 200 x 200 matrices: 0.001957 seconds
    
    optimization: -O4
    Multiplication of two 200 x 200 matrices: 0.128179 seconds
    Addition of two 200 x 200 matrices: 0.001916 seconds
    Subtraction of two 200 x 200 matrices: 0.001898 seconds
   ------------------------------------------------------------  */

#ifndef __MATRIX_HPP__
#define __MATRIX_HPP__

#include "config.hpp"
#include <iostream>
#include <valarray>
#include <vector>
#include "r_valarray.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

/*! \brief Handles numerical computations

    Provides tools for handling numerical computations including
    multiplication, inverses, svd, least squares, etc.
*/
class Matrix {

public:
  Matrix();			// note, I added at default constructor, EDS '02
  Matrix(size_t rows, size_t cols); 
  Matrix(size_t rows, size_t cols, valarray<double> &src);
  Matrix(size_t rows, size_t cols, double *src);
  Matrix(const Matrix&);
  ~Matrix();

  void saveMfile(char *filename, char *label);
  void saveDATfile(char *filename);

  void resize(size_t rows, size_t cols);
  size_t size() const { return m_rows*m_cols; }
  size_t getRowSize() const { return m_rows; }
  size_t getColSize() const { return m_cols; }

  void setColumn(int i, const valarray<double> &X);

  const valarray<double> column(int i);
  const valarray<double> row(int i);

  double& operator()(size_t r, size_t c); // Fortran-style subscripts
  double operator()(size_t r, size_t c) const;

  const Matrix& operator=(const Matrix&);
  const Matrix& operator*=(const Matrix&);
  const Matrix& operator+=(const Matrix&);
  const Matrix& operator-=(const Matrix&);

  const Matrix& operator=(double);
  const Matrix& operator*=(double);
  const Matrix& operator/=(double);
  const Matrix& operator+=(double);
  const Matrix& operator-=(double);

  //double& array() { return *v; }

  //return a subset of the original
  const Matrix subMatrix(int bottom, int left, int top, int right) const;
  const Matrix minorMatrix(int row, int col) const;

  //set a subset
  void setSubMatrix(int row, int col, const Matrix &);

  //numerical, other operations ...
  double bilinear(double x, double y) const;
  void smooth(int filterLength);

  //mathematical functions
  void svd(Matrix& U, valarray<double>& D, Matrix& VT);

  //useful information
  double max();			// the maximum of the matrix
  double min();			// the minimum of the matrix

private:
  valarray<double> *v;
  size_t m_rows, m_cols;	// 
};

ostream& operator<<(ostream& os, Matrix& m);
/* \todo { optimize pow, dotMultiply and dotDivide
   by using internal valarray functionality.}

*/
const Matrix pow(const Matrix &lhs, double rhs);
const Matrix sqrt(const Matrix &src);
/*! \brief returns the sum of all elements in the
  matrix. 

  To get the sum squared, use matrixsum(pow(X, 2.0))
*/
double matrixsum(const Matrix& src);
const Matrix dotMultiply(const Matrix &lhs, const Matrix &rhs);
const Matrix dotDivide(const Matrix &lhs, const Matrix &rhs);

const valarray<double> operator*(const Matrix& m, const valarray<double>& v);
const valarray<double> operator*(const valarray<double>& v, const Matrix& m);
const Matrix operator*(const Matrix& lhs, const Matrix& rhs);
const Matrix operator+(const Matrix& lhs, const Matrix& rhs);
const Matrix operator-(const Matrix& lhs, const Matrix& rhs);
const Matrix operator*(double lhs, const Matrix& rhs);
const Matrix operator+(const Matrix& lhs, const Matrix& rhs);
const Matrix operator+(const Matrix& lhs, double rhs);
const Matrix operator-(const Matrix& lhs, double rhs);
const Matrix operator*(const Matrix& lhs, double rhs);
const Matrix operator/(const Matrix& lhs, double rhs);
/*! \brief Solves a linear system.

    If a square matrix, takes the inverse to solve. 
    If not, then uses least squares to solve.

    This system works best if all components are normlized beforehand,
    or if some component is more important, then weight it appropriately.
*/
const valarray<double> operator/(const Matrix& lhs, const valarray<double>& rhs);
//const vector<double> operator/(const Matrix &lhs, const vector<double> &rhs);

const Matrix outerProduct(const valarray<double>& lhs, const valarray<double> &rhs);

// ------------------------------------------------------------
// mathematical functions
const Matrix inverse(const Matrix& m);
const Matrix transpose(const Matrix& m);
/*!  \brief Computes all eigenvalues and eigenvectors of a real symmetric matrix.

     Eigenvalues are returned in ascending order (if it worked).
*/
const valarray<double> symmetric_eigenvector(const Matrix &m, Matrix &e);
const valarray<double> symmetric_eigen(const Matrix &m);
const valarray<double> eigenvector(const Matrix &m, Matrix &e);
const valarray<double> eigen(const Matrix &m);
const Matrix identityMatrix(size_t rows, size_t cols);
const Matrix zeroMatrix(size_t rows, size_t cols);
const Matrix rotationMatrix3D(double angle, double ux, double uy, double uz);
const Matrix translationMatrix3D(double tx, double ty, double tz);
const Matrix scaleMatrix3D(double sx, double sy, double sz);

/* ! \brief Finds the vector corresponding to the lowest eigenvalue.

     Uses the SVD to find where \ft^TM = 0\f or \fM^Tt = 0\f.
*/
valarray<double> findLeftNullVector(Matrix &M);
valarray<double> findLeftNullVector2(Matrix M);

valarray<double> findRightNullVector(Matrix &M);
valarray<double> findRightNullVector2(Matrix M);
Matrix makeSkewSymmetricMatrix(valarray<double> &x);

double determinant(const Matrix&);
double trace(const Matrix &);
size_t rank(const Matrix&);
double L2norm(const Matrix&);
double norm(const Matrix&);

// ------------------------------------------------------------
// numerical methods
/*! \brief Provides an upper triangular matrix to solve linear systems.

    Uses elementary row operations (adding rows, multiplying by a scalar,
    switching rows) to simplify matrix to an upper triangular.
*/
const Matrix gaussianElimination(const Matrix &A, const valarray<double> &b);

/*! \brief Solves an upper triangular system of equations through backsubstitution.
 */
const valarray<double> backsubstitution(const Matrix &A);

/*! \brief Does a 2D convolution. Returns x = A*B. 

    Should classify what happens if filter is too big or too small for region.
 */
const Matrix convolve2D(const Matrix &A, const Matrix &B);

/*! \brief Computes the eigenvalues of a symmetric 2x2 matrix.

    A direct method that should take less time and require
    less overhead.

		It has been tested and compared against MatLab.
		Fixed to handle [1 0; 0 1] by making sure that
		fabs(ast) > 1e-12.

		Returns values with smallest first
*/
valarray<double> computeSymmetricEigenvalues2x2(double ass, double ast, double att);



#endif //__MATRIX_HPP__
