/*! \file camera.hpp
    
    Handles basic camera operations.
    Used by vision/registration.hpp.

*/

#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include "matrix.hpp"

#include <iostream>
#include <valarray>
#include <cmath>
using namespace std;

class Camera {
public:
  Camera();			// standard perspective camera
  Camera(Matrix &R, valarray<double> &T, double ox, double oy, double fx, double fy);
  Camera(const Camera&);
  ~Camera();

  Camera& operator=(const Camera&);

  void display();

  Matrix& getRotation();
  valarray<double>& getTranslation();

  valarray<double> getRayFromPixel(int im_row, int im_col);

private:
  Matrix m_R;			// rotation
  valarray<double> m_T;		// translation
  double m_ox, m_oy;		// optical center (in pixels)
  double m_fx, m_fy;		// focal length
};



#endif // __CAMERA_HPP__

