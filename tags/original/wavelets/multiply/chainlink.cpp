/*
 *  chainlink.cpp
 *  
 *
 *  Created by Daniel Beatty on Sat Jun 14 2003.
 *  Copyright (c) 2003 DRILL, Texas Tech University. All rights reserved.
 *
 *
 *	Description:  (in LaTeX notation)
 *
 *	The purpose of the chain link class is the provide a loose 2-D structure
 *	which is an array of a structure which is itself an array.  There are
 *	special characteristics of this structure as opposed to a matrix.  
 *	First, each basic element has itself 2 items (the value and the key). 
 * 	Second is in keying track of which elements are empty.  
 *
 *	This class does not have a generic constructor.  Rather it has an 
 * 	allocate and deallocate method.  The allocator is responsible for
 *	generating the arrays and the arrays with in the array.  
 *
 * 	There is a set and get length method for setting manually the length 
 *	counter of the array.  In typical uses such as matrix multiply, this 
 *	will typically be the same as the number of rows/columns of the matrix 
 * 	feeding it.  
 *
 *	There are functions to access the value and keys of the links.  
 *

 *
 */

#include "chainlink.h"
#include <iostream.h>


void chainlink::allocateHooks (int size)
{
	theHooks = new hook[size];
}

void chainlink::allocateHooks (int hooksize, int linksize)
{
	theHooks = new hook[hooksize];
        int i; 
        for ( i = 0; i < hooksize; i++)
            theHooks[i].allocateLinks (linksize);
        length = hooksize;
}

void chainlink::display()
{
  for (int i=0; i<length; i++) {
    cout << i << ": ";
    theHooks[i].display();
    cout << endl;
  }
}




void chainlink::deallocateHooks ()
{
	delete theHooks;
}
int chainlink::getLength ()
{
	return length;
}

void chainlink::setLength (int s)
{
	length = s;
}

double chainlink::getL2Norm (int i)
{
	return theHooks[i].getl2norm();
}

double chainlink::calcL2Norm (int i)
{
	return theHooks[i].calcl2norm();
}

double chainlink::getHookLink (int i, int j)
{
	return theHooks[i].getChLink(j);
}


void chainlink::setHookLink (int i, int j, double val)
{
	if ( i < length)
		theHooks[i].setChLink(j, val);
	else
		cout << "Set operation attempting add value illegally";

}

int chainlink::getHookKey (int i, int j)
{
	return theHooks[i].getKey(j);
}


void chainlink::setHookKey (int i, int j, int key)
{
	if ( i < length)
		theHooks[i].setKey(j, key);
	else
		cout << "Set operation attempting add value illegally";

}

void chainlink::addHookLink (int i, int key, double val)
{
	if ( i < length);
		theHooks[i].addLink(key, val);
}
