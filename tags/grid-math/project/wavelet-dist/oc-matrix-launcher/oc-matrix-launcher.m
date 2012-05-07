#import <Foundation/Foundation.h>
#import <oc-matrix/xformpsi.h>
#import "ConnectionMonitor.h"
#include <sys/socket.h>

/*
 OC-Matrix launcher
 The purpose of this program is launch the OC-matrix framework as a service library. 
 At this moment, xformpsi is the primary service object.  Eventually, service objects
 for FITS files/objects, SDSS Astro-tools and others will join oc-matrix and will be 
 launched from this launcher.  
 */

int main (int argc, const char * argv[]) {
	NSSocketPort *receivePort;
	NSConnection *connection;
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	NSString *serviceType = @"_oc_matrix._tcp.";
	NSString *domain = @"";
	int port = 7000;
	
	NSString *serviceName = @"Psin Wavelet Transform";
	NSNetService *netService;
	
	NSRunLoop *runloop = [ NSRunLoop currentRunLoop];
	ConnectionMonitor *monitor = [[ ConnectionMonitor alloc] init]; 
	xformpsi *aServer = [[xformpsi alloc] init]; 
	
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
	[connection setRootObject:aServer];
	
	// The chatter server is retained by the connection 
	[aServer release];
	
	// Set up the monitor object 
	[connection setDelegate:monitor ];
	[[NSNotificationCenter defaultCenter] addObserver:monitor
											 selector:@selector(connectionDidDie:)
												 name:NSConnectionDidDieNotification
											   object:nil];
	netService = [[NSNetService alloc] initWithDomain:domain
												 type:serviceType 
												 name:serviceName 
												 port:port];
	[netService publish];
	NSLog(@"service published = %@", netService);
	
	// Start the run loop
	[runloop run];
	[connection release]; 
	[monitor release];
	
    // insert code here...
    NSLog(@"Hello, World!");
    [pool release];
    return 0;
}
