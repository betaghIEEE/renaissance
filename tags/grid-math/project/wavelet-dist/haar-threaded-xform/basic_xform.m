//
//  basic_xform.m
//  haar-threaded-xform
//
//  Created by Daniel D. Beatty on 4/29/05.
//  Copyright 2005 __MyCompanyName__. All rights reserved.
//

#import "basic_xform.h"


@implementation basic_xform
-(oneway void) colXform:(matrixType *) workMatrix 
				   from:(int)start 
					 to:(int)finish
{  
    int k, l, n, j;
    int o = [hA size];
    int p = [hD size];
	
    int s2 = [workMatrix m_rows] /2;
    int s = [workMatrix m_rows];
    int cols = [workMatrix m_cols];
	int startCol;
	
	
	if ( finish < cols ) cols = finish;
	if ( (start < cols) && (start >= 0)) startCol = start;
	else 
		startCol = 0;
	
	yTemp = [[myVector init] alloc];
	yTemp2 = [[myVector init] alloc];
	[yTemp allocateWith:s];
	[yTemp2 allocateWith:s];
	
    for ( j = 0; j < cols; j++) { 
        for ( k = 0; k < s; k++)
        {
            [yTemp vector][k] = 0.0;
            [yTemp2 vector][k] = 0.0;
        }
        for ( k = 0; k < s; k++)
            for ( l = 0; l < o; l++)
            {
                n = k -l;
				if ( ( n >= 0) && ( n < s)) {
                    [yTemp vector][k] += [workMatrix matrix][n][j] * [hA vector][l];
                    [yTemp2 vector][k] += [workMatrix matrix][n][j] * [hD vector][l];
					
                }
            }
                
				for ( k = 0; k < s2; k++) {
					[workMatrix matrix][k][j] = [yTemp vector][2*k + 1];
					[workMatrix matrix][k +s2][j] = [yTemp2 vector][2*k+1];
					
				}
    }
		[yTemp release];
		[yTemp2 release];
}

-(oneway void) rowXform:(matrixType *) workMatrix 
				   from:(int)start 
					 to:(int)finish
{    
    int k, l, n, i;
	int startRow;
	
    int o = [hA size];
    int p = [hD size];
	
    int s2 = [workMatrix m_cols] /2;
    int s = [workMatrix m_cols];
    int rows = [workMatrix m_rows];
	
	if ( finish < rows ) rows = finish;
	if ( (start < rows) && (start >= 0)) startRow = start;
	else 
		startRow = 0;
	
	xTemp = [[myVector init] alloc];
	xTemp2 = [[myVector init] alloc];
	
	[xTemp allocateWith:s];
	[xTemp2 allocateWith:s];
	
	NSLog (@"Begining Row x form");
	
	printf ("Rows %d s %d s2 %d ", rows, s, s2);
    for ( i = startRow; i < rows; i++){
        for ( k = 0; k < s; k++)
        {
			printf (" %d ", k);
            [xTemp vector][k] = 0.0;
            [xTemp2 vector][k] = 0.0;
        }    
		NSLog (@"Initializaed xTerm");
        for (k =0; k < s; k++)
            for ( l = 0; l < o; l++)
            {
                n = k -l;
                if ( (n >= 0) && ( n <  s)) {
                    [xTemp vector][k] += [workMatrix matrix][i][n] * [hA vector][l];
                    [xTemp2 vector][k] += [workMatrix matrix][i][n] * [hD vector][l];
                }
                
            }
				
                for ( k = 0; k < s2; k++)
                {
                    [workMatrix matrix][i][k] = [xTemp vector][2*k + 1];
                    [workMatrix matrix][i][k+s2] = [xTemp2 vector][2*k+1];
                }        
				
    }
		[xTemp release];
		[xTemp2 release];
}

-(oneway void) colXiform:(matrixType *) workMatrix 
					from:(int)start 
					  to:(int)finish
{
	int i,j;
    int lenx = [workMatrix m_rows]  / 2;
	int s = [workMatrix m_rows];
    int leny = [workMatrix m_cols];
    double sq2 = sqhalf;
	yTemp = [[myVector alloc] init];
	[yTemp allocateWith:s];
	
	int startCol;
	
	
	if ( finish < leny ) leny = finish;
	if ( (start < leny) && (start >= 0)) startCol = start;
	else 
		startCol = 0;
    
    for ( j = startCol; j < leny; j++) {
        for (i = 0; i < lenx; i++)
        {
            [yTemp vector][(2 *i)  ] = ([workMatrix matrix][i ][j ] - [workMatrix matrix][i + lenx][j] ) * sq2;
            [yTemp vector][(2 *i) + 1] = ([workMatrix matrix][i ][j ] + [workMatrix matrix][i + lenx ][j] ) * sq2;
        }
		for ( i = 0; i < s; i++)
		{
			[workMatrix matrix][i][j] = [yTemp vector][i];
		}
	}
	
}

-(oneway void) rowXiform:(matrixType *) workMatrix 
					from:(int)start 
					  to:(int)finish
{
    int i,j;
    int lenx = [workMatrix m_rows];
    int leny = [workMatrix m_cols] / 2;
	int s = [workMatrix m_cols];
	xTemp = [[myVector alloc] init];
	[xTemp allocateWith:s];
    double sq2 = sqhalf;
	int startRow;
	
	
	if ( finish < lenx ) lenx = finish;
	if ( (start < lenx) && (start >= 0)) startRow = start;
	else 
		startRow = 0;
	
	for ( i = startRow; i < lenx; i++){
		for (j = 0; j < leny; j++)
		{
			[xTemp vector][2 * j] = ([workMatrix matrix][i][j] - [workMatrix matrix][i][j + leny] ) * sq2;
			[xTemp vector][(2 * j) + 1] = ([workMatrix matrix][i][j] + [workMatrix matrix][i][j + leny] ) * sq2;
		}
		for ( i = 0; i < s; i++)
		{
			[workMatrix matrix][i][j] = [yTemp vector][i];
		}
	}
}

-(oneway void) setFilters:(myVector *) average  
			   difference:(myVector *) difference
{
	[hA release];
	[hA release];
	hA = [average copy];
	hD = [difference copy];
}


@end
