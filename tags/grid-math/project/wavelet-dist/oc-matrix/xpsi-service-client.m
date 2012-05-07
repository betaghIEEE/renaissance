//
//  xpsi-service-client.m
//  oc-matrix
//
//  Created by Daniel D. Beatty on 4/25/05.
//  Copyright 2005 __MyCompanyName__. All rights reserved.
//

#import "xpsi-service-client.h"
#import "xpsi-service-protocol.h"
#include <sys/socket.h>

@implementation xpsi_service_client

// Private method to clean up connection and proxy 
// Seems to be leaking NSSocketPorts
-(void) cleanup 
{
	NSConnection *connection = [proxy connectionForProxy];
	[[NSNotificationCenter defaultCenter] removeObserver:self];
	[connection invalidate];
	[proxy release];
	proxy = nil;
}

// Delegate methods

// If the connection goes down, do clean up
- (void) connectionDown:(NSNotification *)note 
{
	NSLog (@"connectionDown:");
	[self cleanup];
}






// Connect to the server
- (void) connectBonjour
{
	BOOL successful;
	NSConnection *connection;
	NSSocketPort *sendPort;
	
	// Create the send port
	//sendPort = [[NSSocketPort alloc] initRemoteWithTCPPort:7000 
	//												  host:serverHostname];
	
	sendPort = [[NSSocketPort alloc] initRemoteWithProtocolFamily:AF_INET 
													   socketType:SOCK_STREAM 
														 protocol:INET_TCP 
														  address:address];
	
	NSLog (@"Address %@,  socketType %d", serverHostname, [sendPort socketType]);
	// Create an NSConnection 
	connection = [NSConnection connectionWithReceivePort:nil
												sendPort:sendPort];
	
	// Set timeouts to something reasonable
	[connection setRequestTimeout:100.0];
	[connection setReplyTimeout:100.0];
	
	// The send port is retained by the connection 
	[sendPort release];
	NSLog (@"Connecting to chatter server");
	
	NS_DURING
		// Get the proxy
		NSLog (@"Retaining proxy...");
		proxy = [[ connection rootProxy] retain];
		NSLog (@"Proxy retained");
		// Get informed when the connection fails
		[[NSNotificationCenter defaultCenter] addObserver: self 
												 selector:@selector(connectionDown:) 
													 name:NSConnectionDidDieNotification 
												   object:connection];
		// By telling the proxy about the protocol for the object
		// it represent, we significantly reduce the network
		// traffic involved in each invocation 
		NSLog (@"Notification Center beginning");
		[proxy setProtocolForProxy:@protocol (xpsiServiceProtocol)];
		NSLog (@"proxy set");
		// Try to subscribe with chosen nickname
		//successful = [proxy subscribeClient:self];
		NSLog (@"Proxy tried subscription");
	NS_HANDLER 
			// If the server does not respond in 10 seconds
			//	this handler will get called 
			NSLog(@"Unable to connect");
			[self cleanup];
	NS_ENDHANDLER
		//NSLog (@"Did not even try to connect");
}


- (void) connectStraight
{
	BOOL successful;
	NSConnection *connection;
	NSSocketPort *sendPort;
	
	// Create the send port
	sendPort = [[NSSocketPort alloc] initRemoteWithTCPPort:7000 
													  host:serverHostname];
	
	//sendPort = [[NSSocketPort alloc] initRemoteWithProtocolFamily:AF_INET 
	//												   socketType:SOCK_STREAM 
	//													 protocol:INET_TCP 
	//													  address:address];
	
	NSLog (@"Address %@,  socketType %d", serverHostname, [sendPort socketType]);
	// Create an NSConnection 
	connection = [NSConnection connectionWithReceivePort:nil
												sendPort:sendPort];
	
	// Set timeouts to something reasonable
	[connection setRequestTimeout:100.0];
	[connection setReplyTimeout:100.0];
	
	// The send port is retained by the connection 
	[sendPort release];
	NSLog (@"Connecting to chatter server");
	
	NS_DURING
		// Get the proxy
		NSLog (@"Retaining proxy...");
		proxy = [[ connection rootProxy] retain];
		NSLog (@"Proxy retained");
		// Get informed when the connection fails
		[[NSNotificationCenter defaultCenter] addObserver: self 
												 selector:@selector(connectDown:) 
													 name:NSConnectionDidDieNotification 
												   object:connection];
		// By telling the proxy about the protocol for the object
		// it represent, we significantly reduce the network
		// traffic involved in each invocation 
		NSLog (@"Notification Center beginning");
		[proxy setProtocolForProxy:@protocol (xpsiServiceProtocol)];
		NSLog (@"proxy set");
		// Try to subscribe with chosen nickname
		//successful = [proxy subscribeClient:self];
		NSLog (@"Proxy tried subscription");
	NS_HANDLER 
		// If the server does not respond in 10 seconds
		//	this handler will get called 
		NSLog(@"Unable to connect");
		[self cleanup];
	NS_ENDHANDLER
	//NSLog (@"Did not even try to connect");
}

- (void) connectWithHostname:(NSString *) name
{
	[serverHostname release];
	serverHostname = name;
	[serverHostname retain];
	[self connectStraight];
	bonjourType = NO;
}



- (void) connectWithAddress:(NSData *) newAddress
{
	[address release];
	address = newAddress;
	[address retain];
	[self connectBonjour];
	bonjourType = YES;
}


-(id) objectCall
{
	if (!proxy) {
		if ( bonjourType ) 
			[self connectBonjour];
		else 
			[self connectStraight];
		// If there is still no proxy, bail
		if (!proxy) return nil;
	}
	return proxy;
	
}

-(void) setHostname: (NSString *) serviceName
{
	[serverHostname release];
	serverHostname = serviceName;
	[serverHostname retain];
}


-(void) setAddress: (NSData *) serviceName
{
	[address release];
	address = serviceName;
	[address retain];
}


-(id) initWithService:(NSString * ) serviceName
{
	[super init];
	[self setHostname:serviceName];
	bonjourType = NO;
	//[self connectWithHostname:serviceName];
}

-(id) initWithAddress:(NSData * ) addresses
{
	[super init];
	[self setAddress:addresses];
	bonjourType = YES;
	//[self connectWithAddress:addresses];
}

-(void) dealloc 
{
	if ( proxy) {
		[self cleanup];
	}
	[address release];
	[serverHostname release];
	[super dealloc];
}

@end
