/*  createDigitHeaderFile.cpp
    
    This is a small segment of code that generates bitmap digit
    header files. Simply compile with renaissance and when
    running pipe the output to whatever header file you want
    to use.
*/


#include "stdlib.h"
#include "string.h"
#include "grayImage.hpp"
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
  int i, r, c;
  char filename[256];

  cout << "#ifndef __DIGITS_HPP__" << endl;
  cout << "#define __DIGITS_HPP__" << endl;
  cout << "\n\n";

  int cols[10];
  int rows[10];

  int maxRows = 0;
  int maxCols = 0;

  for (i=0; i<10; i++) {
    //changes this line to generate different images
    sprintf(filename, "../panRegister/%i.pbm", i);
    GrayImage I(filename);
    if (I.getRowSize() > maxRows)
      maxRows = I.getRowSize();
    if (I.getColSize() > maxCols)
      maxCols = I.getColSize();
    cols[i] = I.getColSize();
    rows[i] = I.getRowSize();
   }
     
   


  for (i=0; i<10; i++) {
    sprintf(filename, "../panRegister/%i.pbm", i);
    GrayImage I(filename);
    cout << "unsigned char _digitI" << i;
    cout << "[" << maxRows <<"][" << maxCols << "] = {" << endl;
    for (r=0; r<maxRows; r++) {
      cout << "\t{ ";
      for (c=0; c<maxCols; c++) {
	if (r < I.getRowSize() && c < I.getColSize())
	  cout << int(I[r][c]);
	else
	  cout << "0";
	if (c < maxCols-1)
	  cout << ", ";
      }
      if (r<maxRows-1)
	cout << "}, " << endl;
      else
	cout << "} " << endl;
    }
    cout << "\t};" << endl;
  }

  cout << "int digitRows[] = {";
  for (i=0; i<9; i++)
    cout << rows[i] << ", ";
  cout << rows[9] << "};" << endl;

  cout << "int digitCols[] = {";
  for (i=0; i<9; i++)
    cout << cols[i] << ", ";
  cout << cols[9] << "};" << endl;

  cout << "#endif // __DIGITS_HPP__" << endl;

  return (0);
}
