/*
 *  myVector.h
 *  numMethod6
 *
 *  Created by Daniel Beatty on Fri Nov 08 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

//#include <Carbon/Carbon.h>

#ifndef __MYVECTOR_H__
#define __MYVECTOR_H__
class myVector
{
    private:
        //vector<double>  m_data;
        double *m_data;
        int size;
    public:
        //matrixType matrixSym (myVector x);
        myVector (int Size);
        myVector ();
	~myVector ();
        double L2 ();
        int Size()  {return size;};
        void Size (int n); // { m_data.reserve(size = n);};
	void allocate (int i);
	void deallocate ();
        //void rowExtract ( matrixType A, int row, int start, int col);
        //void colExtract ( matrixType A, int row, int col, int start);
        myVector& operator= (/*const*/ myVector& x);
        myVector& operator+= (/*const*/ myVector x);
        myVector& operator-= (/*const*/ myVector x);
        myVector& operator*= (/*const*/ double x);
        //myVector operator*= (matrixType A);
        //myVector& mul ( matrixType A);
        double &operator[](int i) { return m_data[i];};
        double dot (/*const*/ myVector &x);
        double dot ();
        double sign (int i); 
        
        myVector& operator/= (const double x);

};
#endif
