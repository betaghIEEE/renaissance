//
//  xpsi-service-clients.h
//  oc-matrix
//
//  Created by Daniel D. Beatty on 4/25/05.
//  Copyright 2005 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "xpsi-service-client.h"

@interface xpsi_service_clients : NSObject {
	
	NSMutableArray *xpsi_services;
	
	/* Added features for Bonjour */
	NSData *address;
	NSNetServiceBrowser *browser;
	NSMutableArray *services;
}


- (void) netServiceBrowser: (NSNetServiceBrowser *) aNetServiceBrowser 
			didFindService:(NSNetService *) aNetService 
				moreComing:(BOOL) moreComing;
-(void) netServiceBrowser:(NSNetServiceBrowser *) aNetServiceBrowser 
		 didRemoveService:(NSNetService *) aNetService 
			   moreComing:(BOOL) moreComing;


-(xpsi_service_client *) issueObjectLocal;
-(out bycopy xpsi_service_client *) issueObjectRemote;
-(void) receiveObjectLocal:(xpsi_service_client *) rtDO;
-(void) receiveObjectRemote:(in bycopy xpsi_service_client *) rtDO;
-(void) initiateSingleDO:(NSString *) hostname;
-(void) initiateCollectiveDO;
-(id)	accessObjectLocalAt:(int) index;


@end
