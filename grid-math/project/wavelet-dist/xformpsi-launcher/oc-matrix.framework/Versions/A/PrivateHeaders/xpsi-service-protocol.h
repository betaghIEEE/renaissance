#import <Foundation/Foundation.h>
#import "matrixType.h" 

@protocol xpsiServiceProtocol 
-( bycopy matrixType *) rowWXform:(in byref matrixType *) workMatrix;
-( bycopy matrixType *) colWXform:(in byref matrixType *) workMatrix;
-( bycopy matrixType *) rowWIXform:(in byref matrixType *) workMatrix;
-( bycopy matrixType *) colWIXform:(in byref matrixType *) workMatrix;
-( bycopy matrixType *) xiFormMatrix: (matrixType *) A withResolution:(int) res;
-( bycopy matrixType *) xFormMatrix: (matrixType *) A withResolution:(int) res;
@end