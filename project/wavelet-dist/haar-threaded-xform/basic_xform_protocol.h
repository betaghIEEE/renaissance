

#import <Foundation/Foundation.h>
#import "matrixType.h"
#import "myVector.h"

@protocol basic_xform_server_remote
-(oneway void) colXform:(matrixType *) workMatrix 
				   from:(int)start 
					 to:(int)finish;
-(oneway void) rowXform:(matrixType *) workMatrix 
				   from:(int)start 
					 to:(int)finish;

-(oneway void) colXiform:(matrixType *) workMatrix 
				   from:(int)start 
					 to:(int)finish;
-(oneway void) rowXiform:(matrixType *) workMatrix 
					from:(int)start 
					  to:(int)finish;
-(oneway void) setFilters:(myVector *) average  
			   difference:(myVector *) difference;


@end


@protocol basic_xform_client_remote
- (oneway void) colXformDone:(matrixType *) doneMatrix;
- (oneway void) colXiformDone:(matrixType *) doneMatrix;
- (oneway void) rowXformDone:(matrixType *) doneMatrix;
- (oneway void) rowXiformDone:(matrixType *) doneMatrix;


@end

@protocol basic_xform_server
-(oneway void) colXform:(matrixType *) workMatrix 
				   from:(int)start 
					 to:(int)finish;
-(oneway void) rowXform:(matrixType *) workMatrix 
				   from:(int)start 
					 to:(int)finish;

-(oneway void) colXiform:(matrixType *) workMatrix 
					from:(int)start 
					  to:(int)finish;
-(oneway void) rowXiform:(matrixType *) workMatrix 
					from:(int)start 
					  to:(int)finish;
-(oneway void) setFilters:(myVector *) average  
			   difference:(myVector *) difference;


@end


@protocol basic_xform_client
- (oneway void) colXformDone;
- (oneway void) colXiformDone;
- (oneway void) rowXformDone;
- (oneway void) rowXiformDone;

@end