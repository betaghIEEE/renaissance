/*! \file filter.hpp
    \brief Creates and uses filters.

    filter contains a set of filters
    to be applied to valarrays, matrices,
    and images.
*/

#ifndef __FILTER_HPP__
#define __FILTER_HPP__

#include "r_valarray.hpp"

valarray<double> createGaussianFilter(int filterLength);

#endif __FILTER_HPP__
