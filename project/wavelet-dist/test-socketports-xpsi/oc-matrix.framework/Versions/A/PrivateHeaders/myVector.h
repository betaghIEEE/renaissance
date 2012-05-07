//
//  myVector.h
//  oc-matrix
//
//  Created by Daniel D. Beatty on 4/4/05.
//  Copyright 2005 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface myVector : NSObject {
	double *m_data;
	int size;
}

-(double) L2;
-(int) size; 
-(void) setSize: (int) n;
-(void) allocateWith: (int) i;
-(void) allocate;
-(void) deallocate;
-(myVector *) copy;
-(myVector *) addEqual: (myVector *) rhs;
-(myVector *) subEqual: (myVector *) rhs;
-(myVector *) divEqual: (double) rhs;
-(myVector *) mulEqual: (double) rhs;
//-(myVector *) crossProduct: (myVector *) rhs;
-(double) dotProduct: (myVector *) rhs;
-(double *) vector;
@end
