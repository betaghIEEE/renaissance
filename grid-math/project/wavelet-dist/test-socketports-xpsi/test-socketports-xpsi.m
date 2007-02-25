#import <Foundation/Foundation.h>
#import <oc-matrix/matrixType.h> 

int main (int argc, const char * argv[]) {
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	
	BOOL successful;
	// Begin and start connection.  
/*	NSConnection *connection;
	NSSocketPort *sendPort;
	
	
	NSString *nickname;
	NSString *serverHostname;
	NSData *address;
	NSNetServiceBrowser *browser;
	NSMutableArray *services;
	NSString *serviceType = @"_oc_matrix._tcp.";
	
	browser = [[NSNetServiceBrowser alloc] init];
	services = [[NSMutableArray array] retain];
	[browser setDelegate:self];
	[browser searchForServicesOfType:@"_chatter._tcp" 
							inDomain:@"local."];
	NSLog (@"begun browser: %@", browser);
	
	// Create the send port
	//sendPort = [[NSSocketPort alloc] initRemoteWithTCPPort:7000 
	//												  host:serverHostname];
	
	sendPort = [[NSSocketPort alloc] initRemoteWithProtocolFamily:AF_INET 
													   socketType:SOCK_STREAM 
														 protocol:INET_TCP 
														  address:address];
	*/
	matrixType *myMat = [[matrixType init] alloc];
	matrixType *xMat = [[matrixType init] alloc];
	matrixType *recMat = [[matrixType init] alloc];
	
	[myMat allocateRows:8 columns:8];
	double temp;
	int i , j;
	for ( i = 0; i < 8; i++)
		for ( j = 0; j < 8; j++)
			[myMat matrix][i][j] = i * j;
	for ( i = 0; i < 8; i++)
		for ( j = 0; j < 8; j++)
		{
			temp = ([myMat matrix][i][j]);
			printf ("Value at i= %d and j = %d is %f \n", i, j, temp);
		}
	
	
	
    // insert code here...
    NSLog(@"Hello, World!");
    [pool release];
    return 0;
}
