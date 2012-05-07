/*
 *  Calculate definite integral using trapezoidial rule 
 *  The function f(x) is hardwired 
 *  Input a, b, n
 *  Output: estimate of integral from a to b of f(x) using n trapizoids.
 */
#include <stdio.h>

float f(float t);  

int main (int arcg, char *arcv[])
{
    float integral;
    float a, b;
    int n;
    float h;
    float x;
    int i;
    
    printf( "Enter a, b, and n\n");
    scanf (" %f %f %d", a, b, n);
    h = (b -a ) /n;
    integral = (f(a) + f(b)) /2.0 ;
    x = a;
    for ( i = 0; i < (n-1); i++) {
        x = x +h;
        integral = integral + f(x);
    }
    integral = integral * h;
    
    printf( "With n = %d trapazoids, our estimates\n" , n);
    printf ("of the integral from %f to %f = %f \n",a,b, integral);
    

} /* main */

float f (float x)
{
    float return_val;
    return_val = 4.0 / (1.0 + a * a );
    return return_val;
}
