package jpackets1;

/**
 * <p>Title: Java Wavelet Packets Libraries</p>
 * <p>Description: Wavelet Packet Libraries </p>
 * <p>Copyright: Copyright (c) 2004</p>
 * <p>Company: Texas Tech University</p>
 * @author Daniel Beatty
 * @version 1.0
 */

public class xformqtree {
  private matrixType s;
  private matrixType t;
  private qtreearray qtree;
  private myVector ha;
  private myVector hd;
  private myVector xTemp;
  private myVector xTemp2;
  private myVector yTemp;
  private myVector yTemp2;
  private int resolution;
  private double sqhalf;
  public xformqtree()
  {
      sqhalf = java.lang.Math.pow (0.5 , 0.5);
      double negsqhalf = (-1) * sqhalf;
      //queue = null;
      //astack = null;
      ha = new myVector();
      hd = new myVector();
      ha.allocate(2);
      hd.allocate(2);
      ha.setM_data(0,sqhalf);
 hd.setM_data(0,sqhalf);
 ha.setM_data(1, sqhalf);
      hd.setM_data(1, negsqhalf);

  }
/*
   public xformqtree(matrixType S)     {
            sqhalf = pow (0.5 , 0.5);
            this.s = S;
            int rows = s.getM_rows();
            int cols = s.getM_cols();
            //s.allocateStorage(rows, cols);
            //axform.getquadtree (*qtree);
            //queue = NULL;
            //astack = NULL;
            ha.allocate(2);
            hd.allocate(2);
            ha.setM_data(0,sqhalf);
       hd.setM_data(0,sqhalf);
       ha.setM_data(1, sqhalf);
            hd.setM_data(1, -sqhalf);
 selfWXform();
        }


     //public xformqtree(xformquadtree axform)
     {
         sqhalf = pow (0.5 , 0.5);
         s =axform.getmatrix ();
         int rows = s.getM_rows();
         int cols = s.getM_cols();
         s.allocateStorage(rows, cols);
         //axform.getquadtree (*qtree);
         //queue = NULL;
         //astack = NULL;
         ha.allocate(2);
         hd.allocate(2);
         ha.setM_data(0,sqhalf);
    hd.setM_data(0,sqhalf);
    ha.setM_data(1, sqhalf);
         hd.setM_data(1, -sqhalf);


     }

    // public xformqtree(qtreearray sample)
     {
         sqhalf = pow (0.5 , 0.5);
         int rows;
         int cols;
         //queue = null;
         //astack = null;
         ha.allocate(2);
         hd.allocate(2);
         ha.setM_data(0,sqhalf);
    hd.setM_data(0,sqhalf);
    ha.setM_data(1, sqhalf);
         hd.setM_data(1, -sqhalf);
         sample.gotoNode(0);
         quadnode temp;
         sample.getNode(temp);
         s.allocateStorage(temp.getRow(), temp.getCol());
         t.allocateStorage(temp.getRow(), temp.getCol());
         //getquadtree (sample);
     }

  */


  boolean epsilon ( double x, int th)
  {
      return epsilon (x, (double) th);
  }

  boolean epsilon ( double x, double thresh)
  {

      if ( java.lang.Math.abs(x) > thresh) return false;
      return true;
  }


  int findX2 (int i)
  {
      int r = i;
      int count = 0;
      int even = 1;
      while ( (r != 0) & ((r % 2) ==0) )
      {
          r = r /2;
          count++;
      }
      return count;
  }
  public matrixType getS() {
    return s;
  }
  public void setS(matrixType s) {
    this.s = s;
  }
  public matrixType getT() {
    return t;
  }
  public void setT(matrixType t) {
    this.t = t;
  }

  void getSstats (quadnode r)
  {
      double sum = 0.0;
      double sum2 = 0.0;
          double max = 0.0;
      double min = java.lang.Math.pow (10.0,37.0);
      int xll = r.getXll();
      int yur = r.getYur ();

    int i, j;
    for ( i = r.getXul(); i < xll; i++)
        for ( j = r.getYul(); j < yur; j++)
        {
            if ( max <= s.getM_data(i,j)) max = s.getM_data(i,j) ;
            if ( min >= s.getM_data(i,j)) min = s.getM_data(i,j);
        }
            r.setMax (max);
    r.setMin ( min);

      //r.setL2energy (s.getEnergy());

  }
  public qtreearray getQtree() {
    return qtree;
  }
  public void setQtree(qtreearray qtree) {
    this.qtree = qtree;
  }


   void getNode (quadnode r)
  {
       r = qtree.getTree(qtree.getCurrent());
  }



  void setAnode (quadnode r, quadnode a)
  {
      if(((r.getRows() % 2) == 0) & ((r.getCols() % 2) ==0))
      {
          int rows = (r.getRows())/2;
          int cols = (r.getCols())/2;
          a.setRows(rows);
          a.setCols (cols);
          a.setXul(r.getXul());
          a.setXll(r.getXul() + rows - 1);
          a.setXur(r.getXur());
          a.setXlr(r.getXur() + rows - 1);
          a.setYul(r.getYul());
          a.setYur(r.getYul() + cols - 1);
          a.setYll(r.getYll());
          a.setYlr(r.getYll() + cols - 1);
          a.setIsnotnull (true);
          a.setId(r.getId() * 4 + 1);
          a.setEpsilon(r.getEpsilon());
      } else a.setIsnotnull(false);

  }

  void setVnode (quadnode r, quadnode a)
  {
      if(((r.getRows() % 2) == 0) & ((r.getCols() % 2) ==0))
      {
          int rows = (r.getRows())/2;
          int cols = (r.getCols())/2;
          a.setRows(rows);
          a.setCols(cols);
          a.setXul (r.getYul());
          a.setYll (r.getYul() + rows - 1);
          a.setXur (r.getYur());
          a.setXlr (r.getYur() + rows - 1);
          a.setYul (r.getYul() + cols);
          a.setYll (r.getYul() + cols);
          a.setYur (r.getYur());
          a.setYlr (r.getYlr());
          a.setIsnotnull(true);
          a.setId (r.getId() * 4 + 2);
          a.setEpsilon (r.getEpsilon());
      } else a.setIsnotnull(false);
  }

  void setHnode (quadnode r, quadnode a)
  {
      if(((r.getRows() % 2) == 0) & ((r.getCols() % 2) ==0))
      {
          int rows = (r.getRows())/2;
          int cols = (r.getCols())/2;
          a.setRows(rows);
          a.setCols(cols);


          a.setXul(r.getXul() + rows);
          a.setXll(r.getXll());
          a.setXur(r.getXur() + rows);
          a.setXlr(r.getXlr());
          a.setYul(r.getYul());
          a.setYll(r.getYll());
          a.setYur(r.getYul() + cols - 1);
          a.setYlr(r.getYll() + cols - 1);
          a.setIsnotnull(true);
          a.setId( r.getId() * 4 + 3);
          a.setEpsilon (r.getEpsilon());
      } else a.setIsnotnull(false);

  }



  void setDnode (quadnode r, quadnode a)
  {
        if(((r.getRows() % 2) == 0) & ((r.getCols() % 2) ==0))
      {
       int rows = (r.getRows())/2;
       int cols = (r.getCols())/2;
       a.setRows(rows);
       a.setCols(cols);


          a.setXul(r.getXul() + rows);
          a.setXll(r.getXll());
          a.setXur(r.getXur() + rows);
          a.setXlr(r.getXlr());
          a.setYul(r.getYul() + cols);
          a.setYll(r.getYul() + cols);
          a.setYur(r.getYur());
          a.setYlr(r.getYlr());
          a.setIsnotnull(true);
          a.setId(r.getId() * 4 + 4);
          a.setEpsilon (r.getEpsilon());
      } else a.setIsnotnull(false);

  }


  void selfRowWXform (quadnode r)
  {

    int k, l, n, i;

    int o = ha.getSize();
    int p = hd.getSize();

    int xul = r.getXul();
    int xll = r.getXll();
    int yul = r.getYul();
    int yur = r.getYur();

    int s2 = r.getCols() / 2;
    int s1 = r.getCols();
    int rows = r.getRows();
    double a = 0;
    double d = 0;

    for (i = 0; i < rows; i++) {
      for (k = 0; k < s1; k++) {
        xTemp.setM_data(k , 0.0);
        xTemp2.setM_data(k ,0.0);
      }

      for (k = 0; k < s1; k++)
        for (l = 0; l < o; l++) {
          //n = s - (k -l ) -1;
          n = k - l;
          if ( (n >= 0) && (n < s1)) {
            a = (s.getM_data(i + xul, n + yul) * ha.getM_data(l)) + xTemp.getM_data(k);
            d = (s.getM_data(i + xul, n + yul) * hd.getM_data(l)) + xTemp2.getM_data(k);
            xTemp.setM_data( k, a);
            xTemp2.setM_data( k, d);
          }
        }

      for (k = 0; k < s2; k++) {
        s.setM_data(i + xul, k + yul,  xTemp.getM_data(2 * k + 1));
        s.setM_data (i + xul, k + s2 + yul, xTemp2.getM_data(2 * k + 1));

      }
    }
  }




  void selfColWXform (quadnode r)
  {


      int k, l, n, j;
      int o = ha.getSize();
      int p = hd.getSize();
      //int xul = r.getXul();
      //int xll = r.getXll();
      //int yul = r.getYul();
      int xul = 0;
      int yul = 0;
      //int yur = r.getYur();

      int s2 = r.getRows() /2;
      int s1 = r.getRows();
      int cols = r.getCols();
      //yTemp.allocate(s);
      j = cols / 2;
      double a, d;
      a = d = 0.0;
      for ( j = 0; j < cols; j++) {
          for ( k = 0; k < s1; k++)
          {
              yTemp.setM_data(k, 0.0);
              yTemp2.setM_data(k, 0.0);
          }
          for ( k = 0; k < s1; k++)
              for ( l = 0; l < o; l++)
              {
                  n = k -l;
                  if ( ( n >= 0) && ( n < s1)) {
            a = (s.getM_data(n + xul, j + yul) * ha.getM_data(l)) + yTemp.getM_data(k);
            d = (s.getM_data(n + xul, j + yul) * hd.getM_data(l)) + yTemp2.getM_data(k);

                      yTemp.setM_data(k, a);
                      yTemp2.setM_data(k, d);
                  }
              }

          for ( k = 0; k < s2; k++) {
                  s.setM_data(k+xul, j+yul,  yTemp.getM_data(2*k + 1));
                  s.setM_data(k+xul +s2, j + yul , yTemp2.getM_data(2*k+1));
          }
      }

  }





  public void selfWXform ()
  {
      int rows = s.getM_row();
      int cols = s.getM_cols();
      int lr = findX2 (rows);
      int lc = findX2 (cols);
      quadnode r;
      int res;
      if ( lr > lc ) res = lc;
      else
          res = lr;
          if ( ((rows / res) < 3 ) | (cols /res) < 3)
      res = res / 2;
      res = 1;
      int vertices = (int) java.lang.Math.pow(4.0, (double)res);
      resolution = vertices;


      qtree = new qtreearray();
      //queue = new queuearray(vertices + 1);
      qtree.initialize (s);

      System.out.print("Just initialized Q Tree");
      qtree.getStats (s, 0);

      r = qtree.getNode (0);
      xTemp = new myVector();
      yTemp = new myVector();
      xTemp2 = new myVector();
      yTemp2 = new myVector();

      xTemp.allocate(cols);
      yTemp.allocate(rows);
      xTemp2.allocate(cols);
      yTemp2.allocate(rows);


      selfWXform (r);
  }



  private void selfWXform (quadnode r)
   {
      quadnode temp; // temp2, a,v,h,d;
      //int ai, vi, hi, di;
      int index;

      //int length = resolution;
      int length4 = resolution/4 ;

      int i,j;
      System.out.println();
      System.out.println("Transforming");

      for ( index = 0; index < length4; index++)
      {
          temp = qtree.getNode (index);
          //selfRowWXform(temp);
          selfColWXform(temp);
          //qtree.getStats(s, r.getId());

      }


  }



  private void selfRowWIXform (quadnode r)
  {
      int startx = r.getXul();
      int starty = r.getYul();
      int endx = r.getXlr();
      int endy = r.getYur();
      int i,j;
      int lenx = r.getRows();
      int leny = r.getCols()/2;
      double sq2 = sqhalf;
      double S1, S2;
      S1 = S2 = 0.0;


      for ( i = 0; i < lenx; i++)
          for (j = 0; j < leny; j++)
          {
 S1 =  t.getM_data(i + r.getXul(), j + r.getYul()) - t.getM_data(i + r.getXul(), j + r.getYul() + leny) * sq2;
 S2 =  t.getM_data(i + r.getXul(),j + r.getYul()) + t.getM_data(i + r.getXul(), j + r.getYul() + leny)  * sq2;
              s.setM_data(i + r.getXul(), 2 * j + r.getYul(), S1);
              s.setM_data(i + r.getXul(), 2 * j + 1 + r.getYul() , S2);
          }

  }


  private void selfColWIXform (quadnode r)
  {
      int startx = r.getXul();
      int starty = r.getYul();
      int endx = r.getXlr();
      int endy = r.getYur();
      int i,j;
      int lenx = r.getRows() /2;
      int leny = r.getCols();
      double sq2 = sqhalf;
      double S1, S2;
 S1 = S2 = 0.0;


      for ( j = 0; j < leny; j++)
          for (i = 0; i < lenx; i++)
          {
 S1 = (s.getM_data(i + r.getXul(), j + r.getYul()) - s.getM_data(i + lenx + r.getXul(), j  + r.getYul()) ) * sq2;
 S2 = (s.getM_data(i + r.getXul(), j + r.getYul()) + s.getM_data(i + lenx + r.getXul(), j  + r.getYul()) ) * sq2;
              t.setM_data(i * 2 + r.getXul() +0,  j + r.getYul(), S1);
              t.setM_data(i *2  + r.getXul()+ 1, j + r.getYul() , S2);
          }
  }


  private boolean selfWIXform (quadnode r)
  {
      quadnode temp;
      //quadnode tempchild;
      //astack->push(r);
      int index;

      int length = resolution / 4;
      int i, j;

      for ( index = length - 1; index >= 0; index--)
      {
          temp = qtree.getNode(index);
          selfColWIXform(temp);
          selfRowWIXform(temp);

      }
      return true;
  }


  public void setmatrix (matrixType A)
  {
      s = A;

  }
  public matrixType getmatrix ()
  {
      return s;
  }


  public matrixType selfWXform (matrixType source)
  {

      setmatrix (source);
      //t = new matrixType();
      //t.allocateStorage(s.getM_row(),s.getM_cols());
      selfWXform();
      return getmatrix ();
  }



  public matrixType selfWIXform (matrixType source)
  {
      quadnode r;
      int lr, lc, res;
      int rows, cols;
      int index;
      setmatrix(source);
      qtree = new qtreearray ();
      //t = new matrixType();
      //t.allocateStorage(s.getM_row(),s.getM_cols());

      qtree.initialize (s);
      System.out.print("Just initialized Q Tree");
      r = qtree.getNode(0);
      rows = s.getM_row();
      cols = s.getM_cols();
      lr = findX2 (rows);
      lc = findX2 (cols);
      //int res;
      if ( lr > lc ) res = lc;
      else
          res = lr;
          if ( ((rows / res) < 3 ) | (cols /res) < 3)
      res = res / 2;
      res = 1;

      int vertices = (int) java.lang.Math.pow(4, res);
      resolution = vertices;
      //astack = new stackarray (vertices);
      //int qtlength = qtree.getLength();

      selfWIXform (r);
      return getmatrix();
  }




}

