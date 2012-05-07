#include "blaswrap.h"
/*  -- translated by f2c (version 19990503).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "f2c.h"

/* Common Block Declarations */

struct {
    doublereal ops, itcnt;
} latime_;

#define latime_1 latime_

struct {
    doublereal opcnt[6], timng[6];
} lstime_;

#define lstime_1 lstime_

/* Table of constant values */

static doublecomplex c_b1 = {0.,0.};
static doublecomplex c_b2 = {1.,0.};
static integer c__1 = 1;
static integer c_n1 = -1;
static integer c__0 = 0;
static integer c__2 = 2;

/* Subroutine */ int zgelsy_(integer *m, integer *n, integer *nrhs, 
	doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, 
	integer *jpvt, doublereal *rcond, integer *rank, doublecomplex *work, 
	integer *lwork, doublereal *rwork, integer *info)
{
    /* Initialized data */

    static integer gelsy = 1;
    static integer geqp3 = 2;
    static integer trsm = 5;
    static integer tzrzf = 3;
    static integer unmqr = 4;
    static integer unmrz = 6;

    /* System generated locals */
    integer a_dim1, a_offset, b_dim1, b_offset, i__1, i__2, i__3, i__4;
    doublereal d__1, d__2;
    doublecomplex z__1;

    /* Builtin functions */
    double z_abs(doublecomplex *);

    /* Local variables */
    static doublereal anrm, bnrm, smin, smax;
    static integer i__, j, iascl, ibscl;
    extern doublereal dopla_(char *, integer *, integer *, integer *, integer 
	    *, integer *);
    static integer ismin, ismax;
    static doublecomplex c1, c2;
    static doublereal wsize, t1, t2;
    static doublecomplex s1, s2;
    extern /* Subroutine */ int zcopy_(integer *, doublecomplex *, integer *, 
	    doublecomplex *, integer *), ztrsm_(char *, char *, char *, char *
	    , integer *, integer *, doublecomplex *, doublecomplex *, integer 
	    *, doublecomplex *, integer *);
    extern doublereal dopbl3_(char *, integer *, integer *, integer *)
	    ;
    extern /* Subroutine */ int zlaic1_(integer *, integer *, doublecomplex *,
	     doublereal *, doublecomplex *, doublecomplex *, doublereal *, 
	    doublecomplex *, doublecomplex *), dlabad_(doublereal *, 
	    doublereal *), zgeqp3_(integer *, integer *, doublecomplex *, 
	    integer *, integer *, doublecomplex *, doublecomplex *, integer *,
	     doublereal *, integer *);
    static integer nb;
    extern doublereal dlamch_(char *);
    static integer mn;
    extern doublereal dsecnd_(void);
    extern /* Subroutine */ int xerbla_(char *, integer *);
    extern integer ilaenv_(integer *, char *, char *, integer *, integer *, 
	    integer *, integer *, ftnlen, ftnlen);
    extern doublereal zlange_(char *, integer *, integer *, doublecomplex *, 
	    integer *, doublereal *);
    static doublereal bignum;
    extern /* Subroutine */ int zlascl_(char *, integer *, integer *, 
	    doublereal *, doublereal *, integer *, integer *, doublecomplex *,
	     integer *, integer *);
    static integer nb1, nb2, nb3, nb4;
    extern /* Subroutine */ int zlaset_(char *, integer *, integer *, 
	    doublecomplex *, doublecomplex *, doublecomplex *, integer *);
    static doublereal sminpr, smaxpr, smlnum;
    static integer lwkopt;
    static logical lquery;
    extern /* Subroutine */ int zunmqr_(char *, char *, integer *, integer *, 
	    integer *, doublecomplex *, integer *, doublecomplex *, 
	    doublecomplex *, integer *, doublecomplex *, integer *, integer *), zunmrz_(char *, char *, integer *, integer *, 
	    integer *, integer *, doublecomplex *, integer *, doublecomplex *,
	     doublecomplex *, integer *, doublecomplex *, integer *, integer *
	    ), ztzrzf_(integer *, integer *, doublecomplex *, 
	    integer *, doublecomplex *, doublecomplex *, integer *, integer *)
	    ;


#define a_subscr(a_1,a_2) (a_2)*a_dim1 + a_1
#define a_ref(a_1,a_2) a[a_subscr(a_1,a_2)]
#define b_subscr(a_1,a_2) (a_2)*b_dim1 + a_1
#define b_ref(a_1,a_2) b[b_subscr(a_1,a_2)]


/*  -- LAPACK driver routine (instrumented to count ops, version 3.0) --   
       Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,   
       Courant Institute, Argonne National Lab, and Rice University   
       June 30, 1999   

       Common blocks to return operation counts and timings   

    Purpose   
    =======   

    ZGELSY computes the minimum-norm solution to a complex linear least   
    squares problem:   
        min || A * X - B ||   
    using a complete orthogonal factorization of A.  A is an M-by-N   
    matrix which may be rank-deficient.   

    Several right hand side vectors b and solution vectors x can be   
    handled in a single call; they are stored as the columns of the   
    M-by-NRHS right hand side matrix B and the N-by-NRHS solution   
    matrix X.   

    The routine first computes a QR factorization with column pivoting:   
        A * P = Q * [ R11 R12 ]   
                    [  0  R22 ]   
    with R11 defined as the largest leading submatrix whose estimated   
    condition number is less than 1/RCOND.  The order of R11, RANK,   
    is the effective rank of A.   

    Then, R22 is considered to be negligible, and R12 is annihilated   
    by unitary transformations from the right, arriving at the   
    complete orthogonal factorization:   
       A * P = Q * [ T11 0 ] * Z   
                   [  0  0 ]   
    The minimum-norm solution is then   
       X = P * Z' [ inv(T11)*Q1'*B ]   
                  [        0       ]   
    where Q1 consists of the first RANK columns of Q.   

    This routine is basically identical to the original xGELSX except   
    three differences:   
      o The permutation of matrix B (the right hand side) is faster and   
        more simple.   
      o The call to the subroutine xGEQPF has been substituted by the   
        the call to the subroutine xGEQP3. This subroutine is a Blas-3   
        version of the QR factorization with column pivoting.   
      o Matrix B (the right hand side) is updated with Blas-3.   

    Arguments   
    =========   

    M       (input) INTEGER   
            The number of rows of the matrix A.  M >= 0.   

    N       (input) INTEGER   
            The number of columns of the matrix A.  N >= 0.   

    NRHS    (input) INTEGER   
            The number of right hand sides, i.e., the number of   
            columns of matrices B and X. NRHS >= 0.   

    A       (input/output) COMPLEX*16 array, dimension (LDA,N)   
            On entry, the M-by-N matrix A.   
            On exit, A has been overwritten by details of its   
            complete orthogonal factorization.   

    LDA     (input) INTEGER   
            The leading dimension of the array A.  LDA >= max(1,M).   

    B       (input/output) COMPLEX*16 array, dimension (LDB,NRHS)   
            On entry, the M-by-NRHS right hand side matrix B.   
            On exit, the N-by-NRHS solution matrix X.   

    LDB     (input) INTEGER   
            The leading dimension of the array B. LDB >= max(1,M,N).   

    JPVT    (input/output) INTEGER array, dimension (N)   
            On entry, if JPVT(i) .ne. 0, the i-th column of A is permuted   
            to the front of AP, otherwise column i is a free column.   
            On exit, if JPVT(i) = k, then the i-th column of A*P   
            was the k-th column of A.   

    RCOND   (input) DOUBLE PRECISION   
            RCOND is used to determine the effective rank of A, which   
            is defined as the order of the largest leading triangular   
            submatrix R11 in the QR factorization with pivoting of A,   
            whose estimated condition number < 1/RCOND.   

    RANK    (output) INTEGER   
            The effective rank of A, i.e., the order of the submatrix   
            R11.  This is the same as the order of the submatrix T11   
            in the complete orthogonal factorization of A.   

    WORK    (workspace/output) COMPLEX*16 array, dimension (LWORK)   
            On exit, if INFO = 0, WORK(1) returns the optimal LWORK.   

    LWORK   (input) INTEGER   
            The dimension of the array WORK.   
            The unblocked strategy requires that:   
              LWORK >= MN + MAX( 2*MN, N+1, MN+NRHS )   
            where MN = min(M,N).   
            The block algorithm requires that:   
              LWORK >= MN + MAX( 2*MN, NB*(N+1), MN+MN*NB, MN+NB*NRHS )   
            where NB is an upper bound on the blocksize returned   
            by ILAENV for the routines ZGEQP3, ZTZRZF, CTZRQF, ZUNMQR,   
            and ZUNMRZ.   

            If LWORK = -1, then a workspace query is assumed; the routine   
            only calculates the optimal size of the WORK array, returns   
            this value as the first entry of the WORK array, and no error   
            message related to LWORK is issued by XERBLA.   

    RWORK   (workspace) DOUBLE PRECISION array, dimension (2*N)   

    INFO    (output) INTEGER   
            = 0: successful exit   
            < 0: if INFO = -i, the i-th argument had an illegal value   

    Further Details   
    ===============   

    Based on contributions by   
      A. Petitet, Computer Science Dept., Univ. of Tenn., Knoxville, USA   
      E. Quintana-Orti, Depto. de Informatica, Universidad Jaime I, Spain   
      G. Quintana-Orti, Depto. de Informatica, Universidad Jaime I, Spain   

    =====================================================================   

       Parameter adjustments */
    a_dim1 = *lda;
    a_offset = 1 + a_dim1 * 1;
    a -= a_offset;
    b_dim1 = *ldb;
    b_offset = 1 + b_dim1 * 1;
    b -= b_offset;
    --jpvt;
    --work;
    --rwork;

    /* Function Body */

    mn = min(*m,*n);
    ismin = mn + 1;
    ismax = (mn << 1) + 1;

/*     Test the input arguments. */

    *info = 0;
    nb1 = ilaenv_(&c__1, "ZGEQRF", " ", m, n, &c_n1, &c_n1, (ftnlen)6, (
	    ftnlen)1);
    nb2 = ilaenv_(&c__1, "ZGERQF", " ", m, n, &c_n1, &c_n1, (ftnlen)6, (
	    ftnlen)1);
    nb3 = ilaenv_(&c__1, "ZUNMQR", " ", m, n, nrhs, &c_n1, (ftnlen)6, (ftnlen)
	    1);
    nb4 = ilaenv_(&c__1, "ZUNMRQ", " ", m, n, nrhs, &c_n1, (ftnlen)6, (ftnlen)
	    1);
/* Computing MAX */
    i__1 = max(nb1,nb2), i__1 = max(i__1,nb3);
    nb = max(i__1,nb4);
/* Computing MAX */
    i__1 = 1, i__2 = mn + (*n << 1) + nb * (*n + 1), i__1 = max(i__1,i__2), 
	    i__2 = (mn << 1) + nb * *nrhs;
    lwkopt = max(i__1,i__2);
    z__1.r = (doublereal) lwkopt, z__1.i = 0.;
    work[1].r = z__1.r, work[1].i = z__1.i;
    lquery = *lwork == -1;
    if (*m < 0) {
	*info = -1;
    } else if (*n < 0) {
	*info = -2;
    } else if (*nrhs < 0) {
	*info = -3;
    } else if (*lda < max(1,*m)) {
	*info = -5;
    } else /* if(complicated condition) */ {
/* Computing MAX */
	i__1 = max(1,*m);
	if (*ldb < max(i__1,*n)) {
	    *info = -7;
	} else /* if(complicated condition) */ {
/* Computing MAX */
	    i__1 = mn << 1, i__2 = *n + 1, i__1 = max(i__1,i__2), i__2 = mn + 
		    *nrhs;
	    if (*lwork < mn + max(i__1,i__2) && ! lquery) {
		*info = -12;
	    }
	}
    }

    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("ZGELSY", &i__1);
	return 0;
    } else if (lquery) {
	return 0;
    }

/*     Quick return if possible   

   Computing MIN */
    i__1 = min(*m,*n);
    if (min(i__1,*nrhs) == 0) {
	*rank = 0;
	return 0;
    }

/*     Get machine parameters */

    lstime_1.opcnt[gelsy - 1] += 2.;
    smlnum = dlamch_("S") / dlamch_("P");
    bignum = 1. / smlnum;
    dlabad_(&smlnum, &bignum);

/*     Scale A, B if max entries outside range [SMLNUM,BIGNUM] */

    anrm = zlange_("M", m, n, &a[a_offset], lda, &rwork[1]);
    iascl = 0;
    if (anrm > 0. && anrm < smlnum) {

/*        Scale matrix norm up to SMLNUM */

	lstime_1.opcnt[gelsy - 1] += (doublereal) (*m * 6 * *n);
	zlascl_("G", &c__0, &c__0, &anrm, &smlnum, m, n, &a[a_offset], lda, 
		info);
	iascl = 1;
    } else if (anrm > bignum) {

/*        Scale matrix norm down to BIGNUM */

	lstime_1.opcnt[gelsy - 1] += (doublereal) (*m * 6 * *n);
	zlascl_("G", &c__0, &c__0, &anrm, &bignum, m, n, &a[a_offset], lda, 
		info);
	iascl = 2;
    } else if (anrm == 0.) {

/*        Matrix all zero. Return zero solution. */

	i__1 = max(*m,*n);
	zlaset_("F", &i__1, nrhs, &c_b1, &c_b1, &b[b_offset], ldb);
	*rank = 0;
	goto L70;
    }

    bnrm = zlange_("M", m, nrhs, &b[b_offset], ldb, &rwork[1]);
    ibscl = 0;
    if (bnrm > 0. && bnrm < smlnum) {

/*        Scale matrix norm up to SMLNUM */

	lstime_1.opcnt[gelsy - 1] += (doublereal) (*m * 6 * *nrhs);
	zlascl_("G", &c__0, &c__0, &bnrm, &smlnum, m, nrhs, &b[b_offset], ldb,
		 info);
	ibscl = 1;
    } else if (bnrm > bignum) {

/*        Scale matrix norm down to BIGNUM */

	lstime_1.opcnt[gelsy - 1] += (doublereal) (*m * 6 * *nrhs);
	zlascl_("G", &c__0, &c__0, &bnrm, &bignum, m, nrhs, &b[b_offset], ldb,
		 info);
	ibscl = 2;
    }

/*     Compute QR factorization with column pivoting of A:   
          A * P = Q * R */

    lstime_1.opcnt[geqp3 - 1] += dopla_("ZGEQPF", m, n, &c__0, &c__0, &c__0);
    t1 = dsecnd_();
    i__1 = *lwork - mn;
    zgeqp3_(m, n, &a[a_offset], lda, &jpvt[1], &work[1], &work[mn + 1], &i__1,
	     &rwork[1], info);
    t2 = dsecnd_();
    lstime_1.timng[geqp3 - 1] += t2 - t1;
    i__1 = mn + 1;
    wsize = mn + work[i__1].r;

/*     complex workspace: MN+NB*(N+1). real workspace 2*N.   
       Details of Householder rotations stored in WORK(1:MN).   

       Determine RANK using incremental condition estimation */

    i__1 = ismin;
    work[i__1].r = 1., work[i__1].i = 0.;
    i__1 = ismax;
    work[i__1].r = 1., work[i__1].i = 0.;
    smax = z_abs(&a_ref(1, 1));
    smin = smax;
    if (z_abs(&a_ref(1, 1)) == 0.) {
	*rank = 0;
	i__1 = max(*m,*n);
	zlaset_("F", &i__1, nrhs, &c_b1, &c_b1, &b[b_offset], ldb);
	goto L70;
    } else {
	*rank = 1;
    }

L10:
    if (*rank < mn) {
	i__ = *rank + 1;
	latime_1.ops = 0.;
	zlaic1_(&c__2, rank, &work[ismin], &smin, &a_ref(1, i__), &a_ref(i__, 
		i__), &sminpr, &s1, &c1);
	zlaic1_(&c__1, rank, &work[ismax], &smax, &a_ref(1, i__), &a_ref(i__, 
		i__), &smaxpr, &s2, &c2);
	lstime_1.opcnt[gelsy - 1] = lstime_1.opcnt[gelsy - 1] + latime_1.ops 
		+ 1.;

	if (smaxpr * *rcond <= sminpr) {
	    lstime_1.opcnt[gelsy - 1] += (doublereal) (*rank * 6);
	    i__1 = *rank;
	    for (i__ = 1; i__ <= i__1; ++i__) {
		i__2 = ismin + i__ - 1;
		i__3 = ismin + i__ - 1;
		z__1.r = s1.r * work[i__3].r - s1.i * work[i__3].i, z__1.i = 
			s1.r * work[i__3].i + s1.i * work[i__3].r;
		work[i__2].r = z__1.r, work[i__2].i = z__1.i;
		i__2 = ismax + i__ - 1;
		i__3 = ismax + i__ - 1;
		z__1.r = s2.r * work[i__3].r - s2.i * work[i__3].i, z__1.i = 
			s2.r * work[i__3].i + s2.i * work[i__3].r;
		work[i__2].r = z__1.r, work[i__2].i = z__1.i;
/* L20: */
	    }
	    i__1 = ismin + *rank;
	    work[i__1].r = c1.r, work[i__1].i = c1.i;
	    i__1 = ismax + *rank;
	    work[i__1].r = c2.r, work[i__1].i = c2.i;
	    smin = sminpr;
	    smax = smaxpr;
	    ++(*rank);
	    goto L10;
	}
    }

/*     complex workspace: 3*MN.   

       Logically partition R = [ R11 R12 ]   
                               [  0  R22 ]   
       where R11 = R(1:RANK,1:RANK)   

       [R11,R12] = [ T11, 0 ] * Y */

    if (*rank < *n) {
	lstime_1.opcnt[tzrzf - 1] += dopla_("ZTZRQF", rank, n, &c__0, &c__0, &
		c__0);
	t1 = dsecnd_();
	i__1 = *lwork - (mn << 1);
	ztzrzf_(rank, n, &a[a_offset], lda, &work[mn + 1], &work[(mn << 1) + 
		1], &i__1, info);
	t2 = dsecnd_();
	lstime_1.timng[tzrzf - 1] += t2 - t1;
    }

/*     complex workspace: 2*MN.   
       Details of Householder rotations stored in WORK(MN+1:2*MN)   

       B(1:M,1:NRHS) := Q' * B(1:M,1:NRHS) */

    lstime_1.opcnt[unmqr - 1] += dopla_("ZUNMQR", m, nrhs, &mn, &c__0, &c__0);
    t1 = dsecnd_();
    i__1 = *lwork - (mn << 1);
    zunmqr_("Left", "Conjugate transpose", m, nrhs, &mn, &a[a_offset], lda, &
	    work[1], &b[b_offset], ldb, &work[(mn << 1) + 1], &i__1, info);
    t2 = dsecnd_();
    lstime_1.timng[unmqr - 1] += t2 - t1;
/* Computing MAX */
    i__1 = (mn << 1) + 1;
    d__1 = wsize, d__2 = (mn << 1) + work[i__1].r;
    wsize = max(d__1,d__2);

/*     complex workspace: 2*MN+NB*NRHS.   

       B(1:RANK,1:NRHS) := inv(T11) * B(1:RANK,1:NRHS) */

    lstime_1.opcnt[trsm - 1] += dopbl3_("ZTRSM ", rank, nrhs, &c__0);
    t1 = dsecnd_();
    ztrsm_("Left", "Upper", "No transpose", "Non-unit", rank, nrhs, &c_b2, &a[
	    a_offset], lda, &b[b_offset], ldb);
    t2 = dsecnd_();
    lstime_1.timng[trsm - 1] += t2 - t1;

    i__1 = *nrhs;
    for (j = 1; j <= i__1; ++j) {
	i__2 = *n;
	for (i__ = *rank + 1; i__ <= i__2; ++i__) {
	    i__3 = b_subscr(i__, j);
	    b[i__3].r = 0., b[i__3].i = 0.;
/* L30: */
	}
/* L40: */
    }

/*     B(1:N,1:NRHS) := Y' * B(1:N,1:NRHS) */

    if (*rank < *n) {
	nb = ilaenv_(&c__1, "UNMRQ", "LC", n, nrhs, rank, &c_n1, (ftnlen)5, (
		ftnlen)2);
	lstime_1.opcnt[unmrz - 1] += dopla_("ZUNMRQ", n, nrhs, rank, &c__0, &
		nb);
	t1 = dsecnd_();
	i__1 = *n - *rank;
	i__2 = *lwork - (mn << 1);
	zunmrz_("Left", "Conjugate transpose", n, nrhs, rank, &i__1, &a[
		a_offset], lda, &work[mn + 1], &b[b_offset], ldb, &work[(mn <<
		 1) + 1], &i__2, info);
	t2 = dsecnd_();
	lstime_1.timng[unmrz - 1] += t2 - t1;
    }

/*     complex workspace: 2*MN+NRHS.   

       B(1:N,1:NRHS) := P * B(1:N,1:NRHS) */

    i__1 = *nrhs;
    for (j = 1; j <= i__1; ++j) {
	i__2 = *n;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    i__3 = jpvt[i__];
	    i__4 = b_subscr(i__, j);
	    work[i__3].r = b[i__4].r, work[i__3].i = b[i__4].i;
/* L50: */
	}
	zcopy_(n, &work[1], &c__1, &b_ref(1, j), &c__1);
/* L60: */
    }

/*     complex workspace: N.   

       Undo scaling */

    if (iascl == 1) {
	lstime_1.opcnt[gelsy - 1] += (doublereal) ((*n * *nrhs + *rank * *
		rank) * 6);
	zlascl_("G", &c__0, &c__0, &anrm, &smlnum, n, nrhs, &b[b_offset], ldb,
		 info);
	zlascl_("U", &c__0, &c__0, &smlnum, &anrm, rank, rank, &a[a_offset], 
		lda, info);
    } else if (iascl == 2) {
	lstime_1.opcnt[gelsy - 1] += (doublereal) ((*n * *nrhs + *rank * *
		rank) * 6);
	zlascl_("G", &c__0, &c__0, &anrm, &bignum, n, nrhs, &b[b_offset], ldb,
		 info);
	zlascl_("U", &c__0, &c__0, &bignum, &anrm, rank, rank, &a[a_offset], 
		lda, info);
    }
    if (ibscl == 1) {
	lstime_1.opcnt[gelsy - 1] += (doublereal) (*n * *nrhs * 6);
	zlascl_("G", &c__0, &c__0, &smlnum, &bnrm, n, nrhs, &b[b_offset], ldb,
		 info);
    } else if (ibscl == 2) {
	lstime_1.opcnt[gelsy - 1] += (doublereal) (*n * *nrhs * 6);
	zlascl_("G", &c__0, &c__0, &bignum, &bnrm, n, nrhs, &b[b_offset], ldb,
		 info);
    }

L70:
    z__1.r = (doublereal) lwkopt, z__1.i = 0.;
    work[1].r = z__1.r, work[1].i = z__1.i;

    return 0;

/*     End of ZGELSY */

} /* zgelsy_ */

#undef b_ref
#undef b_subscr
#undef a_ref
#undef a_subscr


