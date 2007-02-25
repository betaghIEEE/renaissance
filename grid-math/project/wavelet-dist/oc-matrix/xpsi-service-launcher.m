//
//  xpsi-service-launcher.m
//  oc-matrix
//
//  Created by Daniel D. Beatty on 4/25/05.
//  Copyright 2005 __MyCompanyName__. All rights reserved.
//

#include <sys/socket.h>
#import "xpsi-service-launcher.h"
#import "ConnectionMonitor.h"
#import "xformpsi.h"

@implementation xpsi_service_launcher


-(void) launchService:(NSString *) serviceName atPort:(int) port withType:(NSString *) serviceType
{
	NSNetService *netService;
	
	NSRunLoop *runloop = [ NSRunLoop currentRunLoop];
	ConnectionMonitor *monitor = [[ ConnectionMonitor alloc] init]; 
	xformpsi *xpsi = [[xformpsi alloc] init]; 
	
	// Magic fix for socketPort / host leaks
	if ( [NSHost respondsToSelector:@selector(_fixNSHostLeak)] ){
		[NSHost _fixNSHostLeak];
	}
	
	if ( [NSSocketPort respondsToSelector:@selector(_fixNSSocketPortLeak) ] )
	{
		[NSSocketPort _fixNSSocketPortLeak];
	}
	NSLog (@"Attempting to open a port");
	NS_DURING 
		// This server will wait for requests on port 8081
		receivePort = [[NSSocketPort alloc] initWithTCPPort:port ]; 
	NS_HANDLER
		NSLog(@"unable to get port %d", port);
		exit(-1);
	NS_ENDHANDLER
	
	// Create the connection object 
	connection = [ NSConnection connectionWithReceivePort: receivePort
												 sendPort:nil];
	// The receivePort is retained by the connection
	[receivePort release];
	
	// When clients use this connection, they will talk to the ChatterServer
	[connection setRootObject:xpsi];
	
	// The chatter server is retained by the connection 
	[xpsi release];
	
	// Set up the monitor object 
	[connection setDelegate:monitor ];
	[[NSNotificationCenter defaultCenter] addObserver:monitor
											 selector:@selector(connectionDidDie:)
												 name:NSConnectionDidDieNotification
											   object:nil];
	netService = [[NSNetService alloc] initWithDomain:@""
												 type:serviceType 
												 name:serviceName 
												 port:port];
	[netService publish];
	NSLog(@"service published = %@", netService);
	
	// Start the run loop
	[runloop run];
	[connection release]; 
	[monitor release];
	[netService release];
}

@end
