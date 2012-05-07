//
//  matrixType.m
//  oc-matrix
//
//  Created by Daniel D. Beatty on 4/1/05.
//  Copyright 2005 __MyCompanyName__. All rights reserved.
//

#import "matrixType.h"
#import <stdlib.h>

@implementation matrixType
-(id) initWithRows: (int) rows columns:(int) cols
{
	m_rows = rows;
	m_cols = cols;
	[self allocateStorage];
	[self defineEpsilon];
}

-(id) initWithMatrixType:(matrixType *) rhs
{
}

-(void) copyElements: (matrixType *) source
{
	int i; 
	int j;
	double **temp = [source matrix];
	for ( i = 0; i < m_rows; i++)
		for ( j = 0; j < m_cols; j++) 
			m_data[i][j] = [source matrix][i][j];
	shiftX = [source shiftX];
	shiftY = [source shiftY];
	scaleX = [source scaleX];
	scaleY = [source scaleY];
}

-(void) allocateStorage
{
	int i;
	int k;
	if ( m_cols <= 0 || m_rows <= 0) 
	{
		m_data = NULL;
		return;
	}
	k =  m_rows * m_cols;
	m_data = malloc( sizeof(double*) * m_rows);
	m_data[0] = malloc ( sizeof(double) * k);
	for ( i = 1; i < m_rows; i++)
	{
		m_data[i] = m_data[i-1] + m_cols;
		printf ("memory location %d \n ", m_data[i] );
	}
}

-(void) allocateRows: (int) rows columns:(int)cols
{
	m_rows = rows;
	m_cols = cols;
	[self allocateStorage];
}

-(void) deleteStorage
{
	if (m_data != NULL) {
		free(m_data[0]);
		free(m_data);
		m_data = NULL;
	}
}

-(void) dealloc 
{
	[self deleteStorage];
}

-(void) zeroize
{
    int i, j;
    for ( i = 0; i < m_rows; i++)
        for ( j = 0; j < m_cols; j++)
            m_data[i][j] = 0;

}

-(void) defineEpsilon
{
	epsilon = 1/pow(2.0, 32.0);
}

-(BOOL) checkRows: (int) rows columns:(int)cols
{
	return ((rows>=0)&&(rows<m_rows))&&((cols>=0)&&(cols<m_cols)); 
}


-(int) m_cols 
{
	return m_cols;
}

-(int) m_rows 
{
	return m_rows;
}


-(void) addMatrixEqual: (matrixType *) rhs
{
	int i, j;
	[self checkRows:[rhs m_rows] columns:[rhs m_cols]];
	double **rhs_m_data = [rhs matrix];
	for (i=0; i< m_rows; i++)
		for ( j=0; j<m_cols; j++)
			m_data[i][j] += rhs_m_data[i][j];
}

-(void) subMatrixEqual: (matrixType *) rhs
{
	int i, j;
	[self checkRows:[rhs m_rows] columns:[rhs m_cols]];	
	double **rhs_m_data = [rhs matrix];
	for (i=0; i<m_rows; i++)
		for (j=0; j<m_cols; j++)
			m_data[i][j] -= rhs_m_data[i][j];
	
}


-(void) multMatrixEqual: (matrixType *) rhs
{
	int i, j, k;
	if ( m_cols == [rhs m_rows]) 
	{
		double total;
		int rhsCol = [ rhs m_rows];
		double **rhs_m_data = [rhs matrix];
		matrixType *result= [[matrixType init] alloc];
		[result allocateRows:m_rows columns:rhsCol];
		for ( i=0; i< m_rows; i++)
			for ( j=0; j< rhsCol; j++) {
				total = 0;
				for ( k=0; k< m_cols; k++)
					total += m_data[i][k]*rhs_m_data[k][j];
			//[result matrix][i][j] = total;
			}
	}
}

-(void) sparseMatrixMultiplyEqual: (matrixType *) rhs
{
}

-(void) multEqual: (double) rhs
{
	int i, j;
	for ( i=0; i<m_rows; i++)
		for ( j=0; j<m_cols; j++)
			m_data[i][j] *= rhs;
	
}

-(void) divEqual: (double) rhs
{
	int i, j;
	double temp = 1/rhs;
	for ( i=0; i<m_rows; i++)
		for ( j=0; j<m_cols; j++)
			m_data[i][j] *= temp;
	
}

-(void) addEqual: (double) rhs
{
	int i , j;
	for ( i=0; i<m_rows; i++)
		for ( j=0; j<m_cols; j++)
			m_data[i][j] += rhs;
}

-(void) subEqual: (double) rhs
{
	int i, j;
	for ( i=0; i<m_rows; i++)
		for ( j=0; j<m_cols; j++)
			m_data[i][j] -= rhs;
}

-(double) getEnergy 
{
	int i, j;
    double sumcol = 0;
    double sumrow = 0;
    for ( i = 0; i < m_rows; i++) {
        sumcol = 0;
        for ( j = 0 ; j < m_cols; j++)
        {
            sumcol += m_data[i][j] * m_data[i][j] / m_cols;
        }
        sumrow += sumcol / m_rows;
    }
    return sumrow;
}

-(double) getL1Energy 
{
    int i, j;
    double sumcol = 0;
    double sumrow = 0;
    for ( i = 0; i < m_rows; i++) {
        sumcol = 0;
        for ( j = 0 ; j < m_cols; j++)
        {
            sumcol += m_data[i][j];
        }
        sumrow += sumcol;
    }
    return sumrow;
}

-(double**) matrix
{
	return m_data;
}

-(double) shiftX
{
	return scaleY;
}

-(double) shiftY
{
	return scaleY;
}


-(double) scaleX
{
	return scaleY;
}

-(double) scaleY
{
	return scaleY;
}

-(double) getMin
{
	return min;
}

-(double) getMax
{
	return max;
}


-(double) getMaxWithIn: (int) Urows lRows:(int) lRows lCols:(int) lCols rCols:(int) rCols
{
	int i, j;
	int localMax = 0;
	for ( i = Urows; i < lRows; i++)
		for ( j = lCols; j < rCols; j++)
		{
			if (localMax <= m_data[i][j] ) localMax = m_data[i][j];
		}
			return localMax;
}

-(double) getMinWithIn: (int) Urows lRows:(int) lRows lCols:(int) lCols rCols:(int) rCols
{
	int i, j;
	int localMin = 0;
	for ( i = Urows; i < lRows; i++)
		for ( j = lCols; j < rCols; j++)
		{
			if (localMin <= m_data[i][j] ) localMin = m_data[i][j];
		}
			return localMin;
}

-(double) getEnergyWithIn: (int) Urows lRows:(int) lRows lCols:(int) lCols rCols:(int) rCols
{
	int i, j;
    double sumcol = 0;
    double sumrow = 0;
	double numRows = lRows - Urows;
	double numCols = lCols - rCols;
	double numColsInv = 1 /numCols ;
	double numRowsInv = 1/ numRows;
    for ( i = Urows; i < lRows; i++) {
        sumcol = 0;
        for ( j = lCols ; j < rCols; j++)
        {
            sumcol += m_data[i][j] * m_data[i][j] * numColsInv;
        }
        sumrow += sumcol * numRowsInv;
    }
    return sumrow;
}

-(double) getL1EnergyWithIn: (int) Urows lRows:(int) lRows lCols:(int) lCols rCols:(int) rCols
{
	int i, j;
    double sumcol = 0;
    double sumrow = 0;
	double numRows = lRows - Urows;
	double numCols = lCols - rCols;
	double numColsInv = 1 /numCols ;
	double numRowsInv = 1/ numRows;
    for ( i = Urows; i < lRows; i++) {
        sumcol = 0;
        for ( j = lCols ; j < rCols; j++)
        {
            sumcol += m_data[i][j]  * numColsInv;
        }
        sumrow += sumcol * numRowsInv;
    }
    return sumrow;
}


@end
