#include "edge.hpp"

bool operator<(const Edge &a, const Edge &b)
{
  if (MIN(a.m_pt0, a.m_pt1) < MIN(b.m_pt0, b.m_pt1) )
    return true;
  if (MIN(a.m_pt0, a.m_pt1) == MIN(b.m_pt0, b.m_pt1) &&
      MAX(a.m_pt0, a.m_pt1) < MAX(b.m_pt0, b.m_pt1) )
    return true;
  return false;
}
bool operator==(const Edge &a, const Edge &b)
{
  return ( (a.m_pt0 == b.m_pt0 && a.m_pt1 == b.m_pt1) || 
	   (a.m_pt1 == b.m_pt0 && a.m_pt0 == b.m_pt1) );
}
