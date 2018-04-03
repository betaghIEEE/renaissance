#include "point2D.hpp"

Point2D::Point2D() : valarray<double> (2)
{
}

Point2D::Point2D(double x, double y) : valarray<double> (2)
{
  (*this)[0] = x;
  (*this)[1] = y;
}

Point2D::Point2D(const Point2D& p) : valarray<double> (2)
{
  (*this)[0] = p[0];
  (*this)[1] = p[1];
}

const Point2D Point2D::operator=(const Point2D& p)
{
  (*this)[0] = p[0];
  (*this)[1] = p[1];
  
  return (*this);
}
const Point2D Point2D::operator=(const valarray<double>&v)
{
  (*this)[0] = v[0];
  (*this)[1] = v[1];
  
  return (*this);
}

vector<Point2D> load_Point2D_vector(const char *filename)
{
  int num;
  double x, y;
  vector<Point2D> pts;

  ifstream infile(filename);
  
  if (!infile) {
    cerr << "Could not open file: " << filename << endl;
    return pts;

  }

  //first get number of pts;
  infile >> num;
  cout << num << " points" << endl;

  while (!infile.eof()) {
    infile >> x;
    infile >> y;
    if (!infile.eof())
      pts.push_back(Point2D(x,y));
  } 

  return pts;
}
