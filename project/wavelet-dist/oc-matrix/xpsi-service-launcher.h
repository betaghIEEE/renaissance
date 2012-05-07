//
//  xpsi-service-launcher.h
//  oc-matrix
//
//  Created by Daniel D. Beatty on 4/25/05.
//  Copyright 2005 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "xpsi-service-protocol.h"

@interface xpsi_service_launcher : NSObject {
	NSSocketPort *receivePort;
	NSConnection *connection;
}
-(void) launchService:(NSString *) serviceName 
			   atPort:(int) port 
			 withType:(NSString *) serviceType;


@end
