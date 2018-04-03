#include "vectormanipulation.hpp"

void vectorManipulate::insert (int start, int end, myVector &source, myVector &destination)
{
	int i;
	int limit = destination.Size();
	if (( end < limit) && ( start < limit))
		if ( start > end) 
			for ( i = start; i < end; i++)
				destination[i] = source[i];
		else 
			for ( i = end ; i >= start; i--)
				destination[i] = source[i];
}
	
void vectorManipulate::insert (int start, myVector &source, myVector &destination)
{
	int end = source.Size();
	insert ( start, end, source, destination);
}
	
void vectorManipulate::insert (myVector &source, myVector &destination)
{
	int start = 0;
	int end = source.Size();
	
}
	
void vectorManipulate::extract (int start, int end, myVector &source, myVector &destination)
{
	int limit = source.Size();
	int len = abs(start-end)+1;
	if ( (start < limit) && (end < limit)
	{
		destination = myVector (len);
		if ( start > end) 
			for ( i = start; i < end; i++)
				destination[i] = source[i];
			else 
			for ( i = end ; i >= start; i--)
				destination[i] = source[i];
	}
	destination = myVector(0);
}
	
void vectorManipulate::extract (int end, myVector &source, myVector &destination)
{
	int start = 0;
	extract ( start, end, source, destination);
}
