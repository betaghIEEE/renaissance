#include "blaswrap.h"
#include "f2c.h"

/* Subroutine */ int dgghrd_(char *compq, char *compz, integer *n, integer *
	ilo, integer *ihi, doublereal *a, integer *lda, doublereal *b, 
	integer *ldb, doublereal *q, integer *ldq, doublereal *z__, integer *
	ldz, integer *info)
{
/*  -- LAPACK routine (version 3.0) --   
       Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,   
       Courant Institute, Argonne National Lab, and Rice University   
       September 30, 1994   


    Purpose   
    =======   

    DGGHRD reduces a pair of real matrices (A,B) to generalized upper   
    Hessenberg form using orthogonal transformations, where A is a   
    general matrix and B is upper triangular:  Q' * A * Z = H and   
    Q' * B * Z = T, where H is upper Hessenberg, T is upper triangular,   
    and Q and Z are orthogonal, and ' means transpose.   

    The orthogonal matrices Q and Z are determined as products of Givens   
    rotations.  They may either be formed explicitly, or they may be   
    postmultiplied into input matrices Q1 and Z1, so that   

         Q1 * A * Z1' = (Q1*Q) * H * (Z1*Z)'   
         Q1 * B * Z1' = (Q1*Q) * T * (Z1*Z)'   

    Arguments   
    =========   

    COMPQ   (input) CHARACTER*1   
            = 'N': do not compute Q;   
            = 'I': Q is initialized to the unit matrix, and the   
                   orthogonal matrix Q is returned;   
            = 'V': Q must contain an orthogonal matrix Q1 on entry,   
                   and the product Q1*Q is returned.   

    COMPZ   (input) CHARACTER*1   
            = 'N': do not compute Z;   
            = 'I': Z is initialized to the unit matrix, and the   
                   orthogonal matrix Z is returned;   
            = 'V': Z must contain an orthogonal matrix Z1 on entry,   
                   and the product Z1*Z is returned.   

    N       (input) INTEGER   
            The order of the matrices A and B.  N >= 0.   

    ILO     (input) INTEGER   
    IHI     (input) INTEGER   
            It is assumed that A is already upper triangular in rows and   
            columns 1:ILO-1 and IHI+1:N.  ILO and IHI are normally set   
            by a previous call to DGGBAL; otherwise they should be set   
            to 1 and N respectively.   
            1 <= ILO <= IHI <= N, if N > 0; ILO=1 and IHI=0, if N=0.   

    A       (input/output) DOUBLE PRECISION array, dimension (LDA, N)   
            On entry, the N-by-N general matrix to be reduced.   
            On exit, the upper triangle and the first subdiagonal of A   
            are overwritten with the upper Hessenberg matrix H, and the   
            rest is set to zero.   

    LDA     (input) INTEGER   
            The leading dimension of the array A.  LDA >= max(1,N).   

    B       (input/output) DOUBLE PRECISION array, dimension (LDB, N)   
            On entry, the N-by-N upper triangular matrix B.   
            On exit, the upper triangular matrix T = Q' B Z.  The   
            elements below the diagonal are set to zero.   

    LDB     (input) INTEGER   
            The leading dimension of the array B.  LDB >= max(1,N).   

    Q       (input/output) DOUBLE PRECISION array, dimension (LDQ, N)   
            If COMPQ='N':  Q is not referenced.   
            If COMPQ='I':  on entry, Q need not be set, and on exit it   
                           contains the orthogonal matrix Q, where Q'   
                           is the product of the Givens transformations   
                           which are applied to A and B on the left.   
            If COMPQ='V':  on entry, Q must contain an orthogonal matrix   
                           Q1, and on exit this is overwritten by Q1*Q.   

    LDQ     (input) INTEGER   
            The leading dimension of the array Q.   
            LDQ >= N if COMPQ='V' or 'I'; LDQ >= 1 otherwise.   

    Z       (input/output) DOUBLE PRECISION array, dimension (LDZ, N)   
            If COMPZ='N':  Z is not referenced.   
            If COMPZ='I':  on entry, Z need not be set, and on exit it   
                           contains the orthogonal matrix Z, which is   
                           the product of the Givens transformations   
                           which are applied to A and B on the right.   
            If COMPZ='V':  on entry, Z must contain an orthogonal matrix   
                           Z1, and on exit this is overwritten by Z1*Z.   

    LDZ     (input) INTEGER   
            The leading dimension of the array Z.   
            LDZ >= N if COMPZ='V' or 'I'; LDZ >= 1 otherwise.   

    INFO    (output) INTEGER   
            = 0:  successful exit.   
            < 0:  if INFO = -i, the i-th argument had an illegal value.   

    Further Details   
    ===============   

    This routine reduces A to Hessenberg and B to triangular form by   
    an unblocked reduction, as described in _Matrix_Computations_,   
    by Golub and Van Loan (Johns Hopkins Press.)   

    =====================================================================   


       Decode COMPQ   

       Parameter adjustments */
    /* Table of constant values */
    static doublereal c_b10 = 0.;
    static doublereal c_b11 = 1.;
    static integer c__1 = 1;
    
    /* System generated locals */
    integer a_dim1, a_offset, b_dim1, b_offset, q_dim1, q_offset, z_dim1, 
	    z_offset, i__1, i__2, i__3;
    /* Local variables */
    static integer jcol;
    static doublereal temp;
    extern /* Subroutine */ int drot_(integer *, doublereal *, integer *, 
	    doublereal *, integer *, doublereal *, doublereal *);
    static integer jrow;
    static doublereal c__, s;
    extern logical lsame_(char *, char *);
    extern /* Subroutine */ int dlaset_(char *, integer *, integer *, 
	    doublereal *, doublereal *, doublereal *, integer *), 
	    dlartg_(doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *), xerbla_(char *, integer *);
    static integer icompq, icompz;
    static logical ilq, ilz;
#define a_ref(a_1,a_2) a[(a_2)*a_dim1 + a_1]
#define b_ref(a_1,a_2) b[(a_2)*b_dim1 + a_1]
#define q_ref(a_1,a_2) q[(a_2)*q_dim1 + a_1]
#define z___ref(a_1,a_2) z__[(a_2)*z_dim1 + a_1]


    a_dim1 = *lda;
    a_offset = 1 + a_dim1 * 1;
    a -= a_offset;
    b_dim1 = *ldb;
    b_offset = 1 + b_dim1 * 1;
    b -= b_offset;
    q_dim1 = *ldq;
    q_offset = 1 + q_dim1 * 1;
    q -= q_offset;
    z_dim1 = *ldz;
    z_offset = 1 + z_dim1 * 1;
    z__ -= z_offset;

    /* Function Body */
    if (lsame_(compq, "N")) {
	ilq = FALSE_;
	icompq = 1;
    } else if (lsame_(compq, "V")) {
	ilq = TRUE_;
	icompq = 2;
    } else if (lsame_(compq, "I")) {
	ilq = TRUE_;
	icompq = 3;
    } else {
	icompq = 0;
    }

/*     Decode COMPZ */

    if (lsame_(compz, "N")) {
	ilz = FALSE_;
	icompz = 1;
    } else if (lsame_(compz, "V")) {
	ilz = TRUE_;
	icompz = 2;
    } else if (lsame_(compz, "I")) {
	ilz = TRUE_;
	icompz = 3;
    } else {
	icompz = 0;
    }

/*     Test the input parameters. */

    *info = 0;
    if (icompq <= 0) {
	*info = -1;
    } else if (icompz <= 0) {
	*info = -2;
    } else if (*n < 0) {
	*info = -3;
    } else if (*ilo < 1) {
	*info = -4;
    } else if (*ihi > *n || *ihi < *ilo - 1) {
	*info = -5;
    } else if (*lda < max(1,*n)) {
	*info = -7;
    } else if (*ldb < max(1,*n)) {
	*info = -9;
    } else if (ilq && *ldq < *n || *ldq < 1) {
	*info = -11;
    } else if (ilz && *ldz < *n || *ldz < 1) {
	*info = -13;
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("DGGHRD", &i__1);
	return 0;
    }

/*     Initialize Q and Z if desired. */

    if (icompq == 3) {
	dlaset_("Full", n, n, &c_b10, &c_b11, &q[q_offset], ldq);
    }
    if (icompz == 3) {
	dlaset_("Full", n, n, &c_b10, &c_b11, &z__[z_offset], ldz);
    }

/*     Quick return if possible */

    if (*n <= 1) {
	return 0;
    }

/*     Zero out lower triangle of B */

    i__1 = *n - 1;
    for (jcol = 1; jcol <= i__1; ++jcol) {
	i__2 = *n;
	for (jrow = jcol + 1; jrow <= i__2; ++jrow) {
	    b_ref(jrow, jcol) = 0.;
/* L10: */
	}
/* L20: */
    }

/*     Reduce A and B */

    i__1 = *ihi - 2;
    for (jcol = *ilo; jcol <= i__1; ++jcol) {

	i__2 = jcol + 2;
	for (jrow = *ihi; jrow >= i__2; --jrow) {

/*           Step 1: rotate rows JROW-1, JROW to kill A(JROW,JCOL) */

	    temp = a_ref(jrow - 1, jcol);
	    dlartg_(&temp, &a_ref(jrow, jcol), &c__, &s, &a_ref(jrow - 1, 
		    jcol));
	    a_ref(jrow, jcol) = 0.;
	    i__3 = *n - jcol;
	    drot_(&i__3, &a_ref(jrow - 1, jcol + 1), lda, &a_ref(jrow, jcol + 
		    1), lda, &c__, &s);
	    i__3 = *n + 2 - jrow;
	    drot_(&i__3, &b_ref(jrow - 1, jrow - 1), ldb, &b_ref(jrow, jrow - 
		    1), ldb, &c__, &s);
	    if (ilq) {
		drot_(n, &q_ref(1, jrow - 1), &c__1, &q_ref(1, jrow), &c__1, &
			c__, &s);
	    }

/*           Step 2: rotate columns JROW, JROW-1 to kill B(JROW,JROW-1) */

	    temp = b_ref(jrow, jrow);
	    dlartg_(&temp, &b_ref(jrow, jrow - 1), &c__, &s, &b_ref(jrow, 
		    jrow));
	    b_ref(jrow, jrow - 1) = 0.;
	    drot_(ihi, &a_ref(1, jrow), &c__1, &a_ref(1, jrow - 1), &c__1, &
		    c__, &s);
	    i__3 = jrow - 1;
	    drot_(&i__3, &b_ref(1, jrow), &c__1, &b_ref(1, jrow - 1), &c__1, &
		    c__, &s);
	    if (ilz) {
		drot_(n, &z___ref(1, jrow), &c__1, &z___ref(1, jrow - 1), &
			c__1, &c__, &s);
	    }
/* L30: */
	}
/* L40: */
    }

    return 0;

/*     End of DGGHRD */

} /* dgghrd_ */

#undef z___ref
#undef q_ref
#undef b_ref
#undef a_ref

