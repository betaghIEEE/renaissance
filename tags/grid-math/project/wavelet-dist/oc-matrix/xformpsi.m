//
//  xformpsi.m
//  oc-matrix
//
//  Created by Daniel Beatty on 4/1/05.
//  Copyright 2005 __MyCompanyName__. All rights reserved.
//

#import "xformpsi.h"
#import <math.h>
#import <mpi.h>

@implementation xformpsi
-(id) init 
{
	S = [[matrixType init] alloc];
	T = [[matrixType init] alloc];
	ha = [[myVector init] alloc];
	hd = [[myVector init] alloc];
	xTemp = [[myVector init] alloc];
	xTemp2 = [[myVector init] alloc];
	yTemp = [[myVector init] alloc];
	yTemp2 = [[myVector init] alloc];
	ha = [[myVector init] alloc];
	hd = [[myVector init] alloc];
	[ha allocateWith:2];
	[hd allocateWith:2];
	[ha vector][0] = pow(0.5, 0.5);
	[ha vector][1] = pow(0.5, 0.5);
	[hd vector][0] = pow(0.5, 0.5);
	[hd vector][1] = -1 * pow(0.5, 0.5);
	sqhalf = pow(0.5, 0.5);
	NSLog (@"Initialized the PsiN Xform");
	[super init];
}

-(void) getSstats: (struct quadnode *) r
{
    double sum = 0.0;
    double sum2 = 0.0;
	double max = 0.0;
    double min = pow (10.0,37.0);
    int i, j;
	max = [S getMaxWithIn:r->xul lRows:r->xll lCols:r->yul rCols: r->yur ];
	min = [S getMinWithIn:r->xul lRows:r->xll lCols:r->yul rCols: r->yur ];
	sum = [S getL1EnergyWithIn:r->xul lRows:r->xll lCols:r->yul rCols: r->yur ];
	sum = [S getEnergyWithIn:r->xul lRows:r->xll lCols:r->yul rCols: r->yur ];
            r->max = max;
    r->min = min;
    r->L1energy = sum;
    r->L2energy = sum2;
    
}

/*
-(void) setAnode: (struct quadnode *)a withNode: (struct quadnode *) r;
{
    if(((r->rows % 2) == 0) & ((r->cols % 2) ==0))
    {
        int rows = (r->rows)/2;
        int cols = (r->cols)/2;
        a->rows = rows;
        a->cols = cols;
        a->xul = r->xul;
        a->xll = r->xul + rows - 1;
        a->xur = r->xur;
        a->xlr = r->xur + rows - 1;
        a->yul = r->yul;
        a->yur = r->yul + cols - 1;
        a->yll = r->yll;
        a->ylr = r->yll + cols - 1;
        a->isnotnull = 1;
        a->myid = r->myid * 4 + 1;
        a->epsilon = r->epsilon;
		
    } else a->isnotnull = 0;
    
}

-(void) setVnode: (struct quadnode *)a withNode: (struct quadnode *) r;
{
    if(((r->rows % 2) == 0) & ((r->cols % 2) ==0))
    {
        int rows = (r->rows)/2;
        int cols = (r->cols)/2;
        a->rows = rows;
        a->cols = cols;
        a->xul = r->xul;
        a->xll = r->xul + rows - 1;
        a->xur = r->xur;
        a->xlr = r->xur + rows - 1;
        a->yul = r->yul + cols;
        a->yll = r->yul + cols;
        a->yur = r->yur;
        a->ylr = r->ylr;
        a->isnotnull = 1;
        a->myid = r->myid * 4 + 2;
        a->epsilon = r->epsilon;
    } else a->isnotnull = 0;
}

-(void) setHnode: (struct quadnode *)a withNode: (struct quadnode *) r;
{
    if(((r->rows % 2) == 0) & ((r->cols % 2) ==0))
    {
        int rows = (r->rows)/2;
        int cols = (r->cols)/2;
        a->rows = rows;
        a->cols = cols;
        
        a->xul = r->xul + rows;
        a->xll = r->xll;
        a->xur = r->xur + rows;
        a->xlr = r->xlr;
        a->yul = r->yul;
        a->yll = r->yll;
        a->yur = r->yul + cols - 1;
        a->ylr = r->yll + cols - 1;
        a->isnotnull = 1;
        a->myid = r->myid * 4 + 3;
        a->epsilon = r->epsilon;
    } else a->isnotnull = 0;
}

-(void) setDnode: (struct quadnode *)a withNode: (struct quadnode *) r;
{
    if(((r->rows % 2) == 0) & ((r->cols % 2) ==0))
    {
        int rows = (r->rows)/2;
        int cols = (r->cols)/2;
        a->rows = rows;
        a->cols = cols;
        
        a->xul = r->xul + rows;
        a->xll = r->xll;
        a->xur = r->xur + rows;
        a->xlr = r->xlr;
        a->yul = r->yul + cols;
        a->yll = r->yul + cols;
        a->yur = r->yur;
        a->ylr = r->ylr;
        a->isnotnull = 3;
        a->myid = r->myid * 4 + 4;
        a->epsilon = r->epsilon;
    } else a->isnotnull = 0;
}
*/
-(void) rowWXform
{
    
    int k, l, n, i;
	
    int o = [ha size];
    int p = [hd size];
	
    int s2 = [S m_cols] /2;
    int s = [S m_cols];
    int rows = [S m_rows];
	
	xTemp = [[myVector init] alloc];
	xTemp2 = [[myVector init] alloc];
	
	[xTemp allocateWith:s];
	[xTemp2 allocateWith:s];
	
	NSLog (@"Begining Row x form");
	
	printf ("Rows %d s %d s2 %d ", rows, s, s2);
    for ( i = 0; i < rows; i++){
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
                    [xTemp vector][k] += [S matrix][i][n] * [ha vector][l];
                    [xTemp2 vector][k] += [S matrix][i][n] * [hd vector][l];
                }
                
            }
				
                for ( k = 0; k < s2; k++)
                {
                    [S matrix][i][k] = [xTemp vector][2*k + 1];
                    [S matrix][i][k+s2] = [xTemp2 vector][2*k+1];
                }        
		
    }
		[xTemp release];
		[xTemp2 release];
}


-(void) rowWXformFrom:(int) start to:(int) finish
{
    
    int k, l, n, i;
	int startRow;
	
    int o = [ha size];
    int p = [hd size];
	
    int s2 = [S m_cols] /2;
    int s = [S m_cols];
    int rows = [S m_rows];
	
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
                    [xTemp vector][k] += [S matrix][i][n] * [ha vector][l];
                    [xTemp2 vector][k] += [S matrix][i][n] * [hd vector][l];
                }
                
            }
				
                for ( k = 0; k < s2; k++)
                {
                    [S matrix][i][k] = [xTemp vector][2*k + 1];
                    [S matrix][i][k+s2] = [xTemp2 vector][2*k+1];
                }        
				
    }
		[xTemp release];
		[xTemp2 release];
}



-(void) colWXform
{
    
    
    int k, l, n, j;
    int o = [ha size];
    int p = [hd size];
	
    int s2 = [S m_rows] /2;
    int s = [S m_rows];
    int cols = [S m_cols];
	
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
                    [yTemp vector][k] += [S matrix][n][j] * [ha vector][l];
                    [yTemp2 vector][k] += [S matrix][n][j] * [hd vector][l];

                }
            }
                
				for ( k = 0; k < s2; k++) {
					[S matrix][k][j] = [yTemp vector][2*k + 1];
					[S matrix][k +s2][j] = [yTemp2 vector][2*k+1];
					
				}
    }
		 [yTemp release];
		 [yTemp2 release];
	
		
}

-(void) colWXformFrom:(int) start to:(int) finish
{
    
    
    int k, l, n, j;
    int o = [ha size];
    int p = [hd size];
	
    int s2 = [S m_rows] /2;
    int s = [S m_rows];
    int cols = [S m_cols];
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
                    [yTemp vector][k] += [S matrix][n][j] * [ha vector][l];
                    [yTemp2 vector][k] += [S matrix][n][j] * [hd vector][l];
					
                }
            }
                
				for ( k = 0; k < s2; k++) {
					[S matrix][k][j] = [yTemp vector][2*k + 1];
					[S matrix][k +s2][j] = [yTemp2 vector][2*k+1];
					
				}
    }
		[yTemp release];
		[yTemp2 release];
		
		
}

-(void) WXform
{
	int index;
	
    int length = resolution;
    int length4 = resolution/4 + 1;
	ha = [[myVector init] alloc];
	hd = [[myVector init] alloc];
	[ha allocateWith:2];
	[hd allocateWith:2];
	[ha vector][0] = pow(0.5, 0.5);
	[ha vector][1] = pow(0.5, 0.5);
	[hd vector][0] = pow(0.5, 0.5);
	[hd vector][1] = -1 * pow(0.5, 0.5);
	
	NSLog (@"Beggining Transform");	
    for ( index = 0; index < length; index++)
    {
        [self rowWXform];
        [self colWXform];
		NSLog (@"Next Batch");
    }
	[ha release];
	[hd release];
    
}


-(BOOL) WXformWithResolution: (int) res
{
	resolution = res;
	[self WXform];
}

-(bycopy matrixType *) xFormMatrix: (in bycopy matrixType *) A withResolution:(in int) res
{
	NSLog (@"Starting transformation");
	[self setMatrix:A];
	NSLog (@"Just set Matrix");
	//[self WXformWithResolution: res];
	int length  = res;
	int index;
	for ( index = 0; index < length; index++)
    {
        [self rowWXform];
        [self colWXform];
		NSLog (@"Next Batch");
    }
	return [self matrix];
}


-(void) rowWIXform
{
    int i,j;
    int lenx = [S m_rows];
    int leny = [S m_cols] / 2;
    double sq2 = sqhalf;
	
	for ( i = 0; i < lenx; i++)
		for (j = 0; j < leny; j++)
		{
			[S matrix][i][2 * j] = ([T matrix][i][j] - [T matrix][i][j + leny] ) * sq2;
			[S matrix][i][(2 * j) + 1] = ([T matrix][i][j] + [T matrix][i][j + leny] ) * sq2;
		}
}

-(void) colWIXform
{
    int i,j;
    int lenx = [T m_rows]  / 2;
    int leny = [T m_cols];
    double sq2 = sqhalf;
	
    
    for ( j = 0; j < leny; j++)
        for (i = 0; i < lenx; i++)
        {
            [T matrix][(2 *i)  ][ j ] = ([S matrix][i ][j ] - [S matrix][i + lenx][j] ) * sq2;
            [T matrix][(2 *i) + 1][j ] = ([S matrix][i ][j ] + [S matrix][i + lenx ][j] ) * sq2;
        }
}

-(void) rowWIXformFrom:(int) start to:(int) finish
{
    int i,j;
    int lenx = [S m_rows];
    int leny = [S m_cols] / 2;
    double sq2 = sqhalf;
	int startRow;
	
	
	if ( finish < lenx ) lenx = finish;
	if ( (start < lenx) && (start >= 0)) startRow = start;
		 else 
		 startRow = 0;
		 
	for ( i = startRow; i < lenx; i++)
		for (j = 0; j < leny; j++)
		{
			[S matrix][i][2 * j] = ([T matrix][i][j] - [T matrix][i][j + leny] ) * sq2;
			[S matrix][i][(2 * j) + 1] = ([T matrix][i][j] + [T matrix][i][j + leny] ) * sq2;
		}
}

-(void) colWIXformFrom:(int) start to:(int) finish
{
    int i,j;
    int lenx = [T m_rows]  / 2;
    int leny = [T m_cols];
    double sq2 = sqhalf;
	
	int startCol;
	
	
	if ( finish < leny ) leny = finish;
	if ( (start < leny) && (start >= 0)) startCol = start;
		 else 
		 startCol = 0;
    
    for ( j = startCol; j < leny; j++)
        for (i = 0; i < lenx; i++)
        {
            [T matrix][(2 *i)  ][ j ] = ([S matrix][i ][j ] - [S matrix][i + lenx][j] ) * sq2;
            [T matrix][(2 *i) + 1][j ] = ([S matrix][i ][j ] + [S matrix][i + lenx ][j] ) * sq2;
        }
}


-(BOOL) WIXform
{

    int index;
    [T autorelease];
	T = [[matrixType init] alloc];
	[T allocateRows: [S m_rows] columns:[S m_cols]];
	[T copyElements:S];
	sqhalf = pow(0.5, 0.5);
    int length = resolution;
    int i, j;
    for ( index = length ; index > 0; index--)
    {
        [self colWIXform];
        [self rowWIXform];
    }
	return YES;
}

-(BOOL) WIXformWithResolution: (int) res
{
	resolution = res;
	[self WIXform];
}

-(bycopy matrixType *) xiFormMatrix: (in bycopy matrixType *) A withResolution:(in int) res
{
	[self setMatrix:A];
	//[self WIXformWithResolution: res];
	
    [T autorelease];
	T = [[matrixType init] alloc];
	[T allocateRows: [S m_rows] columns:[S m_cols]];
	[T copyElements:S];
    int length = res;
	int index;
    for ( index = length ; index > 0; index--)
    {
        [self colWIXform];
        [self rowWIXform];
    }
	return T;
}

-(void) setMatrix: (matrixType *)A
{
    [S autorelease];
	S = [[matrixType init] alloc];
	[S allocateRows:[A m_rows] columns:[A m_cols]];
    [S copyElements:A];
	//[S retain];
	NSLog (@"Setting Matrix");
}

-(matrixType *) matrix
{
    return S;
}


-( bycopy matrixType *) rowWXform:(in byref matrixType *) workMatrix from:(int) start to:(int)finish
{
	[self setMatrix:workMatrix];
	[self rowWXformFrom:start to:finish];
	return S;
}

-( bycopy matrixType *) colWXform:(in byref matrixType *) workMatrix from:(int) start to:(int)finish
{
	[self setMatrix:workMatrix];
	[self colWXformFrom:start to:finish];
	return S;
}

-( bycopy matrixType *) rowWIXform:(in byref matrixType *) workMatrix
							  from:(int) start to:(int)finish
{
	[self setMatrix:workMatrix];
	[self rowWIXformFrom:start to:finish];
	return S;
}

-( bycopy matrixType *) colWIXform:(in byref matrixType *) workMatrix
							  from:(int) start to:(int)finish
{
	[self setMatrix:workMatrix];
	[self rowWIXformFrom:start to:finish];
	return S;
}

//-( bycopy matrixType *) xiFormMatrix: (matrixType *) A withResolution:(int) res;
//-( bycopy matrixType *) xFormMatrix: (matrixType *) A withResolution:(int) res;

@end
