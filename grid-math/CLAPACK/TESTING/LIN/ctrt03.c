#include "blaswrap.h"
/*  -- translated by f2c (version 19990503).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "f2c.h"

/* Table of constant values */

static integer c__1 = 1;

/* Subroutine */ int ctrt03_(char *uplo, char *trans, char *diag, integer *n, 
	integer *nrhs, complex *a, integer *lda, real *scale, real *cnorm, 
	real *tscal, complex *x, integer *ldx, complex *b, integer *ldb, 
	complex *work, real *resid)
{
    /* System generated locals */
    integer a_dim1, a_offset, b_dim1, b_offset, x_dim1, x_offset, i__1;
    real r__1, r__2;
    complex q__1;

    /* Builtin functions */
    double c_abs(complex *);

    /* Local variables */
    static integer j;
    extern logical lsame_(char *, char *);
    static real xscal;
    extern /* Subroutine */ int ccopy_(integer *, complex *, integer *, 
	    complex *, integer *), caxpy_(integer *, complex *, complex *, 
	    integer *, complex *, integer *), ctrmv_(char *, char *, char *, 
	    integer *, complex *, integer *, complex *, integer *);
    static real tnorm, xnorm;
    static integer ix;
    extern integer icamax_(integer *, complex *, integer *);
    extern doublereal slamch_(char *);
    extern /* Subroutine */ int csscal_(integer *, real *, complex *, integer 
	    *);
    static real smlnum, eps, err;


#define a_subscr(a_1,a_2) (a_2)*a_dim1 + a_1
#define a_ref(a_1,a_2) a[a_subscr(a_1,a_2)]
#define b_subscr(a_1,a_2) (a_2)*b_dim1 + a_1
#define b_ref(a_1,a_2) b[b_subscr(a_1,a_2)]
#define x_subscr(a_1,a_2) (a_2)*x_dim1 + a_1
#define x_ref(a_1,a_2) x[x_subscr(a_1,a_2)]


/*  -- LAPACK test routine (version 3.0) --   
       Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,   
       Courant Institute, Argonne National Lab, and Rice University   
       February 29, 1992   


    Purpose   
    =======   

    CTRT03 computes the residual for the solution to a scaled triangular   
    system of equations A*x = s*b,  A**T *x = s*b,  or  A**H *x = s*b.   
    Here A is a triangular matrix, A**T denotes the transpose of A, A**H   
    denotes the conjugate transpose of A, s is a scalar, and x and b are   
    N by NRHS matrices.  The test ratio is the maximum over the number of   
    right hand sides of   
       norm(s*b - op(A)*x) / ( norm(op(A)) * norm(x) * EPS ),   
    where op(A) denotes A, A**T, or A**H, and EPS is the machine epsilon.   

    Arguments   
    =========   

    UPLO    (input) CHARACTER*1   
            Specifies whether the matrix A is upper or lower triangular.   
            = 'U':  Upper triangular   
            = 'L':  Lower triangular   

    TRANS   (input) CHARACTER*1   
            Specifies the operation applied to A.   
            = 'N':  A *x = s*b     (No transpose)   
            = 'T':  A**T *x = s*b  (Transpose)   
            = 'C':  A**H *x = s*b  (Conjugate transpose)   

    DIAG    (input) CHARACTER*1   
            Specifies whether or not the matrix A is unit triangular.   
            = 'N':  Non-unit triangular   
            = 'U':  Unit triangular   

    N       (input) INTEGER   
            The order of the matrix A.  N >= 0.   

    NRHS    (input) INTEGER   
            The number of right hand sides, i.e., the number of columns   
            of the matrices X and B.  NRHS >= 0.   

    A       (input) COMPLEX array, dimension (LDA,N)   
            The triangular matrix A.  If UPLO = 'U', the leading n by n   
            upper triangular part of the array A contains the upper   
            triangular matrix, and the strictly lower triangular part of   
            A is not referenced.  If UPLO = 'L', the leading n by n lower   
            triangular part of the array A contains the lower triangular   
            matrix, and the strictly upper triangular part of A is not   
            referenced.  If DIAG = 'U', the diagonal elements of A are   
            also not referenced and are assumed to be 1.   

    LDA     (input) INTEGER   
            The leading dimension of the array A.  LDA >= max(1,N).   

    SCALE   (input) REAL   
            The scaling factor s used in solving the triangular system.   

    CNORM   (input) REAL array, dimension (N)   
            The 1-norms of the columns of A, not counting the diagonal.   

    TSCAL   (input) REAL   
            The scaling factor used in computing the 1-norms in CNORM.   
            CNORM actually contains the column norms of TSCAL*A.   

    X       (input) COMPLEX array, dimension (LDX,NRHS)   
            The computed solution vectors for the system of linear   
            equations.   

    LDX     (input) INTEGER   
            The leading dimension of the array X.  LDX >= max(1,N).   

    B       (input) COMPLEX array, dimension (LDB,NRHS)   
            The right hand side vectors for the system of linear   
            equations.   

    LDB     (input) INTEGER   
            The leading dimension of the array B.  LDB >= max(1,N).   

    WORK    (workspace) COMPLEX array, dimension (N)   

    RESID   (output) REAL   
            The maximum over the number of right hand sides of   
            norm(op(A)*x - s*b) / ( norm(op(A)) * norm(x) * EPS ).   

    =====================================================================   


       Quick exit if N = 0   

       Parameter adjustments */
    a_dim1 = *lda;
    a_offset = 1 + a_dim1 * 1;
    a -= a_offset;
    --cnorm;
    x_dim1 = *ldx;
    x_offset = 1 + x_dim1 * 1;
    x -= x_offset;
    b_dim1 = *ldb;
    b_offset = 1 + b_dim1 * 1;
    b -= b_offset;
    --work;

    /* Function Body */
    if (*n <= 0 || *nrhs <= 0) {
	*resid = 0.f;
	return 0;
    }
    eps = slamch_("Epsilon");
    smlnum = slamch_("Safe minimum");

/*     Compute the norm of the triangular matrix A using the column   
       norms already computed by CLATRS. */

    tnorm = 0.f;
    if (lsame_(diag, "N")) {
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
/* Computing MAX */
	    r__1 = tnorm, r__2 = *tscal * c_abs(&a_ref(j, j)) + cnorm[j];
	    tnorm = dmax(r__1,r__2);
/* L10: */
	}
    } else {
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
/* Computing MAX */
	    r__1 = tnorm, r__2 = *tscal + cnorm[j];
	    tnorm = dmax(r__1,r__2);
/* L20: */
	}
    }

/*     Compute the maximum over the number of right hand sides of   
          norm(op(A)*x - s*b) / ( norm(op(A)) * norm(x) * EPS ). */

    *resid = 0.f;
    i__1 = *nrhs;
    for (j = 1; j <= i__1; ++j) {
	ccopy_(n, &x_ref(1, j), &c__1, &work[1], &c__1);
	ix = icamax_(n, &work[1], &c__1);
/* Computing MAX */
	r__1 = 1.f, r__2 = c_abs(&x_ref(ix, j));
	xnorm = dmax(r__1,r__2);
	xscal = 1.f / xnorm / (real) (*n);
	csscal_(n, &xscal, &work[1], &c__1);
	ctrmv_(uplo, trans, diag, n, &a[a_offset], lda, &work[1], &c__1);
	r__1 = -(*scale) * xscal;
	q__1.r = r__1, q__1.i = 0.f;
	caxpy_(n, &q__1, &b_ref(1, j), &c__1, &work[1], &c__1);
	ix = icamax_(n, &work[1], &c__1);
	err = *tscal * c_abs(&work[ix]);
	ix = icamax_(n, &x_ref(1, j), &c__1);
	xnorm = c_abs(&x_ref(ix, j));
	if (err * smlnum <= xnorm) {
	    if (xnorm > 0.f) {
		err /= xnorm;
	    }
	} else {
	    if (err > 0.f) {
		err = 1.f / eps;
	    }
	}
	if (err * smlnum <= tnorm) {
	    if (tnorm > 0.f) {
		err /= tnorm;
	    }
	} else {
	    if (err > 0.f) {
		err = 1.f / eps;
	    }
	}
	*resid = dmax(*resid,err);
/* L30: */
    }

    return 0;

/*     End of CTRT03 */

} /* ctrt03_ */

#undef x_ref
#undef x_subscr
#undef b_ref
#undef b_subscr
#undef a_ref
#undef a_subscr


