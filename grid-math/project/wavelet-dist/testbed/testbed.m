#import <Foundation/Foundation.h>

#import "matrixType.h" 
#import "myVector.h"
#import "xformpsi.h"
#import <mpi.h>

int main (int argc, const char * argv[]) {
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];

    // insert code here...
    NSLog(@"Hello, World!");
	
	matrixType *myMat = [[matrixType init] alloc];
	matrixType *xMat = [[matrixType init] alloc];
	matrixType *recMat = [[matrixType init] alloc];
	matrixType *rec2Mat = [[matrixType init] alloc];
	
	myVector *myVec = [[myVector init] alloc];
	int k = 8;
	[myMat allocateRows:k columns:k];
	[myVec allocateWith:k];
	
	xformpsi *xf = [[xformpsi init] alloc];
	double temp = 0;
	int i , j;
	double **tempMat = [myMat matrix];
	for ( i = 0; i < k; i++)
		for ( j = 0; j < k; j++)
			tempMat[i][j] = i * j;
			//printf ("Value at i= %d and j = %d is %f \n", i, j, temp);
	
	 for ( i = 0; i < k; i++)
		for ( j = 0; j < k; j++)
		{
			temp = ([myMat matrix][i][j]);
			printf ("Value at i= %d and j = %d is %f \n", i, j, temp);
		}
			
			xMat = [xf xFormMatrix: myMat withResolution:1];
	 printf ("Output of the xMatrix \n");
	 for ( i = 0; i < k; i++)
		 for ( j = 0; j < k; j++)
		 {
			 temp = ([xMat matrix][i][j]);
			 printf ("Value at i= %d and j = %d is %f \n", i, j, temp);
		 } 
			 recMat = [ xf xiFormMatrix: xMat withResolution:1];
	 printf ("Output of the recMatrix \n");
	 for ( i = 0; i < k; i++)
		 for ( j = 0; j < k; j++)
		 {
			 temp = ([recMat matrix][i][j]);
			 printf ("Value at i= %d and j = %d is %f \n", i, j, temp);
		 }
			 rec2Mat = [xf matrix];
			 printf ("Output of the re2cMatrix \n");
	 for ( i = 0; i < k; i++)
		 for ( j = 0; j < k; j++)
		 {
			 temp = ([rec2Mat matrix][i][j]);
			 printf ("Value at i= %d and j = %d is %f \n", i, j, temp);
		 }
			 
			 
    [pool release];
    return 0;
}
