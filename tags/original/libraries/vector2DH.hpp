/*! \file vector2DH.hpp
    \brief Class and methods for working with map vectors.

    Vector2DH provides mathematical operations on
    map vectors. 
*/

#ifndef __VECTOR_2DH_HPP__
#define __VECTOR_2DH_HPP__

#include "r_valarray.hpp"
#include <iostream>
using namespace std;

/*! 
    \brief Homogeneous 2D vectors
    
*/
class Vector2DH : public valarray<double>
{
public:
  //constructor
  Vector2DH();
  Vector2DH(double x, double y);
  Vector2DH(double x, double y, double w);
  Vector2DH(const Vector2DH&);

  const Vector2DH operator=(const Vector2DH&);
  const Vector2DH operator=(const valarray<double>&);

  void display();
};

#endif //__VECTOR_2DH_HPP__
