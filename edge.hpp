#ifndef __EDGE_HPP__
#define __EDGE_HPP__

#include "renaissance_config.hpp"
#include <algorithm>
using namespace std;

class Edge{
public:
  Edge() { m_pt0 = -1; m_pt1 = -1; }
  Edge(int pt0, int pt1) {
    m_pt0 = pt0; m_pt1 = pt1;
  }
	
//private:
  int m_pt0;
  int m_pt1;
};

bool operator<(const Edge &a, const Edge &b);
bool operator==(const Edge &a, const Edge &b);

#endif //__EDGE_HPP__

