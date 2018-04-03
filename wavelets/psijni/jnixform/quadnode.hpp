#ifndef __QUAD_NODE__
#define __QUAD_NODE__

class quadnode
{
public: 
    int xul, yul;
    int xll, yll;
    int xur, yur;
    int xlr, ylr;
    int rows, cols;
    double L1energy;
    double L2energy;
    double max;
    double min;
    double epsilon;
    int length;
    int isnotnull;
    int id;
    int NUMBER_OF_GREY_LEVELS;
    void copy(quadnode &source);
};

#endif