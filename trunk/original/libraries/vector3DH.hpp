/*! \file vector3DH.hpp
    \brief Class and methods for working with map vectors.

    Vector3DH provides mathematical operations on
    map vectors. 
*/

#ifndef __VECTOR_3DH_HPP__
#define __VECTOR_3DH_HPP__


#include "r_valarray.hpp"
using namespace std;

/*! 
    \brief Homogeneous 3D vectors
    
*/
class Vector3DH : public valarray<double>
{
public:
  //constructor
  Vector3DH();
  Vector3DH(double x, double y, double z);
  Vector3DH(double x, double y, double z, double w);
  Vector3DH(const Vector3DH&);

  const Vector3DH operator=(const Vector3DH&);
  const Vector3DH operator=(const valarray<double>&);

  void display();
};

#endif //__VECTOR_3DH_HPP__
