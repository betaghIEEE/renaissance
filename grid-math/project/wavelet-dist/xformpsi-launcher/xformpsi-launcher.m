#import <Foundation/Foundation.h>
#import <oc-matrix/xpsi-service-launcher.h>

int main (int argc, const char * argv[]) {
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];

	xpsi_service_launcher *xpsi = [[xpsi_service_launcher alloc] init];
	int port = 7000;
	NSString *serviceType = @"_psin_wavelet_xform._tcp.";
	[xpsi launchService:@"psin-wavelet-transform" 
						 atPort: port 
			   withType:serviceType];
    // insert code here...
    NSLog(@"Hello, World!");
    [pool release];
    return 0;
}
