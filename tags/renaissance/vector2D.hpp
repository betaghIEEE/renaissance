/*! \file vector2D.hpp
    \brief Class and methods for working with vectors.

    Vector2D provides mathematical operations vectors. 
*/

#ifndef __VECTOR_2D_HPP__
#define __VECTOR_2D_HPP__

#include "r_valarray.hpp"
#include <iostream>
using namespace std;

/*! 
    \brief Vector in 2D space.
    
*/
class Vector2D : public valarray<double>
{
public:
  //constructor
  Vector2D();
  Vector2D(double x, double y);
  Vector2D(const Vector2D&);

  const Vector2D operator=(const Vector2D&);
  const Vector2D operator=(const valarray<double>&);

  void display();
};

#endif //__VECTOR_2D_HPP__
