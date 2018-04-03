package jpackets1;

/**
 * <p>Title: Java Wavelet Packets Libraries</p>
 * <p>Description: Wavelet Packet Libraries </p>
 * <p>Copyright: Copyright (c) 2004</p>
 * <p>Company: Texas Tech University</p>
 * @author Daniel Beatty
 * @version 1.0
 */

public class cmddrive {
  public cmddrive() {
  }
  public static void main(String[] args) {
    cmddrive cmddrive1 = new cmddrive();
    matrixType anImage;
    matrixType transformed;
    matrixType answer;
    jpgmio reader = new jpgmio();
    xformqtree xt;
    System.out.println("At least it is running");
    try {
      anImage = reader.getImagePGM2("watch.pgm");
      reader.saveImagePGM2("sample.pgm", anImage);
      xt = new xformqtree();
      transformed = xt.selfWXform(anImage);
      reader.saveImagePGM2("transformed.pgm",transformed);
      //answer = xt.selfWIXform(transformed);
      //reader.saveImagePGM2("restored.pgm", answer);
    } catch (java.io.IOException anerror) {
      System.out.println("That was fubar");
    }

  }

}
