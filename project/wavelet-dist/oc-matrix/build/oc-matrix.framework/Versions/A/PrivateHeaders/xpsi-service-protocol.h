#import <Foundation/Foundation.h>
#import "matrixType.h" 

@protocol xpsiServiceProtocol 
-( bycopy matrixType *) rowWXform:(in byref matrixType *) workMatrix
							 from:(int) start to:(int)finish;
-( bycopy matrixType *) colWXform:(in byref matrixType *) workMatrix
							 from:(int) start to:(int)finish;
-( bycopy matrixType *) rowWIXform:(in byref matrixType *) workMatrix
							  from:(int) start to:(int)finish;
-( bycopy matrixType *) colWIXform:(in byref matrixType *) workMatrix
							  from:(int) start to:(int)finish;
-( bycopy matrixType *) xiFormMatrix: (matrixType *) A withResolution:(int) res;
-( bycopy matrixType *) xFormMatrix: (matrixType *) A withResolution:(int) res;
@end