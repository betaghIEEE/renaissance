#include "image.hpp"

int main(int argc, char *argv[])
{
  CImage image("hello.bmp");
  CImage rows = image.calcRowGradientFast();
  CImage columns = image.calcColGradientFast();
  rows.squareEachElement();
  columns.squareEachElement();
  cout << "Each elements squared\n";
  
  CImage total = rows + columns;
  total.squareRootEachElement();
  cout << "Each element sqrted\n";
  total.save("total.bmp");
  
  image.save("output.bmp");
  return (0);
}
