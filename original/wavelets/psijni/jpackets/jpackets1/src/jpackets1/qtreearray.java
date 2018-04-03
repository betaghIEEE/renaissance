package jpackets1;

import java.awt.*;
import javax.swing.JPanel;

/**
 * <p>Title: Java Wavelet Packets Libraries</p>
 * <p>Description: Wavelet Packet Libraries </p>
 * <p>Copyright: Copyright (c) 2004</p>
 * <p>Company: Texas Tech University</p>
 * @author Daniel Beatty
 * @version 1.0
 */

public class qtreearray extends JPanel {
  BorderLayout borderLayout1 = new BorderLayout();
  private int current;
  private int length;
  private quadnode[] tree;

  public qtreearray() {
    try {
      jbInit();

    }
    catch(Exception ex) {
      ex.printStackTrace();
    }
  }

  public qtreearray(int size) {
    try {
      jbInit();
      length = size;
      tree = new quadnode[size];
      current = 0;
    }
    catch(Exception ex) {
      ex.printStackTrace();
    }
  }


  private void jbInit() throws Exception {
    this.setLayout(borderLayout1);

  }
  public int getCurrent() {
    return current;
  }
  public void setCurrent(int current) {
    this.current = current;
  }
  public int getLength() {
    return length;
  }
  public void setLength(int length) {
    this.length = length;
  }
  public quadnode[] getTree() {
    return tree;
  }
  public void setTree(quadnode[] tree) {
    this.tree = tree;
  }

  public quadnode getTree(int index) {
    return tree[index];
  }
  public void setTree(int index, quadnode tree) {
    this.tree[index] = tree;
  }


  int getA()
  {
        int a = current * 4 + 1;
        return a;
  }

    int getH()
    {
        int a = current * 4 + 3;
        return a;
      }

      int getD()
      {
        int a = current * 4 + 4;
        return a;
      }

      int getV()
      {
        int a = current * 4 + 2;
        return a;
      }
      int getA(int pos)
      {
            int a = pos * 4 + 1;
            return a;
      }

        int getH(int pos)
        {
            int a = pos * 4 + 3;
            return a;
          }

          int getD(int pos)
          {
            int a = pos * 4 + 4;
            return a;
          }

          int getV(int pos)
          {
            int a = pos * 4 + 2;
            return a;
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


          public void getStats (matrixType S, int index)
          {
            double sum = 0.0;
            double sum2 = 0.0;
            double max = 0.0;
            double min = java.lang.Math.pow (10,37.0);
            int i,j;
            for ( i = tree[index].getXul(); i < tree[index].getXll(); i++)
              for ( j = tree[index].getYul(); j < tree[index].getYur(); j++)
              {
                if ( max <= S.getM_data(i,j)) max = S.getM_data(i,j);
                if ( min >= S.getM_data(i,j)) min = S.getM_data(i,j);
                sum += S.getM_data(i,j);
                sum += S.getM_data(i,j) * S.getM_data(i,j);
              }

          }

          public quadnode getNode (int index)
          {
            return tree[index];
          }

private void initAnode (int i)
{
  int root = ( i -1)/4;
  tree[i].setRows(tree[root].getRows()/2);
  tree[i].setCols(tree[root].getCols()/2);
  tree[i].setXul(tree[root].getXul());
  tree[i].setXll(tree[root].getXul()+ tree[i].getRows()-1);
  tree[i].setXur(tree[root].getXur());
  tree[i].setXlr(tree[root].getXur() + tree[i].getRows()-1);

  tree[i].setYul(tree[root].getYul());
  tree[i].setYur(tree[root].getYul() + tree[i].getCols() -1);
  tree[i].setYll(tree[root].getYll());
  tree[i].setYlr(tree[root].getYll() + tree[i].getCols() -1);
  tree[i].setIsnotnull(false);
  tree[i].setId(i);

}
private void initVnode (int i)
{
  int root = ( i -1)/4;
  tree[i].setRows(tree[root].getRows()/2);
  tree[i].setCols(tree[root].getCols()/2);
  tree[i].setXul(tree[root].getXul());
  tree[i].setXll(tree[root].getXul()+ tree[i].getRows()-1);
  tree[i].setXur(tree[root].getXur());
  tree[i].setXlr(tree[root].getXur()+ tree[i].getRows()-1);

  tree[i].setYul(tree[root].getYul() + tree[i].getCols());
  tree[i].setYur(tree[root].getYur());
  tree[i].setYll(tree[root].getYll() + tree[i].getCols());
  tree[i].setYlr(tree[root].getYlr());
  tree[i].setIsnotnull(false);
  tree[i].setId(i);

}

private void initHnode (int i)
{
  int root = ( i -1)/4;
  tree[i].setRows(tree[root].getRows()/2);
  tree[i].setCols(tree[root].getCols()/2);
  tree[i].setXul(tree[root].getXul()+ tree[i].getRows());
  tree[i].setXll(tree[root].getXll());
  tree[i].setXur(tree[root].getXur()+ tree[i].getRows());
  tree[i].setXlr(tree[root].getXlr());

  tree[i].setYul(tree[root].getYul());
  tree[i].setYur(tree[root].getYul() + tree[i].getCols()-1);
  tree[i].setYll(tree[root].getYll() );
  tree[i].setYlr(tree[root].getYll() + tree[i].getCols() -1);
  tree[i].setIsnotnull(false);
  tree[i].setId(i);

}
private void initDnode (int i)
{
  int root = ( i -1)/4;
  tree[i].setRows(tree[root].getRows()/2);
  tree[i].setCols(tree[root].getCols()/2);
  tree[i].setXul(tree[root].getXul()+ tree[i].getRows());
  tree[i].setXll(tree[root].getXll());
  tree[i].setXur(tree[root].getXur()+ tree[i].getRows());
  tree[i].setXlr(tree[root].getXlr());

  tree[i].setYul(tree[root].getYul() + tree[i].getCols());
  tree[i].setYur(tree[root].getYur());
  tree[i].setYll(tree[root].getYll() + tree[i].getCols());
  tree[i].setYlr(tree[root].getYlr());


  tree[i].setIsnotnull(false);
  tree[i].setId(i);

}

public void initialize (matrixType A)
{
  int row = A.getM_row();
  int col = A.getM_cols();
  int i, test;
  int lc = findX2 (col);
  int lr = findX2 (row);
  double treesize;
  System.out.println();
  System.out.print ("The highest power of two is ");
  int lp;
  if  (lc < lr) lp = lc; else lp = lr;
  System.out.print (lp);
  treesize = (java.lang.Math.pow(4.0,(float)lp));
  length = (int)treesize;
  tree = new quadnode [length];
  System.out.print(" and the length is ");
  System.out.println(length);
  tree[0] = new quadnode();
  tree[0].setXul(0);
  tree[0].setYul(0);
  tree[0].setXll(0);
  tree[0].setXur(0);
  tree[0].setYur(0);
  tree[0].setRows(row);
  tree[0].setCols(col);
  System.out.print("Set up root of tree");
  for ( i = 1; i < length; i++)
  {
    test = ( i - 1) % 4;
      tree[i] = new quadnode();
    switch (test){
      case 0: initAnode(i);
        break;
      case 1: initVnode(i); break;
      case 2: initHnode(i); break;
      case 3: initDnode(i); break;
    }
        System.out.print("Setting up node ");
        System.out.println(i);

  }



}
}
