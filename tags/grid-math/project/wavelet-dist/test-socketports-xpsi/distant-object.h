//
//  distant-object.h
//  test-socketports-xpsi
//
//  Created by Daniel D. Beatty on 4/22/05.
//  Copyright 2005 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface distant_object : NSObject {
	
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
