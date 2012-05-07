
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

struct matrix {
double **  mat;
int       row;
int	  col;
};


struct  matrix * get_matrix(int row, int col);
struct matrix * matrix_mult(struct matrix *m1,struct matrix * m2);
void print_matrix(struct matrix *m);
double ** get_mat(int n1, int n2);
double rhs(double x, double y);
void gs_for_poisson(struct matrix * m, int num_its);

void main(){
struct   matrix  * m1;
int  n;
n = 5;
m1 = get_matrix(n,n);

gs_for_poisson(m1, 100);
print_matrix(m1);

 
 }
 
 struct  matrix * get_matrix(int row, int col)
 {struct  matrix *a;
     
      a = (struct matrix *) calloc(1,sizeof(struct matrix));
     if( row <1 ) {
     	printf("row index less than 1\n");
     	return NULL;}
     if( col <1 ) {
     	printf("column index less than 1\n");
     	return NULL;}	
     a->row = row;
     a->col = col;
     a->mat = get_mat(row, col);
     return a;
     }

 

 
 struct matrix * matrix_mult(struct matrix *m1, struct matrix *m2)
 {
 int i, j, k;
 struct matrix  * ans;
 /* error checking!!*/
 
 ans = get_matrix(m1->row, m2->col);

 for(i=0 ; i< m1->row; i++){
 	for(j=0; j<m2->col; j++){
 		for(k=0; k<m1->col; k++)
 		  (ans->mat)[i][j]+=((m1->mat)[i][k])*((m2->mat)[k][j]);}}
 		  
 		  
 		  
 return ans;
 }
 		  
 
 
 void print_matrix(struct matrix *m)
 {
 int i, j;

printf("\n");
printf(" Print %d by %d matrix \n", m->row, m->col);

 printf("\n");
 for(i=0; i<(m->row); i++){
 for(j=0; j<(m->col); j++)
 printf(" %f  ", (m->mat)[i][j]);
 printf("\n");}
 }
        
        

double ** get_mat(int n1, int n2)
{
/*  Purpose: This function allocates space for a n1 by n2 doubly
             indexed array of doubles.

    Return Value:  n1 by n2 matrix implemented as pointer to pointer 
                   to double.  The indexing starts at [0][0] to 
                   [n1-1][n2-1].  And the storage is allocated contiguously.
                   
                   

    Arguments:

        n1              row index (input)
        n2              column index (input)
        
        
        There are no Fatal Errors.

    Related Functions:  free_mat

 */
    int i;
    double ** mat, * temp_ptr;

            /*  Allocate space for the array  */
    temp_ptr   = (double *) calloc(n1*n2, sizeof(double));
    if((void *)temp_ptr == NULL){
       /* *inform = 4; */
        return NULL;
    }

    mat = (double **) calloc(n1, sizeof(double *));
    if((void *)(mat) == NULL){
        /**inform = 4;*/
        return NULL;
    }

    for(i=0; i< n1; i++)
        mat[i] = &(temp_ptr[i*n2]);

    return mat;
}

void gs_for_poisson(struct matrix * m, int num_its)
{
int i,j, k;
double h;

h = 1./(m->col);  /* assume domain is [0,1]^2 */

for(k=0; k<num_its; k++){
   for(i=1; i< m->row -1; i++)
   for(j=1; j< m->col -1; j++){
        (m->mat)[i][j] = ( h*h*rhs(i*h, j*h) + (m->mat)[i+1][j] + (m->mat)[i-1][j] +(m->mat)[i][j+1]+ (m->mat)[i][j-1] )/4;

}
}

return;
}
   

double rhs(double x, double y)
{
return 2.;
}
