#include "mesh.hpp"

const double IDEAL_CREASE_ANGLE = 30.0;

Mesh::Mesh()
{
  m_creaseAngle = IDEAL_CREASE_ANGLE;
  m_ccw = true;
  m_normalPerVertex = true;

  m_face.clear();
  m_vertex.clear();
	m_normal.clear();

  m_isTextured = false;
  m_image_data = NULL;
  m_image_rows = 0;
  m_image_cols = 0;
  m_image_size = 0;
}

Mesh::Mesh(char *filename)
{
  m_creaseAngle = IDEAL_CREASE_ANGLE;
  m_ccw = true;
  m_normalPerVertex = true;

  m_face.clear();
  m_vertex.clear();
	m_normal.clear();

  m_isTextured = false;
  m_image_data = NULL;
  m_image_rows = 0;
  m_image_cols = 0;
  m_image_size = 0;

  read(filename);
}
 
Mesh::Mesh(const Mesh& mesh)
{
  m_creaseAngle = mesh.m_creaseAngle;
  m_ccw = mesh.m_ccw;
  m_normalPerVertex = mesh.m_normalPerVertex;

  m_face = mesh.m_face;
  m_vertex = mesh.m_vertex;
  
  m_isTextured = mesh.m_isTextured;
  m_image_data = NULL;
  if (mesh.m_image_data != NULL)
    copyImage(mesh.m_image_data, mesh.m_image_rows, mesh.m_image_cols, mesh.m_image_size);

	initialize();
}

Mesh::Mesh(vector<Point3D> &vertexList, vector<Face> &faceList)
{
  m_creaseAngle = IDEAL_CREASE_ANGLE;
  m_ccw = true;
  m_normalPerVertex = true;

  m_vertex = vertexList;
  m_face = faceList;

  m_isTextured = false;
  m_image_data = NULL;
  m_image_rows = 0;
  m_image_cols = 0;
  m_image_size = 0;

	initialize();
}

Mesh::Mesh(Matrix &H, double height, double width, bool textured)
{
  m_creaseAngle = IDEAL_CREASE_ANGLE;
  m_ccw = true;
  m_normalPerVertex = true;

  m_isTextured = textured;
  m_image_data = NULL;
  m_image_rows = 0;
  m_image_cols = 0;
  m_image_size = 0;

  m_vertex.resize(H.getRowSize()*H.getColSize());
  m_face.clear();

  int r, c, id;

  for (r=0; r<H.getRowSize(); r++)
    for (c=0; c<H.getColSize(); c++) {
      m_vertex[r*H.getColSize()+c] = Point3D(r/double(H.getRowSize())*height, c/double(H.getColSize())*width, H(r,c));
      if (r != 0 && c != 0) {

	Face f1;
	id = (r-1)*H.getColSize()+(c-1);
	f1.addPoint(id, m_vertex[id]);
	f1.addTextureCoordinate(id, (r-1)/double(H.getRowSize()-1), (c-1)/double(H.getColSize()-1) );

	id = (r-1)*H.getColSize()+(c);
	f1.addPoint(id, m_vertex[id]);
	f1.addTextureCoordinate(id, (r-1)/double(H.getRowSize()-1), (c)/double(H.getColSize()-1) );

	id = (r)*H.getColSize()+(c-1);
	f1.addPoint(id, m_vertex[id]);
	f1.addTextureCoordinate(id, (r)/double(H.getRowSize()-1), (c-1)/double(H.getColSize()-1) );

	Face f2;
	id = (r-1)*H.getColSize()+(c);
	f2.addPoint(id, m_vertex[id]);
	f2.addTextureCoordinate(id, (r-1)/double(H.getRowSize()-1), (c)/double(H.getColSize()-1) );

	id = (r)*H.getColSize()+(c);
	f2.addPoint(id, m_vertex[id]);
	f2.addTextureCoordinate(id, (r)/double(H.getRowSize()-1), (c)/double(H.getColSize()-1) );

	id = (r)*H.getColSize()+(c-1);
	f2.addPoint(id, m_vertex[id]);
	f2.addTextureCoordinate(id, (r)/double(H.getRowSize()-1), (c-1)/double(H.getColSize()-1) );

	m_face.push_back(f1);
	m_face.push_back(f2);
      }
    }
}

Mesh::~Mesh()
{
  if (m_image_data != NULL)
    delete []m_image_data;
}

void Mesh::determineBoundary()
{
	int i;
	int pid0, pid1;
	int count;

	m_boundaryVertex.resize(m_vertex.size());
	for (i=0; i<m_boundaryVertex.size(); i++)
		m_boundaryVertex[i] = true;

	list<Edge> edges = extractEdges();
	for (list<Edge>::iterator e = edges.begin(); e != edges.end(); e++) {

		pid0 = e->m_pt0;
		pid1 = e->m_pt1;

		count = 0;

		for (list<int>::iterator f = m_adjFace[pid0].begin(); f!=m_adjFace[pid0].end(); f++) 
			if (m_face[*f].hasVertexId(pid1))
				count++;

		if (count < 1 || count > 2)
			cerr << "Strange count for edge\n";
		else if (count == 1) {
			m_boundaryVertex[pid0] = true;
			m_boundaryVertex[pid1] = true;
		}
		else if (count == 20) {
			m_boundaryVertex[pid0] = false;
			m_boundaryVertex[pid1] = false;
		}
	}
}

void Mesh::initialize()
{
	//set adjacent vertices
	setAdjacentFaces();
	
	//set adjacent faces
	setAdjacentVertices();

	//set boundary
	determineBoundary();

	//calc normals
	calcNormals();
}

void Mesh::copyImage(unsigned char *data, int rows, int cols, int size)
{
  int j;

  if (size == 0 || size != rows*cols*3) {
    
    if (m_image_data != NULL ) 
      delete []m_image_data;
    m_image_data = NULL;
    m_image_rows = 0;
    m_image_cols = 0;
    m_image_size = 0;
    return;
  }

  m_image_rows = rows;
  m_image_cols = cols;
  m_image_size = rows*cols*3;

  if (m_image_data != NULL)
    delete []m_image_data;
  m_image_data = new unsigned char[m_image_size];
  for (j=0; j<m_image_size; j++)
    m_image_data[j] = data[j];
}

const Mesh& Mesh::operator= (const Mesh& rhs)
{
  if (&rhs != this && rhs.m_face.size() > 0) {
    m_face = rhs.m_face;
    m_vertex = rhs.m_vertex;
    m_creaseAngle = rhs.m_creaseAngle;
    m_ccw = rhs.m_ccw;
    m_normalPerVertex = rhs.m_normalPerVertex;

    m_isTextured = rhs.m_isTextured;
    copyImage(rhs.m_image_data, rhs.m_image_rows, rhs.m_image_cols, rhs.m_image_size);
  }

	initialize();

  return (*this);
}

//old code I wrote that is supposedly fast
void Mesh::read(const char *filename)
{
  //determine format here
  string name = filename;
  string suffix(name, name.rfind(".")+1, name.size()-(name.rfind(".")+1));

  cout << "Suffix is: " << suffix << endl;

  if (suffix=="m")
    readM(filename);
  else if (suffix=="off")
    readOFF(filename);
  else {
    cerr << "ERROR! Mesh::read ... couldn't read " << filename << endl;
    exit(1);
  }

	initialize();
}

void Mesh::readOFF( const char *filename )
{
  unsigned int v, f;
  unsigned int vertices, triangles, edges;
  float c0, c1, c2;
  long int p0, p1, p2;

  FILE	*inFile= fopen( filename, "r" );
  if( !inFile ||
      fscanf( inFile, "OFF %d %d %d ", &vertices, &triangles, &edges )!= 3 )
    {
      cerr << "ERROR! Mesh::readOFF ... couldn't read " << filename << "\n";
      //		cerr << strerror[errno] << endl;
      exit(1);
    }

  cout << "Reading " << vertices << " vertices\n";

  m_face.resize(triangles);
  m_vertex.resize(vertices);

  //read all vertices
  for (v=0; v<vertices; v++) {
    if (fscanf(inFile, "%f %f %f ", &c0, &c1, &c2) != 3) {
      cerr << "ERROR! Mesh::readOFF ... couldn't read " << filename << "\n";
      cerr << "vertex " << v << " failed\n";
      //cerr << strerror[errno] << endl;
      exit(1);
    }
    m_vertex[v] = Point3D(c0, c1, c2);
  }

  //read all the faces ... assume that they are triangles
  for (f=0; f<triangles; f++) {
    if (fscanf(inFile, "3 %d %d %d ", &p0, &p1, &p2) != 3) {
      cerr << "ERROR! Mesh::readOFF ... couldn't read " << filename << "\n";
      cerr << "Face " << f << " failed\n";
      //		  cerr << strerror[errno] << endl;
      exit(1);
    }
    m_face[f].addPoint(p0, m_vertex[p0]);
    m_face[f].addPoint(p1, m_vertex[p1]);
    m_face[f].addPoint(p2, m_vertex[p2]);
  }
  
  if( edges!= 0 )
    cerr << filename << ": ignoring edges\n";

  fclose(inFile);
}

void Mesh::readM(const char *filename)
{
  int i, index;
  double c0, c1, c2; 
  int p0, p1, p2;
  int ch;
  FILE *infile;
  infile = fopen(filename, "rb");

  int p=0, f=0;
  int maxVertex = 0;

  if (!infile) {
    cerr << "ERROR! readM: can't read " << filename << endl;
    exit(1);
  }

  //first determine number of faces, points and the maximum vertex
  //there will be:
  //  f faces
  //  p points
  //  maxVertex ... the 
  while ( (ch=fgetc(infile)) != EOF) {
    if (ch == '#')
      while ( (ch=fgetc(infile)) != '\n') ;
    else if (ch == 'V') {
      fscanf(infile, "ertex  %d %lf %lf %lf\n", &index, &c0, &c1, &c2);
      if (index > maxVertex) 
	maxVertex = index;
      p++;
    }
    else if (ch == 'F') {
      fscanf(infile, "ace %d  %d %d %d\n", &index, &p0, &p1, &p2);
      f++;
    }
    else  //should not be here but eliminate everything anyway
      while ( (ch=fgetc(infile)) != '\n') ;
  }

  int *point_remap = new int[maxVertex+1];
  for (i=0; i<maxVertex+1; i++)
    point_remap[i] = -1;
  m_face.resize(f);
  m_vertex.resize(p);

  p=0;
  f=0;
  
  fclose(infile);
  infile = fopen(filename, "rb");
  while ( (ch=fgetc(infile)) != EOF) {
    if (ch == '#')
      while ( (ch=fgetc(infile)) != '\n') ;
    else if (ch == 'V') {
      fscanf(infile, "ertex  %d %lf %lf %lf\n", &index, &c0, &c1, &c2);
      m_vertex[p] = Point3D(c0, c1, c2);
      point_remap[index] = p;
      p++;
    }
    else if (ch == 'F') {
      fscanf(infile, "ace %d  %d %d %d\n", &index, &p0, &p1, &p2);
      m_face[f].addPoint(point_remap[p0], m_vertex[point_remap[p0]]);
      m_face[f].addPoint(point_remap[p1], m_vertex[point_remap[p1]]);
      m_face[f].addPoint(point_remap[p2], m_vertex[point_remap[p2]]);
      f++;
    }
    else  //should not be here but eliminate everything anyway
      while ( (ch=fgetc(infile)) != '\n') ;
  }
  delete []point_remap;
}

void Mesh::normalize()
{
  int i, j;
	
  //divide by max scale instead of avg scale
	
  //first center
  Point3D total(0,0,0);
  //  Point3D scale(0,0,0);
  Point3D max_value = m_vertex[0];
  Point3D min_value = m_vertex[0];
	
  for (i=0; i<m_vertex.size(); i++) {
    total += m_vertex[i];
    //scale += pow(m_vertex[i], 2.0);
		
    for (j=0; j<3; j++) {
      if (m_vertex[i][j] > max_value[j])
				max_value[j] = m_vertex[i][j];
      if (m_vertex[i][j] < min_value[j])
				min_value[j] = m_vertex[i][j];
    }
  }
	
  //fprintf(stderr, "MAXSCALE: %lf\n", max_scale);
  total /= double(m_vertex.size());
  //scale = sqrt(scale/double(m_vertex.size()) - pow(total, 2.0));
	
  double max_width = 0;
  for (j=0; j<3; j++) {
    if (max_value[j]-min_value[j] > max_width)
      max_width = max_value[j]-min_value[j];
  }

	cout << "max_width is: " << max_width << endl;
	
  Matrix T = identityMatrix(4,4);
  T(0,3) = -total[0];
  T(1,3) = -total[1];
  T(2,3) = -total[2];
	
  applyTransform(T);
	
  Matrix S = identityMatrix(4,4);
  S(0,0) = 2./max_width;
  S(1,1) = 2./max_width;
  S(2,2) = 2./max_width;
	
  applyTransform(S);
	
  //then scale
}

Point3D Mesh::getCenter()
{
  Point3D total(0,0,0);
	
  for (int i=0; i<m_vertex.size(); i++) 
    total += m_vertex[i];

  total /= double(m_vertex.size());
  return total;
}

double Mesh::getMaximumLength()
{
  int i,j;

  if (m_vertex.size() < 2) {
    cerr << "Not enough points to compute maximum length\n";
  }  

	vector<double> min_value(3);
	vector<double> max_value(3);

	for (j=0; j<3; j++)
		min_value[j] = max_value[j] = m_vertex[0][j];
	
	for (i=1; i<m_vertex.size(); i++) {
    for (j=0; j<3; j++) {
      if (m_vertex[i][j] > max_value[j])
				max_value[j] = m_vertex[i][j];
      if (m_vertex[i][j] < min_value[j])
				min_value[j] = m_vertex[i][j];
    }
  }

	for (j=0; j<3; j++)
		max_value[j] -= min_value[j];

  return (double) (*max_element(max_value.begin(), max_value.end()));
}	

void Mesh::calcNormals()
{
  const double COS_MAX_ANGLE = cos(m_creaseAngle/360.0*6.28);
  int faceId, listId, currId, nextId;
  double mx, my, mz;
	list<int>::iterator fIt;

	m_normal.resize(m_vertex.size());
  Vector3D normal, currNormal, faceNormal, totalNormal;
  
  vector<int> pointList;	// list of vertices per face
	
  //first calculate the face normals
  for (faceId=0; faceId<m_face.size(); faceId++) {
    mx = my = mz = 0.0;
    pointList = m_face[faceId].getVertexIds();
    for (listId=0; listId<pointList.size(); listId++) {
      currId = pointList[listId];
      nextId = pointList[((listId+1)%pointList.size())];
			
      mx += (m_vertex[currId][1]-m_vertex[nextId][1])*(m_vertex[currId][2]+m_vertex[nextId][2]);
      my += (m_vertex[currId][2]-m_vertex[nextId][2])*(m_vertex[currId][0]+m_vertex[nextId][0]);
      mz += (m_vertex[currId][0]-m_vertex[nextId][0])*(m_vertex[currId][1]+m_vertex[nextId][1]);
    }
		
    if (m_ccw == true) {
      normal = Vector3D(mx, my, mz);
    }
    else {
      normal = Vector3D(-mx, -my, -mz);
    }
		
    normal /= normL2(normal);
    
    //normal[0] *= -1;
    //normal[1] *= -1;
    //normal[2] *= -1;
		
    m_face[faceId].setFaceNormal(normal);
  }
	
  int count;
	
  for (faceId=0; faceId<m_face.size(); faceId++) {
    pointList = m_face[faceId].getVertexIds();
    faceNormal = m_face[faceId].getFaceNormal();
    for (listId=0; listId<pointList.size(); listId++) {
      totalNormal = Vector3D(0, 0, 0);
      currId=pointList[listId];
      count = 0;
      for (fIt=m_adjFace[currId].begin(); fIt!=m_adjFace[currId].end(); fIt++) {
				currNormal = m_face[*fIt].getFaceNormal();
				
				//compare normal
				if (dotProduct(currNormal, faceNormal) >= COS_MAX_ANGLE) {
					totalNormal += currNormal;
					count++;
				}
      }
      totalNormal /= normL2(totalNormal);
			
      if (count == 0)
				m_face[faceId].addVertexNormal(currId,faceNormal);
      else {
				m_face[faceId].addVertexNormal(currId,totalNormal);
      }
    }
  }

	//now individual normals
	for (currId = 0; currId < m_normal.size(); currId++) {
		totalNormal = 0;
     for (fIt=m_adjFace[currId].begin(); fIt!=m_adjFace[currId].end(); fIt++)
			totalNormal += computeArea(m_face[*fIt])*m_face[*fIt].getFaceNormal();
		totalNormal /= normL2(totalNormal);
		m_normal[currId] = totalNormal;
	}
}

void Mesh::modifyVertices(vector<Point3D> &pts)
{	
  if (m_vertex.size() != pts.size())
    m_vertex.clear();
  m_vertex = pts;

  for (int f=0; f<m_face.size(); f++)
    m_face[f].modifyPoints(pts);

	calcNormals();
}

vector<Face> Mesh::getFaces()
{
  return m_face;
}

vector<Point3D> Mesh::getVertices()
{
  return m_vertex;
}

void Mesh::setCreaseAngle(float angle)
{
  m_creaseAngle = angle;
}

void Mesh::setCCW(bool ccw)
{
  m_ccw = ccw;
}

void Mesh::setNormalPerVertex(bool npv)
{
  m_normalPerVertex = npv;
}

void Mesh::merge(const Mesh &rhs)
{
  if (m_face.size() == 0 && rhs.m_face.size() == 0) {
    // do nothing, both empty
  }
  else if (m_face.size() == 0) {
    //just copy right over
    (*this) = rhs;
  }
  else if (rhs.m_face.size() == 0) {
    //do nothing
  }
  else {
    Mesh endPiece = rhs;
    endPiece.incrementCounters(m_vertex.size());
    m_face.insert(m_face.end(), endPiece.m_face.begin(), endPiece.m_face.end());
    m_vertex.insert(m_vertex.end(), endPiece.m_vertex.begin(), endPiece.m_vertex.end());
  }
	
	//fix ... don't need to compute normals twice
	initialize();
}

void Mesh::incrementCounters(int amount)
{
  for (int i=0; i<m_face.size(); i++) 
    m_face[i].incrementCounters(amount);
}

void Mesh::applyTransform(Matrix &M)
{
  size_t i;
	
  for (i=0; i<m_face.size(); i++)
    m_face[i].applyTransform(M);
	
	
  Point3DH p, q;
  
  for (i=0; i<m_vertex.size(); i++) {
    p = m_vertex[i];
    q = M*p;
    m_vertex[i] = q;
  }

  //need to fix points inside of endPiece
}

void Mesh::setTextureMap(ColorImage &src)
{
  ColorImage I2 = resizeImage(src, 512, 512);
  if (m_image_data != NULL)
    delete []m_image_data;
  m_image_rows = I2.getRowSize();
  m_image_cols = I2.getColSize();
  m_image_size = m_image_rows*m_image_cols*3;
  m_image_data = new unsigned char[m_image_size];

  int r, c, d;

  int count = 0;
  for (r=0; r<m_image_rows; r++)
    for (c=0; c<m_image_cols; c++) 
      for (d=0; d<3; d++)
	m_image_data[count++] = I2[r][c][d];

  m_isTextured = true;
}

void Mesh::addTextureCoordinate(int fID, int pID, float u, float v)
{
  m_face[fID].addTextureCoordinate(pID, u, v);
}

list<Edge> Mesh::extractEdges()
{
  int faceId, listId, listId1;
	
  list<Edge> edges;

  for (faceId=0; faceId < m_face.size(); faceId++) {
    vector<int> pointList = m_face[faceId].getVertexIds();
    for (listId=0; listId< pointList.size(); listId++) {
      listId1 = listId+1;
      if (listId1 >= pointList.size())
	listId1 = 0;
      Edge e(pointList[listId], pointList[listId1]);
      edges.push_back(e);
    }
  }
  edges.sort();
  edges.unique();
	
  return edges;
}

vector<Vector3D> Mesh::getVertexNormals()
{
	return m_normal;
}

void Mesh::smooth(int num_iterations, double lambda, double mu_diff)
{
  double mu = -(lambda+mu_diff);

  Vector3D dv;
  list<int>::iterator p;
  int j;
  vector<Point3D> points2(m_vertex.size());

  // iterative loop: 
  for (size_t i=0; i<num_iterations; i++) {
    for (j=0; j<m_vertex.size(); j++) {
      dv = Vector3D(0,0,0);
      for (p=m_adjVertex[j].begin(); p!=m_adjVertex[j].end(); p++)
				dv += m_vertex[*p]-m_vertex[j];
      dv /= double(m_adjVertex[j].size());
      points2[j] = m_vertex[j]+lambda*dv;
    }

    for (j=0; j<m_vertex.size(); j++) {
      dv = Vector3D(0,0,0);
      for (p=m_adjVertex[j].begin(); p!=m_adjVertex[j].end(); p++)
				dv += points2[*p]-points2[j];
      dv /= double(m_adjVertex[j].size());
      m_vertex[j] = points2[j]+mu*dv;
    }
  }

  for (int f=0; f<m_face.size(); f++)
    m_face[f].modifyPoints(m_vertex);

  calcNormals();
}

class FaceOrientationSet{
public:
	FaceOrientationSet(int fId, Edge e) {
		m_faceId = fId;
		m_edge = e;
	}
	FaceOrientationSet() {
		m_faceId = -1;
	}
	int m_faceId;
	Edge m_edge;

};

void Mesh::fixOrientation()
{
  vector<bool> faceVisited(m_face.size(), false);
	
	FaceOrientationSet fset;
  int faceId;
  int id0, id1;
	int i;
  int pid;
	list<int>::iterator p;
	Edge e;
  vector<int> pointIds;
	queue<FaceOrientationSet> faceQueue;
	
	int currFace = 0;
	while (currFace < m_face.size()) { 
		
		pointIds.clear();pointIds = m_face[currFace].getVertexIds();
		faceQueue.push(FaceOrientationSet(currFace, Edge(pointIds[0], pointIds[1])));
		
		while (!faceQueue.empty()) {
			
			//first retrieve face/edge pair
			fset = faceQueue.front(); faceQueue.pop();
			faceId = fset.m_faceId;
			e = fset.m_edge;
			
			if (faceVisited[faceId] == false) {
				
				faceVisited[faceId] = true;
				pointIds.clear(); pointIds = m_face[faceId].getVertexIds();
				
				i = 0; 
				while (pointIds[i] != e.m_pt0 && i<pointIds.size()) i++;
				if (i >= pointIds.size() || pointIds[i] != e.m_pt0)
					cerr << "Didn't find point\n";
				
				id0 = pointIds[i];
				id1 = pointIds[(i+1)%pointIds.size()];
				
				if (id1 != e.m_pt1) 
					m_face[faceId].flipOrientation();
				
				//repeated in case stuff flipped
				pointIds = m_face[faceId].getVertexIds();
				
				for (i=0; i<pointIds.size(); i++) {
					id0 = pointIds[i];
					id1 = pointIds[(i+1)%pointIds.size()];
					
					list<int> set0 = m_adjFace[id0];
					list<int> set1 = m_adjFace[id1];
					set0.sort();
					set1.sort();
					set0.merge(set1);
					
					for (pid = -1, p=set0.begin(); p!=set0.end(); p++) {
						if (pid == *p) {
							if (faceVisited[pid] == false)  //new one
								faceQueue.push(FaceOrientationSet(pid, Edge(id1, id0)));
						}
						pid = *p;
					}
				}
			}
		}

		cout << "One component\n";
		
		while (currFace < m_face.size() && faceVisited[currFace] == true)
			currFace++;
		
	}
}

/* ---- bad attempt to compute curvature,
   keeping around on the assumption it might help me
	 to fit surfaces later on.



vector<double> fitPlane(vector<Point3D> &pts)
{
	Matrix A(pts.size(), 3);
	valarray<double> b(pts.size());

	for (int i=0; i<pts.size(); i++) {
		A(i,0) = pts[i][0];
		A(i,1) = pts[i][1];
		A(i,2) = 1.0;
		b[i] = -pts[i][2];
	}

	valarray<double> d = A/b;

	vector<double> d2(4);
	d2[0] = d[0];
	d2[1] = d[1];
	d2[2] = 1;
	d2[3] = d[2];

	return d2;
}


//why not just fit to the SVD based eigenvector space?
vector<Vector3D> findDominantVectors(vector<Point3D> &points, Vector3D n)
{
	int i;
	Matrix A(3, points.size()-1);

	for (i=0; i<points.size()-1; i++) {
		Vector3D u = points[i]-points[i+1];
		u = u - dot(u,n)*n;
		A(0,i) = u[0];
		A(1,i) = u[1];
		A(2,i) = u[2];
	}
	Matrix U, VT;
	valarray<double> D;
	A.svd(U, D, VT);

	vector<Vector3D> dom;
	dom.push_back(U.column(0));
	dom.push_back(U.column(1));
	dom.push_back(U.column(2));
	return dom;
}

double computeHeightFromPlane(Point3D &p, vector<double> &S, Vector3D &n)
{
	double h = (S[0]*p[0]+S[1]*p[1]+S[2]*p[2]+S[3])/
		(S[0]*n[0]+S[1]*n[1]+S[2]*n[2]);
	return h;
}

Point2D computeCoordinatesOnPlane(Point3D p, Point3D &center, Vector3D &u, Vector3D &v)
{
	Matrix A(3, 2);
	valarray<double> b(3);
	for (int i=0; i<3; i++) {
		A(i,0) = u[i];
		A(i,1) = v[i];
		b[i] = p[i]-center[i];
	}
	valarray<double> x = A/b;
	return Point2D(x[0], x[1]);
}

double computeTheMeanCurvature(vector<Point3D> &points, Point3D point, ofstream &outfile)
{
	if (points.size() < 6) {
		cout << "Not enough terms: " << points.size() << "\n";
		return 0;
	}

	//first compute plane
	vector<double> plane = fitPlane(points);
	Vector3D n = Vector3D(plane[0], plane[1], plane[2]); // the normal
	n = n/normL2(n);
	vector<Vector3D> doms = findDominantVectors(points, n);

	outfile << dot(doms[0], n) << ", " << dot(doms[1], n) << ", " << dot(doms[2], n) << endl;

	Vector3D u =doms[0] - n*dot(doms[0],n);
	u = u/normL2(u);
	Vector3D v = cross(n,u);

	double h = computeHeightFromPlane(point, plane, n);
	Point3D center = point;
	center += (-h)*n;

	Point3D ptOnPlane;
	Point2D proj;
	ptOnPlane = point;
	ptOnPlane += (-h)*n;
	proj = computeCoordinatesOnPlane(ptOnPlane, center, u, v);
			outfile << proj << endl;

	int i;
	Matrix A(points.size(), 6);
	valarray<double> b(points.size());

	double weight = 1.0;

	for (i=0; i<points.size(); i++) {
		h = computeHeightFromPlane(points[i], plane, n);
		ptOnPlane = points[i];
		ptOnPlane += (-h)*n;
		proj = computeCoordinatesOnPlane(ptOnPlane, center, u, v);

		double dist = normL2(points[i]-point);
		weight = 1.00;//0.25-dist;

		outfile << proj << " ---> " << dist << endl;

		A(i,0) = weight*pow(proj[0], 2.0);
		A(i,1) = weight*pow(proj[1], 2.0);
		A(i,2) = weight*proj[0]*proj[1];	
		A(i,3) = weight*proj[0];
		A(i,4) = weight*proj[1];
		A(i,5) = weight*0.1;
		b[i] = weight*h;
	}
	valarray<double> x = A/b;
	x /= x[5];

	double a0, a1, a2, a3, a4, a5;
	a0 = weight*pow(proj[0], 2.0);
	a1 = weight*pow(proj[1], 2.0);
	a2 = weight*proj[0]*proj[1];	
	a3 = weight*proj[0];
	a4 = weight*proj[1];
	a5 = weight*0.1;
	outfile << "plane is: " << plane[0] << ", " << plane[1] << ", " << plane[2] << ", " << plane[3] << endl;
	outfile << "surface is: " << x << endl;

	//now check array
	for (i=0; i<points.size(); i++) {

		double weight = 1.0;
		double u0 = proj[0];
		double v0 = proj[1];

		h = computeHeightFromPlane(points[i], plane, n);
		ptOnPlane = points[i];
		ptOnPlane += (-h)*n;
		proj = computeCoordinatesOnPlane(ptOnPlane, center, u, v);

		double h2 = a0*u0*u0+a1*v0*v0+a2*u0*v0+a3*u0+a4*v0+a5;
		outfile << h - h2 << endl;
	}
	
	//fit a quadratic and compute derivatives
	double fu = x[3];//d;
	double fv = x[4];//e;
	double fuu = 2*x[0];//2*a;
	double fvv = 2*x[1];//2*b;
	double fuv = x[2];//c;

	double den = 1 + pow(fu, 2.0) + pow(fv, 2.0);

	double K = (fuu*fvv - pow(fuv, 2.0)) / pow(den, 2.0);
	double H = 0.5*(fuu + fvv + fuu*pow(fv,2.0) + fvv*pow(fv,2.0) - 2*fu*fv*fuv)/
		pow(den, 1.5);

	return K;
}
*/

/*!  \brief it computes gaussian and mean curvature for a given point.

		It builds a list of all points in the immediate 2 deep region.
		Then it sends the points and itself to the curvature computation routine.

		I am not sure how well it is going to work where the points are overly
		curved ... when it bends over itself basically ... oh well.

*/
void Mesh::setAdjacentVertices()
{
	m_adjVertex.clear();
	m_adjVertex.resize(m_vertex.size());
	list<Edge> edges = extractEdges();

	for (list<Edge>::iterator eIt=edges.begin(); eIt!=edges.end(); eIt++) {
		int e0 = eIt->m_pt0;
		int e1 = eIt->m_pt1;
		m_adjVertex[e0].push_back(e1);
		m_adjVertex[e1].push_back(e0);
	}
}

void Mesh::setAdjacentFaces()
{
  int i,j;
	m_adjFace.clear();
  m_adjFace.resize(m_vertex.size());
  for (i=0; i<m_face.size(); i++) {
    vector<int> pointids = m_face[i].getVertexIds();
    for (j=0; j<pointids.size(); j++)
      m_adjFace[pointids[j]].push_back(i);
  }
}

bool isVertexInFace(int vertexId, Face &f)
{
	int i=0;
	vector<int> ids = f.getVertexIds();

	while (i<ids.size() && ids[i] != vertexId)
		i++;
	
	if (i < ids.size())
		return true;

	return false;
}	

/* zero transform with Givens rotation

	double t, theta, mv11, mv22;
		theta = (Mvi2(2,2)-Mvi2(1,1))/(2*Mvi2(1,2));
		if (theta < 0) 
			t = -1/(fabs(theta)+sqrt(theta*theta+1));
		else
			t = +1/(fabs(theta)+sqrt(theta*theta+1));

		cout << "t is " << t << endl;

		mv11 = Mvi2(1,1)-t*Mvi2(1,2);
		mv22 = Mvi2(2,2)+t*Mvi2(1,2);
		*/


/*! computes the mean curvature (and very easy to modify to 
    compute gaussian curvature.

	  Uses method from Taubin's paper ... estimating the
		tensor of curvature of a surface from a polyhedral
		approximation.

	Use the ICCV 95 which has the correct Eqn 7 ... divide
	by square of norm.
*/
vector<vector<double> > Mesh::computePrincipalCurvatures()
{
	vector<vector<double> > h(m_vertex.size());
	list<int>::iterator p, q;

	Point3D vi, vj;
	Vector3D ni;
	double kij;
	Vector3D Tij;
	Matrix Mvi(3,3);
	Vector3D E1;
	Vector3D Wvi;
	Matrix Qvi(3,3);
	Matrix Mvi2(3,3);

	double mv11, mv22;
	double kp1, kp2; // principal curvatures
	double wij; // weights
	double totalwij;

	for (int i=0; i<m_vertex.size(); i++) {

		//first compute vertex normal using weights
		ni = 0;
		for (p=getAdjacentFaces(i)->begin(); p!=getAdjacentFaces(i)->end(); p++) 
			ni += computeArea(m_face[*p])*m_face[*p].getFaceNormal();
		ni /= normL2(ni);

		//now compute Mvi
		vi = m_vertex[i];
		Mvi = 0;
		totalwij = 0;

		for (p=getAdjacentVertices(i)->begin(); p!=getAdjacentVertices(i)->end(); p++ ) {

			//find weight
			wij = 0.0;
			for (q=getAdjacentFaces(i)->begin(); q!=getAdjacentFaces(i)->end(); q++) 
				if (isVertexInFace((int)(*p), m_face[*q]) == true) 
					wij += computeArea(m_face[*q]);
			totalwij += wij;
			
			vj = m_vertex[*p];
			kij = 2*dot(ni, vj-vi)/pow(normL2(vj-vi),2.0);
			Tij = (vi-vj)- ni*dot(vi-vj,ni);
			Tij /= normL2(Tij);
			Mvi += wij*kij*outerProduct(Tij, Tij);
		}

		Mvi /= totalwij;

		//now align Mvi with householder
		E1 = Vector3D(1,0,0);
		if (normL2(E1-ni) > normL2(E1+ni))
			Wvi = (E1-ni);
		else
			Wvi = (E1+ni);
		Wvi /= normL2(Wvi);

		Qvi = identityMatrix(3,3);
		Qvi -= 2*outerProduct(Wvi, Wvi);
		Mvi2 = transpose(Qvi)*Mvi*Qvi;

		//now must zero transform
		Matrix M2(2,2);
		M2(0,0) = Mvi2(1,1);
		M2(1,1) = Mvi2(2,2);
		M2(0,1) = M2(1,0) = Mvi2(1,2);

		valarray<double> eig = symmetric_eigen(M2);
		//cout << "Eig is: " << eig << endl;

		mv11 = eig[0];
		mv22 = eig[1];

		kp1 = 3*mv11-mv22;
		kp2 = 3*mv22-mv11;

		h[i].push_back(kp1);
		h[i].push_back(kp2);
	}
	return h;
}

vector<double> Mesh::computeGaussianCurvature()
{
	vector<vector<double> > kp = computePrincipalCurvatures();
	vector<double> k(m_vertex.size());
	for (int i=0; i<m_vertex.size(); i++) 
		k[i] = kp[i][0]*kp[i][1];
	return k;
}

vector<double> Mesh::computeMeanCurvature()
{
	vector<vector<double> > kp = computePrincipalCurvatures();
	vector<double> h(m_vertex.size());
	for (int i=0; i<m_vertex.size(); i++) 
		h[i] = 0.5*(kp[i][0]+kp[i][1]);
	return h;
}

void Mesh::examine()
{
	map<int, int> count;

	for (int i=0; i<m_face.size(); i++) {
		int num = m_face[i].getSize();
		if (count.find(num) == count.end())
			count[num] = 1;
		else
			count[num] += 1;
	}

	for (map<int,int>::iterator p = count.begin(); p!=count.end(); p++) {
		cout << "There are " << (*p).second << " faces with " << (*p).first << " points\n";
	}

	cout << "There are a total of " << m_face.size() << " faces\n";
}

void Mesh::save(char *filename)
{
	int i;
	FILE *fp = fopen(filename, "w");
	for (i=0; i<m_vertex.size(); i++) 
		fprintf(fp, "Vertex %d %lf %lf %lf\n", i, m_vertex[i][0], m_vertex[i][1], m_vertex[i][2]);

	for (i=0; i<m_face.size(); i++) {
		vector<int> fids = m_face[i].getVertexIds();
		fprintf(fp, "Face %d %d %d %d\n", i, fids[0], fids[1], fids[2]);
	}
	fclose(fp);
}

