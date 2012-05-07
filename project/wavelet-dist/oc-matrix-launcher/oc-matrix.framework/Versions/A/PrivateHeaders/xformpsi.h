//
//  xformpsi.h
//  oc-matrix
//
//  Created by Daniel Beatty on 4/1/05.
//  Copyright 2005 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
//#import "quadnode.h"
#import "quadnode.h"
#import "matrixType.h"
#import "myVector.h"

@interface xformpsi : NSObject {
    //performmeasure pm;
    matrixType *S;
    matrixType *T;
    //quadnode *qtree;
    double *histogram;
    int current;
    //queuearray *queue;
    //stackarray *astack;
    myVector *ha;
    myVector *hd;
    myVector *xTemp;
    myVector *xTemp2;
    myVector *yTemp;
    myVector *yTemp2;
    double sqhalf;
    int resolution;
    int treesize;
    int histosize;
    int aval, vval, hval, dval;
}


-(void) getSstats: (struct quadnode * ) r;
//-(void) patchEnergy;

//-(void) generateHistogram;
-(void) rowWXform;
-(void) colWXform;
    //void selfWXform ();

-(void) WXform;
-(void) rowWIXform;
-(void) colWIXform;
-(BOOL) WIXform;

-(BOOL) WIXformWithResolution: (int) res;
-(BOOL) WXformWithResolution: (int) res;

-(matrixType *) xiFormMatrix: (matrixType *) A withResolution:(int) res;
-(matrixType *) xFormMatrix: (matrixType *) A withResolution:(int) res;

-(void) setMatrix: (matrixType *)A;
-(matrixType *) matrix;


@end
