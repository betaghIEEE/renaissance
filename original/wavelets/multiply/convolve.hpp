/*
 *  convolve.h
 *  math02
 *
 *  Created by Daniel Beatty on Sun Sep 22 2002.
 *  Copyright (c) 2002 Texas Tech University. All rights reserved.
 *
 */

//#include <Carbon/Carbon.h>
#ifndef __CONVOLVE_THIS_HPP__
#define __CONVOLVE_THIS_HPP__
//#include <valarray>
//#include "r_valarray.hpp"
#include "myVector.h"

class convolveThis {
    public:
        void convolve ( myVector &src, myVector &h, myVector &y);
        //matrix convolve ( matrix src, matrix h);
        
};

#endif // __CONVOLVE_THIS_HPP__
