//----------------------------------------------------------------
// Bugs:
// 1. setMaxItems: maxItems setting is not propagated to child nodes

// Notes:

// Questions:
// 1. is list.size O(1) or O(n)
//----------------------------------------------------------------

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <cmath>
#include <algorithm>

#include "matrix.hpp"

#define __DEBUG__
//#define __VERBOSE__

#define OCTREENODE_MAX_ITEMS 100
#define TAB "  "
#define OCTREE_MAX_INTEREST 0.5

using namespace std;

// forward reference for friend qualifiers
template <typename T>
class octree;

template <typename T>
struct PointType
{
  T x;
  T y;
  T z;

  PointType<T>(): x(0), y(0), z(0){};
  PointType<T>(T _x, T _y, T _z) : x(_x), y(_y), z(_z){};
  
};

template <typename T>
class octreeNode
{

public:
  
  octreeNode(vector<PointType<T> > &);
  octreeNode(vector<T> &);
  octreeNode(vector<PointType<T> > &, list<vector<T> >&);
  ~octreeNode();

  bool isLeaf();

  void setMaxItems(int);
  void dumpItems();

  int getNumItems();
  int size();
  
  vector<T> getCentroid();

protected:

  // Data
  vector<PointType<T> > Bounds;
  
  list<vector<T> > Items;

  vector<T> Centroid;
  vector<T> Stdev;

  vector<octreeNode*> Children;

  int maxItems;            // maximum number of items contained in the node
  int numItems;            // number of items in the node (subtree if not leaf)
 
  octreeNode<T> *nextLeaf;
  octreeNode<T> *prevLeaf;

  // Functions
  void Split();

  bool isIn(vector<T> &);
  bool empty();

  void computeCentroid();
  void computeStdev();

  T getClosestFaceDistance(vector<T>&);
  T getNearestItem(vector<T> &, vector<T> &);
  T distance(vector<T>&, vector<T> &);

  friend void addItem<T>(octreeNode<T> *&, vector<T> &); 
  friend void deleteNode<T>(octreeNode<T> *&);
  friend void displayNode<T>(octreeNode<T> *&, string);
  friend void getNearestNodeList<T>(octreeNode<T> *&, vector<T> &, 
				    vector<octreeNode<T>*>&, T &);
  
  friend void translate<T>(octreeNode<T> *&, vector<T>&);
  friend void rotate<T>(octreeNode<T> *&, vector<T> &, Matrix &);
  friend void rotate<T>(octreeNode<T> *&, Matrix &);
  friend void buildLeafList<T>(octreeNode<T> *&, vector<octreeNode<T>*>&);
  friend void setMaxInterest<T>(octreeNode<T> *&, T);
  friend void render<T>(octreeNode<T> *&);
  
  friend vector<T> computeCentroid<T>(octreeNode<T> *&, int &);

  friend int depth<T>(octreeNode<T> *&);
  friend int countNodes<T>(octreeNode<T> *&);
  friend int countItems<T>(octreeNode<T> *&);

  friend octreeNode<T> *getFirstLeaf<T>(octreeNode<T> *&);

  friend class octree<T>;

  friend ostream &operator<<<T>(ostream &o, PointType<T> &p);
  friend ostream &operator<<<T>(ostream &o, octreeNode<T> &);

};

template <typename T>
class octree
{
  
public:
  
  octree();
  octree(octreeNode<T> *);
  ~octree();

  void makeRoot(vector<T> &);
  void addItem(vector<T> &);
  void setMaxItems(int);
  void displayInfo();
  void gotoStart();
  //  void goToEnd();
  void translate(vector<T>&);
  void rotate(vector<T>&);
  void rotate(Matrix &);
  void setMaxInterest(T);
  void render();

  bool getNearestItem(vector<T>&, vector<T> &);

  bool getNextItem(vector<T> &);
  bool getNextItem(vector<T> &, int);
  bool getNextLeafStats(vector<T> &, vector<T> &, int &);
  //  vector<T>getPrevItem();

  octreeNode<T> *getFirstLeaf();

  void finalize();

  int size();

private:
  
  octreeNode<T> *root;

  octreeNode<T> *firstLeaf;
  octreeNode<T> *lastLeaf;  

  octreeNode<T> *currentLeaf;

  typename list<vector<T> >::iterator currentItem;

  T maxInterest;

};

//----------------------------------------------------------------------------

template <typename T>
void addItem(octreeNode<T> *&, vector<T> &);

template <typename T>
void deleteNode(octreeNode<T> *&);

template <typename T>
void displayNode(octreeNode<T> *&, string);

template <typename T>
int depth(octreeNode<T> *&);

template <typename T>
int countNodes(octreeNode<T> *&);

template <typename T>
int countItems(octreeNode<T> *&);

template <typename T>
octreeNode<T> *getFirstLeaf(octreeNode<T> *&);

template <typename T>
void translate(octreeNode<T> *&, vector<T> &);

template <typename T>
void rotate(octreeNode<T> *&, vector<T> &, Matrix &);

template <typename T>
void rotate(octreeNode<T> *&, Matrix &);

template <typename T>
vector<T> computeCentroid(octreeNode<T> *&, int &);

template <typename T>
void setMaxInterest(octreeNode<T> *&, T);

template <typename T>
void render(octreeNode<T> *&);

template <typename T>
ostream &operator<<(ostream &, octreeNode<T> &);

template <typename T>
ostream &operator<<(ostream &o, vector<T> &t);

template <typename T>
ostream &operator<<(ostream &o, vector<vector<T> >&t);

template <typename T>
ostream &operator<<(ostream &o, PointType<T> &p);

//----------------------------------------------------------------------------

template <typename T>
octree<T>::octree()
{
  root = NULL;
  firstLeaf = NULL;
  lastLeaf = NULL;
  currentLeaf = NULL;
  maxInterest = OCTREE_MAX_INTEREST;
}

template <typename T>
octree<T>::octree(octreeNode<T> *newNode)
{
  root = newNode;
  firstLeaf = root;
  lastLeaf = root;
  currentLeaf = root;
  maxInterest = OCTREE_MAX_INTEREST;
}

template <typename T>
octree<T>::~octree()
{
  delete root;
}

template <typename T>
void octree<T>::makeRoot(vector<T> &B)
{
  root = new octreeNode<T>(B);
  firstLeaf = root;
  lastLeaf = root;
  currentLeaf = root;
}

template <typename T>
void octree<T>::addItem(vector<T> &newItem)
{
 ::addItem(root,newItem);
}

template <typename T>
void octree<T>::setMaxItems(int m)
{
  root -> setMaxItems(m);
}

template <typename T>
octreeNode<T> *octree<T>::getFirstLeaf()
{
  return ::getFirstLeaf(root);
}

template <typename T>
void octree<T>::gotoStart()
{
  currentLeaf = firstLeaf; 
  currentItem = firstLeaf->Items.begin();
}

template <typename T>
inline bool octree<T>::getNextItem(vector<T> &I)
{

  bool quit = false;

  // if not at the end of an item list within a node...
  if (currentItem!=currentLeaf->Items.end())
    {
      I = *currentItem;
      currentItem++;
      return true;
    }
  else
    {
      currentLeaf = currentLeaf -> nextLeaf;
      if (currentLeaf)
	{
	  currentItem = currentLeaf->Items.begin();
	  I = *currentItem;
	  currentItem++;
	  return true;	  
	}
      else  // no more items
	return false;

    }

}

template <typename T>
inline bool octree<T>::getNextItem(vector<T> &I, int skip)
{

  int i;


  // test for end of leaf
  if (currentItem == currentLeaf->Items.end())
    currentLeaf = currentLeaf -> nextLeaf;

  // test of end of items
  if (!currentLeaf)
    return false; 

  // continue counting from current item
  for (i=0;i<skip;i++)
    {
      currentItem++;
      
      // check for end of leaf node
      if (currentItem == currentLeaf -> Items.end())
	{
	  // Continue in next node      
	  currentLeaf = currentLeaf -> nextLeaf;

	  if (currentLeaf)
	    currentItem = currentLeaf -> Items.begin();	  
	  else
	    return false;   // no more items
	}
    }

  I = *currentItem;
  return true;

}

template <typename T>
inline bool octree<T>::getNextLeafStats(vector<T> &c, vector<T> &s, int &n)
{

  if (currentLeaf == NULL)
    return false;

  c = currentLeaf -> Centroid;
  s = currentLeaf -> Stdev;
  currentLeaf = currentLeaf -> nextLeaf;
  return true;
	
}

template <typename T>
void octree<T>::translate(vector<T> &t)
{
  ::translate(root, t);
}

template <typename T>
void octree<T>::rotate(vector<T> &r)
{

  Matrix alpha(3,3);   // Concatentate matrices after debugging
  Matrix beta(3,3);
  Matrix gamma(3,3);
  Matrix R(3,3);

  double a = (r[ALPHA] * superPI)/180.0;
  double b = (r[BETA] * superPI)/180.0;
  double g = (r[GAMMA] * superPI)/180.0;

 
  // Construct rotation matrices
  // Alpha is rotation around X
  alpha[0][0] = 1;
  alpha[0][1] = 0;
  alpha[0][2] = 0;
  alpha[1][0] = 0;
  alpha[1][1] = cos(a);
  alpha[1][2] = -sin(a);
  alpha[2][0] = 0;
  alpha[2][1] = sin(a);
  alpha[2][2] = cos(a); 

  // Beta is rotation around Y
  beta[0][0] = cos(b);
  beta[0][1] = 0;
  beta[0][2] = sin(b);
  beta[1][0] = 0;
  beta[1][1] = 1;
  beta[1][2] = 0;
  beta[2][0] = -sin(b);
  beta[2][1] = 0;
  beta[2][2] = cos(b); 

  // Gamma is rotation around Z
  gamma[0][0] = cos(g);
  gamma[0][1] = -sin(g);
  gamma[0][2] = 0;
  gamma[1][0] = sin(g);
  gamma[1][1] = cos(g);
  gamma[1][2] = 0;
  gamma[2][0] = 0;
  gamma[2][1] = 0;
  gamma[2][2] = 1;

  R = alpha * beta * gamma;

  ::rotate(root, root -> Centroid, R);

}

template <typename T>
void octree<T>::rotate(Matrix &R)
{ 
  ::rotate(root,R);
}

template <typename T>
bool octree<T>::getNearestItem(vector<T> &query, vector<T> &nearest)
{

  int i = 0;
  int j = 0;

  bool found = false;

  vector<T> t(3,0);

  T minD = maxInterest;
  T d = 0.0;
   
  vector<octreeNode<T>*> nearestNodeList;
  
  octreeNode<T> *p;


  if (!root -> isLeaf())
    getNearestNodeList(root, query, nearestNodeList, maxInterest);
  else
    nearestNodeList.push_back(root);

  // Search the nodes for the nearest item
  if (!nearestNodeList.empty())
    {
      for (i=0;i<nearestNodeList.size();i++)
	{

	  p = nearestNodeList[i];	  
	  d = p -> getNearestItem(query,t);
	  if (d < minD)
	    {
	      minD = d;
	      nearest = t;
	      found = true;
	    }

	}
    }

  return found;

}

template <typename T>
inline int octree<T>::size()
{
  return countItems(root);
}

template <typename T>
void octree<T>::displayInfo()
{

#ifdef __VERBOSE__
  displayNode(root, "");
#endif

  cout << "Tree has " << countNodes(root) << " nodes." << endl;
  cout << "Tree has depth " << depth(root) << "." << endl;
  cout << "Tree has " << countItems(root) << " items." << endl;
  cout << "Centroid: " << root -> Centroid << endl;
}

template <typename T>
void octree<T>::finalize()
{
  octreeNode<T> *p = NULL;

  int itemCount = 0;
  int nodeCount = 0;
  int n = 0;
  int i = 0;


  vector<octreeNode<T>* > leafList;

  // First construct a vector of non-empty leaves
  ::buildLeafList(root,leafList);

  n = leafList.size();

  // Setup the leaf chain
  firstLeaf = leafList[0];
  firstLeaf -> prevLeaf = NULL;
  lastLeaf = leafList[n-1];
  lastLeaf -> nextLeaf = NULL;

  // Setup forward links
  for (i=0;i<(n-1);i++)
    leafList[i] -> nextLeaf = leafList[i+1];

  // Setup backward links
  for (i=1;i<n;i++)
    leafList[i] -> prevLeaf = leafList[i-1];
 
  // compute leaf centroids 
  computeCentroid(root,n);
  
  // Standard devs are only used for rendering,
  // so perform only on leaf nodes
  p = firstLeaf;
  while (p)
    {
      p -> computeStdev();
      p = p -> nextLeaf;      
    }

  //  displayNode(root,"");

}

template <typename T>
void octree<T>::setMaxInterest(T m)
{
  maxInterest = m;
}

template <typename T>
void octree<T>::render()
{
  ::render(root);
}

//----------------------------------------------------------------------------

template <typename T>
octreeNode<T>::octreeNode(vector<PointType<T> > &B)
{

  Bounds.reserve(8);

  Bounds = B;

  Children.clear();
  Items.clear();

  Centroid = vector<T>(3,0);
  Stdev = vector<T>(3,0);

  maxItems = OCTREENODE_MAX_ITEMS;

  numItems = 0;

  nextLeaf = NULL;
  prevLeaf = NULL;

}

template <typename T>
octreeNode<T>::octreeNode(vector<T> &B)
{

  T x1 = B[0];
  T x2 = B[1];
  T y1 = B[2];
  T y2 = B[3];
  T z1 = B[4];
  T z2 = B[5];

  Centroid = vector<T>(3,0);
  Stdev = vector<T>(3,0);

  Bounds.reserve(8);

  Bounds[0] = PointType<T>(x1,y1,z1);
  Bounds[1] = PointType<T>(x1,y1,z2);
  Bounds[2] = PointType<T>(x1,y2,z1);
  Bounds[3] = PointType<T>(x1,y2,z2);
  Bounds[4] = PointType<T>(x2,y1,z1);
  Bounds[5] = PointType<T>(x2,y1,z2);
  Bounds[6] = PointType<T>(x2,y2,z1);
  Bounds[7] = PointType<T>(x2,y2,z2);

  Children.clear();
  Items.clear();

  maxItems = OCTREENODE_MAX_ITEMS;

  numItems = 0;

  nextLeaf = NULL;
  prevLeaf = NULL;

}

template <typename T>
octreeNode<T>::octreeNode(vector<PointType<T> > &B, 
			  list<vector<T> > &oldList)
{

  Centroid = vector<T>(3,0);
  Stdev = vector<T>(3,0);

  Bounds.reserve(8);
  Bounds = B;

  typename list<vector<T> >::iterator p;
  typename list<vector<T> >::iterator q;


  Children.clear();
  Items.clear();
  maxItems = OCTREENODE_MAX_ITEMS;

  // Pick out items that belong in this node, then remove them from the parent's list
  for (p=oldList.begin();p!=oldList.end();)
    if (isIn(*p))
      {
	Items.push_back(*p);	
	p = oldList.erase(p);
      }
    else
      p++;

  numItems = Items.size();

  nextLeaf = NULL;
  prevLeaf = NULL;

}

template <typename T>
octreeNode<T>::~octreeNode()
{

  int i = 0;
  int numChildren = 8;

  // Recursively free child nodes
  if (!Children.empty())
    for (i=0;i<numChildren;i++)
      deleteNode(Children[i]);

}

template <typename T>
inline bool octreeNode<T>::isLeaf()
{
  return (Children.size() == 0);
}

template <typename T>
void octreeNode<T>::setMaxItems(int m)
{
  maxItems = m;
}

template <typename T>
inline int octreeNode<T>::getNumItems()
{
  return numItems;
}

template <typename T>
inline int octreeNode<T>::size()
{
  return numItems;
}

template <typename T>
inline bool octreeNode<T>::empty()
{
  return numItems == 0;
}

template <typename T>
void octreeNode<T>::Split()
{

  int i = 0;
  int j = 0;
  int k = 0;
  int l = 0;
	     
  int numChildren = 8;
  int numPoints = 8;

  short int xBit = 0;
  short int yBit = 0;
  short int zBit = 0;
 
  T xDimSize = 0;
  T yDimSize = 0;
  T zDimSize = 0;

  vector<PointType<T> > newBounds(8);

  xDimSize = fabs(Bounds[7].x - Bounds[0].x)/2;
  yDimSize = fabs(Bounds[7].y - Bounds[0].y)/2;
  zDimSize = fabs(Bounds[7].z - Bounds[0].z)/2;

  // Determine the new nodes (8 of them in this implementation)
  for (i=0;i<numChildren;i++)
    {

      xBit = i & 0x0004;
      yBit = i & 0x0002;
      zBit = i & 0x0001;

      if (!xBit)
	{	  
	  // Down
	  newBounds[0].x = Bounds[0].x;
	  newBounds[1].x = Bounds[0].x;
	  newBounds[2].x = Bounds[0].x;
	  newBounds[3].x = Bounds[0].x;
	  newBounds[4].x = Bounds[0].x + xDimSize;
	  newBounds[5].x = Bounds[0].x + xDimSize;
	  newBounds[6].x = Bounds[0].x + xDimSize;
	  newBounds[7].x = Bounds[0].x + xDimSize;
	}
      else
	{
	  // Up
	  newBounds[0].x = Bounds[0].x + xDimSize;
	  newBounds[1].x = Bounds[0].x + xDimSize;
	  newBounds[2].x = Bounds[0].x + xDimSize;
	  newBounds[3].x = Bounds[0].x + xDimSize;
	  newBounds[4].x = Bounds[7].x;
	  newBounds[5].x = Bounds[7].x;
	  newBounds[6].x = Bounds[7].x;
	  newBounds[7].x = Bounds[7].x;
	}

    if (!yBit)
	{	  
	  // Down
	  newBounds[0].y = Bounds[0].y;
	  newBounds[1].y = Bounds[0].y;
	  newBounds[4].y = Bounds[0].y;
	  newBounds[5].y = Bounds[0].y;
	  newBounds[2].y = Bounds[0].y + yDimSize;
	  newBounds[3].y = Bounds[0].y + yDimSize;
	  newBounds[6].y = Bounds[0].y + yDimSize;
	  newBounds[7].y = Bounds[0].y + yDimSize;
	}
      else
	{
	  // Up
	  newBounds[0].y = Bounds[0].y + yDimSize;
	  newBounds[1].y = Bounds[0].y + yDimSize;
	  newBounds[4].y = Bounds[0].y + yDimSize;
	  newBounds[5].y = Bounds[0].y + yDimSize;
	  newBounds[2].y = Bounds[7].y;
	  newBounds[3].y = Bounds[7].y;
	  newBounds[6].y = Bounds[7].y;
	  newBounds[7].y = Bounds[7].y;
	}

   if (!zBit)
	{	  
	  // Down
	  newBounds[0].z = Bounds[0].z;
	  newBounds[2].z = Bounds[0].z;
	  newBounds[4].z = Bounds[0].z;
	  newBounds[6].z = Bounds[0].z;
	  newBounds[1].z = Bounds[0].z + zDimSize;
	  newBounds[3].z = Bounds[0].z + zDimSize;
	  newBounds[5].z = Bounds[0].z + zDimSize;
	  newBounds[7].z = Bounds[0].z + zDimSize;
	}
      else
	{
	  // Up
	  newBounds[0].z = Bounds[0].z + zDimSize;
	  newBounds[2].z = Bounds[0].z + zDimSize;
	  newBounds[4].z = Bounds[0].z + zDimSize;
	  newBounds[6].z = Bounds[0].z + zDimSize;
	  newBounds[1].z = Bounds[7].z;
	  newBounds[3].z = Bounds[7].z;
	  newBounds[5].z = Bounds[7].z;
	  newBounds[7].z = Bounds[7].z;
	}

      Children.push_back(new octreeNode(newBounds, Items));

    }

  if (!Items.empty())
    {
      cout << "A really bad thing happened: (item list not empty)" << endl;
      //      dumpItems();

      exit(1);
    }
 
}

template <typename T>
inline bool octreeNode<T>::isIn(vector<T> &C)
{
  
  int i = 0;

  bool In = true;


  In &= ((C[0] >= Bounds[0].x) && (C[0] <= Bounds[7].x));
  In &= ((C[1] >= Bounds[0].y) && (C[1] <= Bounds[7].y));
  In &= ((C[2] >= Bounds[0].z) && (C[2] <= Bounds[7].z));
  
  return In;
      
}

#ifdef __DEBUG__
template <typename T>
void octreeNode<T>::dumpItems()
{

  int i = 0;

  typename list<vector<T> >::iterator p;

  cout << endl;
  cout << "Items: " << Items.size() << endl;
  if (!Items.empty())
    for (i=0,p=Items.begin();p!=Items.end();p++,i++)
      cout << i << " " << (*p)[0] << " " << (*p)[1] << " " << (*p)[2] << endl;  
  
}
#endif

template <typename T>
void octreeNode<T>::computeCentroid()
{

  int i = 0;
  int sum = 0;
  int numChildren = 8;

  T n = 0;

  T x = 0;
  T y = 0;
  T z = 0;

 typename list<vector<T> >::iterator p;

 
 if (!Items.empty())
   {
     
     n = numItems;
     for (p=Items.begin();p!=Items.end();p++)
       {
	 x += (*p)[0];
	 y += (*p)[1];
	 z += (*p)[2];
       }
     
     x /= n;
     y /= n;
     z /= n;
     
   }
 
 Centroid[0] = x;
 Centroid[1] = y;
 Centroid[2] = z;
 
}

template <typename T>
void octreeNode<T>::computeStdev()
{

  int i = 0;
  int sum = 0;
  int numChildren = 8;

  T sumx = 0;
  T sumy = 0;
  T sumz = 0;

  T n = 0;

  typename list<vector<T> >::iterator p;

  
  if (!Items.empty())
    {
      
      n = Items.size();
      if (n > 1)
	{
	  for (p=Items.begin();p!=Items.end();p++)
	    {
	      sumx += pow((*p)[0] - Centroid[0],2);
	      sumy += pow((*p)[1] - Centroid[1],2);
	      sumz += pow((*p)[2] - Centroid[2],2);
	    }

	  n--;
	  Stdev[0] = sqrt(sumx / n);
	  Stdev[1] = sqrt(sumy / n);
	  Stdev[2] = sqrt(sumz / n);
 
	} // n > 1
      else
	Stdev = vector<T>(3,0);

    } // if items to process

}

template <typename T>
inline T octreeNode<T>::getClosestFaceDistance(vector<T> &q)
{

  int i = 0;

  // Transform Q into local coordinate system
  vector<T> qp(3,0);

  // Construct the local basis (frame)
  Matrix u(3,1);
  Matrix v(3,1);
  Matrix w(3,1);
  Matrix p(3,1);

  // X dir
  u[0][0] = Bounds[4].x - Bounds[0].x;
  u[1][0] = Bounds[4].y - Bounds[0].y;
  u[2][0] = Bounds[4].z - Bounds[0].z;
  
  // Y dir
  v[0][0] = Bounds[2].x - Bounds[0].x;
  v[1][0] = Bounds[2].y - Bounds[0].y;
  v[2][0] = Bounds[2].z - Bounds[0].z;

  // Z dir
  w[0][0] = Bounds[1].x - Bounds[0].x;
  w[1][0] = Bounds[1].y - Bounds[0].y;
  w[2][0] = Bounds[1].z - Bounds[0].z;

  // normalize the axes
  u = u / sqrt(pow(u[0][0],2)+pow(u[1][0],2)+pow(u[2][0],2));
  v = v / sqrt(pow(v[0][0],2)+pow(v[1][0],2)+pow(v[2][0],2));
  w = w / sqrt(pow(w[0][0],2)+pow(w[1][0],2)+pow(w[2][0],2));

  // Translate the query point
  p[0][0] = q[0] - Centroid[0];
  p[1][0] = q[1] - Centroid[1];
  p[2][0] = q[2] - Centroid[2];

  // Compute the transformed point using the dot product
  qp[0] = (p[0][0]*u[0][0]) + (p[1][0]*u[1][0]) + (p[2][0]*u[2][0]);
  qp[1] = (p[0][0]*v[0][0]) + (p[1][0]*v[1][0]) + (p[2][0]*v[2][0]);
  qp[2] = (p[0][0]*w[0][0]) + (p[1][0]*w[1][0]) + (p[2][0]*w[2][0]);

  // Project the point on the bounding volume
  T uHalf = (Bounds[4].x - Bounds[0].x)/2;
  T vHalf = (Bounds[2].y - Bounds[0].y)/2;  
  T wHalf = (Bounds[1].z - Bounds[0].z)/2; 

  T d = 0.0;
  T D = 0.0;

  // Handle x projection
  if (qp[0] < -uHalf)
    {
      d = qp[0] + uHalf;
      D += (d*d);      
    }
  else
    if (qp[0] > uHalf)
      {
	d = qp[0] - uHalf;
	D += (d*d);
      }

  // Handle y projection
  if (qp[1] < -vHalf)
    {
      d = qp[1] + vHalf;
      D += (d*d);      
    }
  else
    if (qp[1] > vHalf)
      {
	d = qp[1] - vHalf;
	D += (d*d);
      }

  // Handle z projection
  if (qp[2] < -wHalf)
    {
      d = qp[2] + wHalf;
      D += (d*d);      
    }
  else
    if (qp[2] > wHalf)
      {
	d = qp[2] - wHalf;
	D += (d*d);
      }

  return(sqrt(D));

}

template <typename T>
T octreeNode<T>::getNearestItem(vector<T> &q, vector<T> &n)
{

  T minD = 0;
  T d;

  typename list<vector<T> >::iterator p;

  if (Items.empty())
  {
    cout << "fatal" << endl;
    exit(0);
  }


  p = Items.begin();
  minD = distance(q, *p);
  n = *p;
  p++;

  for (;p!=Items.end();p++)
    {
      d = distance(q, *p);
      if (d < minD)
	{
	  minD = d;
	  n = *p;
	}
    }

  return minD;

}

template <typename T>
inline T octreeNode<T>::distance(vector<T> &a, vector<T> &b)
{
  return sqrt(pow(a[0]-b[0],2)+pow(a[1]-b[1],2)+pow(a[2]-b[2],2));
}

template <typename T>
inline vector<T> octreeNode<T>::getCentroid()
{
  return Centroid;
}

//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
template <typename T>
void addItem(octreeNode<T> *&N, vector<T> &newItem)
{
  
  int i = 0;
  int numDim = 3;
  int numChildren = 8;
  
  bool added = false;

  T n;

  vector<T> d(3);

  N -> numItems++;

  // if the current node is a leaf...
  if (N -> isLeaf())
    {
      // Split check compares the actual number of items in the node
      N -> Items.push_back(newItem);
      if (N -> Items.size() > N -> maxItems)   
	  N -> Split();	           
      return;
    }

  // else recurse down the tree...
  for (i=0;i<numChildren;i++)
    if (N -> Children[i] -> isIn(newItem))
    {
      addItem(N -> Children[i], newItem);
      added = true;
      break;
    }
  
  if (!added)
    {
      cout << "A really bad thing happened (item not added)" << endl;
      
      cout << "Item:" << newItem[0] << " " << newItem[1] << " " << newItem[2] << endl;

      //      for (i=0;i<numDim;i++)
      //	cout << "[" << N -> Bounds[i].Min << "," << N -> Bounds[i].Max << "]" << " ";  
      //      cout << endl;

      exit(1);
    }

  return;

}

template <typename T>
void deleteNode(octreeNode<T> *&N)
{

  int i = 0;
  int numChildren = 8;


  if (N == NULL)
    return;

  if (N -> isLeaf())
    N -> Items.clear();
  else   
    for (i=0;i<numChildren;i++)
      deleteNode(N -> Children[i]);

}

template <typename T>
void displayNode(octreeNode<T> *&N, string tab)
{

  int i = 0;
  int numDim = 3;

  cout << tab << N << endl;
  cout << tab << "Bounds: ";
  for (i=0;i<numDim;i++)
    cout << "[" << N -> Bounds[i].Min << "," << N -> Bounds[i].Max << "]" << " ";
  cout << endl;

  cout << tab << "Items: " << N -> Items.size() << " (" << N -> numItems << ")" << endl;

  cout << tab << "Centroid: " << N -> Centroid[0] << " " << N -> Centroid[1] << " ";

  cout << N -> Centroid[2] << endl;
  cout << tab << "Stdev: " << N -> Stdev[0] << " " << N -> Stdev[1] << " ";
  cout << N -> Stdev[2] << endl;
  for (i=0;i<N -> Children.size();i++)
    displayNode(N -> Children[i], tab+TAB);

}

template <typename T>
int depth(octreeNode<T> *&N)
{

  int i = 0;
  int numChildren = 8;
  int h = 0;
  int max = 0;

  if (N == NULL)
    return 0;
 
  if (N -> isLeaf())
    return 1;

  max = 0;
  for (i=0;i<numChildren;i++)
    {
      h = depth(N -> Children[i]);
      if (h > max)
	max = h;
    }

  return max+1;

}

template <typename T>
int countNodes(octreeNode<T> *&N)
{
  
  int i = 0;
  int sum = 0;
  int numChildren = 8;


  if (N == NULL)
    return 0;

  if (N -> isLeaf())
    return 1;

  for (i=0;i<numChildren;i++)
    sum += countNodes(N->Children[i]);

  return sum;
}
    
template <typename T>
int countItems(octreeNode<T> *&N)
{
  
  int i = 0;
  int sum = 0;
  int numChildren = 8;


  if (N == NULL)
    return 0;

  if (N -> isLeaf())
    return N -> Items.size();

  for (i=0;i<numChildren;i++)
    sum += countItems(N->Children[i]);

  return sum;
}

template <typename T>
octreeNode<T>* getFirstLeaf(octreeNode<T> *&N)
{

  // This is function is deprecated
  
  if (N -> isLeaf())
    return N;
  
  return getFirstLeaf(N -> Children[0]);

}

template <typename T>
void getNearestNodeList(octreeNode<T>*&N, vector<T> &q, vector<octreeNode<T>*> &L,
			T &maxInterest)
{

  int i = 0;
  int numChildren = 8;


  if (N==NULL)
    return;

  T d = N -> getClosestFaceDistance(q);
  //  cout << d << endl;

  // if this node is within the interest radius, follow it down
  if (d <= maxInterest)
    {
      // save it only if it is leaf and not empty
      if (N -> isLeaf())
	{
	  if (!N -> empty())
	    L.push_back(N);
	}
      else
	{
	  // inspect the children
	  for (i=0;i<numChildren;i++)
	    getNearestNodeList(N -> Children[i], q, L, maxInterest);
	}
    }
  else
    return;
  
}

template <typename T>
void translate(octreeNode<T> *&N, vector<T> &t)
{

  int i = 0;
  int j = 0;
  int numChildren = 8;
  int numVertices = 8;
  int numDim = 3;


  if (N == NULL)
    return;

 // translate the bounding volume of every node
  for (i=0;i<numVertices;i++)
    {
      N -> Bounds[i].x += t[0];
      N -> Bounds[i].y += t[1];
      N -> Bounds[i].z += t[2];
    }

  // translate the centroid of every node (leaf or non-leaf)
  N -> Centroid[0] += t[0];
  N -> Centroid[1] += t[1];
  N -> Centroid[2] += t[2];

  if (N -> isLeaf())
    {

      if (!N->Items.empty())
	{

	  typename list<vector<T> >::iterator p;
	  for (p=N->Items.begin();p!=N->Items.end();p++)
	    {
	      (*p)[0] += t[0];
	      (*p)[1] += t[1];
	      (*p)[2] += t[2];
	    }
	  
	}
    }      
  else
    for (i=0;i<numChildren;i++)
      translate(N -> Children[i], t);
	      
}

template <typename T>
void rotate(octreeNode<T> *&N, vector<T> &c, Matrix &R)
{

  Matrix p(3,1);

  int i = 0;
  int numChildren = 8;
  int numVertices = 8;
  int numDim = 3;

  if (N == NULL)
    return;

  //-------------------------------------------------------------------

  for (i=0;i<numVertices;i++)
    {
      
      // Translate the volume's minimum extent to the global origin
      p[0][0] = N -> Bounds[i].x - c[0];
      p[1][0] = N -> Bounds[i].y - c[1];
      p[2][0] = N -> Bounds[i].z - c[2];     

      // Rotate it
      p = R * p;
      
      // Translate it back
      N -> Bounds[i].x = p[0][0] + c[0];
      N -> Bounds[i].y = p[1][0] + c[1];
      N -> Bounds[i].z = p[2][0] + c[2];
      
    }

  //-------------------------------------------------------------------
  
  // Rotate the local centroids about the global centroid
  p[0][0] = N -> Centroid[0] - c[0];
  p[1][0] = N -> Centroid[1] - c[1];
  p[2][0] = N -> Centroid[2] - c[2];

  p = R * p;

  N -> Centroid[0] = p[0][0] + c[0];
  N -> Centroid[1] = p[1][0] + c[1];
  N -> Centroid[2] = p[2][0] + c[2];

  if (N -> isLeaf())
    {
     
      if (!N -> empty())
	{

	  typename list<vector<T> >::iterator q;
	  for (q=N->Items.begin();q!=N->Items.end();q++)
	    {

	      // move sub-cloud to the origin
	      p[0][0] = (*q)[0] - c[0];
	      p[1][0] = (*q)[1] - c[1];
	      p[2][0] = (*q)[2] - c[2];

	      // rotate
	      p = R * p;

	      // restore sub-cloud to initial position
	      (*q)[0] = p[0][0] + c[0];
	      (*q)[1] = p[1][0] + c[1];
	      (*q)[2] = p[2][0] + c[2];	      	     
	    }
	  
	}
    }      
  else
    for (i=0;i<numChildren;i++)
      rotate(N -> Children[i], c, R);
	      
}

template <typename T>
void rotate(octreeNode<T> *&N, Matrix &R)
{

  // This rotate does not perform translation

  Matrix p(3,1);

  int i = 0;
  int numChildren = 8;
  int numVertices = 8;


  if (N == NULL)
    return;

 //-------------------------------------------------------------------

  for (i=0;i<numVertices;i++)
    {
      
      // Load
      p[0][0] = N -> Bounds[i].x;
      p[1][0] = N -> Bounds[i].y;
      p[2][0] = N -> Bounds[i].z;     

      // Rotate it
      p = R * p;
      
      // Store
      N -> Bounds[i].x = p[0][0];
      N -> Bounds[i].y = p[1][0];
      N -> Bounds[i].z = p[2][0];
      
    }

  //-------------------------------------------------------------------

  p[0][0] = N -> Centroid[0];
  p[1][0] = N -> Centroid[1];
  p[2][0] = N -> Centroid[2];

  p = R * p;

  N -> Centroid[0] = p[0][0];
  N -> Centroid[1] = p[1][0];
  N -> Centroid[2] = p[2][0];

  //-------------------------------------------------------------------

  if (N -> isLeaf())
    {
     
      if (N -> size() > 0)
	{

	  typename list<vector<T> >::iterator q;
	  for (q=N->Items.begin();q!=N->Items.end();q++)
	    {
	      
	      // Load
	      p[0][0] = (*q)[0];
	      p[1][0] = (*q)[1];
	      p[2][0] = (*q)[2];

	      // rotate
	      p = R * p;

	      // Store
	      (*q)[0] = p[0][0];
	      (*q)[1] = p[1][0];
	      (*q)[2] = p[2][0];	      	     

	    }
	  
	}
    }      
  else
    for (i=0;i<numChildren;i++)
      rotate(N -> Children[i], R);

}

template<typename T>
void buildLeafList(octreeNode<T> *&N, vector<octreeNode<T>*>&leafList)
{

  int i = 0;
  int numChildren = 8;


  if (N -> isLeaf())
    {
      if (N -> Items.size() > 0)
	leafList.push_back(N);
    }
  else
    for (i=0;i<numChildren;i++)
      buildLeafList(N->Children[i], leafList);

}

template <typename T>
vector<T> computeCentroid(octreeNode<T> *&N, int &num)
{

  double weight = 0;
  double countTotal = 0;

  int i = 0;
  int j = 0;
  int numChildren = 8;
  int count = 0;
  
  vector<vector<T > > centroidList;
  vector<double> countList;
  vector<T> centroid(3,0);
  

  if (N -> isLeaf())
    N -> computeCentroid();
  else
    {

      // Determine centroids and counts for child nodes
      for (i=0;i<numChildren;i++)
	{
	  centroid = computeCentroid(N -> Children[i], count);
	  centroidList.push_back(centroid);
	  countList.push_back(double(count));

	  countTotal += double(count);

	}

      // Normalize and sum
      N -> Centroid = vector<T>(3,0);
      for (i=0;i<numChildren;i++)
	{
	  weight = countList[i]/countTotal;
	  
	  for (j=0;j<3;j++)
	    N -> Centroid[j] += (centroidList[i][j] * weight);
	}
      
      N -> numItems = int(countTotal);
  
    }

  num = N -> size();
  
  return N -> Centroid;

}

template <typename T>
void render(octreeNode<T> *&N)
{

  int i = 0;
  int numChildren = 8;

#define __WIREFRAME__
  
  if (N==NULL)
    return;
  
  T x1;
  T x2;
  T y1;
  T y2;
  T z1;
  T z2;

  // Render this level

  // -- Fix Z
  //  glColor3f(1.0f,1.0f,0.0f);

  glColor3f(0.0f,0.0f,1.0f);
#ifdef __WIREFRAME__
  glBegin(GL_LINE_LOOP);
#else
  glBegin(GL_POLYGON);
#endif
  glVertex3f(N->Bounds[0].x, N->Bounds[0].y, N->Bounds[0].z);
  glVertex3f(N->Bounds[2].x, N->Bounds[2].y, N->Bounds[2].z);
  glVertex3f(N->Bounds[6].x, N->Bounds[6].y, N->Bounds[6].z);
  glVertex3f(N->Bounds[4].x, N->Bounds[4].y, N->Bounds[4].z);
  glEnd();
  
glColor3f(0.0f, 1.0f, 0.0f);
#ifdef __WIREFRAME__
  glBegin(GL_LINE_LOOP);
#else
  glBegin(GL_POLYGON);
#endif
  glVertex3f(N->Bounds[1].x, N->Bounds[1].y, N->Bounds[1].z);
  glVertex3f(N->Bounds[3].x, N->Bounds[3].y, N->Bounds[3].z);
  glVertex3f(N->Bounds[7].x, N->Bounds[7].y, N->Bounds[7].z);
  glVertex3f(N->Bounds[5].x, N->Bounds[5].y, N->Bounds[5].z);
  glEnd();

  // Fix X
  glColor3f(0.0f, 1.0f, 1.0f);
#ifdef __WIREFRAME__
  glBegin(GL_LINE_LOOP);
#else
  glBegin(GL_POLYGON);
#endif
  glVertex3f(N->Bounds[0].x, N->Bounds[0].y, N->Bounds[0].z);
  glVertex3f(N->Bounds[1].x, N->Bounds[1].y, N->Bounds[1].z);
  glVertex3f(N->Bounds[3].x, N->Bounds[3].y, N->Bounds[3].z);
  glVertex3f(N->Bounds[2].x, N->Bounds[2].y, N->Bounds[2].z);
  glEnd();

  glColor3f(1.0f,0.0f,0.0f);
#ifdef __WIREFRAME__
  glBegin(GL_LINE_LOOP);
#else
  glBegin(GL_POLYGON);
#endif
  glVertex3f(N->Bounds[4].x, N->Bounds[4].y, N->Bounds[4].z);
  glVertex3f(N->Bounds[5].x, N->Bounds[5].y, N->Bounds[5].z);
  glVertex3f(N->Bounds[7].x, N->Bounds[7].y, N->Bounds[7].z);
  glVertex3f(N->Bounds[6].x, N->Bounds[6].y, N->Bounds[6].z);
  glEnd();

  // Note: Y renderings are redundant


  if (!N -> isLeaf())
    for (i=0;i<numChildren;i++)
      render(N -> Children[i]);
  
}



/*
template <typename T>
void setMaxInterest(octreeNode<T> *&N, T m)
{

  int i = 0;
  int numChildren = 8;


  if (N == NULL)
    return;

  N -> maxInterest = m;

  if (!N -> isLeaf())
    for (i=0;i<numChildren;i++)
      setMaxInterest(Children[i], m);
      
}
*/

//----------------------------------------------------------------------------
template <typename T>
ostream &operator<<(ostream &o, octreeNode<T> &N)
{
  int i = 0;
  int numPoints = 8;

  typename list<vector<T> >::iterator p;

  o << "numItems: " << N.Items.size() << endl;
  o << "Bounds: " << endl;
  for (i=0;i<numPoints;i++)
    o << N.Bounds[i] << endl;
  o << endl;  

  o << "Items: " << endl;
  for (p=N.Items.begin();p!=N.Items.end();p++)
    cout << *p << endl;

  return o;
}

template <typename T>
ostream &operator<<(ostream &o, PointType<T> &p)
{
  o << "[" << p.x << "," << p.y << "," << p.z << "]";
}

template <typename T>
ostream &operator<<(ostream &o, vector<T> &t)
{
  int i = 0;
  
  o << "[";
  for (i=0;i<t.size()-1;i++)
    o << t[i] << ",";
  o << t[i] << "]";
  
  return o;
}

template <typename T>
ostream &operator<<(ostream &o, vector<vector<T> >&t)
{
  int i = 0;
  
  for (i=0;i<t.size();i++)
    o << t[i] << endl;

  return o;
}
