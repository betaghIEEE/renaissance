#import <Foundation/Foundation.h>
#include "mpi.h"
#include "array.h"


void master()
{
    int		com_size, rank, work;
    double		result;
    MPI_Status	status;
    MPI_Comm_size(MPI_COMM_WORLD,&com_size);
    for (rank = 1; rank < com_size; ++rank) 
    {
        work = rank;
        MPI_Send(&work,		/* message buffer */
            1,		/* one data item */
            MPI_INT,	/* data item is an integer */
            rank,		/* destination process rank */
            WORKTAG,	/* user chosen message tag */
            MPI_COMM_WORLD);/* always use this */
    }
    work++;      
    while(work < MWORK){
        MPI_Recv(&result,	/* message buffer */
        1,		/* one data item */
        MPI_DOUBLE,	/* data item is a double real */
        MPI_ANY_SOURCE,	/* receive from any sender */
        MPI_ANY_TAG,	/* receive any type of message */
        MPI_COMM_WORLD,	/* always use this */
        &status);	/* info about received message */
        
        C[status.MPI_TAG]=result;
        D[status.MPI_TAG]=status.MPI_SOURCE;
        
        MPI_Send(&work, 1, MPI_INT, status.MPI_SOURCE,
                 WORKTAG, MPI_COMM_WORLD);
        work++;
    }
    for (rank = 1; rank < com_size; ++rank) {
        MPI_Recv(&result, 1, MPI_DOUBLE, MPI_ANY_SOURCE,MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        C[status.MPI_TAG]=result;
        D[status.MPI_TAG]=status.MPI_SOURCE;
    }
    for (rank = 1; rank < com_size; ++rank) {
        MPI_Send(0, 0, MPI_INT, rank, DIETAG, MPI_COMM_WORLD);
    }
}


void slave()

{
    double		result, x=0.;
    int		work,  i, j;
    MPI_Status	status;
    for(;;){ 
        MPI_Recv(&work, 1, MPI_INT, 0,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
        if (status.MPI_TAG == DIETAG)
        { 
            return;
        }
        result = A[work]+B[work];
        for(i=0; i<999; i++){
            for(j= 0; j< 999; j++){
                x += 1/(1+i+j);}}
        
        MPI_Send(&result, 1, MPI_DOUBLE, 0, work, MPI_COMM_WORLD);
    }
}


int main (int argc,  char * argv[]) {
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];

    // insert code here...
    NSLog(@"Hello, World!");
    int myrank,i;
    for (i=0;i <1000; i++)
    {
        A[i]=i;
        B[i]=i;
        C[i]=0;
    }
    MPI_Init(&argc, &argv);	
    MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
    if (myrank == 0) {
        master();
        for (i=1;i < MWORK; i++)
            printf ("results C[%d]=%f on %f\n",i, C[i],D[i]);
    } else {
	slave();
    }
    MPI_Finalize();			/* cleanup MPI */
    
    [pool release];
    return 0;
}
