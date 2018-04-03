
class testmodule {
    matrixType image;
    pgmio pgm;
    xformqtree xform;
    public static void main (String args[])
    {
        testmodule tm = new testmodule();
        tm.pgm = new pgmio ();
        tm.pgm.getPGM("swalla10.pgm");
        quadnode pic = tm.pgm.qnode;
        quadnode xpic;
        tm.xform = new xformqtree();
        xpic = tm.xform.selfWXform (pic);
        tm.pgm.qnode = xpic;
        tm.pgm.saveImagePGM2 ("test.pgm");
    }
}