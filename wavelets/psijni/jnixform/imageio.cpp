#include "imageio.hpp"
#include <iostream.h>
#include <fstream.h>
#include <math.h>

/*imageType imageIO::getImage (const char* filename)
{
  cout << "Reading: " << filename << endl;
    
  //  try { 
    // Create an image object and read an image 
    Magick::Image image( filename ); 
    
    // Write to BLOB in GRAY format 
    Magick::Blob blob; 
    image.magick( "GRAY" ); 
    image.write( &blob ); 
    
    int rows = image.size().height();
    int cols = image.size().width();
    imageType temp(rows,cols);

    unsigned char *ptr = (unsigned char *) blob.data();
    for (int r=0; r<m_rows; r++)
      for (int c=0; c<m_cols; c++)
	temp.setValue (r,c, (*ptr++)/255.0); // convert to 0.0 - 1.0
    //  }
  //  catch( Exception &error_ ) {
  //    cout << "Caught exception: " << error_.what() << endl; 
  //    return false;
    //  } 
  return (this = temp); 
}


bool imageIO::saveImage (const char * filename)
{
  //create the block of data
    int m_rows = source.getRowSize();
    int m_cols = source.getColSize();
  unsigned char *block = new unsigned char[m_rows*m_cols];
  int value;
  int count = 0;
  for (int r=0; r<m_rows; r++)
    for (int c=0; c<m_cols; c++) {
      value = int( source.getValue(r,c)*255.0+0.5 ); // +0.5 to round
      if (value < 0)
	value = 0;
      if (value > 255)
	value = 255;
      block[count++] = (unsigned char) value;
    }
  void * ptr = (void *) block;

  char imageSize[64];
  Magick::Blob blob(ptr, m_cols*m_rows);
  sprintf(imageSize,"%dx%d",m_cols,m_rows);
  Magick::Geometry geo = imageSize;

  Magick::Image image(blob, geo, 8, "GRAY");
  //  Image image(m_cols, m_rows, "RGB", 0, ptr);

  string name = filename;
  string suffix(name, name.rfind(".")+1, name.size()-(name.rfind(".")+1));

  if (suffix=="jpg"||suffix=="JPG"||suffix=="jpeg"||suffix=="JPEG")
    image.magick("JPEG");
  else if (suffix=="PGM"||suffix=="pgm")
    image.magick("PGM+");
  else if (suffix=="GIF"||suffix=="gif")
    image.magick("GIF");
  else if (suffix=="PNG"||suffix=="png")
    image.magick("PNG");
  else if (suffix=="BMP"||suffix=="bmp")
    image.magick("BMP24");

  image.write(filename);

  delete []block;
  return true;
}
*/

/*
imageType imageIO::getImagePGM5 (const char* filename, matrixType &output)
{

	ifstream infile(filename);
	char buf[100]; 
	if (infile.bad()) 
		errMsg("readPGM: Unable to open file!");
	infile >> buf;
	if (buf != "P5")
		errMsg("readPGM: magic number is not P5!");
	infile >> output.m_cols;
	infile >> output.m_rows;
	infile >> output.NUMBER_OF_GREY_LEVELS;
	output.allocateStorage();
	infile.read(output.m_data[0],output.m_rows*output.m_cols);
	if (infile.bad())
		errMsg("Unable to read entire file!");
	infile.close();
}
*/
int imageIO::charToInt (char temp ) 
{
    int tempInt;
    switch (temp){
        case '1': tempInt = 1;
            break;
        case '2': tempInt = 2;
            break;
        case '3': tempInt = 3;
            break;
        case '4': tempInt = 4;
            break;
        case '5': tempInt = 5;
            break;
        case '6': tempInt = 6;
            break;
        case '7': tempInt = 7;
            break;
        case '8': tempInt = 8;
            break;
        case '9': tempInt = 9;
            break;
        case '0': 
        default:    
            tempInt = 0;
            
        
        
    };
    return tempInt;
        
}

void imageIO::getImagePGM2 (const char* filename, matrixType &output)
{

	ifstream infile(filename);
        char temp;
        int tempInt;
        int rows;
        int cols;
        char buf[100];
        double greyLevel;
        double value;
        
        
	if (infile.bad()) {
		cerr <<"readPGM: Unable to open file! " ;
                cerr << filename;
                    exit (1);
                }
	infile.getline ( buf, 100);
	if ((buf[0] != 'P') && (buf[1] !='2') ){
		cerr <<"readPGM: magic number is not P2!";
                exit (1);
                }
	
        //infile >> temp;
        cerr << temp;
        tempInt = 0;
        //while ((temp >= '0') && (temp <='9')){
           /// tempInt = tempInt * 10 + charToInt(temp);
            infile >> tempInt;
            cerr << tempInt;
            //}
        cols = tempInt;
        
	//infile >> temp;
        tempInt = 0;
        //while ((temp >= '0') && (temp <='9')){
          //  tempInt = tempInt * 10 + charToInt(temp);
            infile >> tempInt;
            cerr << tempInt;
            //}
        rows = tempInt;
        
	//infile >> temp;
        tempInt = 0;
        //while ((temp >= '0') && (temp <='9')){
          //  tempInt = tempInt * 10 + charToInt(temp);
            infile >> tempInt;
            //cout << temp;
            //}
        greyLevel = tempInt;
        output.NUMBER_OF_GREY_LEVELS = tempInt;
        //output.m_rows=rows;
        //output.m_cols= cols;
        cerr << " Before allocation \n";
        output.allocateStorage(rows, cols);
        cerr << "\n After allocation \n";
        //cout << "Row " << rows << "Cols " << cols; 
        for ( int i = 0; i < rows; i++){ 
        	//cout << endl;
	    for (int j = 0; j < cols; j++) {
                infile >> tempInt;
                value = tempInt/greyLevel;
                output[i][j] =value ;
		//cout << value << '\t';
         }
	}
	//infile.read(m_data[0],m_rows*m_cols);
        
	if (infile.bad()){
		cerr << "Unable to read entire file!";
                exit (1);
                }
        cerr << " \n After loop \n";
	infile.close();
        cerr << " \n After close \n";
        //this = tempImage;
        cerr << "Read the file";

}

bool imageIO::saveImagePGM2 (const char * filename, matrixType &input)
{
	int i;
	ofstream outfile(filename);
	outfile << "P2" << endl;
	outfile << input.m_cols << " " << input.m_rows << endl;
	outfile << 255 << endl; //this is 255
	
	for (int r=0; r< input.m_rows; r++) {
		for (int c=0; c< input.m_cols; c++)
                {
                    i = int (ceil (input[r][c] * 255));
                    if ( i > 255) i = 255;
                    if (i < 0 ) i = 0;
                    outfile << i;
                    outfile << " ";
                }
                outfile << endl;
            }

	outfile.close();
        return 1;
}

bool imageIO::saveImagePGM2u (const char * filename, matrixType &input)
{
    int i;
    ofstream outfile(filename);
    outfile << "P2" << endl;
    outfile << input.m_cols << " " << input.m_rows << endl;
    outfile << 255 << endl; //this is 255
    
    for (int r=0; r< input.m_rows; r++) {
        for (int c=0; c< input.m_cols; c++)
        {
            i = int (ceil (input[r][c] * 255));
            outfile << i;
            outfile << " ";
        }
        outfile << endl;
    }
    
    outfile.close();
    return 1;
}

