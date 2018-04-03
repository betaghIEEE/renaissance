#include "stackarray.hpp"

        
stackarray::stackarray(int maxsize)
{
    size = maxsize;
    s = new quadnode [maxsize];
    N = 0;
}

int stackarray::empty () const
{
    return N == 0;
}

void stackarray::push (quadnode &item)
{
    //s[N++] = item;
    s[N++].copy (item);
}

void stackarray::pop (quadnode &item)
{
    item.copy ( s[--N]);
}