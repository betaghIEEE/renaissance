//
//  myVector.m
//  oc-matrix
//
//  Created by Daniel D. Beatty on 4/4/05.
//  Copyright 2005 __MyCompanyName__. All rights reserved.
//

#import "myVector.h"


@implementation myVector
-(double) L2
{    
	int n = size;
    double temp = 0.0;
    int i;
    
    for ( i = 0; i < n; i++)
        temp += m_data[i] * m_data[i];
    return temp;
}

-(int) size
{
	return size;
}

-(void) setSize: (int) n
{
	size = n;
	[self allocate];
}

-(void) allocateWith: (int) i
{
	[self setSize:i];
	NSLog (@"Allocated a vector");
}

-(void) allocate
{
	[self deallocate];
	m_data = (double *) malloc(sizeof (double) * size);
}

-(void) deallocate 
{
	if ( m_data != NULL) free(m_data);
}


-(myVector *) copy
{
	int i, n;
    int m = size;
    //int o = x.Size();
    double temp;
    //if ( m != o)
    //    Size (o);
	myVector *aCopy = [[myVector init] alloc];
	[aCopy allocateWith:m];
    //m_data.reserve(o);
    //n = Size();
    for ( i = 0; i < m; i++)
    {
        //temp = x[i];
        //m_data[i] = temp;
		[aCopy vector][i] = m_data[i];
    }
    return (aCopy);
}

-(myVector *) addEqual: (myVector *) rhs
{
	
    int i ;
    int m = size;
    int o = [rhs size];
    if ( m != o)
        return self;
	
    for ( i = 0; i < m; i++)
        m_data[i] += [rhs vector][i];
    return self;
}

-(myVector *) subEqual: (myVector *) rhs
{
	
    int i ;
    int m = size;
    int o = [rhs size];
    if ( m != o)
        return self;
	
    for ( i = 0; i < m; i++)
        m_data[i] -= [rhs vector][i];
    return self;
}

-(myVector *) divEqual: (double) rhs
{
	
    int i ;
    int m = size;
	double drhs = 0;
	if ( rhs != 0) drhs= 1/ rhs;
	else return self;
	
    for ( i = 0; i < m; i++)
        m_data[i] *= drhs;
    return self;
}

-(myVector *) mulEqual: (double) rhs
{
	
    int i ;
    int m = size;
	
    for ( i = 0; i < m; i++)
        m_data[i] *= rhs;
    return self;
}


//-(myVector *) crossProduct: (myVector *) rhs;

-(double) dotProduct: (myVector *) rhs
{
    int n = [rhs size];
    double temp = 0.0;
    int i;
    
	
	if ( n != size ) return 0;
    for ( i = 0; i < n; i++)
        temp += m_data[i] * [rhs vector][i];
    return temp;
}

-(double *) vector
{
	return m_data;
}


@end
