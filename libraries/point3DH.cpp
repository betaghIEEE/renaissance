#include "point3DH.hpp"

Point3DH::Point3DH() : valarray<double> (4)
{
}

Point3DH::Point3DH(double x, double y, double z, double w) : valarray<double> (4)
{
  (*this)[0] = x;
  (*this)[1] = y;
  (*this)[2] = z;
  (*this)[3] = w;
}

Point3DH::Point3DH(const Point3DH& p) : valarray<double> (4)
{
  (*this)[0] = p[0];
  (*this)[1] = p[1];
  (*this)[2] = p[2];
  (*this)[3] = p[3];
}

const Point3DH Point3DH::operator=(const Point3DH& p)
{
  if (&p != this) {
    (*this)[0] = p[0];
    (*this)[1] = p[1];
    (*this)[2] = p[2];
    (*this)[3] = p[3];
  }
  return (*this);
}

const Point3DH Point3DH::operator=(const Point3D& p)
{
  (*this)[0] = p[0];
  (*this)[1] = p[1];
  (*this)[2] = p[2];
  (*this)[3] = 1.0;
  
  return (*this);
}
const Point3DH Point3DH::operator=(const Vector3D& v)
{
  (*this)[0] = v[0];
  (*this)[1] = v[1];
  (*this)[2] = v[2];
  (*this)[3] = 0.0;
  
  return (*this);
}
const Point3DH Point3DH::operator=(const valarray<double>&v)
{
  (*this)[0] = v[0];
  (*this)[1] = v[1];
  (*this)[2] = v[2];
  (*this)[3] = v[3];
 
  return (*this);
}

