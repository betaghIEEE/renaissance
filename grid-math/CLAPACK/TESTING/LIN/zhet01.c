#include "blaswrap.h"
/*  -- translated by f2c (version 19990503).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "f2c.h"

/* Table of constant values */

static doublecomplex c_b1 = {0.,0.};
static doublecomplex c_b2 = {1.,0.};

/* Subroutine */ int zhet01_(char *uplo, integer *n, doublecomplex *a, 
	integer *lda, doublecomplex *afac, integer *ldafac, integer *ipiv, 
	doublecomplex *c__, integer *ldc, doublereal *rwork, doublereal *
	resid)
{
    /* System generated locals */
    integer a_dim1, a_offset, afac_dim1, afac_offset, c_dim1, c_offset, i__1, 
	    i__2, i__3, i__4, i__5;
    doublereal d__1;
    doublecomplex z__1;

    /* Builtin functions */
    double d_imag(doublecomplex *);

    /* Local variables */
    static integer info, i__, j;
    extern logical lsame_(char *, char *);
    static doublereal anorm;
    extern doublereal dlamch_(char *), zlanhe_(char *, char *, 
	    integer *, doublecomplex *, integer *, doublereal *);
    extern /* Subroutine */ int zlavhe_(char *, char *, char *, integer *, 
	    integer *, doublecomplex *, integer *, integer *, doublecomplex *,
	     integer *, integer *), zlaset_(char *, 
	    integer *, integer *, doublecomplex *, doublecomplex *, 
	    doublecomplex *, integer *);
    static doublereal eps;


#define a_subscr(a_1,a_2) (a_2)*a_dim1 + a_1
#define a_ref(a_1,a_2) a[a_subscr(a_1,a_2)]
#define c___subscr(a_1,a_2) (a_2)*c_dim1 + a_1
#define c___ref(a_1,a_2) c__[c___subscr(a_1,a_2)]
#define afac_subscr(a_1,a_2) (a_2)*afac_dim1 + a_1
#define afac_ref(a_1,a_2) afac[afac_subscr(a_1,a_2)]


/*  -- LAPACK test routine (version 3.0) --   
       Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,   
       Courant Institute, Argonne National Lab, and Rice University   
       September 30, 1994   


    Purpose   
    =======   

    ZHET01 reconstructs a Hermitian indefinite matrix A from its   
    block L*D*L' or U*D*U' factorization and computes the residual   
       norm( C - A ) / ( N * norm(A) * EPS ),   
    where C is the reconstructed matrix, EPS is the machine epsilon,   
    L' is the conjugate transpose of L, and U' is the conjugate transpose   
    of U.   

    Arguments   
    ==========   

    UPLO    (input) CHARACTER*1   
            Specifies whether the upper or lower triangular part of the   
            Hermitian matrix A is stored:   
            = 'U':  Upper triangular   
            = 'L':  Lower triangular   

    N       (input) INTEGER   
            The number of rows and columns of the matrix A.  N >= 0.   

    A       (input) COMPLEX*16 array, dimension (LDA,N)   
            The original Hermitian matrix A.   

    LDA     (input) INTEGER   
            The leading dimension of the array A.  LDA >= max(1,N)   

    AFAC    (input) COMPLEX*16 array, dimension (LDAFAC,N)   
            The factored form of the matrix A.  AFAC contains the block   
            diagonal matrix D and the multipliers used to obtain the   
            factor L or U from the block L*D*L' or U*D*U' factorization   
            as computed by ZHETRF.   

    LDAFAC  (input) INTEGER   
            The leading dimension of the array AFAC.  LDAFAC >= max(1,N).   

    IPIV    (input) INTEGER array, dimension (N)   
            The pivot indices from ZHETRF.   

    C       (workspace) COMPLEX*16 array, dimension (LDC,N)   

    LDC     (integer) INTEGER   
            The leading dimension of the array C.  LDC >= max(1,N).   

    RWORK   (workspace) DOUBLE PRECISION array, dimension (N)   

    RESID   (output) DOUBLE PRECISION   
            If UPLO = 'L', norm(L*D*L' - A) / ( N * norm(A) * EPS )   
            If UPLO = 'U', norm(U*D*U' - A) / ( N * norm(A) * EPS )   

    =====================================================================   


       Quick exit if N = 0.   

       Parameter adjustments */
    a_dim1 = *lda;
    a_offset = 1 + a_dim1 * 1;
    a -= a_offset;
    afac_dim1 = *ldafac;
    afac_offset = 1 + afac_dim1 * 1;
    afac -= afac_offset;
    --ipiv;
    c_dim1 = *ldc;
    c_offset = 1 + c_dim1 * 1;
    c__ -= c_offset;
    --rwork;

    /* Function Body */
    if (*n <= 0) {
	*resid = 0.;
	return 0;
    }

/*     Determine EPS and the norm of A. */

    eps = dlamch_("Epsilon");
    anorm = zlanhe_("1", uplo, n, &a[a_offset], lda, &rwork[1]);

/*     Check the imaginary parts of the diagonal elements and return with   
       an error code if any are nonzero. */

    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	if (d_imag(&afac_ref(j, j)) != 0.) {
	    *resid = 1. / eps;
	    return 0;
	}
/* L10: */
    }

/*     Initialize C to the identity matrix. */

    zlaset_("Full", n, n, &c_b1, &c_b2, &c__[c_offset], ldc);

/*     Call ZLAVHE to form the product D * U' (or D * L' ). */

    zlavhe_(uplo, "Conjugate", "Non-unit", n, n, &afac[afac_offset], ldafac, &
	    ipiv[1], &c__[c_offset], ldc, &info);

/*     Call ZLAVHE again to multiply by U (or L ). */

    zlavhe_(uplo, "No transpose", "Unit", n, n, &afac[afac_offset], ldafac, &
	    ipiv[1], &c__[c_offset], ldc, &info);

/*     Compute the difference  C - A . */

    if (lsame_(uplo, "U")) {
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	    i__2 = j - 1;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		i__3 = c___subscr(i__, j);
		i__4 = c___subscr(i__, j);
		i__5 = a_subscr(i__, j);
		z__1.r = c__[i__4].r - a[i__5].r, z__1.i = c__[i__4].i - a[
			i__5].i;
		c__[i__3].r = z__1.r, c__[i__3].i = z__1.i;
/* L20: */
	    }
	    i__2 = c___subscr(j, j);
	    i__3 = c___subscr(j, j);
	    i__4 = a_subscr(j, j);
	    d__1 = a[i__4].r;
	    z__1.r = c__[i__3].r - d__1, z__1.i = c__[i__3].i;
	    c__[i__2].r = z__1.r, c__[i__2].i = z__1.i;
/* L30: */
	}
    } else {
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	    i__2 = c___subscr(j, j);
	    i__3 = c___subscr(j, j);
	    i__4 = a_subscr(j, j);
	    d__1 = a[i__4].r;
	    z__1.r = c__[i__3].r - d__1, z__1.i = c__[i__3].i;
	    c__[i__2].r = z__1.r, c__[i__2].i = z__1.i;
	    i__2 = *n;
	    for (i__ = j + 1; i__ <= i__2; ++i__) {
		i__3 = c___subscr(i__, j);
		i__4 = c___subscr(i__, j);
		i__5 = a_subscr(i__, j);
		z__1.r = c__[i__4].r - a[i__5].r, z__1.i = c__[i__4].i - a[
			i__5].i;
		c__[i__3].r = z__1.r, c__[i__3].i = z__1.i;
/* L40: */
	    }
/* L50: */
	}
    }

/*     Compute norm( C - A ) / ( N * norm(A) * EPS ) */

    *resid = zlanhe_("1", uplo, n, &c__[c_offset], ldc, &rwork[1]);

    if (anorm <= 0.) {
	if (*resid != 0.) {
	    *resid = 1. / eps;
	}
    } else {
	*resid = *resid / (doublereal) (*n) / anorm / eps;
    }

    return 0;

/*     End of ZHET01 */

} /* zhet01_ */

#undef afac_ref
#undef afac_subscr
#undef c___ref
#undef c___subscr
#undef a_ref
#undef a_subscr


