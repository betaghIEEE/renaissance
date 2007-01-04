/*! \file point3DH.hpp
    \brief Homogeneous 3D coordinates.

    Got to rewrite equality basically.
    Overload some constructors.
*/

#ifndef __POINT_3DH_HPP__
#define __POINT_3DH_HPP__

#include "r_valarray.hpp"
#include "point3D.hpp"
#include "vector3D.hpp"

#include <valarray>
#include <iostream>
using namespace std;

/*! 
    \brief Basic class holding 3DH points.
    
*/

class Point3D;
class Vector3D;

class Point3DH : public valarray<double>
{
public:
  //constructor
  Point3DH();
  Point3DH(double x, double y, double z, double w);
  Point3DH(const Point3DH&);

  const Point3DH operator=(const Point3DH&);
  const Point3DH operator=(const Point3D&);
  const Point3DH operator=(const Vector3D&);
  const Point3DH operator=(const valarray<double>&);

  void display();
};

#endif //__POINT_3DH_HPP__
