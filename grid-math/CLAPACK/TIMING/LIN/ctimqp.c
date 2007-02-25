#include "blaswrap.h"
/*  -- translated by f2c (version 19990503).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "f2c.h"

/* Table of constant values */

static integer c__1 = 1;
static integer c__0 = 0;
static integer c__2 = 2;

/* Subroutine */ int ctimqp_(char *line, integer *nm, integer *mval, integer *
	nval, integer *nlda, integer *ldaval, real *timmin, complex *a, 
	complex *copya, complex *tau, complex *work, real *rwork, integer *
	iwork, real *reslts, integer *ldr1, integer *ldr2, integer *nout, 
	ftnlen line_len)
{
    /* Initialized data */

    static char subnam[6*1] = "CGEQPF";
    static integer modes[2] = { 2,3 };
    static integer iseed[4] = { 0,0,0,1 };

    /* Format strings */
    static char fmt_9999[] = "(1x,a6,\002 timing run not attempted\002,/)";
    static char fmt_9998[] = "(/\002 *** Speed of \002,a6,\002 in megaflops "
	    "***\002)";
    static char fmt_9997[] = "(5x,\002line \002,i2,\002 with LDA = \002,i5)";

    /* System generated locals */
    integer reslts_dim1, reslts_dim2, reslts_offset, i__1, i__2, i__3;

    /* Builtin functions   
       Subroutine */ int s_copy(char *, char *, ftnlen, ftnlen);
    integer s_wsfe(cilist *), do_fio(integer *, char *, ftnlen), e_wsfe(void),
	     s_wsle(cilist *), e_wsle(void);

    /* Local variables */
    static integer ilda;
    static real cond;
    static integer mode;
    static real dmax__;
    static integer info;
    static char path[3];
    static real time;
    static integer i__, m, n;
    static char cname[6];
    static integer imode, minmn;
    extern doublereal sopla_(char *, integer *, integer *, integer *, integer 
	    *, integer *);
    extern /* Subroutine */ int icopy_(integer *, integer *, integer *, 
	    integer *, integer *);
    static real s1, s2;
    static integer ic;
    extern /* Subroutine */ int sprtb5_(char *, char *, char *, integer *, 
	    integer *, integer *, integer *, integer *, integer *, real *, 
	    integer *, integer *, integer *, ftnlen, ftnlen, ftnlen);
    static integer im;
    extern /* Subroutine */ int cgeqpf_(integer *, integer *, complex *, 
	    integer *, integer *, complex *, complex *, real *, integer *);
    extern doublereal slamch_(char *);
    extern /* Subroutine */ int atimck_(integer *, char *, integer *, integer 
	    *, integer *, integer *, integer *, integer *, ftnlen);
    extern doublereal second_(void);
    extern /* Subroutine */ int clacpy_(char *, integer *, integer *, complex 
	    *, integer *, complex *, integer *), atimin_(char *, char 
	    *, integer *, char *, logical *, integer *, integer *, ftnlen, 
	    ftnlen, ftnlen), clatms_(integer *, integer *, char *, integer *, 
	    char *, real *, integer *, real *, real *, integer *, integer *, 
	    char *, complex *, integer *, complex *, integer *);
    extern doublereal smflop_(real *, real *, integer *);
    static real untime;
    static logical timsub[1];
    static integer lda, icl;
    static real ops;

    /* Fortran I/O blocks */
    static cilist io___8 = { 0, 0, 0, fmt_9999, 0 };
    static cilist io___27 = { 0, 0, 0, fmt_9998, 0 };
    static cilist io___28 = { 0, 0, 0, fmt_9997, 0 };
    static cilist io___29 = { 0, 0, 0, 0, 0 };



#define subnam_ref(a_0,a_1) &subnam[(a_1)*6 + a_0 - 6]
#define reslts_ref(a_1,a_2,a_3) reslts[((a_3)*reslts_dim2 + (a_2))*\
reslts_dim1 + a_1]


/*  -- LAPACK timing routine (version 3.0) --   
       Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,   
       Courant Institute, Argonne National Lab, and Rice University   
       June 30, 1999   


    Purpose   
    =======   

    CTIMQP times the LAPACK routines to perform the QR factorization with   
    column pivoting of a COMPLEX general matrix.   

    Two matrix types may be used for timing.  The number of types is   
    set in the parameter NMODE and the matrix types are set in the vector   
    MODES, using the following key:   
       2.  BREAK1    D(1:N-1)=1 and D(N)=1.0/COND in CLATMS   
       3.  GEOM      D(I)=COND**(-(I-1)/(N-1)) in CLATMS   
    These numbers are chosen to correspond with the matrix types in the   
    test code.   

    Arguments   
    =========   

    LINE    (input) CHARACTER*80   
            The input line that requested this routine.  The first six   
            characters contain either the name of a subroutine or a   
            generic path name.  The remaining characters may be used to   
            specify the individual routines to be timed.  See ATIMIN for   
            a full description of the format of the input line.   

    NM      (input) INTEGER   
            The number of values of M and N contained in the vectors   
            MVAL and NVAL.  The matrix sizes are used in pairs (M,N).   

    MVAL    (input) INTEGER array, dimension (NM)   
            The values of the matrix row dimension M.   

    NVAL    (input) INTEGER array, dimension (NM)   
            The values of the matrix column dimension N.   

    NLDA    (input) INTEGER   
            The number of values of LDA contained in the vector LDAVAL.   

    LDAVAL  (input) INTEGER array, dimension (NLDA)   
            The values of the leading dimension of the array A.   

    TIMMIN  (input) REAL   
            The minimum time a subroutine will be timed.   

    A       (workspace) COMPLEX array, dimension (LDAMAX*NMAX)   
            where LDAMAX and NMAX are the maximum values of LDA and N.   

    COPYA   (workspace) COMPLEX array, dimension (LDAMAX*NMAX)   

    TAU     (workspace) COMPLEX array, dimension (min(M,N))   

    WORK    (workspace) COMPLEX array, dimension (3*max(MMAX,NMAX))   

    RWORK   (workspace) REAL array, dimension (2*NMAX)   

    IWORK   (workspace) INTEGER array, dimension (2*NMAX)   

    RESLTS  (workspace) REAL array, dimension   
                        (LDR1,LDR2,NLDA)   
            The timing results for each subroutine over the relevant   
            values of MODE, (M,N), and LDA.   

    LDR1    (input) INTEGER   
            The first dimension of RESLTS.  LDR1 >= max(1,NM).   

    LDR2    (input) INTEGER   
            The second dimension of RESLTS.  LDR2 >= max(1,NM).   

    NOUT    (input) INTEGER   
            The unit number for output.   

    =====================================================================   

       Parameter adjustments */
    --mval;
    --nval;
    --ldaval;
    --a;
    --copya;
    --tau;
    --work;
    --rwork;
    --iwork;
    reslts_dim1 = *ldr1;
    reslts_dim2 = *ldr2;
    reslts_offset = 1 + reslts_dim1 * (1 + reslts_dim2 * 1);
    reslts -= reslts_offset;

    /* Function Body   

       Extract the timing request from the input line. */

    s_copy(path, "Complex precision", (ftnlen)1, (ftnlen)17);
    s_copy(path + 1, "QP", (ftnlen)2, (ftnlen)2);
    atimin_(path, line, &c__1, subnam, timsub, nout, &info, (ftnlen)3, (
	    ftnlen)80, (ftnlen)6);
    if (! timsub[0] || info != 0) {
	goto L80;
    }

/*     Check that M <= LDA for the input values. */

    s_copy(cname, line, (ftnlen)6, (ftnlen)6);
    atimck_(&c__1, cname, nm, &mval[1], nlda, &ldaval[1], nout, &info, (
	    ftnlen)6);
    if (info > 0) {
	io___8.ciunit = *nout;
	s_wsfe(&io___8);
	do_fio(&c__1, cname, (ftnlen)6);
	e_wsfe();
	goto L80;
    }

/*     Set the condition number and scaling factor for the matrices   
       to be generated. */

    dmax__ = 1.f;
    cond = 1.f / slamch_("Precision");

/*     Do for each pair of values (M,N): */

    i__1 = *nm;
    for (im = 1; im <= i__1; ++im) {
	m = mval[im];
	n = nval[im];
	minmn = min(m,n);

/*        Do for each value of LDA: */

	i__2 = *nlda;
	for (ilda = 1; ilda <= i__2; ++ilda) {
	    lda = ldaval[ilda];
	    for (imode = 1; imode <= 2; ++imode) {
		mode = modes[imode - 1];

/*              Generate a test matrix of size m by n using the   
                singular value distribution indicated by MODE. */

		i__3 = n;
		for (i__ = 1; i__ <= i__3; ++i__) {
		    iwork[n + i__] = 0;
/* L10: */
		}
		clatms_(&m, &n, "Uniform", iseed, "Nonsymm", &rwork[1], &mode,
			 &cond, &dmax__, &m, &n, "No packing", &copya[1], &
			lda, &work[1], &info);

/*              CGEQPF:  QR factorization with column pivoting */

		clacpy_("All", &m, &n, &copya[1], &lda, &a[1], &lda);
		icopy_(&n, &iwork[n + 1], &c__1, &iwork[1], &c__1);
		ic = 0;
		s1 = second_();
L20:
		cgeqpf_(&m, &n, &a[1], &lda, &iwork[1], &tau[1], &work[1], &
			rwork[1], &info);
		s2 = second_();
		time = s2 - s1;
		++ic;
		if (time < *timmin) {
		    clacpy_("All", &m, &n, &copya[1], &lda, &a[1], &lda);
		    icopy_(&n, &iwork[n + 1], &c__1, &iwork[1], &c__1);
		    goto L20;
		}

/*              Subtract the time used in CLACPY and ICOPY. */

		icl = 1;
		s1 = second_();
L30:
		s2 = second_();
		untime = s2 - s1;
		++icl;
		if (icl <= ic) {
		    clacpy_("All", &m, &n, &copya[1], &lda, &a[1], &lda);
		    icopy_(&n, &iwork[n + 1], &c__1, &iwork[1], &c__1);
		    goto L30;
		}

		time = (time - untime) / (real) ic;
		ops = sopla_("CGEQPF", &m, &n, &c__0, &c__0, &c__1)
			;
		reslts_ref(imode, im, ilda) = smflop_(&ops, &time, &info);

/* L40: */
	    }
/* L50: */
	}
/* L60: */
    }

/*     Print tables of results */

    io___27.ciunit = *nout;
    s_wsfe(&io___27);
    do_fio(&c__1, subnam_ref(0, 1), (ftnlen)6);
    e_wsfe();
    if (*nlda > 1) {
	i__1 = *nlda;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    io___28.ciunit = *nout;
	    s_wsfe(&io___28);
	    do_fio(&c__1, (char *)&i__, (ftnlen)sizeof(integer));
	    do_fio(&c__1, (char *)&ldaval[i__], (ftnlen)sizeof(integer));
	    e_wsfe();
/* L70: */
	}
    }
    io___29.ciunit = *nout;
    s_wsle(&io___29);
    e_wsle();
    sprtb5_("Type", "M", "N", &c__2, modes, nm, &mval[1], &nval[1], nlda, &
	    reslts[reslts_offset], ldr1, ldr2, nout, (ftnlen)4, (ftnlen)1, (
	    ftnlen)1);
L80:
    return 0;

/*     End of CTIMQP */

} /* ctimqp_ */

#undef reslts_ref
#undef subnam_ref


