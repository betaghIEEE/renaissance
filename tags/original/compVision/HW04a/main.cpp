
//#include "matrixType.h"
//#include <iostream>
#include "correlate.h"
//#include <mathlink.h>

int main (int argc, const char * argv[]) {
    // insert code here...
 /*   MLENV env;
    MLINK link;
    long errno;
    char *param = "";
    cout << "Hello, World!\n";
    
    env = MLInitialize(0);
    //link = MLOpenArgv(env, argv, argv+argc, &errno);
    link = MLOpenString( env, param, &errno);
    MLActivate(link);
    */
    //calType *cal = new calType();
    //cal->computeSVD(argc,argv);
    const char* leftFigure = "leftParameters";
    const char* rightFigure = "rightParameters";
    const char* leftImage = "leftDan.pgm";
    const char* rightImage = "rightDan.pgm";
    const char* worldPic = "worldPic.wrl";
    
    correlate *view = new correlate (leftFigure, rightFigure);
    view->computeViaRaw(leftImage,rightImage);
    view->computeWorldPoints();
    view->produceVRML(worldPic);
     
    return 0;
}
