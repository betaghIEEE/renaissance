/*! \file r_valarray.hpp
    \brief Provides fixes to the valarray class.

    The valarray class from the STL doesn't handle nonmember
    operations in the way that makes easy inclusion. Functions like
    operator*(valarray<T>& const, const valarray<T>&) return 
    expressions and not valarrays.
    
    To combat that, r_valarray.hpp redclares all of the nonmember
    operations for both valarray<double> and valarray<int>.
*/

#ifndef __R_VALARRAY_HPP__
#define __R_VALARRAY_HPP__

#include <valarray>
#include <iostream>
#include <cmath>
using namespace std;

//first double
const valarray<double> operator+(const valarray<double> &lhs, const valarray<double> &rhs);
const valarray<double> operator-(const valarray<double> &lhs, const valarray<double> &rhs);
const valarray<double> operator/(const valarray<double> &lhs, const valarray<double> &rhs);
const valarray<double> operator*(const valarray<double> &lhs, const valarray<double> &rhs);

const valarray<double> operator/(const valarray<double> &lhs, double rhs);
const valarray<double> operator*(double lhs, const valarray<double> &rhs);

double dotProduct(const valarray<double> &lhs, const valarray<double> &rhs);
double dot(const valarray<double> &lhs, const valarray<double> &rhs);
valarray<double> dotMultiply(const valarray<double> &lhs, const valarray<double> &rhs);
const valarray<double> crossProduct(const valarray<double> &lhs, const valarray<double> &rhs);
const valarray<double> cross(const valarray<double> &lhs, const valarray<double> &rhs);
const valarray<double> sqrt(const valarray<double> &src);
double sum(const valarray<double> &src);
ostream& operator<<(ostream& os, const valarray<double>& src);

//should also do it for:
//%,^,&,|,<<,>>,&&,||,==,!=,<,>,<=,>=,atan2, and pow

//also single cases of
//abs, acos, asin, atan, cos, cosh, exp, log, log10, sin, sinh, sqrt, tan, tanh

//now int
const valarray<int> operator+(const valarray<int> &lhs, const valarray<int> &rhs);
const valarray<int> operator-(const valarray<int> &lhs, const valarray<int> &rhs);
const valarray<int> operator/(const valarray<int> &lhs, const valarray<int> &rhs);
const valarray<int> operator*(const valarray<int> &lhs, const valarray<int> &rhs);


//other interesting operations
double normL2(const valarray<double> &); // sum(pow(valarray,2))
double normL2(const valarray<int> &);

double distance(const valarray<double> &, const valarray<double> &);
double distance(const valarray<int> &, const valarray<int> &);

//convolution operators
/*! \brief convolves lhs with rhs.

    Defined to be
    \f[ A(m) = \sum_{n=-K}^K B(-n+K) A(m+n)
    \f]
    truncates excess.
    Actually defined by
    \f[ A(m) = \sum_{n=0}^K B(K-n) A(i-K/2+n)
    \f]
*/
const valarray<double> convolve(const valarray<double> &, const valarray<double> &);


#endif //__R_VALARRAY_HPP__
