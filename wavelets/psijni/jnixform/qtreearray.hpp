#ifndef __QTREEARRAY_HPP__
#define __QTREEARRAY_HPP__
#include "quadnode.hpp"
#include "matrixType.h"

class qtreearray {
private:
    int current;
    quadnode *tree;
    int length;
public:
        qtreearray ();
    qtreearray (int length);
    bool allocate (int length);
    bool deallocate ();
    bool initialize (int i);
    bool initialize (matrixType &S);
    int findX2 (int i);
    bool getNode (int i, quadnode &value );
    bool setNode (int i, quadnode &value );
    void getNode (quadnode &value);
    void setNode (quadnode &value);
    
   
    void setIsNotNull (int value);
    void setIsNotNull (int value, int index);
    int getIsNotNull ();
    int getIsNotNull (int index);
    void getStats ( matrixType &S);
    void getStats ( matrixType &S, int index);
    
    bool div4 (int i, int base);
    void killNode ();
    void killNode (int i);
    void killNode (quadnode &node);
        
    void setNode (quadnode &source, quadnode &result);
    bool gotoNode (int i);
    int getA();
    int getH();
    int getD();
    int getV();
    int getA(int pos);
    int getH(int pos);
    int getD(int pos);
    int getV(int pos);
    
    int getXUL () { return tree[current].xul;}
    int getYUL () { return tree[current].yul;}
    int getXLL () { return tree[current].xll;}
    int getYLL () { return tree[current].yll;}
    int getXUR () { return tree[current].xur;}
    int getYUR () { return tree[current].yur;}
    int getXLR () { return tree[current].xlr;}
    int getYLR () { return tree[current].ylr;}
    int getRows () { return tree[current].rows;}
    int getCols () { return tree[current].cols;}
    double getL1 () { return tree[current].L1energy;}
    double getL2 () { return tree[current].L2energy;}
    double getMax () { return tree[current].max;}
    double getMin () { return tree[current].min;}
    double getEpsilon () { return tree[current].epsilon;}
    
    void initAnode ( int i);
    void initVnode ( int i);
    void initHnode ( int i);
    void initDnode ( int i);
    
    int getLength();
    int isleaf ();
    int isleaf (int i);
    int isgrandparent ();
    int isgrandparent (int i);
    
    
    
};

#endif