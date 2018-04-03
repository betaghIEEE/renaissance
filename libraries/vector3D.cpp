#include "vector3D.hpp"

Vector3D::Vector3D() : valarray<double> (3)
{
}

Vector3D::Vector3D(double x, double y, double z) : valarray<double> (3)
{
  (*this)[0] = x;
  (*this)[1] = y;
  (*this)[2] = z;
}

Vector3D::Vector3D(const Vector3D& p) : valarray<double> (3)
{
  (*this)[0] = p[0];
  (*this)[1] = p[1];
  (*this)[2] = p[2];
}

Vector3D::Vector3D(const valarray<double>&v) : valarray<double> (3)
{
  (*this)[0] = v[0];
  (*this)[1] = v[1];
  (*this)[2] = v[2];
}
const Vector3D Vector3D::operator=(const Vector3D& p)
{
  (*this)[0] = p[0];
  (*this)[1] = p[1];
  (*this)[2] = p[2];
  
  return (*this);
}
const Vector3D Vector3D::operator=(const Point3DH& p)
{
  (*this)[0] = p[0];
  (*this)[1] = p[1];
  (*this)[2] = p[2];
  
  return (*this);
}
const Vector3D Vector3D::operator=(const valarray<double>&v)
{
  (*this)[0] = v[0];
  (*this)[1] = v[1];
  (*this)[2] = v[2];
 
  return (*this);
}
const Vector3D Vector3D::operator=(double v)
{
  (*this)[0] = v;
  (*this)[1] = v;
  (*this)[2] = v;
 
  return (*this);
}
