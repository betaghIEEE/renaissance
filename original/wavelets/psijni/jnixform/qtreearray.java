

class qtreearray 
{

    private int current;
    private quadnode tree[];
    private int length;
    public qtreearray ()
        {
            current = 0;
            tree = null;
            length = 0;
        }
    public qtreearray (int slength) { allocate(slength);}
    
    public void allocate (int slength){
        current = 0;
        tree = new quadnode[slength];
        length = slength;
    }
    public void deallocate ()
    {
        if ( tree != null){
            tree = null;
        }
    }
    
    public void initialize (int i){
	tree[i].xul = 0;
	tree[i].yul = 0;
	tree[i].xur = 0;
	tree[i].yur = 0;
	tree[i].xll = 0;
	tree[i].yll = 0;
	tree[i].xlr = 0;
	tree[i].ylr = 0;
	tree[i].rows = 0;
	tree[i].cols = 0;
	tree[i].L1energy = 0.0;
	tree[i].L2energy = 0.0;
	tree[i].max = 0.0;
	tree[i].min = 0.0;
	tree[i].epsilon = 0.0;
	tree[i].length = 0;
	tree[i].isnotnull = 0;
	tree[i].id = i;
	tree[i].NUMBER_OF_GREY_LEVELS = 0;
    }
    
    //public quadnode getNode (int i );
    //public void setNode (int i, quadnode value );
    //public quadnode getNode ();
    //public void setNode (quadnode value);
    
    //public boolean div4 (int i, int base);
    //public void killNode ();
    //public void killNode (int i);
    //public void killNode (quadnode node);
        
    //public void setNode (quadnode source, quadnode result);
    
    public void gotoNode (int i) { current = i; }
    public quadnode getRoot () { return tree[0];}
    public quadnode getA() { return tree[current*4 +1];}
    public quadnode getH() { return tree[current * 4 + 3];}
    public quadnode getD() { return tree[current * 4 + 4];}
    public quadnode getV() { return tree[current * 4 + 2];}
    public quadnode getNode () { return tree[current];}
    public quadnode getAnode() {  return tree[current* 4 + 1];}
    public quadnode getHnode() {  return tree[current * 4 + 3];}
    public quadnode getDnode() {  return tree[current * 4 + 4];}
    public quadnode getVnode() {  return tree[current * 4 + 2];}
	public int getParent () { current= current -1 ; current = current /4; return current;}
    public int gotoA() { return current = current * 4 + 1;}
    public int gotoH() { return current = current * 4 + 3;}
    public int gotoD() { return current = current * 4 + 4;}
    public int gotoV() { return current = current * 4 + 2;}
	public int gotoParent () { return current = (current - 1) /4;}
    public int getA(int pos) { return pos * 4 + 1;}
    public int getH(int pos) { return pos * 4 + 1;}
    public int getD(int pos) { return pos * 4 + 1;}
    public int getV(int pos) { return pos * 4 + 1;}
    
    public int getLength() { return length; }
	public int getXUL () { return tree[current].xul;}
	public int getYUL () { return tree[current].yul;}
	public int getXLL () { return tree[current].xll;}
	public int getYLL () { return tree[current].yll;}
	public int getXUR () { return tree[current].xur;}
	public int getYUR () { return tree[current].yur;}
	public int getXLR () { return tree[current].xlr;}
	public int getYLR () { return tree[current].ylr;}
	public int getRows () { return tree[current].rows;}
	public int getCols () { return tree[current].cols;}
	public int isNull () { return tree[current].isnotnull;}
	public int getID () { return tree[current].id;}
	public double getMax () { return tree[current].max;}
	public double getMin () { return tree[current].min;}
	public double getEpsilon () { return tree[current].max;}
	public double getL1Energy () { return tree[current].L1energy;}
	public double getL2Energy () { return tree[current].L2energy;}
	public double getValue (int index) { return tree[current].values[index];}
    public void setValue (int index, double value) { tree[current].values[index] = value;}
    //public void allocate (int size) { tree[current].values = new double[size]; }
    public int getGrey () { return tree[current].NUMBER_OF_GREY_LEVELS;}
    public void setGrey (int grey) { tree[current].NUMBER_OF_GREY_LEVELS = grey;}
    
    
    public void setNode (quadnode r)
    {
        int index = r.id;
        if(index < length)
        {
            tree[index] = new quadnode();
        }
        
    }
    
    public void setCurrentNode (quadnode r)
    {
        int index = current;
        if(index < length)
        {
            tree[index] = new quadnode();
            tree[index].id = index;
        } 
    }
    
    public void setRootNode (quadnode r)
    {
        int index = 0;
        if(index < length)
        {
            tree[index] = new quadnode();
            tree[index].id = index;
        } 
    }
    
    private boolean setAnode (quadnode r)
    {
        int index = r.id * 4 + 1;
        if(index < length)
        {
            tree[index] = new quadnode();
            int rows = (r.rows)/2;
            int cols = (r.cols)/2;
            tree[index].rows = rows;
            tree[index].cols = cols;
            tree[index].xul = r.xul;
            tree[index].xll = r.xul + rows - 1;
            tree[index].xur = r.xur;
            tree[index].xlr = r.xur + rows - 1;
            tree[index].yul = r.yul;
            tree[index].yur = r.yul + cols - 1;
            tree[index].yll = r.yll;
            tree[index].ylr = r.yll + cols - 1;
            tree[index].isnotnull = 1;
            tree[index].id = r.id * 4 + 1;
        } else return false;
        return true;
    }
    
    private boolean setVnode (quadnode r)
    {
        int index = r.id * 4 + 2;
        if(index < length)
        {
            tree[index] = new quadnode();
            int rows = (r.rows)/2;
            int cols = (r.cols)/2;
            tree[index].rows = rows;
            tree[index].cols = cols;
            tree[index].xul = r.xul;
            tree[index].xll = r.xul + rows - 1;
            tree[index].xur = r.xur;
            tree[index].xlr = r.xur + rows - 1;
            tree[index].yul = r.yul + cols;
            tree[index].yll = r.yul + cols;
            tree[index].yur = r.yur;
            tree[index].ylr = r.ylr;
            tree[index].isnotnull = 1;
            tree[index].id = r.id * 4 + 2;
        } else return false;
        return true;
    }
    
    private boolean setHnode (quadnode r)
    {
        
        int index = r.id * 4 + 3;
        if(index < length)
        {
            tree[index] = new quadnode();
            int rows = (r.rows)/2;
            int cols = (r.cols)/2;
            tree[index].rows = rows;
            tree[index].cols = cols;
            
            tree[index].xul = r.xul + rows;
            tree[index].xll = r.xll;
            tree[index].xur = r.xur + rows;
            tree[index].xlr = r.xlr;
            tree[index].yul = r.yul;
            tree[index].yll = r.yll;
            tree[index].yur = r.yul + cols - 1;
            tree[index].ylr = r.yll + cols - 1;
            tree[index].isnotnull = 1;
            tree[index].id = r.id * 4 + 3;
        } else return false;
        return true;
    }
    
    private boolean setDnode (quadnode r)
    {
        int index = r.id * 4 + 4;
        if(index < length)
        {
            tree[index] = new quadnode();
            int rows = (r.rows)/2;
            int cols = (r.cols)/2;
            tree[index].rows = rows;
            tree[index].cols = cols;
            
            tree[index].xul = r.xul + rows;
            tree[index].xll = r.xll;
            tree[index].xur = r.xur + rows;
            tree[index].xlr = r.xlr;
            tree[index].yul = r.yul + cols;
            tree[index].yll = r.yul + cols;
            tree[index].yur = r.yur;
            tree[index].ylr = r.ylr;
            tree[index].id = r.id * 4 + 4;
        } else return false;
        return true;
    }
    
    public boolean setChildren (quadnode r)
    {
        if ( setAnode (r) & setVnode (r) & setHnode (r) & setDnode (r) )
            return true;
        return false;
    }
    
    public void killChildren (quadnode r)
    {
        int index = r.id;
        tree[index * 4 +1] = null;
        tree[index * 4 +2] = null;
        tree[index * 4 +3] = null;
        tree[index * 4 +4] = null;
        
    }
	
    public boolean isleaf ()
    {
        int tobetested = current;
        if ( (tobetested * 4 + 1) < length) {
            if ( tree[tobetested * 4 +1] != null) return false;
            if (tree[tobetested * 4 +2] != null) return false;
            if (tree[tobetested * 4 +1] != null) return false;
            if (tree[tobetested * 4 +4] != null) return false;
        }
        return true;
    }
    
    public boolean isleaf (int i)
    {
        int tobetested = i;
        if ( (tobetested * 4 + 1) < length) {
            if ( tree[tobetested * 4 +1] != null) return false;
            if (tree[tobetested * 4 +2] != null) return false;
            if (tree[tobetested * 4 +1] != null) return false;
            if (tree[tobetested * 4 +4] != null) return false;
        }
        return true;
    }
    
    public boolean isleaf (quadnode r)
    {
        int tobetested = r.id;
        if ( (tobetested * 4 + 1) < length) {
            if ( tree[tobetested * 4 +1] != null) return false;
            if (tree[tobetested * 4 +2] != null) return false;
            if (tree[tobetested * 4 +1] != null) return false;
            if (tree[tobetested * 4 +4] != null) return false;
        }
        return true;
    }
    
    public boolean isgrandparent ()
    {
        int tobetested = current;
        if ( !isleaf (tobetested * 4 + 1)) return false;
        if ( !isleaf (tobetested * 4 + 2)) return false;
        if ( !isleaf (tobetested * 4 + 3)) return false;
        if ( !isleaf (tobetested * 4 + 4)) return false;
        return true;
    }
    
    public boolean isgrandparent (int i)
    {
        int tobetested = i;
        if ( !isleaf (tobetested * 4 + 1)) return false;
        if ( !isleaf (tobetested * 4 + 2)) return false;
        if ( !isleaf (tobetested * 4 + 3)) return false;
        if ( !isleaf (tobetested * 4 + 4)) return false;
        return true;
    }
    
    public boolean isgrandparent (quadnode r)
    {
        int tobetested = r.id;
        if ( !isleaf (tobetested * 4 + 1)) return false;
        if ( !isleaf (tobetested * 4 + 2)) return false;
        if ( !isleaf (tobetested * 4 + 3)) return false;
        if ( !isleaf (tobetested * 4 + 4)) return false;
        return true;
    }
    
}