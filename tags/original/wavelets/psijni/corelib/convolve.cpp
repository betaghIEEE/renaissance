/*
 *  convolve.cpp
 *  math02
 *
 *  Created by Daniel Beatty on Sun Sep 22 2002.
 *  Copyright (c) 2002 Texas Tech University. All rights reserved.
 *
 */

#include "convolve.hpp"
#include <iostream.h>

void convolveThis::convolve ( myVector &src, myVector &h, myVector &y)
{
    int i, j, l, m, n;
    m = n = src.Size();
    l = h.Size();
    //myVector y;
    y.Size(n);
    double x, ht, temp;
    x = ht = 0;
    
    for ( i = 0; i < m; i++)
        y[i] = 0;
    for ( i = 0; i < m; i++)
        for ( j =0; j < l; j++)
        {
            n = i -j;
            if ((n > 0) && (n < m))
            {
                //x = src[n];
                //ht = h[j];
                temp = y[i];
                temp += src[n] * h[j];
                y[i] = temp;
            }
        }
    //cout << endl;
    //for ( i = 0; i < n ; i++)
        //cout << "\t\t" << y[n]; 
    //cout << endl;
    //return y;
}
