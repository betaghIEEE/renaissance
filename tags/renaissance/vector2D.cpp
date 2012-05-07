#include "vector2D.hpp"

Vector2D::Vector2D() : valarray<double> (2)
{
}

Vector2D::Vector2D(double x, double y) : valarray<double> (2)
{
  (*this)[0] = x;
  (*this)[1] = y;
}

Vector2D::Vector2D(const Vector2D& p) : valarray<double> (2)
{
  (*this)[0] = p[0];
  (*this)[1] = p[1];
}

const Vector2D Vector2D::operator=(const Vector2D& p)
{
  (*this)[0] = p[0];
  (*this)[1] = p[1];
  
  return (*this);
}
const Vector2D Vector2D::operator=(const valarray<double>&v)
{
  (*this)[0] = v[0];
  (*this)[1] = v[1];
  
  return (*this);
}
