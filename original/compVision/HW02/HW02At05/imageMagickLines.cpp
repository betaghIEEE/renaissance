/*
 *  imageMagickLines.cpp
 *  HW02At05
 *
 *  Created by Daniel Beatty on Mon Feb 25 2002.
 *  Copyright (c) 2001 Texas Tech University. All rights reserved.
 *
 */

#include "imageMagickLines.h"

imageType imageAcquistion::getImage (const char* filename);
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
  return temp; 
}


bool imageAcquistion::saveImage (const char * filename, imageType &source)
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