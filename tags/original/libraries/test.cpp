
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

#include "octree.hpp"

int main()
{

  int i = 0;

  vector<double> B(6);

  B[0] = 0;
  B[1] = 10;
  B[2] = 0;
  B[3] = 10;
  B[4] = 0;
  B[5] = 10;

  octree<double> *o = new octree<double>(B);

 
  vector<double> point(3,0.0);
  for (i=0;i<101;i++)
    o -> addItem(point);

  delete o;
  return 0;

}
