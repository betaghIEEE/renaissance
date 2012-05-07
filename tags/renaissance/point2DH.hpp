/*! \file point2DH.hpp
    \brief Class and methods for working with map points.

    Point2DH provides mathematical operations on
    map points. 
*/

#ifndef __POINT_2DH_HPP__
#define __POINT_2DH_HPP__

#include "r_valarray.hpp"
#include <iostream>
using namespace std;

/*! 
    \brief 2D homogeneous points
    
*/
class Point2DH : public valarray<double>
{
public:
  //constructor
  Point2DH();
  Point2DH(double x, double y);
  Point2DH(double x, double y, double w);
  Point2DH(const Point2DH&);

  const Point2DH operator=(const Point2DH&);
  const Point2DH operator=(const valarray<double>&);

  void display();
};

#endif //__POINT_2DH_HPP__
