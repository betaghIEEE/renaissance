#include "quadtree.h"
#include "stdlib.h"
using namespace std;


quadtree::quadtree()
{
    length = 0;
    A = NULL;
    V = NULL;
    H = NULL;
    D = NULL;
    *values = NULL;
}

quadtree::quadtree (const quadtree &node)
{
}

double quadtree::getValue (int index)
{
    return values[index];
}

void quadtree::setValue (int index, double value)
{
    values[index] = value;
}