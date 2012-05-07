#include "myVector.h"
#include "xform2D.hpp"
#include "xform.hpp"
#include <iostream.h>
#include <math.h>

void wave2comp::rowWaveletXform ( matrixType &source, matrixType &result)
{
	int i, j, n;
	int rLimit, cLimit;
	rLimit = source.m_rows;
	cLimit = source.m_cols;
	myVector S(cLimit);
	myVector R;
	result.allocateStorage (rLimit, cLimit);
	xform wave1D;
	
	for ( i = 0; i < rLimit; i++)
	{
		n = 0;
		for ( j = 0; j < cLimit; j++)
		{
			S[j] = source[i][j];
			//n = n + 1;
		}
		//cout << "Row ready " << i << endl;
		wave1D.hWave (S,R);
		//cout << "Row wavelet complete for row " << i <<endl;
		//cout << "R Size " << R.Size() << " and column size " << cLimit << endl;
		n = 0;
		for (j = 0 ; j < cLimit; j++)
		{
			result[i][j] = R[j];
			//n = n +1;
		}
		//cout << "Row wavelet committed for row " << i <<endl;
		//R.deallocate();
			
	}
}

void wave2comp::colWaveletXform ( matrixType &source, matrixType &result)
{

	int i, j, n;
	int rLimit, cLimit;
	rLimit = source.m_rows;
	cLimit = source.m_cols;
	myVector S(rLimit);
	myVector R;
	xform wave1D;
	result.allocateStorage(source.m_rows, source.m_cols);
	//cout << endl << "In the Column Wavelet Transfrom " << endl;
	for ( j = 0; j < cLimit; j++)
	{
		n=0;
		for ( i = rLimit -1; i >= 0; i--)
		{
			S[n] = source[i][j];
			n++;
		}
		//cout << "Column allocated for computation for column " << j << endl;
		wave1D.hWave (S,R);
		//cout << "Column transform computated for column " << j << endl;
		n = 0;
		for ( i = rLimit -1; i >= 0; i--)
		{
			result[i][j] = R[n];
			n++;
		}
		//cout << "Column committed for column " << j << endl;	
	}
}

void wave2comp::waveletXform (matrixType &source, matrixType &result)
{
	matrixType rowWave;
	//cout << endl << "Allocated Result" ;
	//rowWaveletXform (source, result);
	rowWaveletXform (source, rowWave);
	
	//colWaveletXform (source, rowWave);
	//rowWaveletXform ( rowWave, result);
	
	//cout << endl << "Row wavelet complete" ;
	colWaveletXform ( rowWave, result);
	//colWaveletXform ( source, result);
}

void wave2comp::rowWaveletXformInverse (matrixType &source, matrixType &result)
{
	int k = source.m_rows;
	int l = source.m_cols;
	int i, j, n;

	myVector S(l);
	myVector R;
	result.allocateStorage (k,l);
	xform wave1D;
	
	for ( i = 0; i < k; i++)
	{
		for ( j = 0; j < l; j++)
		{
			//n = l -j -1;
			S[i] = source[i][j];
		}
		wave1D.hWaveInverse (S, R);
		for ( j = 0; j < l; j++)
		{
			//n = l -j -1;
			result[i][j] = R[j];
		}
		
	}

		
}

void wave2comp::colWaveletXformInverse (matrixType &source, matrixType &result)
{
	int k = source.m_rows;
	int l = source.m_cols;
	int i, j, n;

	myVector S(l);
	myVector R;
	result.allocateStorage (k,l);
	xform wave1D;
	
	for ( j = 0; j < l; j++)
	{
		for ( i = 0; i < k; i++)
		{
			n = k -i -1;
			S[n] = source[i][j];
		}
		wave1D.hWaveInverse (S, R);
		for ( i = 0; i < k; j++)
		{
			n = k -i -1;
			result[i][j] = R[n];
		}
		
	}

		
}

void wave2comp::waveletXformInverse (matrixType &source, matrixType &result)
{
	matrixType rowWave;
	rowWaveletXformInverse (source, rowWave);
	colWaveletXformInverse ( rowWave, result);

}


void wave2comp::filterThresh ( matrixType &input, matrixType &output, int thresh)
{
	int k = input.m_rows;
	int l = input.m_cols;
	int i, j;
	output.allocateStorage( k,l);
	for ( i = 0; i < k; i++)
		for ( j =0; j < l; j++)
		if ( epsilon (input[i][j]))
			output[i][j] = 0.0;
			else
			output[i][j] = input[i][j];
}

bool wave2comp::epsilon ( double x)
{
	double t = pow (10,-10);
	if ( x > t)
		return false;
	if ( x < -t)
		return false;
	return true;
}
