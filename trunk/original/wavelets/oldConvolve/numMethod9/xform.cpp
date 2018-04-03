#include "convolve.hpp"
#include "haarGen.hpp"
#include "plotResults.hpp"
#include <math.h>
#include <iostream.h>
#include "xform.hpp"

double xform::log2 (double t)
{
	return (log(t)/log(2.0));
}

int xform::log2 (int t)
{
	double td = t;
	int r = int (floor (log2 (td)));
	return r;
}

void xform::join (myVector &left, myVector &right, myVector &result)
{
	result.deallocate();
	int sl, sr, s, i;
	sl = left.Size();
	sr = right.Size();
	s = sl + sr;
	result.allocate(s);
	for ( i = 0; i < sl; i++)
		result[i] = left[i];
	for ( i = sl; i < s; i++)
		result[i] = right[i-sl];
}

void xform::evenSplit ( myVector &input, myVector &result)
{
	int size = input.Size();
	int i;
	int l = size/2;
	result.allocate(l);
	for ( i = 0; i < l; i++)
		result[i] = input[i*2];
}

void xform::forceInsert (myVector &source, myVector &result)
{
	int start = 0;
	int l1 = source.Size();
	int ltwo = result.Size();
/*
	int end = l1 + start;
	int i;
	if ( (l1 < ltwo) && (start < end) & (end < ltwo))
		for ( i = start; i < end; i++)
			result[i] = source[i];	

*/
	int i;
	if ( l1 <= ltwo)
	{
		//cout << "Copy in progress " << endl;
		for ( i = 0; i < l1; i++)
			result[i]= source[i];
	}
}

void xform::forceInsert (myVector &source, myVector &result, int start)
{
	
	int l1 = source.Size();
	int ltwo = result.Size();
	int end = l1 + start;
	int i;
	if ( (l1 < ltwo) && (start < end) & (end < ltwo))
		for ( i = start; i < end; i++)
			result[i] = source[i-start];
	
}

void xform::extract (myVector &source, myVector &result, int start, int end)
{
	int l1, l2, i, j;
	l1 = source.Size();
	
	if ( (0 < start) && (start < end) && (end < l1))
	{
		//result.deallocate();
		l2 = end - start;
		//result.allocate();
		for ( i = start; i < end; i++)
		{
			j = i - start;
			result[j] = source[i];
		}
	}
		
}

void xform::hWave (myVector &input, myVector &result)
{
	hWave(input, result, 1);
}

void xform::hWaveMax (myVector &input, myVector &result)
{
	int l = input.Size();
	int res = log2(l);
	hWave (input, result, res);
}

void xform::hWave (myVector &input, myVector &result, int resolution)
{
	myVector S;
	myVector Ra, Rd, R, F;
	haarGen haarD;
	haarD.genDiff(HD);
	haarD.genAve(HA);
	plotResults test;
	convolveThis myConvolve;
	int l, ls, i, l_2, p2, end;
	l = ls = input.Size();
	S.allocate(ls);
	test.plotVal("sample.hf", input);
	//cout << "Just allocated S at size " << ls << endl;
	result.deallocate();
	result.allocate(ls);
	forceInsert (input, S);
	test.plotVal("sample.cp",S);
	l_2 = log2(l);
	if (resolution < l_2)
		l_2 = resolution;
	for ( i = 0; i < l_2; i++)
	{
		myConvolve.convolve (S, HA, Ra);
		myConvolve.convolve (S, HD, Rd);
		test.plotVal ("haar.R1", Ra, Rd);
		join ( Ra, Rd, R);
		test.plotVal ("haar.J1" , R);
		cout << " Just joined Ra and Rd " << R.Size() << endl;
		evenSplit ( R, F);
		test.plotVal ("haar.F1" , F);
		cout << "Conducted even split" << F.Size() <<endl;
		forceInsert (F, result);
		cout << "Conducted force Insert " << result.Size() << endl;
		p2 = int (floor(pow(double(2), int(i + 1))));
		end = int (l / p2);
		S.deallocate();
                cout << "S is deallocated " << endl;
		S.allocate(end);
        F.deallocate();
        cout << " F is deallocated " << endl;
        Ra.deallocate ();
        cout << "Ra is deallocated " << endl;
        Rd.deallocate ();
        cout << "Rd is deallocated " << endl;
        //R.deallocate(); 
        //cout << " R is deallocated " << endl;                
        cout << "S reallocated " << end << endl;

		extract ( result, S, 0, end);
                        

		
	}
        S.deallocate();
        cout << " S is deallocate " << endl;
        cout << "R size " << R.Size() << endl;
        R.deallocate();
        cout << "R deallocated " << endl;
        

}

void xform::hWaveInverse (myVector &input, myVector &result)
/*
	Haar Wavelet Inverse:
	Purpose: Return result as the inverse transform of a simple Haar Wavelet 			Transform.  The general method to the madness is as follows:
		for all i element of [0,l]  where l is the length of the source
			result[2*i] = (source[i] + source [i+l/2]) * sqrt (1/2);
		obviously sqrt of 1/2 is constant as is l/2, and need be
			computed only once.  
*/
{
	int l = input.Size();
	int l2 = l/2;
        cout << " Preallocation " << endl;
	result.allocate(l);
        cout << "Allocated Result" << endl;
	double sqhalf = pow(0.5, 0.5);
	int i, j, k, m;
        j = 0; 
        result[j] = input[0] + input[l2];
        result[j] *= sqhalf;
	for ( i = 1; i < l2; i++)
	{
		j = 2 *i;
		k = j - 1;
		m = l2 + i;
		result[j] = input[i] + input[m];
		result[k] = input[i] - input[m];
		result[j] *= sqhalf;
		result[k] *= sqhalf;
	}
	
}

void xform::hWaveInverse (myVector &input, myVector &result, int resolution)
/*
	Haar Wavelet Inverse - Multi-Resolution
	Purpose: To separate a vector into segments to transformed, and 
		used in the transform of the whole.  Note this version is based
		on the wavelet represenation AD1D2...Dn  where the each resolution
		is simple a resolution on the previous average.  Other versions
		based on other multi-resolution methods should probably be 
		constructed.
*/
{
	myVector S, R, T;
	int i;
	int p2;
	int end;
	int l = input.Size();
        int r = resolution;
	int l2 = int (pow(2.0,r));
	int fsize = l / l2 * 2; 
	S.allocate (fsize);
	//R.allocate (fsize);
	cout << "Allocated temporary source array" << endl;
	result.allocate (fsize);
        cout << "Allocated Result" << endl;
	forceInsert (input, result);
	extract (result, S, 0, fsize);
	
	
	for ( i = 0; i < l2; i++)
	{
		hWaveInverse (S,R);
		forceInsert (R, result);
		p2 = l2 - i + 2; 
		end = int (l/p2);
		S.deallocate();
		S.allocate (end);
		extract (result, S, 0, end);
		R.deallocate();
	}

	
}

void xform::filterThresh ( myVector &input, myVector &output, int thresh)
{
	int k = input.Size();
	int i, j;
	output.allocate( k);
	for ( i = 0; i < k; i++)
		if ( epsilon (input[i]))
			output[i] = 0.0;
			else
			output[i] = output[i];
}


bool xform::epsilon ( double x)
{
	double t = pow (double(10.0),-10);
	if ( x > t)
		return false;
	if ( x < -t)
		return false;
	return true;
}

