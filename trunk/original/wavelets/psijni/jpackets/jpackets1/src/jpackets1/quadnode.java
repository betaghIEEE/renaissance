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

public class quadnode extends JPanel {
  BorderLayout borderLayout1 = new BorderLayout();
  private int xul;
  private int yul;
  private int xur;
  private int yur;
  private int xll;
  private int yll;
  private int xlr;
  private int ylr;
  private int rows;
  private int cols;
  private double l2energy;
  private double max;
  private double min;
  private double epsilon;
  private boolean isnotnull;
  private int id;
  private int NUMBER_OF_GREY_LEVELS;

  public quadnode() {
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
  public int getXul() {
    return xul;
  }
  public void setXul(int xul) {
    this.xul = xul;
  }
  public int getYul() {
    return yul;
  }
  public void setYul(int yul) {
    this.yul = yul;
  }
  public int getXur() {
    return xur;
  }
  public void setXur(int xur) {
    this.xur = xur;
  }
  public int getYur() {
    return yur;
  }
  public void setYur(int yur) {
    this.yur = yur;
  }
  public int getXll() {
    return xll;
  }
  public void setXll(int xll) {
    this.xll = xll;
  }
  public int getYll() {
    return yll;
  }
  public void setYll(int yll) {
    this.yll = yll;
  }
  public int getXlr() {
    return xlr;
  }
  public void setXlr(int xlr) {
    this.xlr = xlr;
  }
  public int getYlr() {
    return ylr;
  }
  public void setYlr(int ylr) {
    this.ylr = ylr;
  }
  public int getRows() {
    return rows;
  }
  public void setRows(int rows) {
    this.rows = rows;
  }
  public int getCols() {
    return cols;
  }
  public void setCols(int cols) {
    this.cols = cols;
  }
  public double getL2energy() {
    return l2energy;
  }
  public void setL2energy(double l2energy) {
    this.l2energy = l2energy;
  }
  public double getMax() {
    return max;
  }
  public void setMax(double max) {
    this.max = max;
  }
  public double getMin() {
    return min;
  }
  public void setMin(double min) {
    this.min = min;
  }
  public double getEpsilon() {
    return epsilon;
  }
  public void setEpsilon(double epsilon) {
    this.epsilon = epsilon;
  }
  public boolean isIsnotnull() {
    return isnotnull;
  }
  public void setIsnotnull(boolean isnotnull) {
    this.isnotnull = isnotnull;
  }
  public int getId() {
    return id;
  }
  public void setId(int id) {
    this.id = id;
  }
  public int getNUMBER_OF_GREY_LEVELS() {
    return NUMBER_OF_GREY_LEVELS;
  }
  public void setNUMBER_OF_GREY_LEVELS(int NUMBER_OF_GREY_LEVELS) {
    this.NUMBER_OF_GREY_LEVELS = NUMBER_OF_GREY_LEVELS;
  }
}
