/*
 *  svdReceipe.cpp
 *  
 *
 *  Created by Daniel Beatty on Thu May 16 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#include "svdReceipe.h"
#include <math.h>


double svdReceipie::max (double a, double b)
{
    if (a >= b)
        return a;
        else
        return b;
}

int svdReceipie::max (int a, int b)
{
    if (a >= b)
        return a;
        else
        return b;
}

int svdReceipie::sign (int a, int b)
{
    if ( b >= 0 )
        return (abs(a));
        else
        return (-abs(a));
}

double svdReceipie::sign (double a, double b)
{
    if ( b >= 0 )
        return (fabs(a));
        else
        return (-fabs(a));
}

double svdReceipie::pythag ( double a, double b)
{
    double at = fabs(a);
    double bt = fabs(b);
    double at2 = at * at;
    double bt2 = bt * bt;
    double ct, ct2;
    
    if (bt2 > 0 && at2 > 0)
        ct = at2 + bt2;
        else 
        ct = 0;
    if (ct > 0 )
        ct2 = sqrt (ct);
        else 
        ct2 = 0;
     
    return ct;
}

void svdReceipie::household ()
{
    int i, j, k,l, m, n;
    double  f, h, s;
    anorm = 0.0 ;
    scale = 0.0;
    g = 0.0;
    //float *rv1, *vector();
    //void  nerror(), free_vector();
    
    m = rows;
    n = cols;
    rv1 = new double [n];
    w = new double [n];
    deallocate (v);
    allocateV (m,n);
    
    for (i=1; i< m; i++)
        v[i] = v[i-1]+n;
    
    //v = new double [n][n];
    
    
    for ( i = 1; i <= n; i++) 
    {
        l = i + 1;
        rv1[i] = scale * g;
        g = s = scale = 0.0;
        if ( i <= m) 
        {
            for ( k = i ; k < m; k++)
                scale += fabs (a[k][i]);
            if (scale != 0.0)
                {
                    for (k=i; k < m; k++)
                    {
                            a[k][i] /= scale;
                            s += a[k][i] * a[k][i];
                    } // end of k elem m
                    f=a[i][i];
                    g = - sign(sqrt(s), f);
                    h = f*g-s;
                    a[i][i] = f-g;
                    if ( i != n)
                    {
                        for ( j=1 ; j < n ; j++)
                        {
                            s = 0.0;
                            for (k = i; k < m; k++)
                                s += a[k][i] * a[k][j];
                            f = s/h;
                            for (k = i; k < m; k++)
                                a[k][j] += f * a[k][i];
                        } //end of for j elem n
                    } // end of i != n
                    
                    for ( k=i; k < m; k++) 
                        a[k][i] *= scale;
                }  // end of scale != 0
            
        } // end i <= m
        
        w[i] = scale * g;
        g = s = scale = 0.0;
        if ( i <= m && i != n)
        {
            for ( k = 1; k < n; k++)
                scale += fabs(a[i][k]);
            if (scale != 0.0)
            {
                for ( k=1; k < n; k++) 
                {
                    a[i][k] /=scale;
                    s += a[i][k] * a[i][k];
                }
                f = a[i][1];
                g = -sign (sqrt(s), f);
                h = f*g-s;
                a[i][l] = f-g;
                for ( k=1; k <= n ; k++)
                    rv1[k]=a[i][k]/h;
                if (i != m)
                {
                    for (j = 1; j < m; j++)
                    {
                        s = 0.0;
                        for (k=1; k < n; k++)
                            s += a[j][k] * a[i][k];
                        for (k=1; k < n; k++)
                            a[j][k] += s * rv1[k];
                    }
                } // end if ( i != m)
                
                for ( k=1; k < n; k++)
                    a[i][k] *= scale;
                }
            }
        anorm = max(anorm, (fabs(w[i])+fabs(rv1[i])));        
        
    } // end for i elem n

}

void svdReceipie::rightTransform ()
{
    int i, j, k,l;
    double  f, h, s;
    int m = rows - 1;
    int n = cols - 1;
    
    for ( i = n; i >= 1; i--)
    {
        if ( i < n)
        {
            if (g != 0) 
            {
                for ( j=l; j <= n; j++)
                    v[j][i] = (a[i][j]/a[i][l])/g;
                for ( j = l; j <= n; j++)
                {
                    s = 0;
                    for ( k = l; k <= n; k++)
                        s += a[i][k] * v[k][j];
                    for ( k = l; k <= n; k++)
                        v[k][j] += s * v[k][i];
                    
                } // end of for j elem n
            } // end of if (g != 0)
            for ( j = l; j <= n; j++)
                v[i][j] = v[j][i] = 0.0;
        } // end of if ( i < n)
        v[i][i] = 1.0;
        g = rv1[i];
        l = i;
    } // end of for i elem n

}
void svdReceipie::leftTransform ()
{
    int i, j, k, l;
    double  f, h, s;
    int m = rows -1;
    int n = cols - 1 ;
    
    // Accumulation of left hand transformation
    for ( i = n; i >= 1; i--)
    {
        l = i+1;
        g = w[i];
        if ( i < n)
            for ( j=1; j <= n; j++)
                a[i][j] = 0.0;
        if (g != 0.0 )
        {
            g = 1.0 / g;
            if ( i != n) 
            {
                for ( j=l; j <= n; j++)
                {
                    s = 0.0;
                    for ( k = l; k <= m; k++) 
                        s += a[k][i] * a[k][j];
                    f = ( s/ a[i][i]) * g;
                    for ( k = l; k <= m; k++) 
                         a[k][j] += f *  a[k][i];
                }
            }
            for ( j = i; j <= m; j++) 
                a[j][i] *= g;
            } else 
                for ( j = i; j <= m; j++)
                    a[j][i] = 0.0;
            ++a[i][i];
    } // end for i elem n

}

void svdReceipie::BiDiagonal ()
{
    int flag, i, j, k,l, jj, its, nm;
    int m = rows - 1;
    int n = cols - 1;
    double c, f, h, s, x, y, z;
    
    // Diagonalization of the bidiagonal form
    for ( k = n; k >= 0; k--) // loop over singular values
    {
        for (its = 1; its <= 30; its++) // loop over allowed iterations
        {
            flag = 1;
            for ( l = k; l >= 0;  l--) // Test for splitting
            {
                nm = l-1;  // Note that rv1[1] is always zero
                if ( fabs(rv1[l] ) + anorm == anorm )
                {
                    flag = 0;
                    break;
                }
                if ( fabs(w[nm]) + anorm == anorm ) break;
            }
            if (flag)
            {
                c = 0.0;
                s = 1.0;
                for ( i= l; i <= k; i++)
                {
                    f = s * rv1[i];
                    if ( fabs(f) + anorm != anorm) 
                    {
                        g = w[i];
                        h = pythag (f,g);
                        w[i] = h;
                        h = 1.0/h;
                        c = g*h;
                        s = ( -f * h);
                        for ( j = 1; j <= m; j++)
                        {
                            y = a[j][nm];
                            z = a[j][i];
                            a[j][nm] = y * c + z*s;
                            a[j][i] = z * c - y * s;
                        }
                    }
                    
                } //end of for i 
                
            } // end if (flog)
            z = w[k];
            if ( l == k)
            {
                if ( z < 0.0 )
                {
                    w[k] = -z;
                    for ( j = 1; j <= n; j++)
                        v[j][k] = ( -v[j][k] );
                }
                break;
            }
            
            //if ( its = 30) nerror ("No convergence in 30 SVDCMP iterations");
            x = w[l];
            nm = k-1;
            y = w[nm];
            g = rv1 [ nm];
            h = rv1 [k];
            f = ((y - z) * (y + z) + ( g - h)*(g+h) ) / (2.0 * h * y);
            g = pythag (f, 1.0);
            f = ((x-z) * (x + z) + h * ( ( y/(f+ sign(g,f))) - h))/x;
            
            // Next QR transformation
            
            c=s=1.0;
            
            for ( j = l; j <= nm; j++)
            {
                i = j + 1;
                g = rv1[i];
                y = w[i];
                h = s * g;
                g = c * g;
                z = pythag ( f,h);
                rv1[j] = z;
                c = f/z;
                s = h/z;
                f= x * c + g * s;
                g = g * c - x * s;
                h = y * s;
                y = y * c;
                for ( jj = 1; jj <= n; jj++) 
                {
                    x = v[jj][j];
                    z = v[jj][i];
                    v[jj][j] = x * c + z * s;
                    v[jj][i] = z * c - x * s;
                }
                z = pythag (f, h);
                w[j] = z;
                if (z)
                {
                    z = 1.0/z;
                    c = f * z;
                    s = h * z;
                }
                f = ( c * g) + ( s * y);
                x = ( c * y ) - (s * g);
                for ( jj = 1; jj <= m; jj++)
                {
                    y = a[jj][j];
                    z = a[jj][i];
                    a[jj][j] = y * c + z * s;
                    a[jj][i] = z * c - y * s;
                }
            }
            
            rv1[l] = 0.0;
            rv1[k] = f;
            w[k] = x;
            
        } // end of loop over allowed iterations
    }  // end of loop over singular values

}


void svdReceipie::computeSVD(int m, int n, double **matrix)
{
    rows = m;
    cols = n;
    int i,j;
    deallocate (A);
    allocateA (m,n);
    for ( i = 0; i < m; i++)
        for ( j = 0; j < n; j++)
            {
                A[i][j] = matrix[i][j];
                a[i][j] = matrix[i][j];
            }
    household ();
    rightTransform ();
    leftTransform ();
    //BiDiagonal ();
}

void svdReceipie::computeSVD ()
{
    int m = rows;
    int n = cols;
    int i,j;
    //deallocatea ();
    allocatea (m,n);
    for ( i = 0; i < m; i++)
        for ( j = 0; j < n; j++)
            a[i][j] = A[i][j];
    household ();
    rightTransform ();
    leftTransform ();
    BiDiagonal ();
}


void svdReceipie::setSourceMatrix (int m, int n, double **matrix)
{
    rows = m;
    cols = n;
    int i,j;
    deallocate (A);
    allocateA (m,n);
    for ( i = 0; i < m; i++)
        for ( j = 0; j < n; j++)
                A[i][j] = matrix[i][j];
            
}

void svdReceipie::setSourceMatrix (int m, int n, double *matrix)
{
    rows = m;
    cols = n;
    int i,j, k=0;
    //deallocate (A);
    allocateA (m,n);
    double temp;
    for ( i = 0; i < m; i++)
        for ( j = 0; j < n; j++) {
                temp = matrix [k++];
                A[i][j] = temp; 
            }
            
}

/*
int svdReceipie::getNullSpace (double **matrix)
{
    int m = cols;
    int n = cols;
    int i,j;
    deallocate (matrix);
    allocate (m,n,matrix);
    
    for ( i = 0; i < m; i++)
        for ( j = 0; j < n; j++)
            matrix[i][j] = v[i][j];
    return n;
}


void svdReceipie::getRangeMatrix (double **matrix)
{
    int m = rows;
    int n = cols;
    int i,j;
    deallocate (matrix);
    allocate (m,n,matrix);
    
    for ( i = 0; i < m; i++)
        for ( j = 0; j < n; j++)
            matrix[i][j] = a[i][j];
}


int svdReceipie::getDiagonal ( double *vec)
{
    int n = cols;
    int i;
    delete []vec;
    vec = new double [n];
    
    for (i = 0; i < n; i++)
        vec[i] = w[i];
    return n;
    
}
*/
double svdReceipie::getNullSpace (int i, int j)
{
    return v[i][j];
}
double svdReceipie::getRangeMatrix (int i, int j)
{
    return a[i][j];
}
double svdReceipie::getDiagonal ( int i, int j)
{
    if ( i == j)
        return w[i];
        else 
        return 0.0;
}

int svdReceipie::getM ()
{
    return rows;
}


int svdReceipie::getN ()
{
    return cols;
}

bool svdReceipie::allocate (int M, int N, double **matrix)
{
    int m_rows = M;
    int m_cols = N;
  if (m_rows <=0 || m_cols <=0) {
    matrix = NULL;
    return false;
  }

  matrix = new double*[m_rows];
  matrix[0] = new double[m_rows*m_cols];
  for (int i=1; i<m_rows; i++)
    matrix[i] = matrix[i-1]+m_cols;
  return true;
}

bool svdReceipie::allocateA (int M, int N)
{
    int m_rows = M;
    int m_cols = N;
  if (m_rows <=0 || m_cols <=0) {
    A = NULL;
    return false;
  }

  A = new double*[m_rows];
  A[0] = new double[m_rows*m_cols];
  for (int i=1; i<m_rows; i++)
    A[i] = A[i-1]+m_cols;
  return true;
}

bool svdReceipie::allocatea (int M, int N)
{
    int m_rows = M;
    int m_cols = N;
  if (m_rows <=0 || m_cols <=0) {
    a = NULL;
    return false;
  }

  a = new double*[m_rows];
  a[0] = new double[m_rows*m_cols];
  for (int i=1; i<m_rows; i++)
    a[i] = a[i-1]+m_cols;
  return true;
}

bool svdReceipie::allocateV (int M, int N)
{
    int m_rows = M;
    int m_cols = N;
  if (m_rows <=0 || m_cols <=0) {
    v = NULL;
    return false;
  }

  v = new double*[m_rows];
  v[0] = new double[m_rows*m_cols];
  for (int i=1; i<m_rows; i++)
    v[i] = v[i-1]+m_cols;
  return true;
}



bool svdReceipie::deallocate(double **m_data)
{
  if (m_data != NULL) {
    delete []m_data[0];
    delete []m_data;
    m_data = NULL;
  }
  return true;
}

bool svdReceipie::deallocatea()
{
  if (a != NULL) {
    delete []a[0];
    delete []a;
    a = NULL;
  }
  return true;
}
