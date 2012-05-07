#include "myVector.h"
#include "xform2D.hpp"
#include "xform.hpp"
//#include "convolve.hpp"
#include "haarGen.hpp"
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

void wave2comp::selfColWXform (int j, int rows, matrixType &W)
{
    int k, l, n;
    myVector yTemp;
    int o = hA.Size();
    int p = hD.Size();
    int s2 = rows /2;
    int s = rows;
	
        for ( k = 0; k < s; k++)
        {
            yA[k] = 0.0;
            yD[k] = 0.0;
        }
        //cout << "Column xForm initialized "<< j << endl;
        for ( k = 0; k < s; k++)
            for ( l = 0; l < o; l++)
            {
                n = k -l;
		//n = s - (k -l ) -1;
                if ( ( n >= 0) && ( n < s)){
                    yA[k] += W[n][j] * hA[l];
                    yD[k] += W[n][j] * hD[l];
                }
            }
            
        for ( k = 0; k < s2; k++)
        {
            W[/*s-1-*/k][j] = yA[2*k + 1];
        
            W[s2 +k][j] = yD[2*k + 1];
        }

}


void wave2comp::selfColWXform (matrixType &source, matrixType &result, myVector &ha, myVector &hd)
{
    int k, l, n, j;
    myVector yTemp;
    int o = ha.Size();
    int p = hd.Size();
    int s2 = source.m_rows /2;
    int s = source.m_rows;
	yTemp.allocate(s);
	j = source.m_cols / 2;
	//for ( k = 0; k < s; k++)
		//cout << "\t" << source[k][j];
	//cout << "List one " << endl;
	//for ( k = 0; k < o ; k++)
		//cout << "\t" << ha[k];
	//cout << "H list " << endl;
    for ( j = 0; j < source.m_cols; j++) { 
        for ( k = 0; k < s; k++)
            yTemp[k] = 0.0;
    
        for ( k = 0; k < s; k++)
            for ( l = 0; l < o; l++)
            {
                n = k -l;
		//n = s - (k -l ) -1;
                if ( ( n >= 0) && ( n < s))
                    yTemp[k] += source[n][j] * ha[l];
            }
        //for ( k = 0; k < s; k++)
		//cout << "\t" << yTemp[k];
        for ( k = 0; k < s2; k++)
            result[/*s-1-*/k][j] = yTemp[2*k + 1];
        
        for ( k = 0; k < s; k++)
            yTemp[k] = 0.0;
        for ( k = 0; k < s; k++)
            for ( l = 0; l < o; l++)
            {
                n = k - l;
		//n = s - (k -l ) -1;
                if ( ( n >= 0 ) && ( n < s ))
                    yTemp[k] += source[n][j] * hd[l];
            }
        for ( k = 0; k < s2; k++)
            result[/*s2 - 1 -*/ s2 + k][j] = yTemp[2*k + 1];

	//for ( k = 0; k < s; k++)
		//cout << "\t" << result[k][j];
    }
}

void wave2comp::selfRowWXform (int i, int col, matrixType &W)
{
    int k, l, n;
    //myVector xTemp;
	
    int o = hA.Size();
    int p = hD.Size();
    int s2 = col /2;
    int s = col;
    
    //for ( i = 0; i < source.m_rows; i++){
        for ( k = 0; k < s; k++)
        {
                xA[k] = 0.0;
                xD[k] = 0.0;
        }
     //cout << " Initialized temporary vector " << i << endl;       
        for (k =0; k < s; k++)
            for ( l = 0; l < o; l++)
            {
                //n = s - (k -l ) -1;
		n = k -l;
                if ( (n >= 0) && ( n <  s))
                    xA[k] += W[i][n] * hA[l];
                    xD[k] += W[i][n] * hD[l];
            }
	//cout << "Aquired average " ;
        for ( k = 0; k < s2; k++)
        {
            W[i][k] = xA[2*k + 1];
            W[i][s2+k] = xD[2*k + 1];
        }
}

void wave2comp::selfRowWXform (matrixType &source, matrixType &result, myVector &ha, myVector &hd)
{
    int k, l, n, i;
    myVector xTemp;
	
    int o = ha.Size();
    int p = hd.Size();
    int s2 = source.m_cols /2;
    int s = source.m_cols;
	xTemp.allocate(s);
    //cout << "In Row Transform " << endl;
    for ( i = 0; i < source.m_rows; i++){
        for ( k = 0; k < s; k++)
            xTemp[k] = 0.0;
     //cout << " Initialized temporary vector " << i << endl;       
        for (k =0; k < s; k++)
            for ( l = 0; l < o; l++)
            {
                //n = s - (k -l ) -1;
		n = k -l;
                if ( (n >= 0) && ( n <  s))
                    xTemp[k] += source[i][n] * ha[l];
            }
	//cout << "Aquired average " ;
        for ( k = 0; k < s2; k++)
        {
            result[i][k] = xTemp[2*k + 1];
        }
    
        for ( k = 0; k < s; k++)
            xTemp[k] = 0.0;
    	//cout << "Getting Difference " ;
        for ( k = 0; k < s; k++)
        {
            for ( l = 0; l < p; l++)
            {
                //n = s - (k-l) - 1;
		n = k -l;
                if (( n >= 0) && ( n < s ))
                    xTemp[k] += source[i][n] * hd[l];
            }
        }
	//cout << "Acquired Difference " ;
        for ( k = 0; k < s2; k++)
        {
            result[i][k + s2] = xTemp[2*k + 1];
        }
	//cout << "Stored Difference " << endl;
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

void wave2comp::selfWXform (matrixType &source, matrixType &result)
{
	matrixType rowWave;
	//cout << endl << "Allocated Result" ;
	//rowWaveletXform (source, result);
        myVector ha, hd;
	double sqhalf = pow (0.5, 0.5);
        haarGen genWave;
        //ha = genWave.genAve();
        //hd = genWave.genDiff();
	ha.allocate(2);
	hd.allocate(2);
	ha[0] = hd[0] = ha[1] = sqhalf;
	hd[1] = -sqhalf;
	rowWave.allocateStorage (source.m_rows, source.m_cols);
	result.allocateStorage ( source.m_rows, source.m_cols);
	//cout << endl << "Starting Row Transform" << endl;
	selfRowWXform (source, rowWave , ha, hd);
	cout << endl << " Starting Column Transform " << endl;
	selfColWXform ( rowWave, result, ha, hd);
	//colWaveletXform ( source, result);
}

void wave2comp::selfWXform (matrixType &source, matrixType &result, int res)
{
	matrixType rowWave;
        int i, j, s, p;
        int row, col;
	double sqhalf = pow (0.5, 0.5);
        cout << "Allocating Memory" << endl;
        hA.allocate(2);
	hD.allocate(2);
        xA.allocate(source.m_rows);
        xD.allocate(source.m_rows);
        yA.allocate(source.m_cols);
        yD.allocate(source.m_cols);
        cout << "Assigning Haar " << endl;
        hA[0] = hD[0] = hA[1] = sqhalf;
	hD[1] = -sqhalf;
        rowWave.allocateStorage (source.m_rows, source.m_cols);
	result.allocateStorage ( source.m_rows, source.m_cols);
        cout << "Memory assignment half done " << source.m_rows << " " << source.m_cols << endl;
        for ( i = 0; i < source.m_rows ; i++)
            for ( j = 0; j < source.m_cols; j++)
                rowWave[i][j] = source[i][j];
        cout << "Memory allocated : " << endl;
        for ( s = 0; s < res; s++)
        {
            p = int (pow(2.0, s));
            row = source.m_rows / p;
            col = source.m_cols / p; 
            cout << endl << "Starting Row Transform" << s << endl;
            for ( i = 0; i < row; i++)
                selfRowWXform (i, col, rowWave);
            cout << endl << " Starting Column Transform "<< s << endl;
            for ( j = 0; j < col; j++)
                selfColWXform ( j, row, rowWave);
        }
        cout << "Transform complete " << endl;
        for ( i = 0; i < source.m_rows ; i++)
            for ( j = 0; j < source.m_cols; j++)
                result[i][j] = rowWave[i][j];
        
        hD.deallocate();
        hA.deallocate();
        yD.deallocate();
        yA.deallocate();
        xA.deallocate();
        xD.deallocate();
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
	cout << endl;
	cout << "Row Inverse transform has begun" << endl;
	cout << "Number of row " << k ;
	cout << "\t Number of columns " << l;
	for ( i = 0; i < k; i++)
	{
		cout << "Do ";
		for ( j = 0; j < l; j++)
		{
			n = l -j -1;
			S[n] = source[i][j];
		}
		wave1D.hWaveInverseO (S, R);
		for ( j = 0; j < l; j++)
		{
			n = l -j -1;
			result[i][j] = R[n];
		}
		cout << i << "\t";
	}
	cout << "Done processing rows " << endl;

		
}

void wave2comp::colWaveletXformInverse (matrixType &source, matrixType &result)
{
	int k = source.m_rows;
	int l = source.m_cols;
	int i, j, n;

	myVector S(k);
	myVector R;
	result.allocateStorage (k,l);
	xform wave1D;
	cout << endl ;
	cout << " Begining Column Inverse Transform" << endl;

	
	for ( j = 0; j < l; j++)
	{
//		cout << "Do " ;
		for ( i = 0; i < k; i++)
		{
			//n = k -i -1;
			S[i] = source[i][j];
		}
//		cout << j ;
		wave1D.hWaveInverseO (S, R);
//		cout << " t ";
		for ( i = 0; i < k; i++)
		{
			//n = k -i -1;
//			cout << i << " ";
			result[i][j] = R[i];
			
		}
		cout << "\t" << j ;
	}

//	cout << endl;	
}

void wave2comp::waveletXformInverse (matrixType &source, matrixType &result)
{
	matrixType rowWave;
	//rowWaveletXformInverse (source, rowWave);
	rowWaveletXformInverse (source, result);
	cout << " About to do column restore" << endl;
	//colWaveletXformInverse ( rowWave, result);
	//colWaveletXformInverse (source , result);

}


void wave2comp::selfRowWaveletXformInverse (matrixType &source, matrixType &result, int i)
{
        int l2 = source.m_cols /2;
	int j, n, p, r;
	double sqhalf = pow (0.5, 0.5);
	for ( j = 0; j < l2; j++)
	{
		result[i][2*j+0] = (source[i][j] - source[i][j+l2]) * sqhalf;
		result[i][2*j+1] = (source[i][j] + source[i][j+l2]) * sqhalf;
	}
}

void wave2comp::selfColWaveletXformInverse (matrixType &source, matrixType &result, int j)
{
	int k = source.m_rows - 1;
	int k2 = source.m_rows /2 ;
	int i, m, o, q;
	double sqhalf = pow (0.5, 0.5);
	//cout << "Col " << j ; //<< " " << " K2 " << k2 << " ";
	for ( i = 0; i < k2 ; i++)
	{
                result[2*i+0][j] = (source[i][j] - source[k2+i][j]) * sqhalf;
		result[2*i+1][j] = (source[i][j] + source[k2+i][j]) * sqhalf;
	}
}

void wave2comp::selfRowWaveletXformInverse (matrixType &W, int col, int i)
{

	//int l = source.m_cols - 1;
	int l2 = col /2;
	int j, n, p, r;
	double sqhalf = pow (0.5, 0.5);
        //for ( j = 0; j < col; j++)
          //  sX[j] = 0.0;
	for ( j = 0; j < l2; j++)
	{
		sX[2*j+0] = (W[i][j] - W[i][j+l2]) * sqhalf;
		sX[2*j+1] = (W[i][j] + W[i][j+l2]) * sqhalf;
	}
        for ( j = 0; j < col; j++)
            W[i][j] = sX[j]; 
    
}

void wave2comp::selfColWaveletXformInverse (matrixType &W, int row, int j)
{
	//int k = source.m_row - 1;
	int k2 = row /2 ;
	int i, m, o, q;
	double sqhalf = pow (0.5, 0.5);
	//cout << "Col " << j ; //<< " " << " K2 " << k2 << " ";
        //for ( i = 0; i < row; i++)
          //  sY[i] = 0.0;
	for ( i = 0; i < k2 ; i++)
	{
		sY[2*i+0] = (W[i][j] - W[k2+i][j]) * sqhalf;
		sY[2*i+1] = (W[i][j] + W[k2+i][j]) * sqhalf;
		
	}
        for ( i = 0; i < row; i++)
            W[i][j] = sY[i];
	
}

void wave2comp::selfWaveletXformInverse (matrixType &source, matrixType &result)
{
	int row, col;
	int i, j;
	row = source.m_rows;
	col = source.m_cols;
	matrixType rowWave;
	rowWave.allocateStorage (row, col);
	result.allocateStorage (row, col);
	cout << endl;
	cout << " Begin Inverse Wavelet Transform "; 

	for ( j = 0; j < col; j++)
		selfColWaveletXformInverse ( source, rowWave, j);
	for ( i = 0; i < row; i++)
		selfRowWaveletXformInverse ( rowWave, result, i);

	
}

void wave2comp::selfWaveletXformInverse (matrixType &source, matrixType &result, int res)
{
	int row, col;
	int i, j, k, p;
	row = source.m_rows;
	col = source.m_cols;
	matrixType rowWave;
	rowWave.allocateStorage (row, col);
	result.allocateStorage (row, col);
        sX.allocate(col);
        sY.allocate(row);
	cout << endl;
	cout << " Begin Inverse Wavelet Transform "; 
        //for ( i = 0; i < source.m_rows; i++)
          //  for ( j = 0; j < source.m_cols; j++)
            //    rowWave[i][j] = source[i][j];
       filterThresh (source, rowWave, 0.02);
        for ( k = res - 1; k >= 0; k--)
	//for ( k = 0; k < res; k++)
        {
            p = int ( pow (2.0, k));
            cout << "" << endl;
            row = source.m_rows / p;
            col = source.m_cols / p;
            for ( j = 0; j < col; j++)
                    selfColWaveletXformInverse ( rowWave, row, j);
            for ( i = 0; i < row; i++)
                    selfRowWaveletXformInverse ( rowWave, col, i);
        }
        //for ( i = 0; i < row; i++)
           // selfRowWaveletXformInverse (rowWave, result, i);
        for ( i = 0; i < source.m_rows; i++)
            for ( j = 0; j < source.m_cols; j++)
                result[i][j] = rowWave[i][j];
       
	
}


void wave2comp::filterThresh ( matrixType &input, matrixType &output, int thresh)
{
	int k = input.m_rows;
	int l = input.m_cols;
	int i, j;
	output.allocateStorage( k,l);
	for ( i = 0; i < k; i++)
		for ( j =0; j < l; j++)
		if ( epsilon (input[i][j], thresh))
			output[i][j] = 0.0;
			else
			output[i][j] = input[i][j];
}

void wave2comp::filterThresh ( matrixType &input, matrixType &output, double thresh)
{
	int k = input.m_rows;
	int l = input.m_cols;
	int i, j;
	output.allocateStorage( k,l);
	for ( i = 0; i < k; i++)
		for ( j =0; j < l; j++)
		if ( epsilon (input[i][j], thresh))
			output[i][j] = 0.0;
			else
			output[i][j] = input[i][j];
}

bool wave2comp::epsilon ( double x, int th)
{
	double t = pow (th,-10);
	if ( x > t)
		return false;
	if ( x < -t)
		return false;
	return true;
}

bool wave2comp::epsilon ( double x, double thresh)
{
	double t = thresh;
	if ( x > t)
		return false;
	if ( x < -t)
		return false;
	return true;
}
