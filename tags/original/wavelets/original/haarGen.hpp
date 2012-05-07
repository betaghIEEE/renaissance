#ifndef __HAARGEN_HPP__
#define __HAARGEN_HPP__
//#include "r_valarray.hpp"
//#include <vector.h>
#include "myVector.h" 

class haarGen{
        private:
           
	public:
		const myVector& genDiff(int s, int l);
		const myVector& genDiff(int s);
		const myVector& genAve (int s);
                /*const*/ myVector& genDiff();
		/*const*/ myVector& genAve ();
                /*const*/ void genDiff(myVector& diff);
		/*const*/ void genAve (myVector& ave);
                const myVector& genSample ();
		const myVector& genSample (int Size);	
		void genSample (int Size, myVector &sample);	
};

#endif
