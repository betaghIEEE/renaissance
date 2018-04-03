/*! \file Mesh.hpp
 
	 Representation of a mesh.

	 This should store:
	 * faces
	 * vertices
	 * vertex normals
	 * faces adjacent to vertices
	 * faces adjacent to faces (?)
	 * vertices adjacent to vertices

	 Every mesh should contain:
	 * set of correct points
	 * set of correct faces
	 * set of correct normals
	 * set of correct adjacent portions

   OpenGL interaction should be provided by
   Mesh2GL.

*/


#ifndef __MESH_HPP__
#define __MESH_HPP__

#include "face.hpp"
#include "point3D.hpp"
#include "vector3D.hpp"
#include "edge.hpp"
#include "colorImage.hpp"

#include <list>
#include <vector>
#include <queue>
#include <map>
#include <set>
#include <cstdio>
#include <algorithm>
using namespace std;

class Mesh{

public:
  //constructors and destructors
  Mesh();
  Mesh(char *filename );
  Mesh(const Mesh& );
  Mesh(vector<Point3D> &vertexList, vector<Face> &faceList);
  Mesh(Matrix &H, double width, double height, bool textured=false);
  ~Mesh();

	//handles basic setup of mesh
	void initialize();

  //overloaded operators
  const Mesh& operator= (const Mesh& );

  //I/O operations
  void read(const char *filename);
  void save(char *filename);	// not implemented

  //return values;
  vector<Face> getFaces();
  vector<Point3D> getVertices();
	vector<Vector3D> getVertexNormals();

	vector<Face>* getFacePtr() { return &m_face; }
  vector<Point3D>* getVertexPtr() { return &m_vertex; }
	vector<Vector3D>* getNormalPtr() { return &m_normal; }

  bool isTextured() { return m_isTextured; }
  int getTextureRows() { return m_image_rows; }
  int getTextureCols() { return m_image_cols; }
  unsigned char *getTextureData() { return m_image_data; }

	Point3D getCenter();
	double getMaximumLength();

  //set values
  void setFaces(vector<Face> faces) { m_face = faces; }
  void setVertices(vector<Point3D> pts) { m_vertex = pts; }
	void modifyVertices(vector<Point3D> &pts);

  //join operators
  void merge(const Mesh&);

  //settings
  void calcNormals();
  void setCreaseAngle(float creaseAngle);
  void setCCW(bool ccw);
  void setNormalPerVertex(bool normalPerVertex);
  void applyTransform(Matrix &M);

  //texture 
	void setTextureMap(ColorImage &);
  void addTextureCoordinate(int fID, int pID, float u, float v);

  //other
  void normalize();
  list<Edge> extractEdges();
	void examine();
  void fixOrientation();

	//operations
	void smooth(int num_iterations = 10, double lambda = 0.33, double mu_diff = 0.01);

	vector<double> computeMeanCurvature();
	vector<double> computeGaussianCurvature();
	vector<vector<double> > computePrincipalCurvatures();

	list<int>* getAdjacentVertices(int i) { return &m_adjVertex[i]; }
	list<int>* getAdjacentFaces(int i) { return &m_adjFace[i]; }
	bool isBoundary(int i) { return m_boundaryVertex[i]; }

protected:
  //variables
  vector<Face>				m_face;
  vector<Point3D>			m_vertex;
	vector<bool>        m_boundaryVertex; // which vertices are boundary vertices
	vector<Vector3D>		m_normal;
	vector<list<int> >	m_adjVertex; // adjacent points to a given point
	vector<list<int> >	m_adjFace;  // adjacent faces to a given point

	//general info
  double							m_creaseAngle;
  bool								m_ccw;
  bool								m_normalPerVertex;

  //texture info
  bool								m_isTextured;
  unsigned char*			m_image_data;
  int									m_image_rows;
  int									m_image_cols;
  int									m_image_size;

private:
  //internal functions
  void incrementCounters(int amount); // done so merge can occur
  void copyImage(unsigned char *data, int rows, int cols, int size);
	void determineBoundary();
	void setAdjacentFaces();
	void setAdjacentVertices();

	//individual implementation of I/O functions
  void readM(const char *filename);
	void readOFF(const char *filename );
};


#endif //__MESH_HPP__
