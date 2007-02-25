//
//  main.m
//  ChatterClient
//
//  Created by Daniel D. Beatty on 4/19/05.
//  Copyright __MyCompanyName__ 2005. All rights reserved.
//

#import <Cocoa/Cocoa.h>

int main(int argc, char *argv[])
{
	if ( [NSHost respondsToSelector:@selector(_fixNSHostLeak) ] ){
		[NSHost _fixNSHostLeak];
	}
	if ([NSSocketPort respondsToSelector:@selector(_fixNSSocketPortLeak)])
	{
		[NSSocketPort _fixNSSocketPortLeak];
	}
    return NSApplicationMain(argc,  (const char **) argv);
}
