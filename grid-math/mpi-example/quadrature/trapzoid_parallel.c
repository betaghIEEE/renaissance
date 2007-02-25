/*
 *  Calculate definite integral using trapezoidial rule 
 *  The function f(x) is hardwired 
 *  Input a, b, n
 *  Output: estimate of integral from a to b of f(x) using n trapizoids.
 */
#include <stdio.h>
/* MPI functions to be used */  
#include <mpi.h>
#define MASTER_RANK 0
#define TRUE 1
#define FALSE 0
float f (float a);

int main (int argc, char **argv)
{
    int n, i, pool_size, my_rank, i_am_the_master = FALSE;
    time_t t0, t1, t2;
    clock_t c0, c1, c2; 
    long count; 
    double b, c;
    double mypi, pi, h, sum, x, a;
    MPI_Init (&argc, &argv);
    MPI_Comm_size (MPI_COMM_WORLD, &pool_size);
    MPI_Comm_rank (MPI_COMM_WORLD, &my_rank); 
    n = 10;
    t0 = time(NULL);
    c0 = clock (); 
    
    if ( my_rank == MASTER_RANK) i_am_the_master = TRUE;
    if (i_am_the_master)
    {
        if (n==0) n=100;
    }
    
    MPI_Bcast(&n, 1, MPI_INT, MASTER_RANK, MPI_COMM_WORLD);
    
    h   = 1.0 / (double) n;
    sum = 0.0;
    for (i = my_rank + 1; i <= n; i += pool_size) {
        x = h * ((double)i - 0.5);
        sum += f(x);
    }
    mypi = h * sum;
    
    MPI_Reduce(&mypi, &pi, 1, MPI_DOUBLE, MPI_SUM, MASTER_RANK,
               MPI_COMM_WORLD);
    
    if (i_am_the_master) printf("\npi is approximately %.16f\n", pi);
    
    MPI_Finalize ();    
}

float f (float x)
{
    float return_val;
    return_val = 4.0 / (1.0 + x * x );
    return return_val;
}