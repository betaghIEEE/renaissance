/*
	Class: Vector Manipulate
	Purpose: To manipulate a vector by either inserting a new component of
	data or extract data from a vector and returning the desired portion.
*/

#ifndef _VECTORMANIPULATE_HPP
#define _VECTORMANIPULATE_HPP
class vectorManipulate 
{
	void insert (int start, int end, myVector &source, myVector
	&destination);
	void insert (int start, myVector &source, myVector
	&destination);
	void insert (myVector &source, myVector
	&destination);
	void extract (int start, int end, myVector &source, myVector
	&destination);
	void extract (int end, myVector &source, myVector
	&destination);
};
#endif
