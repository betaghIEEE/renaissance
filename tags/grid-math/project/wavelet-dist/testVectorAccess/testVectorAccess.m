#import <Foundation/Foundation.h>
#import <oc-matrix/matrixType.h> 
#import <oc-matrix/myVector.h>
#import <oc-matrix/xformpsi.h>

int main (int argc, const char * argv[]) {
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	
    // insert code here...
    NSLog(@"Hello, World!");
	matrixType *myMat = [[matrixType init] alloc];
	matrixType *xMat = [[matrixType init] alloc];
	matrixType *recMat = [[matrixType init] alloc];
	myVector *myVec = [[myVector init] alloc];
	[myMat allocateRows:8 columns:8];
	[myVec allocateWith:8];
	xformpsi *xf = [[xformpsi init] alloc];
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
		xMat = [xf xFormMatrix: myMat withResolution:1];		
	/*	
	for ( i = 0; i < 8; i++)
		for ( j = 0; j < 8; j++)
		{
			temp = ([xMat matrix][i][j]);
			printf ("Value at i= %d and j = %d is %f \n", i, j, temp);
		}
	/*
			[xMat release];
			xMat =[xf xiFormMatrix: myMat withResolution:1];		
	
	for ( i = 0; i < 8; i++)
		for ( j = 0; j < 8; j++)
		{
			temp = ([xMat matrix][i][j]);
			printf ("Value at i= %d and j = %d is %f \n", i, j, temp);
		}
	for ( i = 0; i < 8; i++)
		[myVec vector][i] = i;
	for ( i= 0; i < 8; i++)
	{
		temp = ([myVec vector][i]);
		printf ("Value at i= %d  is %f \n", i, temp);
	}
	*/
		
    [pool release];
    return 0;
}
