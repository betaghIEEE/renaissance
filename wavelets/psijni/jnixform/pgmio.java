

import java.io.*;

public class pgmio {
    
    qtreearray qtree;
    quadnode qnode;
    double temp;
    
    public void setvalue (int index) { 
        qnode.values[index] = temp; 
        //System.out.println(index);
    };
    public void settemp (double value) {
        //System.out.println(" Temp = " + value); 
        temp = value; 
    }
    
    public void setRow (int value) { qnode.rows = value;}
    public void setCol (int value) { qnode.cols = value;}
    
    public void setvalue (int index, double value) { qnode.values[index] = value;}
    public void allocate (int size) { qnode.length = size; qnode.values = new double[size]; System.out.println("Allocation complete " + size); }
    public void setGrey (int grey) { System.out.println ("Java Grey level " + grey); qnode.NUMBER_OF_GREY_LEVELS = grey;}
    
    public static int checkchar (char test)
    {
        int temp = 0;
        if ( (test >= '0') & (test <= '9'))
            temp = (int) test - (int)'0';
        else temp = -1;
        return temp;
    }
    public static int getInt (String value, int start)
    {
        boolean quit = false;
        int i = start;
        int cap = 0;
        char temp = '0';
        int test;
        while ( (i < value.length()) & !quit)
        {
            temp = value.charAt(i++);
            test = checkchar (temp);
            if ((test >= 0) & ( test <= 9)) cap = cap * 10 + test;
            else quit = true;
        }
        start = i;
        return cap;
    }
    
    public static void parsePGM (byte data[])
{
        int length = data.length;
        int i;
        String value = new String(data);
        i = 0;
        while ( i < length ) System.out.println( getInt (value, i) +" " + i);
        
        
        
        
}


    public void saveImagePGM2 ( String filename) 
    {
        int i, j;
        int len;
        int value;
        FileOutputStream outfile;
        try {
            outfile= new FileOutputStream(filename);
            byte data[] = new byte[1024];
            String P2 = "P2";
            String comment = "";
            String RC =  "\n" + String.valueOf(qnode.cols) + " " + String.valueOf(qnode.rows) + "\n";
            RC += String.valueOf(qnode.NUMBER_OF_GREY_LEVELS) + "\n";
            String body = "";
            for ( i = 0; i < 2; i++)
                data[i] = (byte) P2.charAt(i);
            data[2] = '\n';
            for ( i = 0; i < comment.length(); i++)
                data[i+3] = (byte) comment.charAt(i);
            for ( i = 0; i < RC.length(); i++)
                data[i + comment.length() + P2.length()] = (byte)RC.charAt(i);
            data[comment.length() + RC.length() + P2.length()] = '\n';
            outfile.write (data, 0, P2.length() + comment.length() + RC.length());
            //parsePGM(data);
            System.out.println(getInt ( RC, 0));
            System.out.println(getInt (RC, 5));
            for ( i = 0; i < qnode.length; i++) 
            {
                value = (int)(qnode.values[i] * qnode.NUMBER_OF_GREY_LEVELS);
                body = new String (value + " ");
                len = body.length();
                data = new byte [len];
                //if ( len > 4 ) len = 4;
                for ( j = 0 ; j < len; j++){
                    data[j] = (byte) body.charAt(j);
                }
                outfile.write(data);
            }
        } catch ( IOException e ) { System.out.println (e); }
    }

static {
    System.loadLibrary("pgmio"); 
} 

public native void getPGM (String filename) ;
private native void savePGM (String filename);
    
    public static void main (String args[])
    {
        pgmio test = new pgmio();
        test.qtree = new qtreearray(1);
        test.qnode = new quadnode();
        test.getPGM ("swalla10.pgm");
        test.saveImagePGM2 ("test.pgm");
    }
}