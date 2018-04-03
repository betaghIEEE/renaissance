package jpackets1;
import java.io.FileInputStream;
import java.io.FileReader;
import java.io.FileWriter;

/**
 * <p>Title: Java Wavelet Packets Libraries</p>
 * <p>Description: Wavelet Packet Libraries </p>
 * <p>Copyright: Copyright (c) 2004</p>
 * <p>Company: Texas Tech University</p>
 * @author Daniel Beatty
 * @version 1.0
 */

public class jpgmio {
  public jpgmio() {
  }

private int index;
private int state;

private int numprocess ( String rowstring)
{
	int row = 0;
	return numprocess (rowstring, row);
}

private int numprocess (String rowstring, int row)
{

  //int col;
  //index = 0;
  //int state = 0;
  int cstate = 3;
  int fstate = 5;
  int length = rowstring.length();
  int index = this.index;
  while ((index < length) & (state != fstate))
  {
    switch (state) {
      case 0: switch (rowstring.charAt(index)) {
        case ' ':
          index++;
          break;
        case '#':
            state = cstate;
            index++;
            break;
        case '\n': index++; break;

        case '0':
          row *= 10;
          index++;
          state=1;
          break;
        case '1':
          row =  row * 10 + 1;
          index++;
          state=1;
          break;
        case '2':
          row =  row * 10 + 2;
          index++;
          state = 1;
          break;
        case '3':
          row =  row * 10 + 3;
          index++;
          state=1;
          break;
        case '4':
          row =  row * 10 + 4;
          index++;
          state=1;
          break;
        case '5':
          row =  row * 10 + 5;
          index++;
          state=1;
          break;
        case '6':
          row =  row * 10 + 6;
          index++;
          state=1;
          break;
        case '7':
          row =  row * 10 +7;
          index++;
          state=1;
          break;
        case '8':
          row = row * 10 + 8;
          index++;
          state=1;
          break;
        case '9':
          row = row * 10 +9;
          index++;
          state = 1;
          break;
        default:
          index++;
      }
      break;
      case 1: switch (rowstring.charAt(index))
        {
          case ' ':
            //index++;
            state = fstate;
            break;
          case '#':
            state = fstate;
            break;
        case '\n': index++; state = fstate; break;
          case '0':
            row =row * 10 + 0;
            index++;
            break;
          case '1':
            row = row * 10 + 1;
            index++;
            break;
          case '2':
            row = row * 10 + 2;
	    index++;
            break;
          case '3':
            row = row * 10 + 3;
            index++;
            break;
          case '4':
            row = row * 10 + 4;
            index++;
            break;
          case '5':
            row = row * 10 + 5;
            index++;
            break;
          case '6':
            row = row * 10 + 6;
            index++;
            break;
          case '7':
            row = row * 10 + 7;
            index++;
            break;
          case '8':
            row = row * 10 + 8;
            index++;
            break;
          case '9':
            row = row * 10 + 9;
            index++;
            break;
          default:
            index++;

      } break;

    case 3: if (rowstring.charAt(index++) == '\n') state=0;break;
    case 5: break;
    default:state = fstate;

      }
      System.out.print("State of the reading");
      System.out.print(state);
      System.out.print(" ");
      System.out.print(index);
      System.out.print(" ");
      System.out.print(rowstring.charAt(index));

    }
    this.index = index;
    System.out.println("Reading Complete");
  return row;

}



  public matrixType getImagePGM2 (String filename) throws java.io.IOException
  {
    matrixType anImage = new matrixType();
    //java.io.FileInputStream fin = new java.io.FileInputStream (filename);
    FileInputStream fin;

    StringBuffer textFile = new StringBuffer();
    int charBytes;
    //fin = new FileInputStream (filename);
    //java.io.BufferedInputStream bin = new java.io.BufferedInputStream(fin);
    FileReader afile = new FileReader(filename);
    char[] line = new char[80];
    charBytes = 0;
    String P2 = "";
    String rowstring = null;
    String colstring = null;
    String grayscale = null;
    double pixel;
    int bytesread = 0 ;
    int bytestoread = 2;
    bytesread += afile.read(line,bytesread, bytestoread);
    P2 = new String(line);
    if (P2.equals("P2"))  return null;
    textFile.append("P2");
    charBytes = afile.read();
    bytestoread = 80;
    //  Readin file
    while ( charBytes != -1)
    {
      textFile.append((char)charBytes);
    charBytes = afile.read();
    }
    afile.close();

    // Process rows and cols:
    index = 3;
    int temp;
    System.out.println(textFile.charAt(index));
    int row = ( numprocess (textFile.toString()));
    System.out.println(row);
    state = 0;
    int col = ( numprocess (textFile.toString()));
    System.out.println(col);
    state = 0;
    //index++;
    int gray = numprocess(textFile.toString(),0);
    System.out.println(gray);
    double grayinv = 1.0/(double)gray;
    System.out.println(grayinv);

    // Process gray scale
    anImage.setM_row(row);
    anImage.setM_cols(col);
    anImage.setGraylevel(gray);
	anImage.allocateStorage(row,col);

    // Process body
    int i, j;
    for ( i = 0; i < row; i++)
      for ( j = 0; j < col; j++){
        state = 0;
        pixel = (double)numprocess(textFile.toString());

        anImage.setM_data(i, j, pixel);
        System.out.print(pixel);
        System.out.print(" ");
      }
    System.out.println();
    anImage.multiplyby(grayinv);
  return anImage;
  }

public void saveImagePGM2 (String filename, matrixType anImage) throws java.io.IOException
{
  FileWriter fileOut = new FileWriter(filename);
  fileOut.write("P2\n");
  String value;
  int row, col, gray;
  int i, j, level;
  Integer temprow;
  Integer tempcol;
  Integer tempgray;
  Integer temp;
  row = anImage.getM_row();
  col = anImage.getM_cols();
  gray = anImage.getGraylevel();
  temprow = new Integer(row);
  tempcol = new Integer(col);
  tempgray = new Integer(gray);
  //value = temp.toString();
  fileOut.write( temprow.toString());
  fileOut.write(" ");
  fileOut.write( tempcol.toString());
  fileOut.write("\n");
  fileOut.write(tempgray.toString());
  fileOut.write("\n");
  for ( i = 0; i < row; i++)
    for (j = 0; j < col; j++)
    {
      level = (int) (anImage.getM_data(i,j) * gray);
      if ( (level <= gray) & (level >= 0)) {
        temp = new Integer(level);
        fileOut.write(temp.toString());
        fileOut.write(" ");
      }
    }
  fileOut.close();



}

}
