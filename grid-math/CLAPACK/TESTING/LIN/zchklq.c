#include "blaswrap.h"
/*  -- translated by f2c (version 19990503).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "f2c.h"

/* Common Block Declarations */

struct {
    integer infot, nunit;
    logical ok, lerr;
} infoc_;

#define infoc_1 infoc_

struct {
    char srnamt[6];
} srnamc_;

#define srnamc_1 srnamc_

/* Table of constant values */

static integer c__2 = 2;
static integer c__0 = 0;
static integer c_n1 = -1;
static integer c__1 = 1;
static integer c__3 = 3;

/* Subroutine */ int zchklq_(logical *dotype, integer *nm, integer *mval, 
	integer *nn, integer *nval, integer *nnb, integer *nbval, integer *
	nxval, integer *nrhs, doublereal *thresh, logical *tsterr, integer *
	nmax, doublecomplex *a, doublecomplex *af, doublecomplex *aq, 
	doublecomplex *al, doublecomplex *ac, doublecomplex *b, doublecomplex 
	*x, doublecomplex *xact, doublecomplex *tau, doublecomplex *work, 
	doublereal *rwork, integer *iwork, integer *nout)
{
    /* Initialized data */

    static integer iseedy[4] = { 1988,1989,1990,1991 };

    /* Format strings */
    static char fmt_9999[] = "(\002 M=\002,i5,\002, N=\002,i5,\002, K=\002,i"
	    "5,\002, NB=\002,i4,\002, NX=\002,i5,\002, type \002,i2,\002, tes"
	    "t(\002,i2,\002)=\002,g12.5)";

    /* System generated locals */
    integer i__1, i__2, i__3, i__4, i__5;

    /* Builtin functions   
       Subroutine */ int s_copy(char *, char *, ftnlen, ftnlen);
    integer s_wsfe(cilist *), do_fio(integer *, char *, ftnlen), e_wsfe(void);

    /* Local variables */
    static integer mode, imat, info;
    static char path[3];
    static integer kval[4];
    static char dist[1], type__[1];
    static integer nrun, i__;
    extern /* Subroutine */ int alahd_(integer *, char *);
    static integer k, m, n, nfail, iseed[4];
    extern /* Subroutine */ int zget02_(char *, integer *, integer *, integer 
	    *, doublecomplex *, integer *, doublecomplex *, integer *, 
	    doublecomplex *, integer *, doublereal *, doublereal *);
    static doublereal anorm;
    static integer minmn, nerrs;
    extern /* Subroutine */ int zlqt01_(integer *, integer *, doublecomplex *,
	     doublecomplex *, doublecomplex *, doublecomplex *, integer *, 
	    doublecomplex *, doublecomplex *, integer *, doublereal *, 
	    doublereal *), zlqt02_(integer *, integer *, integer *, 
	    doublecomplex *, doublecomplex *, doublecomplex *, doublecomplex *
	    , integer *, doublecomplex *, doublecomplex *, integer *, 
	    doublereal *, doublereal *);
    static integer lwork;
    extern /* Subroutine */ int zlqt03_(integer *, integer *, integer *, 
	    doublecomplex *, doublecomplex *, doublecomplex *, doublecomplex *
	    , integer *, doublecomplex *, doublecomplex *, integer *, 
	    doublereal *, doublereal *), zlatb4_(char *, integer *, integer *,
	     integer *, char *, integer *, integer *, doublereal *, integer *,
	     doublereal *, char *);
    static integer nb, ik, im, in, kl, nk;
    extern /* Subroutine */ int alaerh_(char *, char *, integer *, integer *, 
	    char *, integer *, integer *, integer *, integer *, integer *, 
	    integer *, integer *, integer *, integer *);
    static integer ku, nt, nx;
    extern /* Subroutine */ int alasum_(char *, integer *, integer *, integer 
	    *, integer *);
    static doublereal cndnum;
    extern /* Subroutine */ int xlaenv_(integer *, integer *), zlacpy_(char *,
	     integer *, integer *, doublecomplex *, integer *, doublecomplex *
	    , integer *), zlarhs_(char *, char *, char *, char *, 
	    integer *, integer *, integer *, integer *, integer *, 
	    doublecomplex *, integer *, doublecomplex *, integer *, 
	    doublecomplex *, integer *, integer *, integer *), zgelqs_(integer *, integer *, integer *, 
	    doublecomplex *, integer *, doublecomplex *, doublecomplex *, 
	    integer *, doublecomplex *, integer *, integer *), zlatms_(
	    integer *, integer *, char *, integer *, char *, doublereal *, 
	    integer *, doublereal *, doublereal *, integer *, integer *, char 
	    *, doublecomplex *, integer *, doublecomplex *, integer *);
    static doublereal result[7];
    extern /* Subroutine */ int zerrlq_(char *, integer *);
    static integer lda, inb;

    /* Fortran I/O blocks */
    static cilist io___33 = { 0, 0, 0, fmt_9999, 0 };



/*  -- LAPACK test routine (version 3.0) --   
       Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,   
       Courant Institute, Argonne National Lab, and Rice University   
       September 30, 1994   


    Purpose   
    =======   

    ZCHKLQ tests ZGELQF, ZUNGLQ and CUNMLQ.   

    Arguments   
    =========   

    DOTYPE  (input) LOGICAL array, dimension (NTYPES)   
            The matrix types to be used for testing.  Matrices of type j   
            (for 1 <= j <= NTYPES) are used for testing if DOTYPE(j) =   
            .TRUE.; if DOTYPE(j) = .FALSE., then type j is not used.   

    NM      (input) INTEGER   
            The number of values of M contained in the vector MVAL.   

    MVAL    (input) INTEGER array, dimension (NM)   
            The values of the matrix row dimension M.   

    NN      (input) INTEGER   
            The number of values of N contained in the vector NVAL.   

    NVAL    (input) INTEGER array, dimension (NN)   
            The values of the matrix column dimension N.   

    NNB     (input) INTEGER   
            The number of values of NB and NX contained in the   
            vectors NBVAL and NXVAL.  The blocking parameters are used   
            in pairs (NB,NX).   

    NBVAL   (input) INTEGER array, dimension (NNB)   
            The values of the blocksize NB.   

    NXVAL   (input) INTEGER array, dimension (NNB)   
            The values of the crossover point NX.   

    NRHS    (input) INTEGER   
            The number of right hand side vectors to be generated for   
            each linear system.   

    THRESH  (input) DOUBLE PRECISION   
            The threshold value for the test ratios.  A result is   
            included in the output file if RESULT >= THRESH.  To have   
            every test ratio printed, use THRESH = 0.   

    TSTERR  (input) LOGICAL   
            Flag that indicates whether error exits are to be tested.   

    NMAX    (input) INTEGER   
            The maximum value permitted for M or N, used in dimensioning   
            the work arrays.   

    A       (workspace) COMPLEX*16 array, dimension (NMAX*NMAX)   

    AF      (workspace) COMPLEX*16 array, dimension (NMAX*NMAX)   

    AQ      (workspace) COMPLEX*16 array, dimension (NMAX*NMAX)   

    AL      (workspace) COMPLEX*16 array, dimension (NMAX*NMAX)   

    AC      (workspace) COMPLEX*16 array, dimension (NMAX*NMAX)   

    B       (workspace) COMPLEX*16 array, dimension (NMAX*NRHS)   

    X       (workspace) COMPLEX*16 array, dimension (NMAX*NRHS)   

    XACT    (workspace) COMPLEX*16 array, dimension (NMAX*NRHS)   

    TAU     (workspace) COMPLEX*16 array, dimension (NMAX)   

    WORK    (workspace) COMPLEX*16 array, dimension (NMAX*NMAX)   

    RWORK   (workspace) DOUBLE PRECISION array, dimension (NMAX)   

    IWORK   (workspace) INTEGER array, dimension (NMAX)   

    NOUT    (input) INTEGER   
            The unit number for output.   

    =====================================================================   

       Parameter adjustments */
    --iwork;
    --rwork;
    --work;
    --tau;
    --xact;
    --x;
    --b;
    --ac;
    --al;
    --aq;
    --af;
    --a;
    --nxval;
    --nbval;
    --nval;
    --mval;
    --dotype;

    /* Function Body   

       Initialize constants and the random number seed. */

    s_copy(path, "Zomplex precision", (ftnlen)1, (ftnlen)17);
    s_copy(path + 1, "LQ", (ftnlen)2, (ftnlen)2);
    nrun = 0;
    nfail = 0;
    nerrs = 0;
    for (i__ = 1; i__ <= 4; ++i__) {
	iseed[i__ - 1] = iseedy[i__ - 1];
/* L10: */
    }

/*     Test the error exits */

    if (*tsterr) {
	zerrlq_(path, nout);
    }
    infoc_1.infot = 0;
    xlaenv_(&c__2, &c__2);

    lda = *nmax;
    lwork = *nmax * max(*nmax,*nrhs);

/*     Do for each value of M in MVAL. */

    i__1 = *nm;
    for (im = 1; im <= i__1; ++im) {
	m = mval[im];

/*        Do for each value of N in NVAL. */

	i__2 = *nn;
	for (in = 1; in <= i__2; ++in) {
	    n = nval[in];
	    minmn = min(m,n);
	    for (imat = 1; imat <= 8; ++imat) {

/*              Do the tests only if DOTYPE( IMAT ) is true. */

		if (! dotype[imat]) {
		    goto L50;
		}

/*              Set up parameters with ZLATB4 and generate a test matrix   
                with ZLATMS. */

		zlatb4_(path, &imat, &m, &n, type__, &kl, &ku, &anorm, &mode, 
			&cndnum, dist);

		s_copy(srnamc_1.srnamt, "ZLATMS", (ftnlen)6, (ftnlen)6);
		zlatms_(&m, &n, dist, iseed, type__, &rwork[1], &mode, &
			cndnum, &anorm, &kl, &ku, "No packing", &a[1], &lda, &
			work[1], &info);

/*              Check error code from ZLATMS. */

		if (info != 0) {
		    alaerh_(path, "ZLATMS", &info, &c__0, " ", &m, &n, &c_n1, 
			    &c_n1, &c_n1, &imat, &nfail, &nerrs, nout);
		    goto L50;
		}

/*              Set some values for K: the first value must be MINMN,   
                corresponding to the call of ZLQT01; other values are   
                used in the calls of ZLQT02, and must not exceed MINMN. */

		kval[0] = minmn;
		kval[1] = 0;
		kval[2] = 1;
		kval[3] = minmn / 2;
		if (minmn == 0) {
		    nk = 1;
		} else if (minmn == 1) {
		    nk = 2;
		} else if (minmn <= 3) {
		    nk = 3;
		} else {
		    nk = 4;
		}

/*              Do for each value of K in KVAL */

		i__3 = nk;
		for (ik = 1; ik <= i__3; ++ik) {
		    k = kval[ik - 1];

/*                 Do for each pair of values (NB,NX) in NBVAL and NXVAL. */

		    i__4 = *nnb;
		    for (inb = 1; inb <= i__4; ++inb) {
			nb = nbval[inb];
			xlaenv_(&c__1, &nb);
			nx = nxval[inb];
			xlaenv_(&c__3, &nx);
			nt = 2;
			if (ik == 1) {

/*                       Test ZGELQF */

			    zlqt01_(&m, &n, &a[1], &af[1], &aq[1], &al[1], &
				    lda, &tau[1], &work[1], &lwork, &rwork[1],
				     result);
			} else if (m <= n) {

/*                       Test ZUNGLQ, using factorization   
                         returned by ZLQT01 */

			    zlqt02_(&m, &n, &k, &a[1], &af[1], &aq[1], &al[1],
				     &lda, &tau[1], &work[1], &lwork, &rwork[
				    1], result);
			} else {
			    result[0] = 0.;
			    result[1] = 0.;
			}
			if (m >= k) {

/*                       Test ZUNMLQ, using factorization returned   
                         by ZLQT01 */

			    zlqt03_(&m, &n, &k, &af[1], &ac[1], &al[1], &aq[1]
				    , &lda, &tau[1], &work[1], &lwork, &rwork[
				    1], &result[2]);
			    nt += 4;

/*                       If M>=N and K=N, call ZGELQS to solve a system   
                         with NRHS right hand sides and compute the   
                         residual. */

			    if (k == m && inb == 1) {

/*                          Generate a solution and set the right   
                            hand side. */

				s_copy(srnamc_1.srnamt, "ZLARHS", (ftnlen)6, (
					ftnlen)6);
				zlarhs_(path, "New", "Full", "No transpose", &
					m, &n, &c__0, &c__0, nrhs, &a[1], &
					lda, &xact[1], &lda, &b[1], &lda, 
					iseed, &info);

				zlacpy_("Full", &m, nrhs, &b[1], &lda, &x[1], 
					&lda);
				s_copy(srnamc_1.srnamt, "ZGELQS", (ftnlen)6, (
					ftnlen)6);
				zgelqs_(&m, &n, nrhs, &af[1], &lda, &tau[1], &
					x[1], &lda, &work[1], &lwork, &info);

/*                          Check error code from ZGELQS. */

				if (info != 0) {
				    alaerh_(path, "ZGELQS", &info, &c__0, 
					    " ", &m, &n, nrhs, &c_n1, &nb, &
					    imat, &nfail, &nerrs, nout);
				}

				zget02_("No transpose", &m, &n, nrhs, &a[1], &
					lda, &x[1], &lda, &b[1], &lda, &rwork[
					1], &result[6]);
				++nt;
			    } else {
				result[6] = 0.;
			    }
			} else {
			    result[2] = 0.;
			    result[3] = 0.;
			    result[4] = 0.;
			    result[5] = 0.;
			}

/*                    Print information about the tests that did not   
                      pass the threshold. */

			i__5 = nt;
			for (i__ = 1; i__ <= i__5; ++i__) {
			    if (result[i__ - 1] >= *thresh) {
				if (nfail == 0 && nerrs == 0) {
				    alahd_(nout, path);
				}
				io___33.ciunit = *nout;
				s_wsfe(&io___33);
				do_fio(&c__1, (char *)&m, (ftnlen)sizeof(
					integer));
				do_fio(&c__1, (char *)&n, (ftnlen)sizeof(
					integer));
				do_fio(&c__1, (char *)&k, (ftnlen)sizeof(
					integer));
				do_fio(&c__1, (char *)&nb, (ftnlen)sizeof(
					integer));
				do_fio(&c__1, (char *)&nx, (ftnlen)sizeof(
					integer));
				do_fio(&c__1, (char *)&imat, (ftnlen)sizeof(
					integer));
				do_fio(&c__1, (char *)&i__, (ftnlen)sizeof(
					integer));
				do_fio(&c__1, (char *)&result[i__ - 1], (
					ftnlen)sizeof(doublereal));
				e_wsfe();
				++nfail;
			    }
/* L20: */
			}
			nrun += nt;
/* L30: */
		    }
/* L40: */
		}
L50:
		;
	    }
/* L60: */
	}
/* L70: */
    }

/*     Print a summary of the results. */

    alasum_(path, nout, &nfail, &nrun, &nerrs);

    return 0;

/*     End of ZCHKLQ */

} /* zchklq_ */

