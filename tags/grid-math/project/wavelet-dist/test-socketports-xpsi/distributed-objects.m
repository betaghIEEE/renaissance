//
//  distributed-objects.m
//  test-socketports-xpsi
//
//  Created by Daniel D. Beatty on 4/24/05.
//  Copyright 2005 __MyCompanyName__. All rights reserved.
//

#import "distributed-objects.h"


@implementation distributed_objects


-(id) initWithService:(NSString * ) serviceName
{
	browser = [[NSNetServiceBrowser alloc] init];
	services = [[NSMutableArray array] retain];
	[browser setDelegate:self];
	[browser searchForServicesOfType:serviceName
							inDomain:@"local."];
	NSLog (@"begun browser: %@", browser);
}

/* Bonjour methods  */
- (void) netServiceBrowser: (NSNetServiceBrowser *) aNetServiceBrowser 
			didFindService:(NSNetService *) aNetService 
				moreComing:(BOOL) moreComing
{
	NSLog (@"Adding new service");
	[services addObject:aNetService];
	NSString *aName = [aNetService name];
	int count = [services count] - 1;
	NSLog (@"The name of the service is %@, at index %d", aName, count);
	[aNetService setDelegate:self];
	[aNetService resolve];
	if (!moreComing) {
		[hostField reloadData];
	}
}

-(void) netServiceBrowser:(NSNetServiceBrowser *) aNetServiceBrowser 
		 didRemoveService:(NSNetService *) aNetService 
			   moreComing:(BOOL) moreComing
{
	NSLog (@"Removing service");
	NSEnumerator *enumerator = [services objectEnumerator];
	NSNetService *currentNetService;
	while ( currentNetService = [enumerator nextObject] ) {
		if ( [[currentNetService name] isEqual:[aNetService name]] &&
			 [[currentNetService type] isEqual:[aNetService type]] &&
			 [[currentNetService domain] isEqual:[aNetService domain]]) {
			[services removeObject:currentNetService];
			break;
		}
	}
	if ( !moreComing) {
		[hostField reloadData];
	}
}



-(distributed_object *) issueObjectLocal
{
	distributed_object *tDO;
	int count = [services count];
	if ( count > 0 )
	{
		tDO = [ services objectAtIndex];
		[tDO retain];
		[services removeObject:tDO];
		return tDO;
	}
	return nil;
}


-(out bycopy distributed_object *) issueObjectRemote
{	
	return [self issueObjectLocal];
}

-(void) receiveObjectLocal:(distributed_Object *) rtDO
{
	distributed_object *tDO = rtDO;
	[tDO retain];
}

-(void) receiveObjectRemote(in bycopy distributed_object *);
-(void) initiateSingleDO:(NSString *) hostname;
-(void) initiateCollectiveDO;
-(id)	accessObjectLocalAt:(int) index;


@end
