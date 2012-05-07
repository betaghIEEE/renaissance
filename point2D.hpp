/*! \file point2D.hpp
    \brief Class and methods for working with map points.

    Point2D provides mathematical operations on
    map points. 
*/

#ifndef __POINT_2D_HPP__
#define __POINT_2D_HPP__

#include "r_valarray.hpp"
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

/*! 
    \brief Points in 2D space.
    
*/
class Point2D : public valarray<double>
{
public:
  //constructor
  Point2D();
  Point2D(double x, double y);
  Point2D(const Point2D&);

  const Point2D operator=(const Point2D&);
  const Point2D operator=(const valarray<double>&);

  void display();
};


vector<Point2D> load_Point2D_vector(const char *filename);


#endif //__POINT_2D_HPP__
