#include <iostream>
#include "EdgeAndLines.h"

int main (int argc, const char * argv[]) {
    // insert code here...
    //cout << "Hello, World!\n";
    //imageAcquistion *dataImage = new imageAcquistion();
    imageType myImage;
    myImage.getImagePGM2("../pictures/hello1.pgm");
    EdgeAndLines *testEdge = new EdgeAndLines ();
    //myImage = dataImage->getImage("hello.jpg");
    myImage.saveImagePGM2("../pictures/imageRegurge.pgm");
    testEdge->setImage(myImage);
    testEdge->computeImageEdges();
    myImage = testEdge-> getGaussianImage ();
    myImage.saveImagePGM2("../pictures/imageGaussian.pgm");
    myImage = testEdge->getJrow();
    myImage.saveImagePGM2("../pictures/Jrow.pgm");
    myImage = testEdge->getJcolumn();
    myImage.saveImagePGM2("../pictures/Jcol.pgm");
    myImage = testEdge->getEdgeStrength();
    myImage.saveImagePGM2("../pictures/EdgeStrength.pgm");
    myImage = testEdge->getEdgeDirection();
    myImage.saveImagePGM2("../pictures/EdgeDirection.pgm");
    myImage = testEdge->getIN();
    myImage.saveImagePGM2("../picturesPictures/IN.pgm");
    myImage = testEdge->getEdgeCardDirection();
    myImage.saveImagePGM2("../pictures/card.pgm");
    myImage = testEdge->getHysterisisOutline ();
    myImage.saveImagePGM2("../pictures/hyster.pgm");
    myImage = testEdge->getEdges();
    myImage.saveImagePGM2("../pictures/Edge.pgm");
    
    testEdge->parseMatrix();
    testEdge->obtainSlopeList(3.0);
    //testEdge->plotList();
    //myImage = testEdge.getImageLine();
    return 0;
}
