package jpackets1;


/**
 * <p>Title: Java Wavelet Packets Libraries</p>
 * <p>Description: Wavelet Packet Libraries </p>
 * <p>Copyright: Copyright (c) 2004</p>
 * <p>Company: Texas Tech University</p>
 * @author Daniel Beatty
 * @version 1.0
 */

public class myVector  {

  private double[] m_data;
  private int size;

  public myVector() {
    try {
      jbInit();
    }
    catch(Exception ex) {
      ex.printStackTrace();
    }
  }

  private void jbInit() throws Exception {
    //this.setLayout(borderLayout1);
  }
  public double[] getM_data() {
    return m_data;
  }
  public void setM_data(double[] m_data) {
    this.m_data = m_data;
  }
  public int getSize() {
    return size;
  }
  public void setSize(int size) {
    this.size = size;
  }

  public void setM_data (int index, double value)
  {
    m_data[index] = value;
  }
  public double getM_data (int index)
  {
    return m_data[index];
  }

public void allocate (int size)
{
  this.size = size;
  this.m_data = new double[size];
}

}
