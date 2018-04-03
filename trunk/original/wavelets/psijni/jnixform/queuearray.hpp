#ifndef __QUEUEARRAY_HPP__
#define __QUEUEARRAY_HPP__

#include "quadnode.hpp"

class queuearray
{
private:
    // iternal structure
    quadnode *q; int N, head, tail;
public:
        queuearray (int size);
    int empty();
    int put (quadnode &myItem);
    //quadnode get ();
    void get (quadnode &myItem);
    void setquadnode (quadnode &source, quadnode &result);
    int getLength () { return N;}
};

#endif;