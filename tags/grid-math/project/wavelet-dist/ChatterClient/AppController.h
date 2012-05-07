/* AppController */

#import <Cocoa/Cocoa.h>
#import "ChatterServing.h"

@interface AppController : NSObject <ChatterUsing>
{
    //IBOutlet NSTextField *hostField;
    IBOutlet NSTextField *messageField;
    IBOutlet NSTextField *nicknameField;
    IBOutlet NSTextView *textView;
	NSString *nickname;
	NSString *serverHostname;
	id proxy;
	
	/* Added features for Bonjour */
	NSData *address;
	NSNetServiceBrowser *browser;
	NSMutableArray *services;
	IBOutlet NSComboBox *hostField;
}
- (IBAction)sendMessage:(id)sender;
- (IBAction)subscribe:(id)sender;
- (IBAction)unsubscribe:(id)sender;


/* Added methods for Bonjour */
-(int) numberOfItemsInComboBox:(NSComboBox *)aComboBox;
-(id) comboBox:(NSComboBox *) aComboBox 
	objectValueForItemAtIndex:(int) index;
-(unsigned int) comboBox:(NSComboBox *) aComboBox 
	indexOfItemWithStringValue:(NSString *) string;
@end
