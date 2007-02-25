//
//  basic_xform.h
//  haar-threaded-xform
//
//  Created by Daniel D. Beatty on 4/29/05.
//  Copyright 2005 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "basic_xform_protocol.h"
#import "matrixType.h"
#import "myVector.h"

@interface basic_xform : NSObject <basic_xform_server> {
	myVector *hA;
	myVector *hD;
	myVector *xTemp;
	myVector *xTemp2;
	myVector *yTemp;
	myVector *yTemp2;
	double sqhalf;
}


@end
