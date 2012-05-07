/*! \file face.hpp 

    Face provides a compact structure for efficient rendering of polygons.

    ----------------------------------------------------------------------  */

#ifndef __FACE_HPP__
#define __FACE_HPP__

#include "matrix.hpp"
#include "point3D.hpp"
#include "point2D.hpp"
#include "point3DH.hpp"
#include "vector3D.hpp"

#include <cmath>
#include <vector>
using namespace std;

class Face{
public:
  Face();
  
  //add values
  void addPoint(int id, Point3D point);
  void addTextureCoordinate(int pointid, float x, float y);
  void addVertexNormal(int pointid, Vector3D &);
  void addVertexNormal(int pointid, vector<float> );
  void setFaceNormal(Vector3D& nrm) { m_faceNormal = nrm; }

  //set values
  void setNormalPerVertex(bool);
  int getVertexNormalSize() { return m_vertexNormal.size(); }
	void modifyPoints(vector<Point3D> &pts);

  //get values
  vector<int> getVertexIds();
  vector<Point3D> getPoints() { return m_point; }
  Vector3D getFaceNormal() { return m_faceNormal; }
  vector<Vector3D> getVertexNormals() { return m_vertexNormal; }
  int getVertexId(int v);
  int getSize();
  vector<Point2D> getTextureCoordinates() { return m_texCoord; }

	bool hasVertexId(int id) { 
		for (int j=0; j<m_pointId.size(); j++)
			if (m_pointId[j] == id)
				return true;
			return false;
	}
	void replaceVertexId(int src, int dst) { 
		for (int j=0; j<m_pointId.size(); j++)
			if (m_pointId[j] == src)
				m_pointId[j] = dst;
	}
		
	

  void incrementCounters(int amount);

	//transforms
  void applyTransform(Matrix &M);
	void flipOrientation();

private:
  vector<int>      m_pointId;
  vector<Vector3D> m_vertexNormal;
  vector<Point3D>  m_point;
  vector<Point2D>  m_texCoord;

  Vector3D         m_faceNormal;

  bool m_normalPerVertex;
};


//operations on face
double computeArea(Face &f);

#endif //__FACE_HPP__
    
