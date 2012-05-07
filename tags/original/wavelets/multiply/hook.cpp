/*
 *  hook.cpp
 *  
 *
 *  Created by Daniel Beatty on Sat Jun 14 2003.
 *  Copyright (c) 2003 Drill, Texas Tech University. 
 *	All rights reserved.
 *
 *
 *
 *	Description:  (in LaTeX notation)

 *
 *	The purpose of the hook class is to provide an array based
 * 	list structure.  It is a little less suffisticated than a 
 *	linked list.  However, versions of hook class can include
 *	a sort array to keep the hook links in order.  For 
 *	operations like matrix multiplication, it is expected that 
 *	data will be inserted in order, and therefore the need for 
 *	a sort is not as great.  Also, a function to delete elements
 *	is not provided.  
 
 *	The class has two constructors.  One default for initializing
 *	an empty hook.  The other is for generating a hook with
 *	specified number of links.  It also has a deconstructor 
 *	as well.  Not that allocateLinks and deallocateLinks are 
 *	provided as stand alone functions that do the same thing.

 *
 *	For the primitive data types, simple get and set functions
 *	are provided for simple access.  There is a possibility of 
 *	providing pointer based access.  Since both are really 
 *	method calls, then measurements of performance may be 
 *	in order.
 *
 *	Get, set, and add link functions are provide to access 
 *	the array of links.  The concept of add is requires that 
 *	a distinct difference between length and maxsize be made.
 *	Maxsize is a member variable that specifies the maximum 
 *	size for which the array of links is set.  Length is the 
 *	actual number elements that known to be good, accurate and 
 *	useful. A safe guard on the add/insert function is provided
 *	to prevent an error in writing.
 *
 *	The l2norm function is provided for convenance and is 
 *	not automatically calcuated upon the insertion of another
 *	element.  
 *
 *


 *
 
 */

#include "hook.h"
#include <iostream.h>

hook::hook()
{
	length = 0;
	l2norm = 0.0;
	links = 0;  // Null pointer
}

hook::hook(int size)
{
	length = 0;
	maxsize = size;
	l2norm = 0.0;
	links = new chlink[size];
}

void hook::allocateLinks (int size)
{
	links = new chlink[size];
	length = 0;
	l2norm = 0.0;
	maxsize = size;
}

hook::~hook()
{
	deallocateLinks();
}

void hook::display()
{
  for (int i=0; i<length; i++)
    cout << links[i].value << " ";
}

void hook::deallocateLinks ()
{
	delete links;
}

void hook::setLength (int s)
{
	length = s;
}

int hook::getLength ()
{
    return length;
}

double hook::getl2norm ()
{
	return l2norm;
}

double hook::calcl2norm ()
{
	int i;
	l2norm = 0;
	for ( i = 0; i < length; i++)
	{
		l2norm += links[i].value * links[i].value;
	}
	return l2norm;
}

double hook::getChLink (int i)
{
	return links[i].value;
}

void hook::setChLink (int i, double value)
{
	links[i].value = value;
}

int hook::getKey (int i)
{
	return links[i].key;
}

void hook::setKey (int i, int k)
{
	links[i].key = k;
}

void hook::addLink (int key, double val)
{
    int temp;
	if (length < maxsize)
	{
		temp = length++;
		links[temp].value = val;
		links[temp].key = key;
	}
	else 
		cout << "\nAttempted to an illegal insertion";
}
