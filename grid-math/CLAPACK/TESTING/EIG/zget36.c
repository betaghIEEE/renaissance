#include "blaswrap.h"
/*  -- translated by f2c (version 19990503).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "f2c.h"

/* Table of constant values */

static doublecomplex c_b1 = {0.,0.};
static doublecomplex c_b2 = {1.,0.};
static integer c__3 = 3;
static integer c__1 = 1;
static integer c__7 = 7;
static integer c__10 = 10;
static integer c__11 = 11;
static integer c__200 = 200;

/* Subroutine */ int zget36_(doublereal *rmax, integer *lmax, integer *ninfo, 
	integer *knt, integer *nin)
{
    /* System generated locals */
    integer i__1, i__2, i__3, i__4;

    /* Builtin functions */
    integer s_rsle(cilist *), do_lio(integer *, integer *, char *, ftnlen), 
	    e_rsle(void);

    /* Local variables */
    static doublecomplex diag[10];
    static integer ifst, ilst;
    static doublecomplex work[200];
    static integer info1, info2, i__, j, n;
    static doublecomplex q[100]	/* was [10][10] */, ctemp;
    extern /* Subroutine */ int zhst01_(integer *, integer *, integer *, 
	    doublecomplex *, integer *, doublecomplex *, integer *, 
	    doublecomplex *, integer *, doublecomplex *, integer *, 
	    doublereal *, doublereal *);
    static doublereal rwork[10];
    static doublecomplex t1[100]	/* was [10][10] */, t2[100]	/* 
	    was [10][10] */;
    extern /* Subroutine */ int zcopy_(integer *, doublecomplex *, integer *, 
	    doublecomplex *, integer *);
    extern doublereal dlamch_(char *);
    extern /* Subroutine */ int zlacpy_(char *, integer *, integer *, 
	    doublecomplex *, integer *, doublecomplex *, integer *), 
	    zlaset_(char *, integer *, integer *, doublecomplex *, 
	    doublecomplex *, doublecomplex *, integer *);
    static doublereal result[2];
    extern /* Subroutine */ int ztrexc_(char *, integer *, doublecomplex *, 
	    integer *, doublecomplex *, integer *, integer *, integer *, 
	    integer *);
    static doublereal eps, res;
    static doublecomplex tmp[100]	/* was [10][10] */;

    /* Fortran I/O blocks */
    static cilist io___2 = { 0, 0, 0, 0, 0 };
    static cilist io___7 = { 0, 0, 0, 0, 0 };



#define q_subscr(a_1,a_2) (a_2)*10 + a_1 - 11
#define q_ref(a_1,a_2) q[q_subscr(a_1,a_2)]
#define t1_subscr(a_1,a_2) (a_2)*10 + a_1 - 11
#define t1_ref(a_1,a_2) t1[t1_subscr(a_1,a_2)]
#define t2_subscr(a_1,a_2) (a_2)*10 + a_1 - 11
#define t2_ref(a_1,a_2) t2[t2_subscr(a_1,a_2)]
#define tmp_subscr(a_1,a_2) (a_2)*10 + a_1 - 11
#define tmp_ref(a_1,a_2) tmp[tmp_subscr(a_1,a_2)]


/*  -- LAPACK test routine (version 3.0) --   
       Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,   
       Courant Institute, Argonne National Lab, and Rice University   
       September 30, 1994   


    Purpose   
    =======   

    ZGET36 tests ZTREXC, a routine for reordering diagonal entries of a   
    matrix in complex Schur form. Thus, ZLAEXC computes a unitary matrix   
    Q such that   

       Q' * T1 * Q  = T2   

    and where one of the diagonal blocks of T1 (the one at row IFST) has   
    been moved to position ILST.   

    The test code verifies that the residual Q'*T1*Q-T2 is small, that T2   
    is in Schur form, and that the final position of the IFST block is   
    ILST.   

    The test matrices are read from a file with logical unit number NIN.   

    Arguments   
    ==========   

    RMAX    (output) DOUBLE PRECISION   
            Value of the largest test ratio.   

    LMAX    (output) INTEGER   
            Example number where largest test ratio achieved.   

    NINFO   (output) INTEGER   
            Number of examples where INFO is nonzero.   

    KNT     (output) INTEGER   
            Total number of examples tested.   

    NIN     (input) INTEGER   
            Input logical unit number.   

    ===================================================================== */


    eps = dlamch_("P");
    *rmax = 0.;
    *lmax = 0;
    *knt = 0;
    *ninfo = 0;

/*     Read input data until N=0 */

L10:
    io___2.ciunit = *nin;
    s_rsle(&io___2);
    do_lio(&c__3, &c__1, (char *)&n, (ftnlen)sizeof(integer));
    do_lio(&c__3, &c__1, (char *)&ifst, (ftnlen)sizeof(integer));
    do_lio(&c__3, &c__1, (char *)&ilst, (ftnlen)sizeof(integer));
    e_rsle();
    if (n == 0) {
	return 0;
    }
    ++(*knt);
    i__1 = n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	io___7.ciunit = *nin;
	s_rsle(&io___7);
	i__2 = n;
	for (j = 1; j <= i__2; ++j) {
	    do_lio(&c__7, &c__1, (char *)&tmp_ref(i__, j), (ftnlen)sizeof(
		    doublecomplex));
	}
	e_rsle();
/* L20: */
    }
    zlacpy_("F", &n, &n, tmp, &c__10, t1, &c__10);
    zlacpy_("F", &n, &n, tmp, &c__10, t2, &c__10);
    res = 0.;

/*     Test without accumulating Q */

    zlaset_("Full", &n, &n, &c_b1, &c_b2, q, &c__10);
    ztrexc_("N", &n, t1, &c__10, q, &c__10, &ifst, &ilst, &info1);
    i__1 = n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	i__2 = n;
	for (j = 1; j <= i__2; ++j) {
	    i__3 = q_subscr(i__, j);
	    if (i__ == j && (q[i__3].r != 1. || q[i__3].i != 0.)) {
		res += 1. / eps;
	    }
	    i__3 = q_subscr(i__, j);
	    if (i__ != j && (q[i__3].r != 0. || q[i__3].i != 0.)) {
		res += 1. / eps;
	    }
/* L30: */
	}
/* L40: */
    }

/*     Test with accumulating Q */

    zlaset_("Full", &n, &n, &c_b1, &c_b2, q, &c__10);
    ztrexc_("V", &n, t2, &c__10, q, &c__10, &ifst, &ilst, &info2);

/*     Compare T1 with T2 */

    i__1 = n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	i__2 = n;
	for (j = 1; j <= i__2; ++j) {
	    i__3 = t1_subscr(i__, j);
	    i__4 = t2_subscr(i__, j);
	    if (t1[i__3].r != t2[i__4].r || t1[i__3].i != t2[i__4].i) {
		res += 1. / eps;
	    }
/* L50: */
	}
/* L60: */
    }
    if (info1 != 0 || info2 != 0) {
	++(*ninfo);
    }
    if (info1 != info2) {
	res += 1. / eps;
    }

/*     Test for successful reordering of T2 */

    zcopy_(&n, tmp, &c__11, diag, &c__1);
    if (ifst < ilst) {
	i__1 = ilst;
	for (i__ = ifst + 1; i__ <= i__1; ++i__) {
	    i__2 = i__ - 1;
	    ctemp.r = diag[i__2].r, ctemp.i = diag[i__2].i;
	    i__2 = i__ - 1;
	    i__3 = i__ - 2;
	    diag[i__2].r = diag[i__3].r, diag[i__2].i = diag[i__3].i;
	    i__2 = i__ - 2;
	    diag[i__2].r = ctemp.r, diag[i__2].i = ctemp.i;
/* L70: */
	}
    } else if (ifst > ilst) {
	i__1 = ilst;
	for (i__ = ifst - 1; i__ >= i__1; --i__) {
	    i__2 = i__;
	    ctemp.r = diag[i__2].r, ctemp.i = diag[i__2].i;
	    i__2 = i__;
	    i__3 = i__ - 1;
	    diag[i__2].r = diag[i__3].r, diag[i__2].i = diag[i__3].i;
	    i__2 = i__ - 1;
	    diag[i__2].r = ctemp.r, diag[i__2].i = ctemp.i;
/* L80: */
	}
    }
    i__1 = n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	i__2 = t2_subscr(i__, i__);
	i__3 = i__ - 1;
	if (t2[i__2].r != diag[i__3].r || t2[i__2].i != diag[i__3].i) {
	    res += 1. / eps;
	}
/* L90: */
    }

/*     Test for small residual, and orthogonality of Q */

    zhst01_(&n, &c__1, &n, tmp, &c__10, t2, &c__10, q, &c__10, work, &c__200, 
	    rwork, result);
    res = res + result[0] + result[1];

/*     Test for T2 being in Schur form */

    i__1 = n - 1;
    for (j = 1; j <= i__1; ++j) {
	i__2 = n;
	for (i__ = j + 1; i__ <= i__2; ++i__) {
	    i__3 = t2_subscr(i__, j);
	    if (t2[i__3].r != 0. || t2[i__3].i != 0.) {
		res += 1. / eps;
	    }
/* L100: */
	}
/* L110: */
    }
    if (res > *rmax) {
	*rmax = res;
	*lmax = *knt;
    }
    goto L10;

/*     End of ZGET36 */

} /* zget36_ */

#undef tmp_ref
#undef tmp_subscr
#undef t2_ref
#undef t2_subscr
#undef t1_ref
#undef t1_subscr
#undef q_ref
#undef q_subscr


