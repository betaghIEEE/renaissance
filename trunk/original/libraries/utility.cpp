#include "utility.hpp"
#include <time.h>

void tellTime(char *text) 
{
  static clock_t start;
  clock_t end = clock();
  cout << text << " took " << double(end-start)/CLOCKS_PER_SEC << " seconds" << endl;
  start = clock();
}
