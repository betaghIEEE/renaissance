#include "r_valarray.hpp"

#include <iostream>
using namespace std;

//double operations
const valarray<double> operator+(const valarray<double> &lhs, const valarray<double> &rhs)
{
  valarray<double> x = lhs;
  x += rhs;

  return x;
}
const valarray<double> operator-(const valarray<double> &lhs, const valarray<double> &rhs)
{
  valarray<double> x = lhs;
  x -= rhs;

  return x;
}
const valarray<double> operator*(const valarray<double> &lhs, const valarray<double> &rhs)
{
  valarray<double> x = lhs;
  x *= rhs;

  return x;
}

const valarray<double> operator*(double lhs, const valarray<double> &rhs)
{
	valarray<double> x = rhs;
	x *= lhs;

	return x;
}

const valarray<double> operator/(const valarray<double> &lhs, const valarray<double> &rhs)
{
  valarray<double> x = lhs;
  x /= rhs;

  return x;
}
const valarray<double> operator/(const valarray<double> &lhs, double rhs)
{
  valarray<double> x = lhs;
  x /= rhs;

  return x;
}
const valarray<double> operator/(double lhs, const valarray<double> &rhs)
{
  valarray<double> x = rhs;
  x *= lhs;

  return x;
}
double dotProduct(const valarray<double> &lhs, const valarray<double> &rhs)
{
  double x = 0;
  for (int i=0; i<lhs.size(); i++)
    x += lhs[i]*rhs[i];
  return x;
}
double dot(const valarray<double> &lhs, const valarray<double> &rhs)
{
  double x = 0;
  for (int i=0; i<lhs.size(); i++)
    x += lhs[i]*rhs[i];
  return x;
}
valarray<double> dotMultiply(const valarray<double> &lhs, const valarray<double> &rhs)
{
  valarray<double> X(lhs.size());
  for (int i=0; i<lhs.size(); i++)
    X[i] = lhs[i]*rhs[i];
  return X;
}
const valarray<double> crossProduct(const valarray<double> &lhs, const valarray<double> &rhs)
{
  valarray<double> x(lhs.size());
  if (lhs.size() != 3 || rhs.size() != 3) {
    cerr << "Error: crossProduct: One of the vectors is not 3x3" << endl;
    return x;
  }

  x[0] = lhs[1]*rhs[2]-lhs[2]*rhs[1];
  x[1] = lhs[2]*rhs[0]-lhs[0]*rhs[2];
  x[2] = lhs[0]*rhs[1]-lhs[1]*rhs[0];

  return x;
}
const valarray<double> cross(const valarray<double> &lhs, const valarray<double> &rhs)
{
  valarray<double> x(lhs.size());
  if (lhs.size() != 3 || rhs.size() != 3) {
    cerr << "Error: crossProduct: One of the vectors is not 3x3" << endl;
    return x;
  }

  x[0] = lhs[1]*rhs[2]-lhs[2]*rhs[1];
  x[1] = lhs[2]*rhs[0]-lhs[0]*rhs[2];
  x[2] = lhs[0]*rhs[1]-lhs[1]*rhs[0];

  return x;
}

const valarray<double> sqrt(const valarray<double> &src)
{
  valarray<double> x(src.size());
  for (int i=0; i<src.size(); i++)
    x[i] = sqrt(src[i]);
  return x;
}
double sum(const valarray<double> &src)
{
  double x = 0.0;
  for (int i=0; i<src.size(); i++)
    x += src[i];
  return x;
}

ostream& operator<<(ostream& os, const valarray<double>& src)
{
  for (int i=0; i<src.size()-1; i++)
    os << src[i] << ", ";
  if (src.size() > 0)
    os << src[src.size()-1];

  return os;
}



//int operations
const valarray<int> operator+(const valarray<int> &lhs, const valarray<int> &rhs)
{
  valarray<int> x = lhs;
  x += rhs;

  return x;
}
const valarray<int> operator-(const valarray<int> &lhs, const valarray<int> &rhs)
{
  valarray<int> x = lhs;
  x -= rhs;

  return x;
}
const valarray<int> operator*(const valarray<int> &lhs, const valarray<int> &rhs)
{
  valarray<int> x = lhs;
  x *= rhs;

  return x;
}
const valarray<int> operator/(const valarray<int> &lhs, const valarray<int> &rhs)
{
  valarray<int> x = lhs;
  x /= rhs;

  return x;
}

double normL2(const valarray<double> &v)
{
  double total = 0;
  for (int i=0; i<v.size(); i++)
    total += v[i]*v[i];
  return sqrt(total);
}

double normL2(const valarray<int> &v)
{
  double total = 0;
  for (int i=0; i<v.size(); i++)
    total += v[i]*v[i];
  return sqrt(total);
}
double distance(const valarray<double> &lhs, const valarray<double> &rhs)
{
  double total = 0.0;
  for (int i=0; i<lhs.size(); i++)
    total += pow(lhs[i]-rhs[i], 2);
  return sqrt(total);
}

double distance(const valarray<int> &lhs, const valarray<int> &rhs)
{
  double total = 0.0;
  for (int i=0; i<lhs.size(); i++)
    total += pow(double(lhs[i]-rhs[i]), double(2));
  return sqrt(total);
}

const valarray<double> convolve(const valarray<double> &lhs, const valarray<double> &rhs)
{
  valarray<double> result(lhs.size());
  double sum;
  int a;
  
  for (int i=0; i<lhs.size(); i++) {
    sum = 0;
    a = i-int(rhs.size())/2;
    for (int j=rhs.size()-1; j>=0; j--, a++) {
      if (a >= 0 && a < lhs.size() ) 
	sum += lhs[a]*rhs[j];
    }
    result[i] = sum;
  }

  return result;
}

