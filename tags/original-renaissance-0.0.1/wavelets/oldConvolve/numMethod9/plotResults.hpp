#ifndef __PLOT_RESULTS_HPP__
#define __PLOT_RESULTS_HPP__
//#include "r_valarray.hpp"
#include <stddef.h>
#include "myVector.h"
using namespace std;


class plotResults 
{
	public:
		void plotVal (char *output, myVector A, myVector B);
		void plotVal (char *output, /*const*/ myVector &A);
		
};

#endif
