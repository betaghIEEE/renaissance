/*
 *      Quad Tree/Trie Reference for the 2-D Wavelet Transform
 *          Needed are four branches
 *              A branch
 *              V branch
 *              H branch
 *              D branch
 *          Each leaf should
 *              point the position in the matrix to which it belongs
 *              Include a serialization to transfer back and forth between
 *              the Java libraries, the C++ libraries, and the FITS files.
 *              
 */

class quadtree 
{
private:
    double *values;
    int length;
    int xul, yul;
    int xll, yll;
    int xur, yur;
    int xlr, ylr;
    int rows, cols;
    
public:
        quadtree();
    quadtree (const quadtree &node);
    quadtree (double *values, int length);
    int getlength () { return length;}
    double getValue (int index);
    void setValue (int index, double value);
    quadtree *A;
    quadtree *V;
    quadtree *H;
    quadtree *D;
    void setUL (int x, int y) { xul = x; yul = y;}
    int getXUL ()  { return xul;}
    int getYUL () { return yul;}
    void setLL (int x, int y)  { xll = x; yll = y;} 
    int getXLL ()  { return xll;}
    int getYLL () { return yul;}

    void setUR (int x, int y)  { xur = x; yur = y;} 
    int getXUR ()  { return xur;}
    int getYUR () { return yur;}
    
    void setLR (int x, int y)  { xlr = x; ylr = y;} 
    int getXLR ()  { return xlr;}
    int getYLR () { return yur;}
    
    int getRows () { return rows;}
    int getCols () { return cols;} 
    void setRows (int r) { rows = r;}
    void setCols (int c) { cols = c;}
    
    
    
    
/*
    void addA ();
    void addA (quadtree *node);
    void addV ();
    void addV (quadtree *node);
    void addH ();
    void addH (quadtree *node);
    void addD ();
    void addD (quadtree *node);
  */  
};
