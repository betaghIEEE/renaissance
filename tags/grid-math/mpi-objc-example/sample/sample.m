#import <Foundation/Foundation.h>
#import <mpi.h>

int main (int argc, const char * argv[]) {
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];

    // insert code here...
    int err = 0;
    err = MPI_Init (&argc, &argv);
    NSLog(@"Hello, World!");
    //err = MPI_Init (&argc, &argv);
    MPI_Finalize();
    [pool release];
    return 0;
}
