#ifndef __XFORM_HPP__
#define __XFORM_HPP__
#include "myVector.h"
class xform
{
	private:
		myVector HA;
		myVector HD;
	public:
		void join (myVector &left, myVector &right, myVector &result);
		void evenSplit ( myVector &input, myVector &result);
		void oddSplit ( myVector &input, myVector &result);
                void oddPick ( myVector &a, myVector &d , myVector &result);
		void forceInsert (myVector &source, myVector &result);
		void forceInsert (myVector &source, myVector &result, int
		start);
		void extract (myVector &source, myVector &result, int start, int
		end);
		void hWave (myVector &input, myVector &result);
		void hWave (myVector &input, myVector &result, int
		resolution);
		void hWaveMax (myVector &input, myVector &result);
		double log2 (double t);
		int log2 (int t);
		void hWaveInverseE (myVector &input, myVector &result);
		void hWaveInverseE (myVector &input, myVector &result, int resolution);
		void hWaveInverseO (myVector &input, myVector &result);
		void hWaveInverseO (myVector &input, myVector &result, int resolution);
		void filterThresh ( myVector &input, myVector &output, int thresh);
		bool epsilon ( double x );
		
};
#endif 
