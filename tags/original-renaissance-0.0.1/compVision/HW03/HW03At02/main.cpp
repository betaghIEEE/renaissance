//#include <iostream.h>
#include "calibrate.h"

int main (int argc, const char * argv[]) {
    // insert code here...
    //cout << "Hello, World!\n";
    //cout << "Get Left Parameters " << endl;
    const char* filenameL = "worldCoordinates";
    calibrate *leftcal = new calibrate (filenameL);
    leftcal->getMint();
    leftcal->getT();
    leftcal->getR();
    leftcal->getFocalLength();
    return 0;
}
