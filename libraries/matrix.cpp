/*	-----------------------------------------------------------------------
	Matrix.cpp

	Notes:
	1/31/05  Sak Added M_PI definition			 
	1/31/05  Not needed .. in config.hpp

	----------------------------------------------------------------------- */


#include "matrix.hpp"

// ------------------------------------------------------------------
// special for lapack routines
// ------------------------------------------------------------------

void copyMatrixToLapack(int rows, int cols, const Matrix input, double * output)
{
  int r, c;
  int count = 0;
  for (c=0; c<cols; c++) 
    for (r=0; r<rows; r++) 
      output[count++] = input(r,c);
}

void copyLapackToMatrix(int rows, int cols, const double *input, Matrix &output)
{
  int r, c;
  int count = 0;
  for (c=0; c<cols; c++)
    for (r=0; r<rows; r++) 
      output(r,c) = input[count++];
}


extern "C" void dgetrf_(
        const int &m,                   // (input)
        const int &n,                   // (input)
        double *a,                      // a[n][lda] (input/output)
        const int &lda,                 // (input)
        int *ipiv,                      // ipiv[min(m,n)] (output)
        int &info                       // (output)
        );

extern "C" void dgetri_(
        const int &n,                   // (input)
        double *a,                      // a[n][lda] (input/output)
        const int &lda,                 // (input)
        const int *ipiv,                // ipiv[n] (input)
        double *work,                   // work[lwork] (workspace/output)
        const int &lwork,               // (input)
        int &info                       // (output)
        );

extern "C" void dgesvd_(
        const char &jobu,               // (input)
        const char &jobvt,              // (input)
        const int &m,                   // (input)
        const int &n,                   // (input)
        double *a,                      // a[n][lda] (input/output)
        const int &lda,                 // (input)
        double *s,                      // s[min(m,n)] (output)
        double *u,                      // u[ucol][ldu] (output)
        const int &ldu,                 // (input)
        double *vt,                     // vt[n][ldvt] (output)
        const int &ldvt,                // (input)
        double *work,                   // work[lwork] (workspace/output)
        const int &lwork,               // (input)
        int &info                       // (output)
        );

//asymmetric and real
extern "C" void dgeev_(const char &jobvl,	// compute the left eigenvectors (V/N)
		       const char &jobvr,	// compute the right eigenvectors (V/N)
		       const int &n,		// the order of the matrix
		       double *a,		// the array
		       const int &lda,		// the leading dimension
		       double *wr,		// real part eigenvalues
		       double *wi,		// imaginary part eigenvalues
		       double *vl,		// the array to store the left eigenvectgors
		       const int &ldvl,		// the leading dimension
		       double *vr,		// the array to store the right eigenvectors
		       const int &ldvr,		// the leading dimension of vr
		       double *work,		// workspace ... if info ==0, this returns best space
		       const int &lwork,	// thje dimension of the workspace
		       int &info);		// the success

//symmetric and real
extern "C" int dsyev_(
		      const char &jobz,	     // 'N' : eigen values only, 'V' : include eigenvectors
		      const char &uplo,	     // 'U' : upper triangle is store, 'L': lower
		      const int &n,	     // 'order of the matrix
		      double *a,	     // double precision array (LDA, N)
		      const int &lda,	     // leading dimension of array
		      double *w,	     // egienvalues in ascending order
		      double *work,	     // workspace, if info=0, work(1) returns optimal lwork
		      const int &lwork,	     // the length of the array work
		      int &info		     // 0 successful exit, < 0, -ith element broken
		      );



//  ------------------------------------------------------------------

Matrix::Matrix()
{
  m_rows = 0;
  m_cols = 0;
  v = NULL;
}

Matrix::Matrix(size_t r, size_t c)
{
  m_rows = r;
  m_cols = c;
  v = new valarray<double>(r*c);
}

Matrix::Matrix(size_t r, size_t c, valarray<double> &src)
{
  m_rows = r;
  m_cols = c;
  v = new valarray<double>(m_rows*m_cols);
  *v = src;
}
Matrix::Matrix(size_t r, size_t c, double *src)
{
  m_rows = r;
  m_cols = c;
  v = new valarray<double>(m_rows*m_cols);
  int i=0;
  for (int r2=0; r2<m_rows; r2++)
    for (int c2=0; c2<m_cols; c2++)
      (*this)(r2,c2) = src[i++];
}

Matrix::Matrix(const Matrix& source)
{
  m_rows = source.m_rows;
  m_cols = source.m_cols;
  if (m_rows*m_cols != 0) {
    v = new valarray<double>(m_rows*m_cols);
    *v = *(source.v);
  }

  else 
    v = NULL;
}

Matrix::~Matrix()
{
  delete v;
  v = NULL;
}

void Matrix::saveMfile(char *filename, char *label)
{
  size_t r, c;
  ofstream outfile(filename);

  outfile << label << " = [\n";
  for (r=0; r<m_rows; r++) {
    for (c=0; c<m_cols; c++) 
      outfile << (*this)(r,c) << "\t";
    outfile << ";\n";
  }

  outfile << "];\n";
  outfile.close();
}

void Matrix::saveDATfile(char *filename)
{
  size_t r,c;
  ofstream outfile(filename);

  for (r=0; r<m_rows; r++) {
    for (c=0; c<m_cols; c++) 
      outfile << (*this)(r,c) << "\t";
    outfile << "\n";
  }
  outfile.close();
}

void Matrix::resize(size_t rows, size_t cols)
{
  if (m_rows != rows || m_cols != cols) {
    m_rows = rows;
    m_cols = cols;
    delete v;
    if (m_rows*m_cols != 0)
      v = new valarray<double>(m_rows*m_cols); 
  }
}

void Matrix::setColumn(int i, const valarray<double> &X)
{
  if (i >= m_cols)
    cerr << "bad column param in setColumn" << endl;

  for (int j=0; j<m_rows && j<X.size(); j++)
    (*this)(j,i) = X[j];
}

const valarray<double> Matrix::column(int i)
{
  valarray<double> c(m_rows);
  for (int j=0; j<m_rows; j++)
    c[j] = (*this)(j,i);
  return c;
}
const valarray<double> Matrix::row(int i)
{
  valarray<double> c(m_cols);
  for (int j=0; j<m_cols; j++)
    c[j] = (*this)(i,j);
  return c;
}

const Matrix& Matrix::operator=(const Matrix& source)
{
  if (m_rows != source.m_rows || m_cols != source.m_cols) {
    m_rows = source.m_rows;
    m_cols = source.m_cols;
    delete v;
    if (m_rows*m_cols != 0)
      v = new valarray<double>(m_rows*m_cols); 
  }
  if (m_rows*m_cols != 0)
    *v = *(source.v);
  else 
    v = NULL;

  return (*this);
}
double& Matrix::operator()(size_t x, size_t y)
{
  return (*v)[y*m_rows+x];
}
double Matrix::operator()(size_t x, size_t y) const
{
  return (*v)[y*m_rows+x];
}
const Matrix& Matrix::operator=(double d)
{
  (*v) = d;
  return *this;
}
const Matrix& Matrix::operator*=(double d)
{
  (*v) *= d;
  return *this;
}
const Matrix& Matrix::operator/=(double d)
{
  (*v) /= d;
  return *this;
}
const Matrix& Matrix::operator+=(double d)
{
  (*v) += d;
  return *this;
}
const Matrix& Matrix::operator-=(double d)
{
  (*v) -= d;
  return *this;
}

const Matrix& Matrix::operator*=(const Matrix& m)
{ 
  if (m_cols != m.m_rows) 
    cerr << "Error!!! Matrix::operator*= m_cols != m.m_rows" << endl;

  size_t r, c, k;
  Matrix X(m_rows, m.m_cols);

  for (r=0; r<m_rows; r++)
    for (c=0; c<m.m_cols; c++) {
      X(r,c) = 0.0;
      for (k=0; k<m_cols; k++)
	X(r,c) += (*this)(r,k)*m(k,c);
    }

  (*this) = X;
  return *this;
}
const Matrix& Matrix::operator+=(const Matrix& m)
{
  *v += *(m.v);
  return *this;
}
const Matrix& Matrix::operator-=(const Matrix& m)
{
  *v -= *(m.v);
  return *this;
}

ostream& operator<<(ostream& os, Matrix& m)
{
  for(int x=0; x<m.getRowSize(); x++) {
    for(int y=0; y<m.getColSize(); y++)
      os<<m(x,y)<<"\t";
    os << "\n";
  }
  return os;
}

//----------------------------------------------------------------------
//numerical, other operations
double Matrix::bilinear(double x, double y) const
{
  int r = int(y);
  int c = int(x);

  //first check boundary conditions
  if ((r<0) || (r>m_rows-2) || (c<0) || (c>m_cols-2))
    //invalid ... outside boundaries
    return -1;

  double t = (x-c);
  double u = (y-r);

  return ((1-t)*(1-u)*(*this)(r,c) +
	  (1-t)*u*(*this)(r+1,c) +
	  t*u*(*this)(r+1,c+1) +
	  t*(1-u)*(*this)(r,c+1));
}


// ------------------------------------------------------------
// mathematical functions
const Matrix identityMatrix(size_t rows, size_t cols)
{
  Matrix X(rows, cols);
  for (int r=0; r<rows; r++)
    for (int c=0; c<cols; c++)
      X(r,c) = (r==c);

  return X;
}

const Matrix zeroMatrix(size_t rows, size_t cols)
{
  Matrix X(rows, cols);
  for (int r=0; r<rows; r++)
    for (int c=0; c<cols; c++)
      X(r,c) = 0;
  return X;
}

const valarray<double> eigen(const Matrix &m)
{
  int rows = m.getRowSize();
  int cols = m.getColSize();

  double *A = new double[rows*cols];
  double *evalr = new double[rows];
  double *evali = new double[rows];
  int ldvl = rows;
  int ldvr = rows;
  double *vl = new double[rows*cols];
  double *vr = new double[rows*cols];
  
  copyMatrixToLapack(rows, cols, m, A);

  int info;
  double *work = new double[1];
  int size = 1;

  //this is done initially to determine how big to make the workspace
  dgeev_('N', 'N', cols, A, rows, evalr, evali, vl, ldvl, vr, ldvr, work, -1, info);
  if (info == 0){
    size = int(work[0]);
    delete []work;
    work = new double[size];
  }

  //this is done initially to determine how big to make the workspace
  dgeev_('N', 'N', cols, A, rows, evalr, evali, vl, ldvl, vr, ldvr, work, size, info);

  delete []A;
  delete []work;
  delete []vl;
  delete []vr;

  valarray<double> results(rows);
  for (int i=0;i<rows;i++)
    results[i] = evalr[i];

  delete []evalr;
  delete []evali;

  return results;
}

const valarray<double> symmetric_eigen(const Matrix &m)
{
  int rows = m.getRowSize();
  int cols = m.getColSize();

  double *A = new double[rows*cols];
  double *evals = new double[rows];
  copyMatrixToLapack(rows, cols, m, A);

  int info;
  double *work = new double[1];
  int size = 1;

  int i = 0;


  //this is done initially to determine how big to make the workspace
  dsyev_('N', 'U', cols, A, rows, evals, work, -1, info);
  if (info == 0){
    size = int(work[0]);
    delete []work;
    work = new double[size];
  }

  //this is done initially to determine how big to make the workspace
  dsyev_('N', 'U', cols, A, rows, evals, work, size, info);

  delete []A;
  delete []work;
  
  valarray<double> results(rows);
  for (i=0;i<rows;i++)
    results[i] = evals[i];

  delete []evals;

  return results;
}

const valarray<double> symmetric_eigenvector(const Matrix &m, Matrix &e)
{
  int rows = m.getRowSize();
  int cols = m.getColSize();

  int i = 0;

  double *A = new double[rows*cols];
  double *evals = new double[rows];
  copyMatrixToLapack(rows, cols, m, A);

  int info;
  double *work = new double[1];
  int size = 1;

  //this is done initially to determine how big to make the workspace
  dsyev_('V', 'U', cols, A, rows, evals, work, -1, info);
  if (info == 0){
    size = int(work[0]);
    delete []work;
    work = new double[size];
  }

  //this is done initially to determine how big to make the workspace
  dsyev_('V', 'U', cols, A, rows, evals, work, size, info);

  copyLapackToMatrix(m.getRowSize(), m.getColSize(), A, e);
  
  delete []A;
  delete []work;
  
  valarray<double> results(rows);
  for (i=0;i<rows;i++)
    results[i] = evals[i];

  delete [] evals;

  return results;
}

const valarray<double> eigenvector(const Matrix &m, Matrix &e)
{
  int rows = m.getRowSize();
  int cols = m.getColSize();

  int i = 0;

  double *A = new double[rows*cols];
  double *evalr = new double[rows];
  double *evali = new double[rows];
  int ldvl = rows;
  int ldvr = rows;
  double *vl = new double[rows*cols];
  double *vr = new double[rows*cols];
  copyMatrixToLapack(rows, cols, m, A);

  int info;
  double *work = new double[1];
  int size = 1;

  cout << "See how big space is" << endl;

  //this is done initially to determine how big to make the workspace
  dgeev_('V', 'V', cols, A, rows, evalr, evali, vl, ldvl, vr, ldvr, work, -1, info);
  if (info == 0){
    size = int(work[0]);
    delete []work;
    work = new double[size];
  }

  cout << "Now do the operations" << endl;
  dgeev_('V', 'V', cols, A, rows, evalr, evali, vl, ldvl, vr, ldvr, work, size, info);

  copyLapackToMatrix(m.getRowSize(), m.getColSize(), vr, e);
  
  delete []A;
  delete []work;
  delete []vr;
  delete []vl;
  
  valarray<double> results(rows);
  for (i=0;i<rows;i++)
    results[i] = evalr[i];

  delete [] evalr;
  delete [] evali;

  return results;
}

const Matrix inverse(const Matrix& m)
{
  int N = m.getRowSize();
  if (m.getColSize() != N) {
    cerr << "Matrix::inverse --- Matrix not square!\n";
    exit(1);
  }
  Matrix I(N,N);

  double *A = new double[N*N];
  int info;
  int *ipiv = new int[N];

  copyMatrixToLapack(N, N, m, A);
  dgetrf_(N, N, A, N, ipiv, info);
  double *work = new double[1];
  int size = 1;
  //this is done initially to determine how big to make the workspace
  dgetri_(N, A, N, ipiv, work, -1, info);
  if (info == 0) {
    size = int(work[0]);
    delete []work;
    work = new double[size];
  }
  dgetri_(N, A, N, ipiv, work, size, info);
  copyLapackToMatrix(N, N, A, I);
  
  delete []work;
  delete []A;
  delete []ipiv;

  return I;
}

//#define __max(a,b) ( (a) > (b) ? (a) : (b))
//#define __min(a,b) ( (a) < (b) ? (a) : (b))


//singular value decomposition
//taken from yeti
void Matrix::svd(Matrix& U, valarray<double>& D, Matrix& VT)
{
  int i;

  double *A = new double[m_rows*m_cols];
  copyMatrixToLapack(m_rows, m_cols, (*this), A);

  double *S = new double[__min(int(m_rows), int(m_cols))];
  double *U2 = new double[m_rows*m_rows];//min(m_rows, m_cols)];
  int ldvt = m_cols;//min(m_rows, m_cols);
  double *VT2 = new double[m_cols*m_cols];
  int lwork = __max(3*__min(int(m_rows), int(m_cols))+__max(int(m_rows), int(m_cols)),5*__min(int(m_rows),int(m_cols)));
  double * WORK = new double[lwork];
  int info;

  dgesvd_( 'A', 'A', m_rows, m_cols, A, m_rows, S, U2, m_rows, VT2, ldvt, WORK, lwork, info );

  U.resize(m_rows, m_rows);//min(m_rows, m_cols));
  copyLapackToMatrix(m_rows, m_rows, U2, U);
  D.resize(__min(m_rows, m_cols));
  for (i=0; i<__min(m_rows, m_cols); i++) 
    D[i] = S[i];
  VT.resize(m_cols, m_cols);
  copyLapackToMatrix(m_cols, m_cols, VT2, VT);

  delete []A;
  delete []U2;
  delete []S;
  delete []VT2;
  delete []WORK;
}

const Matrix rotationMatrix3D(degree angle, double ux, double uy, double uz)
{
  radian radAngle;
  radAngle = double(angle)/360.0*2.0*M_PI;

	double norm = sqrt(ux*ux+uy*uy+uz*uz);
	ux /= norm;
	uy /= norm;
	uz /= norm;

  double s, c;

  Matrix R(4, 4);
  R = 0.0;

  c = cos(radAngle);
  s = sin(radAngle);

  R(0, 0) = c+(1-c)*ux*ux;
  R(0, 1) = (1-c)*uy*ux - s*uz;
  R(0, 2) = (1-c)*uz*ux + s*uy;
  
  R(1, 0) = (1-c)*ux*uy+s*uz;
  R(1, 1) = c + (1-c)*uy*uy;
  R(1, 2) = (1-c)*uz*uy - s*ux;
  
  R(2, 0) = (1-c)*ux*uz - s*uy;
  R(2, 1) = (1-c)*uy*uz + s*ux;
  R(2, 2) = c  +(1-c)*uz*uz;

  R(3, 3) = 1.0;

  return R;
}
const Matrix transpose(const Matrix& m)
{
  int r,c;

  int rows = m.getRowSize();
  int cols = m.getColSize();
  Matrix T(cols, rows);

  for (r=0; r<rows; r++)
    for (c=0; c<cols; c++) 
      T(c,r) = m(r,c);

  return T;
}

const Matrix translationMatrix3D(double tx, double ty, double tz)
{
  Matrix T(4,4);
  T = 0;
  T(0,0) = 1;
  T(1,1) = 1;
  T(2,2) = 1;
  T(3,3) = 1;
  T(0,3) = tx;
  T(1,3) = ty;
  T(2,3) = tz;
  return T;
}
const Matrix scaleMatrix3D(double sx, double sy, double sz)
{
  Matrix T(4,4);
  T = 0;
  T(0,0) = sx;
  T(1,1) = sy;
  T(2,2) = sz;
  T(3,3) = 1;
  return T;
}
//const Matrix scaleMatrix3D(double sx, double sy, double sz);

// ------------------------------------------------------------
// extra functions
// ------------------------------------------------------------
double Matrix::max()
{
  double m = (*v)[0];
  for (int i=1; i<m_rows*m_cols; i++)
    if ((*v)[i] > m)
      m = (*v)[i];
  return m;
}
double Matrix::min()
{
  double m = (*v)[0];
  for (int i=1; i<m_rows*m_cols; i++)
    if ((*v)[i] < m)
      m = (*v)[i];
  return m;
}



//-------------------------------------------------------------

const valarray<double> operator*(const Matrix& m, const valarray<double>& v)
{
  if (m.getColSize()!=v.size()) cerr << "wrong number of elements in m*v\n";

  size_t i, k;
  valarray<double> res(m.getRowSize());

  for (i=0; i<m.getRowSize(); i++) {
    res[i] = 0.0;
    for (k=0; k<v.size(); k++)
      res[i] += m(i,k)*v[k];
  }

  return res;
}
const valarray<double> operator*(const valarray<double>& v, const Matrix &m)
{
  if (m.getRowSize()!=v.size()) cerr << "wrong number of elements in m*v\n";

  valarray<double> res(m.getColSize());
  size_t i, k;

  for (i = 0; i<m.getColSize(); i++) {
    res[i] = 0.0;
    for (k=0; k<v.size(); k++) 
      res[i] += v[k]*m(k,i);
  }

  return res;
}

const Matrix operator*(const Matrix& lhs, const Matrix& rhs)
{
  size_t r, c, k;
  Matrix X(lhs.getRowSize(), rhs.getColSize());
  Matrix X2(lhs.getRowSize(), rhs.getColSize());

  for (r=0; r<X.getRowSize(); r++)
    for (c=0; c<X.getColSize(); c++) {
      X(r,c) = 0.0;
      for (k=0; k<lhs.getColSize(); k++)
	X(r,c) += lhs(r,k)*rhs(k,c);
    }

  /*  for (r=0; r<X.getRowSize(); r++)
    for (c=0; c<X.getColSize(); c++) 
      X2(r,c) = mul(lhs.row(r),rhs.column(c));

  double total = 0.0;
  for (r=0; r<X.getRowSize(); r++)
    for (c=0; c<X.getColSize(); c++) 
      total += fabs(X(r,c)-X2(r,c));

      cout << "Difference is " << total/(X.getRowSize()*X.getColSize()) << endl;*/


  return X;
}
const Matrix operator+(const Matrix& lhs, const Matrix& rhs)
{
  Matrix X = lhs;
  X += rhs;
  return X;
}
const Matrix operator-(const Matrix& lhs, const Matrix& rhs)
{
  Matrix X = lhs;
  X -= rhs;
  return X;
}

const Matrix operator*(double lhs, const Matrix& rhs)
{
  Matrix X = rhs;
  X *= lhs;
  
  return X;
}

const Matrix operator+(const Matrix &lhs, double rhs)
{
  Matrix X = lhs;
  X += rhs;

  return X;
}
const Matrix operator-(const Matrix &lhs, double rhs)
{
  Matrix X = lhs;
  X -= rhs;

  return X;
}
const Matrix operator*(const Matrix &lhs, double rhs)
{
  Matrix X = lhs;
  X *= rhs;

  return X;
}
const Matrix operator/(const Matrix &lhs, double rhs)
{
  Matrix X = lhs;
  X /= rhs;

  return X;
}

const valarray<double> operator/(const Matrix& lhs, const valarray<double>& rhs)
{
  if (lhs.getRowSize() == lhs.getColSize()) { //then square
    Matrix I = inverse(lhs);
    return I*rhs;
  }
  
  return inverse(transpose(lhs)*lhs)*transpose(lhs)*rhs;
}
/*
const vector<double> operator/(const Matrix& lhs, const vector<double>& rhs)
{
  if (lhs.getRowSize() == lhs.getColSize()) { //then square
    Matrix I = inverse(lhs);
    return I*rhs;
  }
  
  return inverse(transpose(lhs)*lhs)*transpose(lhs)*rhs;
}
*/
const Matrix pow(const Matrix &lhs, double rhs)
{
  Matrix X = lhs;
  for (int r=0; r<X.getRowSize(); r++)
    for (int c=0; c<X.getColSize(); c++)
      X(r,c) = pow(X(r,c), rhs);
  return X;
}
const Matrix sqrt(const Matrix &src)
{
  Matrix X(src.getRowSize(), src.getColSize());
  for (int r=0; r<X.getRowSize(); r++)
    for (int c=0; c<X.getColSize(); c++)
      X(r,c) = sqrt(src(r,c));
  return X;
}

double matrixsum(const Matrix& src)
{
  double total = 0;
  for (int r=0; r<src.getRowSize(); r++)
    for (int c=0; c<src.getColSize(); c++)
      total += src(r,c);
  return total;
}

const Matrix dotMultiply(const Matrix &lhs, const Matrix &rhs)
{
  Matrix X(lhs.getRowSize(), lhs.getColSize());
  for (int r=0; r<X.getRowSize(); r++)
    for (int c=0; c<X.getColSize(); c++)
      X(r,c) = lhs(r,c)*rhs(r,c);
  return X;
}

const Matrix dotDivide(const Matrix &lhs, const Matrix &rhs)
{
  Matrix X = lhs;
  for (int r=0; r<X.getRowSize(); r++)
    for (int c=0; c<X.getColSize(); c++)
      X(r,c) /= rhs(r,c);
  return X;
}

void Matrix::smooth(int filterLength)
{
  int r, c, l;
  double total;
  double x;

  valarray<double> filter(2*filterLength+1);

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
	total += (*this)(r+l,c)*filter[l+filterLength];
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
      (*this)(r,c) = total;
    }

  for (c=filterLength-1; c>=0; c--)
    for (r=0; r<m_rows; r++) {
      (*this)(r,c) = (*this)(r,c+1);
      (*this)(r,m_cols-1-c) = (*this)(r,m_cols-1-c-1);
    }
}

void Matrix::setSubMatrix(int row, int col, const Matrix &m)
{
  int r, c;

  if (row+m.getRowSize() <= m_rows && col+m.getColSize() <= m_cols) {
    for (r=0; r<m.getRowSize(); r++)
      for (c=0; c<m.getColSize(); c++) 
	(*this)(row+r,col+c) = m(r,c);
  }
}


const Matrix Matrix::subMatrix(int aRow, int aCol, int bRow, int bCol) const
{
  int rows = bRow - aRow;
  int cols = bCol - aCol;

  Matrix X(rows+1, cols+1);
  for (int r=0; r<=rows; r++)
    for (int c=0; c<=cols; c++)
      X(r,c) = (*this)(r+aRow,c+aCol);

  return X;
}


const Matrix Matrix::minorMatrix(int row, int col) const
{
  int r2, c2;
  int r3, c3;
  Matrix B(m_rows-1, m_cols-1);
  for (r2=0; r2<B.getRowSize(); r2++)
    for (c2=0; c2<B.getColSize(); c2++) {
      r3 = r2;
      if (r3 >= row)
	r3++;
      c3 = c2;
      if (c3 >= col)
	c3++;
      B(r2,c2) = (*this)(r3,c3);
    }

  return B;
}

const valarray<double> backsubstitution(const Matrix &A)
{
  valarray<double> X(A.getRowSize());
  double value;
  
  for (int r=A.getRowSize()-1; r>=0; r--) {
    value = A(r, A.getColSize()-1);
    for (int c=r+1; c<A.getColSize()-1; c++)
      value -= A(r,c)*X[c];
    X[r] = value/A(r,r);
  }

  return X;
}

const Matrix gaussianElimination(const Matrix &A, const valarray<double> &b)
{
  int rows = A.getRowSize();
  int r, r2, c;
  int maxPivotIndex;
  double arr, factor;
  double temp;
  const double EPSILON = 1e-6;

  //first set up matrix
  Matrix A2(rows, A.getColSize()+1);
  for (r=0; r<rows; r++) {
    for (c=0; c<A.getColSize(); c++)
      A2(r,c) = A(r,c);
    A2(r,A2.getColSize()-1) = b[r];
  }

  //
  int cols = A2.getColSize();

  for (r=0; r<rows; r++) {

    //first find pivot (largest absolute element along column)
    // pivoting is implemented ... everything works fine, 
    // but equations are reordered ... not a problem
    maxPivotIndex = r;
    for (r2=r+1; r2<rows; r2++)
      if (fabs(A2(r2,r)) > fabs(A2(maxPivotIndex,r)))
	maxPivotIndex = r2;
    if (maxPivotIndex != r) //must switch the rows
      for (c=r; c<cols; c++) {
	temp = A2(r, c);
	A2(r, c) = A2(maxPivotIndex, c);
	A2(maxPivotIndex, c) = temp;
      }
    
    // now divide by pivot ... make sure not zero
    arr = A2(r,r);
    if (fabs(arr) < EPSILON) {
      cerr << "Matrix is not full rank. " << endl;
    } 
    else {
      for (c=r; c<cols; c++) 
	A2(r,c) /= arr;
    
      //eliminate rows beneath
      for (r2=r+1; r2<rows; r2++) {
	factor = -A2(r2,r);
	for (c=r; c<cols; c++)
	  A2(r2, c) += A2(r,c)*factor;
      }
    }
  }

  return A2;
}

double determinant( const Matrix &A)
{
  int r;
  double det = 0.0;

  if (A.getRowSize() == 1 && A.getColSize() == 1)
    return A(0,0);

  for (r=0; r<A.getRowSize(); r++)
    det += A(r,0)*pow(float(-1),float(r))*determinant(A.minorMatrix(r, 0));

  return det;
}

double trace(const Matrix &A)
{
  size_t r;
  double sum = 0.0;

  // Note: square matrix is assumed
  for (r=0; r<A.getRowSize(); r++)
    sum += A(r,r);

  return sum;
}


double L2norm(const Matrix &A)
{
  
  // Currently, supports column vector and row vector
 
  int r;
  int c;
  int rowSize = A.getRowSize();
  int colSize = A.getColSize();

  double norm = 0.0;

  if (rowSize == 1 && colSize == 1)
    return A(0,0);

  // column vector
  if (colSize == 1)
    {
      for (r=0;r<rowSize;r++)
	norm += A(r,0)*A(r,0);
      return sqrt(norm);
    }

  // row vector
  if (rowSize == 1)
    {
      for (c=0;c<colSize;c++)
	norm += A(0,c)*A(0,c);
      return sqrt(norm);
    }
  
  return 0;
}

double norm(const Matrix &A)
{
  int r, c;
  double norm = 0.0;
  for (r=0; r<A.getRowSize(); r++)
    for (c=0; c<A.getColSize(); c++)
      norm += pow(A(r,c),2.0);
  return sqrt(norm/(A.getRowSize()*A.getColSize()));
}

//added mina->maxa, minb->maxb to avoid checking
//at boundary elements and wasting cycles
//should be faster
//-EDS 07/15/03
const Matrix convolve2D(const Matrix &A, const Matrix &B)
{
  Matrix result(A.getRowSize(), A.getColSize());

  double sum;
  int mina, maxa;
  int minb, maxb;
  int a, b;
  int k, l;

  for (int i=0; i<A.getRowSize(); i++)
    for (int j=0; j<A.getColSize(); j++) {
      sum = 0;

      mina = i-int(B.getRowSize())/2;
      maxa = i+int(B.getRowSize()+1)/2;
      k = B.getRowSize()-1;

      if (mina<0) {
	k += mina;
	mina = 0;
      }
      if (maxa>=A.getRowSize())
	maxa = A.getRowSize()-1;

      for (a=mina; a<maxa; k--, a++) {

	minb = j-int(B.getColSize())/2;
	maxb = j+int(B.getColSize())/2;
	l = B.getColSize()-1;

	if (minb<0) {
	  l += minb;
	  minb = 0;
	}
	if (maxb>=A.getColSize())
	  maxb = A.getColSize()-1;

	for (b=minb; b<=maxb; l--, b++) 
	  sum += A(a,b)*B(k,l);
      }

      result(i,j) = sum;
    }

  return result;
}

const Matrix outerProduct(const valarray<double>& lhs, const valarray<double> &rhs)
{
  Matrix result(lhs.size(), rhs.size());

  for (int r=0; r<lhs.size(); r++)
    for (int c=0; c<rhs.size(); c++)
      result(r,c) = lhs[r]*rhs[c];

  return result;
}


valarray<double> findLeftNullVector(Matrix &M)
{
  size_t i;

  Matrix U, VT;
  valarray<double> D;
  M.svd(U, D, VT);

  //would like to compare here

  valarray<double> u(U.getColSize());
  for (i=0; i<U.getColSize(); i++)
    u[i] = U(i,U.getRowSize()-1);

  return u;
}
valarray<double> findLeftNullVector2(Matrix M)
{
  Matrix U, VT;
  valarray<double> D;
  M.svd(U, D, VT);

  valarray<double> u = U.column(U.getRowSize()-1);
  return u;
}

//Find t where Mt = 0
valarray<double> findRightNullVector(Matrix &M)
{
  size_t i;

  Matrix U, VT;
  valarray<double> D;
  M.svd(U, D, VT);
 
  valarray<double> v(VT.getRowSize());
  for (i=0; i<VT.getRowSize(); i++)
    v[i] = VT(VT.getColSize()-1,i);

  return v;
}
//Find t where Mt = 0
valarray<double> findRightNullVector2(Matrix M)
{
  Matrix U, VT;
  valarray<double> D;
  M.svd(U, D, VT);

  valarray<double> v = VT.row(VT.getColSize()-1);
  return v;
}
Matrix makeSkewSymmetricMatrix(valarray<double> &x)
{
  if (x.size() != 3) {
    cerr << "ERROR: incoming valarray of incorrect size!" << endl;
    return identityMatrix(3,3);
  }

  Matrix M(3,3);
  M(0,0) = 0;
  M(0,1) = -x[2];
  M(0,2) = x[1];

  M(1,0) = x[2];
  M(1,1) = 0;
  M(1,2) = -x[0];

  M(2,0) = -x[1];
  M(2,1) = x[0];
  M(2,2) = 0;

  return M;
}

valarray<double> computeSymmetricEigenvalues2x2(double ass, double ast, double att)
{
	double epsilon = 1e-12;
	valarray<double> e(2);
	
	//perform a given's rotation {
	if (fabs(ast) < epsilon) {
		e[0] = ass;
		e[1] = att;
	}
	else {
		
		double d = sqrt(pow(ass-att,2.0)+4*ast*ast);
		double sin2t = 2*ast/d;
		double cos2t = (ass-att)/d;
		double d2 = sqrt(2*(1+cos2t));
		double sint = fabs(sin2t)/d2;
		double cost = (1+cos2t)/d2;
		if (cost*ast<0) //check the sign
			cost = -cost;
		
		e[0] = ass*cost*cost+2*ast*sint*cost+att*sint*sint;
		e[1] = ass*sint*sint-2*ast*sint*cost+att*cost*cost;
	}
	
	if (e[0] > e[1]) {
		double tmp = e[0];
		e[0] = e[1];
		e[1] = tmp;
	}
	return e;
	}

