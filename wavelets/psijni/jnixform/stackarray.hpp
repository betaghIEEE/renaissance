
#ifndef __STACKARRAY_HPP__
#define  __STACKARRAY_HPP__
#include "quadnode.hpp"

class stackarray
{
    private:
        quadnode *s; int N; int size;
    public:
        stackarray(int maxsize);
        int empty () const;
        void push (quadnode &item);
        void pop (quadnode &item);
        int getLength () { return size;}
};


#endif