#include "point3D.hpp"

Point3D::Point3D() : valarray<double> (3)
{
}

Point3D::Point3D(double x, double y, double z) : valarray<double> (3)
{
  (*this)[0] = x;
  (*this)[1] = y;
  (*this)[2] = z;
}

Point3D::Point3D(const Point3D& p) : valarray<double> (3)
{
  (*this)[0] = p[0];
  (*this)[1] = p[1];
  (*this)[2] = p[2];
}

const Point3D Point3D::operator=(const Point3D& p)
{
  if (&p != this) {
    (*this)[0] = p[0];
    (*this)[1] = p[1];
    (*this)[2] = p[2];
  }
  
  return (*this);
}
const Point3D Point3D::operator=(const Point3DH& p)
{
  (*this)[0] = p[0]/p[3];
  (*this)[1] = p[1]/p[3];
  (*this)[2] = p[2]/p[3];
  
  return (*this);
}
const Point3D Point3D::operator=(const valarray<double>&v)
{
  (*this)[0] = v[0];
  (*this)[1] = v[1];
  (*this)[2] = v[2];
 
  return (*this);
}

double dist(Point3D &x, Point3D &y)
{
  return sqrt(pow(x[0]-y[0], 2.0)+
	      pow(x[1]-y[1], 2.0)+
	      pow(x[2]-y[2], 2.0));
}
