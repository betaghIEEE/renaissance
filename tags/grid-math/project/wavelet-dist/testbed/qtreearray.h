//
//  qtreearray.h
//  oc-matrix
//
//  Created by Daniel Beatty on 4/1/05.
//  Copyright 2005 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "quadnode.h"

@interface qtreearray : NSObject {
    int current;
    struct quadnode *tree;
    int length;
}

-(BOOL) allocate: (int) length;
-(BOOL) deallocate;
//-(BOOL) initialize (int i);
//-(BOOL) initialize (matrixType &S);
-(BOOL) findX2:(int )i;
-(BOOL) getNode:(struct quadnode *) valueAt: (int) i;
-(BOOL) setNode: (struct quadnode *) node At:(int) i;
-(quadnode *) getNode;
-(void) setNode (quadnode &value);

-(void) setIsNotNull: (int) value;
-(void) setIsNotNullAt: (int) index withValue: (int) value;
-(BOOL) getIsNotNull;
-(BOOL) getIsNotNull: (int) index;
-(void) getStats: ( matrixType *) S;
-(void) getStatsAt:(int) index with: ( matrixType *) S;

-(BOOL) div4At: (int) i withBase:(int) base;
-(void) killNode;
-(void) killNode: (int )i;
-(void) killNode: (quadnode *)node;

-(quadnode *) setNode (quadnode *) source;
-(BOOL) gotoNode (int) i;
-(int) getAAt:(int) pos;
-(int) getHAt:(int) pos;
-(int) getDAt:(int) pos;
-(int) getVAt:(int) pos;
-(int) getAAt:(int) pos;
-(int) getHAt:(int) pos;
-(int) getDAt:(int) pos;
-(int) getVAt:(int) pos;


/*
 int getXUL () { return tree[current].xul;}
 int getYUL () { return tree[current].yul;}
 int getXLL () { return tree[current].xll;}
 int getYLL () { return tree[current].yll;}
 int getXUR () { return tree[current].xur;}
 int getYUR () { return tree[current].yur;}
 int getXLR () { return tree[current].xlr;}
 int getYLR () { return tree[current].ylr;}
 int getRows () { return tree[current].rows;}
 int getCols () { return tree[current].cols;}
 double getL1 () { return tree[current].L1energy;}
 double getL2 () { return tree[current].L2energy;}
 double getMax () { return tree[current].max;}
 double getMin () { return tree[current].min;}
 double getEpsilon () { return tree[current].epsilon;}
 
 void initAnode ( int i);
 void initVnode ( int i);
 void initHnode ( int i);
 void initDnode ( int i);
 
 int getLength();
 int isleaf ();
 int isleaf (int i);
 int isgrandparent ();
 int isgrandparent (int i);
 
 */

@end
