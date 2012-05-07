/*! \file point2I.hpp
    \brief Class and methods for indexing image information.

    point2I is utilized to represent positions in images.
    These can include features such as corners.
    In addition, methods are presented to produce
    visual display of these features as well.
*/

#ifndef __POINT_2I_HPP__
#define __POINT_2I_HPP__

#include "grayImage.hpp"
#include "colorImage.hpp"
#include "r_valarray.hpp"
#include "point3D.hpp"

#include <list>
#include <vector>
#include <fstream>
#include <iostream>
using namespace std;

/*! 
    \brief Basic class holding 2I points.
    
*/

class Point2I : public valarray<int>
{
public:
  //constructor
  Point2I();
  Point2I(int x, int y);
  Point2I(const Point2I&);

  const Point2I operator=(const Point2I&);
  const Point2I operator=(const valarray<int>&);
  /*! \brief sets point2I to the homogeneous 3D point
   */
  const Point2I operator=(const Point3D&);

  void display();
};

/*! \brief Draws boxes around points on a color image.
*/
const ColorImage drawPoints(const ColorImage &image, vector<Point2I> &points, int boxRadius=7,
			     unsigned char red=255, unsigned char green=0, unsigned char blue=0);
/*! \brief Draws boxes around points on a color image.
*/
const ColorImage drawPoints(const ColorImage &image, list<Point2I> &points, int boxRadius=7,
			     unsigned char red=255, unsigned char green=255, unsigned char blue=255);

/*! \brief Marks an x on the image and puts a label on the right
 */
const ColorImage labelPoints(const ColorImage &image, vector<Point2I> &points, 
			      unsigned char red=255, unsigned char green=255, unsigned char blue=255);

/*! \brief Marks an x on the image and puts a label on the right
 */
const ColorImage labelPoints(const ColorImage &image, vector<Point2I> &points, vector<int> &ids,
			      unsigned char red=255, unsigned char green=255, unsigned char blue=255);

void markPoints(char *filename, const ColorImage &image, vector<Point2I> &points,
		unsigned char red=255, unsigned char green=255, unsigned char blue=255);

// no ids in a list so no need to label
/* \brief Marks an x on the image and puts a label on the right
 */
//const ColorImage labelPoints(const ColorImage &image, list<Point2I> &points, 
//			      unsigned char red=255, unsigned char green=255, unsigned char blue=255);

void save_Point2I_vector(vector<Point2I> &pts, const char *filename);
vector<Point2I> load_Point2I_vector(const char *filename);

#endif //__POINT_2I_HPP__
