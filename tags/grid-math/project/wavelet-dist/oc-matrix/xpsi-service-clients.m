//
//  xpsi-service-clients.m
//  oc-matrix
//
//  Created by Daniel D. Beatty on 4/25/05.
//  Copyright 2005 __MyCompanyName__. All rights reserved.
//

#import "xpsi-service-clients.h"


@implementation xpsi_service_clients

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
//		[hostField reloadData];
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
//		[hostField reloadData];
	}
}



-(xpsi_service_client *) issueObjectLocal
{
	xpsi_service_client *tDO;
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


-(out bycopy xpsi_service_client *) issueObjectRemote
{	
	return [self issueObjectLocal];
}

-(void) receiveObjectLocal:(xpsi_service_client *) rtDO
{
	xpsi_service_client *tDO = rtDO;
	[xpsi_services addObject:tDO];
}

-(void) receiveObjectRemote:(in bycopy xpsi_service_client *) rtDO
{
	xpsi_service_client *tDO = rtDO;
	[xpsi_services addObject:tDO];
}


-(void) initiateSingleDO:(NSString *) hostname
{
	xpsi_service_client *tDO = [[xpsi_service_client alloc] init];
	[xpsi_service_client connectWithHostname:hostname];
	[xpsi_services addObject:tDO];
	[tDO release];
}

-(void) initiateCollectiveDO
{
	NSEnumerator *serviceEnum;
	xpsi_service_client *tDO;
	NSNetService *anotherService;
	serviceEnum = [services objectEnumerator];
	while ( (anotherService = [serviceEnum nextObject]) == nil ){
		tDO = [[xpsi_service_client alloc] init];
		[tDO connectWithAddress:[anotherService addresses]];
		[xpsi_services addObject:tDO];
		[tDO release];
	}
	
}

-(xpsi_service_client *)	accessObjectLocalAt:(int) index
{
	xpsi_service_client *tDO = [xpsi_services objectAtIndex:index];
}


@end
