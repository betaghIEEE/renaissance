#import "AppController.h"
#include <sys/socket.h>


@implementation AppController
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

// Show message coming in from server
- (oneway void) showMessage: (in bycopy NSString *) message 
			   fromNickname:(in bycopy NSString *) n
{
	NSLog (@"Attempting message transfer");
	NSString *string = [NSString stringWithFormat:@"%@ says, \"%@\"\n", 
		n, message ];
	NSLog (@"Got the contents ");
	NSTextStorage *currentContents = [textView textStorage];
	NSRange range = NSMakeRange ([currentContents length], 0);
	[currentContents replaceCharactersInRange:range withString:string]; 
	range.length = [string length];
	[textView scrollRangeToVisible:range];
	// Beep to get user's attention
	NSBeep();
}

// Accessors 
- (bycopy NSString *)nickname 
{
	return nickname;
}

- (void) setNickname:(NSString *)s
{
	[s retain];
	[nickname release];
	nickname =s;
}

-(void) setServerHostname: (NSString *) s
{
	[s retain];
	[serverHostname release];
	serverHostname = s; 
}

-(void) setAddress:(NSData *) s
{
	[s retain];
	[address release];
	address = s; 
}

// Connect to the server
- (void) connect
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
								selector:@selector(connectDown:) 
								name:NSConnectionDidDieNotification 
								object:connection];
		// By telling the proxy about the protocol for the object
		// it represent, we significantly reduce the network
		// traffic involved in each invocation 
		NSLog (@"Notification Center beginning");
		[proxy setProtocolForProxy:@protocol (ChatterServing)];
		NSLog (@"proxy set");
		// Try to subscribe with chosen nickname
		successful = [proxy subscribeClient:self];
		NSLog (@"Proxy tried subscription");
		if (successful) {
			[messageField setStringValue:@"Connected"];
		} else {
			[messageField setStringValue:@"Nickname not available"];
			[self cleanup];
		}
	NS_HANDLER 
		// If the server does not respond in 10 seconds
		//	this handler will get called 
		[messageField setStringValue:@"Unable to connect"];
		[self cleanup];
	NS_ENDHANDLER
	NSLog (@"Did not even try to connect");
}


- (IBAction)sendMessage:(id)sender
{
	NSString *inString;
	
	// If there is no proxy; try to connect.
	if (!proxy) {
		[self connect];
		// If there is still no proxy, bail
		if (!proxy) return;
	}
	
	// Read the message from the text field
	inString = [messageField stringValue];
	NS_DURING
		// Send a message to the server
		[proxy sendMessage:inString fromClient:self];
	NS_HANDLER
		// If something goes wrong 
		[messageField setStringValue:@"The connection is down"];
		[self cleanup];
	NS_ENDHANDLER
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

// Read hostname and nickname then connect
/*
 - (IBAction)subscribe:(id)sender
{
	// Is the user already subscribed?
	if ( proxy) {
		[messageField setStringValue:@"unsubscribe first!"];
	} else {
		// Read the hostname and nickname from UI
		NSLog (@"Beginning subscription");
		[self setServerHostname:[hostField stringValue]];  
		[self setNickname:[nicknameField stringValue]];
		
		NSLog (@"...Acquired nickname and host name");
		// Connect 
		[self connect];
	}
}
*/


- (IBAction)subscribe:(id)sender
{
	NSNetService *currentService;
	NSArray *addresses;
	
	// Is the user already subscribed?
	if (proxy) {
		[messageField setStringValue:@"unsubscribe first!"];
	} else {
		// What is the select service in the combobox?
		currentService = [services objectAtIndex: [hostField indexOfSelectedItem]];
		addresses = [currentService addresses];
		
		// Did it resolve?
		if ( [addresses count] == 0) {
			[messageField setStringValue:@"Unable to resolve address"];
			return;
		}
		
		// Just take the first address
		[self setAddress:[addresses objectAtIndex:0]];
		[self setNickname:[nicknameField stringValue]];
		// Connect to selecteed server
		[self connect];
	}
}

- (IBAction)unsubscribe:(id)sender
{
	NS_DURING
		[proxy unsubscribeClient:self];
		[messageField setStringValue:@"Unsubscribed"];
		[self cleanup];
	NS_HANDLER
		[messageField setStringValue:@"Error unsubscribing"];
	NS_ENDHANDLER
}

// Combo box methods
- (int) numberOfItemsInComboBox:(NSComboBox *) aComboBox 
{
	return [services count];
}


- (id)comboBox:(NSComboBox *) aComboBox 
	objectValueForItemAtIndex:(int) index
{
	NSNetService *item;
	item = [services objectAtIndex:index];
	NSString *name = [NSString stringWithFormat:@"%@ at %@", [item name], [item addresses] ];
	NSLog (name);
	return [item name];
}

-(unsigned int) comboBox:(NSComboBox *) aComboBox 
	indexOfItemWithStringValue:(NSString *) string
{
	unsigned int k, max;
	NSNetService *item;
	max = [services count]; 
	for ( k = 0; k < max; k++) 
	{
		item = [services objectAtIndex:k]; 
		if ( [string isEqual:[item name] ] ) {
			return k;
		}
	}
	return 0;
}



// Delegate methods

// If the connection goes down, do clean up
- (void) connectionDown:(NSNotification *)note 
{
	NSLog (@"connectionDown:");
	[messageField setStringValue:@"connection down"];
	[self cleanup];
}

// If the app terminates, unsubscribe. 
- (NSApplicationTerminateReply) applicationShouldTerminate:(NSApplication *) app
{
	NSLog (@"invalidating connection");
	if (proxy ) {
		[proxy unsubscribeClient:self];
		[[proxy connectionForProxy] invalidate];
	}
	return NSTerminateNow;
}


-(void) awakeFromNib
{
	browser = [[NSNetServiceBrowser alloc] init];
	services = [[NSMutableArray array] retain];
	[browser setDelegate:self];
	[browser searchForServicesOfType:@"_chatter._tcp" 
						   inDomain:@"local."];
	NSLog (@"begun browser: %@", browser);
}

/*
-(id) init {
	[super init];
	services = [[NSMutableArray alloc] init];
}
*/
- (void) dealloc
{
	[self cleanup];
	[super dealloc];
}

@end
