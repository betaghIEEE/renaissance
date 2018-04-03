#include "matrixnode"

#ifndef MATRIX_LIST
#define MATRIX_LIST

class matrixlist
{
    private:
        matrixnode *list;
        int arraysize;
        int length;
        int current;
    public: 
        int getNextI () { current++; return list[current].i;}
        int getNextJ () { current++; return list[current].j;}
        int getI () { return list[current].i;}
        int getJ () { return list[current].i;}
        void getIJ (int &ival, int &jval) { ival = list[current].i; jval = list[current].j;}
        void setI (int value) { list[current].i = value;}
        void setJ (int value) { list[current].j = value;}
        void setBool (bool value) {list[current].sparse = value;}
        void setNext (int ival, int jval, bool value) {  list[current].i = ival; list[current].j = jval; list[current].sparse = value;}
        void goFirst () { current = 0;}
        void goLast () { current = length -1;}
        bool goNext () { if (current < (length -1)) current++; else return false; return true;}
}


#endif 