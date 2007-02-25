//
//  xpsi-service-client.h
//  oc-matrix
//
//  Created by Daniel D. Beatty on 4/25/05.
//  Copyright 2005 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface xpsi_service_client : NSObject {
	NSData *address;
	NSString *serverHostname;
	id proxy;
	BOOL bonjourType;
}
-(id) initWithService:(NSString * ) serviceName;
-(id) initWithAddress:(NSData * ) addresses;
	// Connect to the server

- (void) connectWithAddress:(NSData *) newAddress;
- (void) connectWithHostname:(NSString *) name;


-(id) objectCall;

@end
