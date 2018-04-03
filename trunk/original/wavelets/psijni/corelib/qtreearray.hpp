#ifndef __QTREEARRAY_HPP__
#define __QTREEARRAY_HPP__

class qtreearray {
private:
    int current;
    quadnode *tree;
    int length;
public:
    void getNode (int i, quadnode &value );
    void setNode (int i, quadnode &value );
    void getNode (quadnode &value);
    void setNode (quadnode &value);

    void setNode (quadnode &source, quadnode &result);
    void gotoNode (int i);
    int getA();
    int getH();
    int getD();
    int getV();
    int getLength();
    int isleaf ();
    int noleaving ();
    void retrieve (quadnode *tree, int length);
    void save (quadtree *tree, int length);
    
    
};

#endif