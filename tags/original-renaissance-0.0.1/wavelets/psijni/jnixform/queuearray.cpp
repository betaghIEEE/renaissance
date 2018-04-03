#include "queuearray.hpp"

queuearray::queuearray (int size) 
{ 
    N = size + 1; 
    q = new quadnode[size + 1];  
    head = N; 
    tail = 0;
}

int queuearray::empty()
{
    return head % N == tail;
}

void queuearray::setquadnode (quadnode &source, quadnode &result)
{
    result.copy(source);
}

int queuearray::put (quadnode &myItem)
{
    //q[tail++] = item;
    setquadnode( myItem, q[tail]);
    tail++;
    tail = tail % N;
}

void queuearray::get (quadnode &myItem)
{
    head = head % N;
    setquadnode ( q[head], myItem);
    head++;
    
}
