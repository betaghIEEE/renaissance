//
//  ConnectionMonitor.m
//  chatterd
//
//  Created by Daniel D. Beatty on 4/19/05.
//  Copyright 2005 __MyCompanyName__. All rights reserved.
//

#import "ConnectionMonitor.h"


@implementation ConnectionMonitor

-(BOOL) connection:(NSConnection *) ancestor 
	shouldMakeNewConnection:(NSConnection *) conn
{
	NSLog (@"creating new connection: %d total connection" , 
		   [[NSConnection allConnections] count] );
	return YES;
}

- (void) connectionDidDie: (NSNotification *) note
{
	NSConnection *connection = [note object];
	NSLog (@"connection did die: %@", connection);
}

-(void)netService:(NSNetService *) sender
	didNotPublish:(NSDictionary *) errorDict
{
	NSLog(@"failed to publish = %@", errorDict);
}

@end
