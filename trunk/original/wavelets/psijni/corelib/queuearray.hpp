#ifndef __QUEUEARRAY_HPP__
#define __QUEUEARRAY_HPP__

class queuearray
{
private:
    // iternal structure
    quadnode *q; int N, head, tail;
public:
        queuearray (int size);
    int empty();
    int put (quadnode &myItem);
    quadnode get ();
    void get (quadnode &myItem);
};

#endif;