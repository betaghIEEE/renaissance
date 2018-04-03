/*
 *  myVector.cpp
 *  numMethod6
 *
 *  Created by Daniel Beatty on Fri Nov 08 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#include "myVector.h"
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

using namespace std;

double myVector::L2 ()
{
    double temp = dot();
    return temp;
}


myVector& myVector::operator= (myVector& x)
{
    int i, n;
    int m = Size();
    int o = x.Size();
    double temp;
    if ( m != o)
        Size (o);
    //m_data.reserve(o);
    n = Size();
    for ( i = 0; i < n; i++)
    {
        temp = x[i];
        m_data[i] = temp;
    }
    return (*this);
}

myVector& myVector::operator+= (/*const*/ myVector x)
{
    int i /*, n*/;
    int m = Size();
    int o = x.Size();
    if ( m != o)
        return (*this);
    //n = Size();
    for ( i = 0; i < m; i++)
        m_data[i] += x[i];
    return (*this);
}
myVector& myVector::operator-= (/*const*/ myVector x){
    int i /*, n*/;
    int m = Size();
    int o = x.Size();
    if ( m != o)
        return (*this);
    //n = Size();
    for ( i = 0; i < m; i++)
        m_data[i] -= x[i];
    return (*this);
}

myVector& myVector::operator*= (/*const*/ double x)
{
    int i, n;
    n = Size();
    for ( i = 0; i < n; i++)
        m_data[i] *= x;
    return (*this);
}

/*

myVector& myVector::mul ( matrixType A )

{
            int n = Size();
            int i;
            myVector y;
            matrixType temp(n,1);
            matrixType B;
            for (i = 0; i < n; i++)
                temp[i][0] = m_data[i];
            B = A; 
            B *= temp;
            for (i = 0; i < n; i++) {
                m_data[i] = y[i] = B[i][0];
            }
            return y;
}
*/
double myVector::dot ()
{
    int n = Size();
    double temp = 0.0;
    int i;
    double temp2;
    for ( i = 0; i < n; i++)
    {
    	temp2 = m_data[i];
        temp += pow ( temp2, double(2));
	}
    return temp;
}

double myVector::dot (/*const*/ myVector &x)
{
    int n = Size();
    double temp = 0.0;
    int i;
    
    for ( i = 0; i < n; i++)
        temp += m_data[i] * x[i];
    return temp;
}


myVector& myVector::operator/= (/*const*/ double x)
{
    int i, n;
    n = Size();
    for ( i = 0; i < n; i++)
        m_data[i] /= x;
    return (*this);
}

double myVector::sign (int i)
{
    double temp = 1;
        if ( m_data[i] < 0)
            temp = -1;
    return temp;
}

void myVector::Size (int n)
{
    //if ( m_data != NULL)
    //    delete m_data;
    m_data = new double[n];
    size = n;
}

void myVector::deallocate ()
{
	if (size > 0)
		//delete []m_data;
	if ( m_data != NULL) {
		delete []m_data;
			m_data = NULL;
		}
	size = 0;	
}

void myVector::allocate (int i)
{
	Size(i);
}
myVector::myVector (int Size)
{
    size = Size;
    m_data = new double[size];
    for ( int i = 0; i < size; i++)
        m_data[i] = 0.0;
}

myVector::myVector ()
{
    size = 0;
    m_data = NULL;
}

myVector::~myVector ()
{
	deallocate();
}
