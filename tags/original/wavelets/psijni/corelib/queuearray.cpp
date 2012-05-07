#include "queuearray.hpp"

queuearray::queuearray (int size) 
{ 
    N = size + 1; 
    q = new Item[size + 1];  
    head = N; 
    tail = 0;
}

int queuearray::empty()
{
    return head % N == tail;
}

void queuearray::setquadnode (quadnode &source, quadnode &result)
{
    result.xul = source[head].xul;
    result.yul = source[head].yul;
    result.xur = source[head].xur;
    result.yur = source[head].yur;
    result.xll = source[head].xll;
    result.xlr = source[head].xlr;
    result.yll = source[head].yll;
    result.ylr = source[head].ylr;
    result.energy = source[head].energy;
    result.max = source[head].max;
    result.min = source[head].min;
    result.length = source[head].length;
    result.rows = source[head].rows;
    result.cols = source[head].cols;
}

int queuearray::put (quadnode &myItem)
{
    //q[tail++] = item;
    setquadnode( myItem, q[tail]);
    tail++;
    tail = tail % N;
}

void queuearray::get (quadnode &value)
{
    head = head % N;
    setquadnode ( q[head], myItem);
    head++;
    
}
