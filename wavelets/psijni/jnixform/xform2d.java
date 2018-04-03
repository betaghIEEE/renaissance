

class xform2dinterface {
    private qtreearray mytree;
    
    public void getA();
    public void getD();
    public void getH();
    public void getD();
    public int getCurrentXUL ();
    public int getCurrentYUL ();
    public int getCurrentXUR ();
    public int getCurrentYUR ();
    public int getCurrentXLL ();
    public int getCurrentYLL ();
    public int getCurrentXLR ();
    public int getCurrentYLR ();
    public double getCurrentL1Energy ();
    public double getCurrentL2Energy ();
    public double getCurrentMax ();
    public int getCurrentIsNull ();
    public int getCurrentLength ();
    public int getCurrentID ();
    public double getCurrentValues (int index);
    
    private native int xformquadtree ();
    
    public void xformquadtree (mymatrix source);
    
    public qtreearray mytree ();
    
    public mymatrix xformquadtreeinverse (qtreearray source);
    
}