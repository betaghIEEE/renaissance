#include "face.hpp"

Face::Face()
{
  m_pointId.clear();
  m_point.clear();
  m_texCoord.clear();
}

void Face::addPoint(int id, Point3D point)
{
  m_pointId.push_back(id);
  m_point.push_back(point);

  m_vertexNormal.resize(m_point.size());
  m_texCoord.resize(m_point.size());
}

void Face::addVertexNormal(int pointid, Vector3D &normal)
{
  size_t i;
  for (i=0; i<m_pointId.size(); i++)
    if (m_pointId[i] == pointid)
      m_vertexNormal[i] = normal;
}

void Face::addVertexNormal(int pointid, vector<float> normal)
{
  size_t i;
  for (i=0; i<m_pointId.size(); i++)
    if (m_pointId[i] == pointid)
      m_vertexNormal[i] = Vector3D(normal[0], normal[1], normal[2]);
}

void Face::addTextureCoordinate(int pointid, float x, float y)
{
  size_t i;
  for (i=0; i<m_pointId.size(); i++)
    if (m_pointId[i] == pointid)
      m_texCoord[i] = Point2D(x, y);
}

void Face::setNormalPerVertex(bool value)
{
  m_normalPerVertex = value;
}

vector<int> Face::getVertexIds()
{
  return m_pointId;
}

void Face::modifyPoints(vector<Point3D> &pts)
{
	for (int i=0; i<m_pointId.size(); i++)
		m_point[i] = pts[m_pointId[i]];
}

int Face::getVertexId(int v)
{
  return m_pointId[v];
}

int Face::getSize()
{
  return m_pointId.size();
}

void Face::incrementCounters(int amount)
{
  for (int i=0; i<m_pointId.size(); i++)
    m_pointId[i] += amount;
}

void Face::applyTransform(Matrix &M)
{
  int i;

  Point3DH p, q;
  Point3DH n, v;
  
  for (i=0; i<m_point.size(); i++) {
    p = m_point[i];
    q = M*p;
    m_point[i] = q;
  }

  for (i=0; i<m_vertexNormal.size(); i++) {
    n = m_vertexNormal[i];
    v = M*n;
    m_vertexNormal[i] = v;
  }

  n = m_faceNormal;
  v = M*n;
  m_faceNormal = n;
}

/*
void Face::calculateNormal()
{
  int i, j;

  double mx, my, mz;
  double numerator;
  double minimal_numerator_value = 0.0000001;
  mx = my = mz = 0.0;

  for (i=0; i<m_point.size(); i++) {
    j = (i+1)%m_point.size();
    mx += (m_point[i][1]-m_point[j][1])*(m_point[i][2]+m_point[j][2]);
    my += (m_point[i][2]-m_point[j][2])*(m_point[i][0]+m_point[j][0]);
    mz += (m_point[i][0]-m_point[j][0])*(m_point[i][1]+m_point[j][1]);
  }
   
  numerator = sqrt(mx*mx+my*my+mz*mz);
  if (numerator > minimal_numerator_value) {
    mx /= numerator;
    my /= numerator;
    mz /= numerator;
  }

  vector<float> normal(3);
  normal[0] = mx;
  normal[1] = my;
  normal[2] = mz;

  m_normal.resize(1);//m_point.size());
  m_normal[0] = normal;
  m_normalPerVertex = false;
  //  for (int i=0; i<m_point.size(); i++)
  //    m_normal[i] = normal;
}
*/
void Face::flipOrientation()
{
	int i;

	vector<int> pointId2(m_pointId.size());
	for (i=0; i<m_pointId.size(); i++)
		pointId2[m_pointId.size()-1-i] = m_pointId[i];
	m_pointId = pointId2;

	//flip vertex normals and change sign
	vector<Vector3D> vertexNormal2(m_vertexNormal.size());
	for (i=0; i<m_vertexNormal.size(); i++)
		vertexNormal2[m_vertexNormal.size()-1-i] = -m_vertexNormal[i];
	m_vertexNormal = vertexNormal2;

	vector<Point3D> point2(m_point.size());
	for (i=0; i<m_point.size(); i++)
		point2[m_point.size()-1-i] = m_point[i];
	m_point = point2;

	vector<Point2D> texCoord2(m_texCoord.size());
	for (i=0; i<m_texCoord.size(); i++)
		texCoord2[m_texCoord.size()-1-i] = m_texCoord[i];
	m_texCoord = texCoord2;

	m_faceNormal = -m_faceNormal;
}

double computeArea(Face &f)
{
	vector<Point3D> points = f.getPoints();
	if (points.size() != 3) 
		cerr << "ERROR: computeArea: points.size() is " << points.size() << endl;
	Vector3D u = points[1] - points[0];
	Vector3D v = points[0] - points[2];
	Vector3D n = cross(u,v);

	return normL2(n)/2;
}