#include "camera.hpp"

Camera::Camera()
{
  m_R.resize(3,3);
  m_T.resize(3);
}

Camera::Camera(Matrix &R, 
	       valarray<double> &T, 
	       double ox, double oy, 
	       double fx, double fy)
{
  m_R = R;
  m_T.resize(3); m_T = T;
  m_ox = ox; m_oy = oy;
  m_fx = fx; m_fy = fy;
}

Camera::Camera(const Camera& src)
{
  m_R = src.m_R;
  m_T.resize(3); m_T = src.m_T;
  m_ox = src.m_ox; m_oy = src.m_oy;
  m_fx = src.m_fx; m_fy = src.m_fy;
}

Camera::~Camera()
{
}

Camera& Camera::operator=(const Camera& src)
{
  m_R = src.m_R;
  m_T = src.m_T;
  m_ox = src.m_ox; m_oy = src.m_oy;
  m_fx = src.m_fx; m_fy = src.m_fy;
  return *this;
}

Matrix& Camera::getRotation()
{
  return m_R;
}

valarray<double>& Camera::getTranslation()
{
  return m_T;
}

valarray<double> Camera::getRayFromPixel(int im_row, int im_col)
{
  valarray<double> r(3);
  
  r[0] = -(im_row-m_ox)*m_fx;
  r[1] = -(im_col-m_oy)*m_fy;
  r[2] = -sqrt((m_fx*m_fx+m_fy*m_fy)/2.0);

  return r;
}

void Camera::display()
{
  cout << "The Rotation Matrix:\n" << m_R;
  cout << "The Translation Vector: " << m_T << endl;
  cout << "The Focal Lengths: ( " << m_fx << ", " << m_fy << ") " << endl;
  cout << "The Optical Center: ( " << m_ox << ", " << m_oy << ") " << endl;
}

