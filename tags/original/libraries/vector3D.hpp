/*! \file vector3D.hpp
    \brief 3D vectors.

*/

#ifndef __VECTOR_3D_HPP__
#define __VECTOR_3D_HPP__

#include "r_valarray.hpp"
#include "point3DH.hpp"

#include <valarray>
#include <iostream>
using namespace std;

class Point3DH;

/*! 
    \brief 3D vectors

    3D vectors. Built using valarray. Most operations are handled
    internally using valarray.  Should be optimized naturally by a
    good implementation of valarray.
*/
class Vector3D : public valarray<double>
{
public:
  //constructor
  Vector3D();
  Vector3D(double x, double y, double z);
  Vector3D(const Vector3D&);
  Vector3D(const valarray<double> &);

  const Vector3D operator=(const Vector3D&);
  const Vector3D operator=(const Point3DH&);
  const Vector3D operator=(const valarray<double>&);
  const Vector3D operator=(double);
};

#endif //__VECTOR_3D_HPP__
