/*! \file point3D.hpp
    \brief Class and methods for working with map points.

    Point3D provides mathematical operations on
    map points. 
*/

#ifndef __POINT_3D_HPP__
#define __POINT_3D_HPP__

#include "r_valarray.hpp"
#include "point3DH.hpp"

#include <valarray>
#include <iostream>
using namespace std;

class Point3DH;

/*! 
    \brief Basic class holding 3D points.
    
*/

class Point3D : public valarray<double>
{
public:
  //constructor
  Point3D();
  Point3D(double x, double y, double z);
  Point3D(const Point3D&);

  const Point3D operator=(const Point3D&);
  const Point3D operator=(const Point3DH&);
  const Point3D operator=(const valarray<double>&);

  void display();
};

double dist(Point3D &x, Point3D &y);

#endif //__POINT_3D_HPP__
