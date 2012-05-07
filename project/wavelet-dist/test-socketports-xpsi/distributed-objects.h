//
//  distributed-objects.h
//  test-socketports-xpsi
//
//  Created by Daniel D. Beatty on 4/24/05.
//  Copyright 2005 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "distributed-object.h"

@interface distributed_objects : NSObject {
	NSMutableArray *services;
	

}
- (void) netServiceBrowser: (NSNetServiceBrowser *) aNetServiceBrowser 
			didFindService:(NSNetService *) aNetService 
				moreComing:(BOOL) moreComing;
-(void) netServiceBrowser:(NSNetServiceBrowser *) aNetServiceBrowser 
		 didRemoveService:(NSNetService *) aNetService 
			   moreComing:(BOOL) moreComing


-(distributed_object *) issueObjectLocal;
-(out bycopy distributed_object *) issueObjectRemote;
-(void) receiveObjectLocal(distributed_Object *);
-(void) receiveObjectRemote(in bycopy distributed_object *);
-(void) initiateSingleDO:(NSString *) hostname;
-(void) initiateCollectiveDO;
-(id)	accessObjectLocalAt:(int) index;

@end
