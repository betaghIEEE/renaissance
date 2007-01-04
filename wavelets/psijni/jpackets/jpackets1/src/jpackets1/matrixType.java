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

public class matrixType extends JPanel {
  BorderLayout borderLayout1 = new BorderLayout();
  private double[][] m_data;
  private int m_row;
  private int m_cols;
  private int graylevel;

  public matrixType() {
    try {
      jbInit();
    }
    catch(Exception ex) {
      ex.printStackTrace();
    }
  }


  private void jbInit() throws Exception {
    this.setLayout(borderLayout1);
  }

  public static void main(String[] args) {
    matrixType matrixType1 = new matrixType();
  }

  public void allocateStorage (int m, int n) { m_data = new double[m][n];}

  public double[][] getM_data() {
    return m_data;
  }
  public void setM_data(double[][] m_data) {
    this.m_data = m_data;
  }
  public double getM_data(int row, int col) {
    return m_data[row][col];
  }
  public void setM_data(int row, int col, double m_data) {
    this.m_data[row][col] = m_data;
  }

  public int getGraylevel() {
    return graylevel;
  }
  public void setGraylevel(int gray) {
    this.graylevel = gray;
  }

  public int getM_row() {
    return m_row;
  }
  public void setM_row(int m_row) {
    this.m_row = m_row;
  }
  public int getM_cols() {
    return m_cols;
  }
  public void setM_cols(int m_cols) {
    this.m_cols = m_cols;
  }
  public matrixType multiplyby (double value)
  {
    int i, j;
    for ( i = 0; i< m_row; i++)
      for ( j = 0; j < m_cols; j++)
        m_data[i][j] *= value;
    return this;
  }
}
