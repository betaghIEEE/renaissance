#include "blaswrap.h"
/*  -- translated by f2c (version 19990503).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "f2c.h"

/* Common Block Declarations */

struct {
    integer iparms[100];
} claenv_;

#define claenv_1 claenv_

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

static integer c__3 = 3;
static integer c__1 = 1;
static integer c__12 = 12;
static integer c__0 = 0;
static integer c__132 = 132;
static integer c__16 = 16;
static integer c__4 = 4;
static integer c__8 = 8;
static integer c__2 = 2;
static integer c__5 = 5;
static integer c__6 = 6;

/* Main program */ MAIN__(void)
{
    /* Initialized data */

    static real threq = 2.f;
    static char intstr[10] = "0123456789";

    /* Format strings */
    static char fmt_9994[] = "(\002 Tests of the COMPLEX LAPACK routines "
	    "\002,/\002 LAPACK VERSION 3.0, released June 30, 1999 \002,//"
	    "\002 The following parameter values will be used:\002)";
    static char fmt_9996[] = "(\002 Invalid input value: \002,a4,\002=\002,i"
	    "6,\002; must be >=\002,i6)";
    static char fmt_9995[] = "(\002 Invalid input value: \002,a4,\002=\002,i"
	    "6,\002; must be <=\002,i6)";
    static char fmt_9993[] = "(4x,a4,\002:  \002,10i6,/11x,10i6)";
    static char fmt_9992[] = "(/\002 Routines pass computational tests if te"
	    "st ratio is \002,\002less than\002,f8.2,/)";
    static char fmt_9999[] = "(/\002 Execution not attempted due to input er"
	    "rors\002)";
    static char fmt_9991[] = "(\002 Relative machine \002,a,\002 is taken to"
	    " be\002,e16.6)";
    static char fmt_9990[] = "(/1x,a3,\002:  Unrecognized path name\002)";
    static char fmt_9989[] = "(/1x,a3,\002 routines were not tested\002)";
    static char fmt_9988[] = "(/1x,a3,\002 driver routines were not teste"
	    "d\002)";
    static char fmt_9998[] = "(/\002 End of tests\002)";
    static char fmt_9997[] = "(\002 Total time used = \002,f12.2,\002 seco"
	    "nds\002,/)";

    /* System generated locals */
    integer i__1, i__2;
    real r__1;
    cllist cl__1;

    /* Builtin functions */
    integer s_rsle(cilist *), e_rsle(void), s_wsfe(cilist *), e_wsfe(void), 
	    do_lio(integer *, integer *, char *, ftnlen), do_fio(integer *, 
	    char *, ftnlen);
    /* Subroutine */ int s_stop(char *, ftnlen);
    integer s_wsle(cilist *), e_wsle(void), s_rsfe(cilist *), e_rsfe(void);
    /* Subroutine */ int s_copy(char *, char *, ftnlen, ftnlen);
    integer f_clos(cllist *);

    /* Local variables */
    static char path[3];
    static integer mval[12], nval[12], nrhs;
    static complex work[20856]	/* was [132][158] */, a[153384]	/* was [21912]
	    [7] */, b[8448]	/* was [2112][4] */;
    static integer lafac, i__, j, k;
    static real s[264];
    static logical fatal;
    static char aline[72];
    extern logical lsame_(char *, char *);
    static integer nbval[12], nmats;
    static char c1[1], c2[2];
    static integer nsval[12], nxval[12], iwork[3300];
    static real s1, s2, rwork[19832];
    extern /* Subroutine */ int cchkq3_(logical *, integer *, integer *, 
	    integer *, integer *, integer *, integer *, integer *, real *, 
	    complex *, complex *, real *, real *, complex *, complex *, real *
	    , integer *, integer *);
    static integer nbval2[12], ic, la, nb;
    extern /* Subroutine */ int cchkgb_(logical *, integer *, integer *, 
	    integer *, integer *, integer *, integer *, integer *, integer *, 
	    real *, logical *, complex *, integer *, complex *, integer *, 
	    complex *, complex *, complex *, complex *, real *, integer *, 
	    integer *), cchkge_(logical *, integer *, integer *, integer *, 
	    integer *, integer *, integer *, integer *, integer *, real *, 
	    logical *, integer *, complex *, complex *, complex *, complex *, 
	    complex *, complex *, complex *, real *, integer *, integer *), 
	    cchkhe_(logical *, integer *, integer *, integer *, integer *, 
	    integer *, integer *, real *, logical *, integer *, complex *, 
	    complex *, complex *, complex *, complex *, complex *, complex *, 
	    real *, integer *, integer *), cchkpb_(logical *, integer *, 
	    integer *, integer *, integer *, integer *, integer *, real *, 
	    logical *, integer *, complex *, complex *, complex *, complex *, 
	    complex *, complex *, complex *, real *, integer *);
    static integer nm, nn;
    extern /* Subroutine */ int cchkeq_(real *, integer *), cchktb_(logical *,
	     integer *, integer *, integer *, integer *, real *, logical *, 
	    integer *, complex *, complex *, complex *, complex *, complex *, 
	    complex *, real *, integer *), cchkhp_(logical *, integer *, 
	    integer *, integer *, integer *, real *, logical *, integer *, 
	    complex *, complex *, complex *, complex *, complex *, complex *, 
	    complex *, real *, integer *, integer *), cchkgt_(logical *, 
	    integer *, integer *, integer *, integer *, real *, logical *, 
	    complex *, complex *, complex *, complex *, complex *, complex *, 
	    real *, integer *, integer *), alareq_(char *, integer *, logical 
	    *, integer *, integer *, integer *), cchklq_(logical *, 
	    integer *, integer *, integer *, integer *, integer *, integer *, 
	    integer *, integer *, real *, logical *, integer *, complex *, 
	    complex *, complex *, complex *, complex *, complex *, complex *, 
	    complex *, complex *, complex *, real *, integer *, integer *);
    extern doublereal slamch_(char *);
    extern /* Subroutine */ int cchkpo_(logical *, integer *, integer *, 
	    integer *, integer *, integer *, integer *, real *, logical *, 
	    integer *, complex *, complex *, complex *, complex *, complex *, 
	    complex *, complex *, real *, integer *), cchkpp_(logical *, 
	    integer *, integer *, integer *, integer *, real *, logical *, 
	    integer *, complex *, complex *, complex *, complex *, complex *, 
	    complex *, complex *, real *, integer *), cchkql_(logical *, 
	    integer *, integer *, integer *, integer *, integer *, integer *, 
	    integer *, integer *, real *, logical *, integer *, complex *, 
	    complex *, complex *, complex *, complex *, complex *, complex *, 
	    complex *, complex *, complex *, real *, integer *, integer *);
    extern doublereal second_(void);
    extern /* Subroutine */ int cchkpt_(logical *, integer *, integer *, 
	    integer *, integer *, real *, logical *, complex *, real *, 
	    complex *, complex *, complex *, complex *, complex *, real *, 
	    integer *), cchkqp_(logical *, integer *, integer *, integer *, 
	    integer *, real *, logical *, complex *, complex *, real *, real *
	    , complex *, complex *, real *, integer *, integer *), cchkqr_(
	    logical *, integer *, integer *, integer *, integer *, integer *, 
	    integer *, integer *, integer *, real *, logical *, integer *, 
	    complex *, complex *, complex *, complex *, complex *, complex *, 
	    complex *, complex *, complex *, complex *, real *, integer *, 
	    integer *);
    extern logical lsamen_(integer *, char *, char *);
    extern /* Subroutine */ int cchkrq_(logical *, integer *, integer *, 
	    integer *, integer *, integer *, integer *, integer *, integer *, 
	    real *, logical *, integer *, complex *, complex *, complex *, 
	    complex *, complex *, complex *, complex *, complex *, complex *, 
	    complex *, real *, integer *, integer *), cchksp_(logical *, 
	    integer *, integer *, integer *, integer *, real *, logical *, 
	    integer *, complex *, complex *, complex *, complex *, complex *, 
	    complex *, complex *, real *, integer *, integer *), cchktp_(
	    logical *, integer *, integer *, integer *, integer *, real *, 
	    logical *, integer *, complex *, complex *, complex *, complex *, 
	    complex *, complex *, real *, integer *), cchktr_(logical *, 
	    integer *, integer *, integer *, integer *, integer *, integer *, 
	    real *, logical *, integer *, complex *, complex *, complex *, 
	    complex *, complex *, complex *, real *, integer *), cchksy_(
	    logical *, integer *, integer *, integer *, integer *, integer *, 
	    integer *, real *, logical *, integer *, complex *, complex *, 
	    complex *, complex *, complex *, complex *, complex *, real *, 
	    integer *, integer *), cdrvgb_(logical *, integer *, integer *, 
	    integer *, real *, logical *, complex *, integer *, complex *, 
	    integer *, complex *, complex *, complex *, complex *, complex *, 
	    real *, complex *, real *, integer *, integer *), cchktz_(logical 
	    *, integer *, integer *, integer *, integer *, real *, logical *, 
	    complex *, complex *, real *, real *, complex *, complex *, real *
	    , integer *), cdrvge_(logical *, integer *, integer *, integer *, 
	    real *, logical *, integer *, complex *, complex *, complex *, 
	    complex *, complex *, complex *, complex *, real *, complex *, 
	    real *, integer *, integer *), cdrvgt_(logical *, integer *, 
	    integer *, integer *, real *, logical *, complex *, complex *, 
	    complex *, complex *, complex *, complex *, real *, integer *, 
	    integer *), cdrvhe_(logical *, integer *, integer *, integer *, 
	    real *, logical *, integer *, complex *, complex *, complex *, 
	    complex *, complex *, complex *, complex *, real *, integer *, 
	    integer *), cdrvhp_(logical *, integer *, integer *, integer *, 
	    real *, logical *, integer *, complex *, complex *, complex *, 
	    complex *, complex *, complex *, complex *, real *, integer *, 
	    integer *), cdrvpb_(logical *, integer *, integer *, integer *, 
	    real *, logical *, integer *, complex *, complex *, complex *, 
	    complex *, complex *, complex *, complex *, real *, complex *, 
	    real *, integer *);
    static real thresh;
    extern /* Subroutine */ int cdrvls_(logical *, integer *, integer *, 
	    integer *, integer *, integer *, integer *, integer *, integer *, 
	    integer *, real *, logical *, complex *, complex *, complex *, 
	    complex *, complex *, real *, real *, complex *, real *, integer *
	    , integer *), cdrvpo_(logical *, integer *, integer *, integer *, 
	    real *, logical *, integer *, complex *, complex *, complex *, 
	    complex *, complex *, complex *, complex *, real *, complex *, 
	    real *, integer *);
    static logical tstchk;
    extern /* Subroutine */ int cdrvpp_(logical *, integer *, integer *, 
	    integer *, real *, logical *, integer *, complex *, complex *, 
	    complex *, complex *, complex *, complex *, complex *, real *, 
	    complex *, real *, integer *), cdrvpt_(logical *, integer *, 
	    integer *, integer *, real *, logical *, complex *, real *, 
	    complex *, complex *, complex *, complex *, complex *, real *, 
	    integer *);
    static logical dotype[30];
    extern /* Subroutine */ int cdrvsp_(logical *, integer *, integer *, 
	    integer *, real *, logical *, integer *, complex *, complex *, 
	    complex *, complex *, complex *, complex *, complex *, real *, 
	    integer *, integer *), cdrvsy_(logical *, integer *, integer *, 
	    integer *, real *, logical *, integer *, complex *, complex *, 
	    complex *, complex *, complex *, complex *, complex *, real *, 
	    integer *, integer *);
    static integer ntypes;
    static logical tsterr, tstdrv;
    static integer lda, nnb;
    static real eps;
    static integer nns, nnb2;

    /* Fortran I/O blocks */
    static cilist io___6 = { 0, 5, 0, 0, 0 };
    static cilist io___7 = { 0, 6, 0, fmt_9994, 0 };
    static cilist io___8 = { 0, 5, 0, 0, 0 };
    static cilist io___10 = { 0, 6, 0, fmt_9996, 0 };
    static cilist io___11 = { 0, 6, 0, fmt_9995, 0 };
    static cilist io___12 = { 0, 5, 0, 0, 0 };
    static cilist io___15 = { 0, 6, 0, fmt_9996, 0 };
    static cilist io___16 = { 0, 6, 0, fmt_9995, 0 };
    static cilist io___17 = { 0, 6, 0, fmt_9993, 0 };
    static cilist io___18 = { 0, 5, 0, 0, 0 };
    static cilist io___20 = { 0, 6, 0, fmt_9996, 0 };
    static cilist io___21 = { 0, 6, 0, fmt_9995, 0 };
    static cilist io___22 = { 0, 5, 0, 0, 0 };
    static cilist io___24 = { 0, 6, 0, fmt_9996, 0 };
    static cilist io___25 = { 0, 6, 0, fmt_9995, 0 };
    static cilist io___26 = { 0, 6, 0, fmt_9993, 0 };
    static cilist io___27 = { 0, 5, 0, 0, 0 };
    static cilist io___29 = { 0, 6, 0, fmt_9996, 0 };
    static cilist io___30 = { 0, 6, 0, fmt_9995, 0 };
    static cilist io___31 = { 0, 5, 0, 0, 0 };
    static cilist io___33 = { 0, 6, 0, fmt_9996, 0 };
    static cilist io___34 = { 0, 6, 0, fmt_9995, 0 };
    static cilist io___35 = { 0, 6, 0, fmt_9993, 0 };
    static cilist io___36 = { 0, 5, 0, 0, 0 };
    static cilist io___38 = { 0, 6, 0, fmt_9996, 0 };
    static cilist io___39 = { 0, 6, 0, fmt_9995, 0 };
    static cilist io___40 = { 0, 5, 0, 0, 0 };
    static cilist io___42 = { 0, 6, 0, fmt_9996, 0 };
    static cilist io___43 = { 0, 6, 0, fmt_9993, 0 };
    static cilist io___48 = { 0, 5, 0, 0, 0 };
    static cilist io___50 = { 0, 6, 0, fmt_9996, 0 };
    static cilist io___51 = { 0, 6, 0, fmt_9993, 0 };
    static cilist io___52 = { 0, 5, 0, 0, 0 };
    static cilist io___54 = { 0, 6, 0, fmt_9992, 0 };
    static cilist io___55 = { 0, 5, 0, 0, 0 };
    static cilist io___57 = { 0, 5, 0, 0, 0 };
    static cilist io___59 = { 0, 5, 0, 0, 0 };
    static cilist io___61 = { 0, 6, 0, fmt_9999, 0 };
    static cilist io___63 = { 0, 6, 0, fmt_9991, 0 };
    static cilist io___64 = { 0, 6, 0, fmt_9991, 0 };
    static cilist io___65 = { 0, 6, 0, fmt_9991, 0 };
    static cilist io___66 = { 0, 6, 0, 0, 0 };
    static cilist io___68 = { 0, 5, 1, "(A72)", 0 };
    static cilist io___76 = { 0, 6, 0, fmt_9990, 0 };
    static cilist io___77 = { 0, 6, 0, fmt_9989, 0 };
    static cilist io___85 = { 0, 6, 0, fmt_9989, 0 };
    static cilist io___87 = { 0, 6, 0, fmt_9988, 0 };
    static cilist io___90 = { 0, 6, 0, fmt_9989, 0 };
    static cilist io___91 = { 0, 6, 0, fmt_9988, 0 };
    static cilist io___92 = { 0, 6, 0, fmt_9989, 0 };
    static cilist io___93 = { 0, 6, 0, fmt_9988, 0 };
    static cilist io___94 = { 0, 6, 0, fmt_9989, 0 };
    static cilist io___95 = { 0, 6, 0, fmt_9988, 0 };
    static cilist io___96 = { 0, 6, 0, fmt_9989, 0 };
    static cilist io___97 = { 0, 6, 0, fmt_9988, 0 };
    static cilist io___98 = { 0, 6, 0, fmt_9989, 0 };
    static cilist io___99 = { 0, 6, 0, fmt_9988, 0 };
    static cilist io___100 = { 0, 6, 0, fmt_9989, 0 };
    static cilist io___101 = { 0, 6, 0, fmt_9988, 0 };
    static cilist io___102 = { 0, 6, 0, fmt_9989, 0 };
    static cilist io___103 = { 0, 6, 0, fmt_9988, 0 };
    static cilist io___104 = { 0, 6, 0, fmt_9989, 0 };
    static cilist io___105 = { 0, 6, 0, fmt_9988, 0 };
    static cilist io___106 = { 0, 6, 0, fmt_9989, 0 };
    static cilist io___107 = { 0, 6, 0, fmt_9988, 0 };
    static cilist io___108 = { 0, 6, 0, fmt_9989, 0 };
    static cilist io___109 = { 0, 6, 0, fmt_9988, 0 };
    static cilist io___110 = { 0, 6, 0, fmt_9989, 0 };
    static cilist io___111 = { 0, 6, 0, fmt_9989, 0 };
    static cilist io___112 = { 0, 6, 0, fmt_9989, 0 };
    static cilist io___113 = { 0, 6, 0, fmt_9989, 0 };
    static cilist io___114 = { 0, 6, 0, fmt_9989, 0 };
    static cilist io___115 = { 0, 6, 0, fmt_9989, 0 };
    static cilist io___116 = { 0, 6, 0, fmt_9989, 0 };
    static cilist io___117 = { 0, 6, 0, fmt_9989, 0 };
    static cilist io___118 = { 0, 6, 0, fmt_9989, 0 };
    static cilist io___119 = { 0, 6, 0, fmt_9989, 0 };
    static cilist io___120 = { 0, 6, 0, fmt_9989, 0 };
    static cilist io___121 = { 0, 6, 0, fmt_9990, 0 };
    static cilist io___123 = { 0, 6, 0, fmt_9998, 0 };
    static cilist io___124 = { 0, 6, 0, fmt_9997, 0 };



#define a_subscr(a_1,a_2) (a_2)*21912 + a_1 - 21913
#define a_ref(a_1,a_2) a[a_subscr(a_1,a_2)]
#define b_subscr(a_1,a_2) (a_2)*2112 + a_1 - 2113
#define b_ref(a_1,a_2) b[b_subscr(a_1,a_2)]


/*  -- LAPACK test routine (version 3.0) --   
       Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,   
       Courant Institute, Argonne National Lab, and Rice University   
       June 30, 1999   

    Purpose   
    =======   

    CCHKAA is the main test program for the COMPLEX linear equation   
    routines.   

    The program must be driven by a short data file. The first 14 records   
    specify problem dimensions and program options using list-directed   
    input.  The remaining lines specify the LAPACK test paths and the   
    number of matrix types to use in testing.  An annotated example of a   
    data file can be obtained by deleting the first 3 characters from the   
    following 38 lines:   
    Data file for testing COMPLEX LAPACK linear equation routines   
    7                      Number of values of M   
    0 1 2 3 5 10 16        Values of M (row dimension)   
    7                      Number of values of N   
    0 1 2 3 5 10 16        Values of N (column dimension)   
    1                      Number of values of NRHS   
    2                      Values of NRHS (number of right hand sides)   
    5                      Number of values of NB   
    1 3 3 3 20             Values of NB (the blocksize)   
    1 0 5 9 1              Values of NX (crossover point)   
    30.0                   Threshold value of test ratio   
    T                      Put T to test the LAPACK routines   
    T                      Put T to test the driver routines   
    T                      Put T to test the error exits   
    CGE   11               List types on next line if 0 < NTYPES < 11   
    CGB    8               List types on next line if 0 < NTYPES <  8   
    CGT   12               List types on next line if 0 < NTYPES < 12   
    CPO    9               List types on next line if 0 < NTYPES <  9   
    CPP    9               List types on next line if 0 < NTYPES <  9   
    CPB    8               List types on next line if 0 < NTYPES <  8   
    CPT   12               List types on next line if 0 < NTYPES < 12   
    CHE   10               List types on next line if 0 < NTYPES < 10   
    CHP   10               List types on next line if 0 < NTYPES < 10   
    CSY   11               List types on next line if 0 < NTYPES < 11   
    CSP   11               List types on next line if 0 < NTYPES < 11   
    CTR   18               List types on next line if 0 < NTYPES < 18   
    CTP   18               List types on next line if 0 < NTYPES < 18   
    CTB   17               List types on next line if 0 < NTYPES < 17   
    CQR    8               List types on next line if 0 < NTYPES <  8   
    CRQ    8               List types on next line if 0 < NTYPES <  8   
    CLQ    8               List types on next line if 0 < NTYPES <  8   
    CQL    8               List types on next line if 0 < NTYPES <  8   
    CQP    6               List types on next line if 0 < NTYPES <  6   
    CTZ    3               List types on next line if 0 < NTYPES <  3   
    CLS    6               List types on next line if 0 < NTYPES <  6   
    CEQ   

    Internal Parameters   
    ===================   

    NMAX    INTEGER   
            The maximum allowable value for N.   

    MAXIN   INTEGER   
            The number of different values that can be used for each of   
            M, N, or NB   

    MAXRHS  INTEGER   
            The maximum number of right hand sides   

    NIN     INTEGER   
            The unit number for input   

    NOUT    INTEGER   
            The unit number for output   

    ===================================================================== */


    s1 = second_();
    lda = 132;
    fatal = FALSE_;

/*     Read a dummy line. */

    s_rsle(&io___6);
    e_rsle();

/*     Report values of parameters. */

    s_wsfe(&io___7);
    e_wsfe();

/*     Read the values of M */

    s_rsle(&io___8);
    do_lio(&c__3, &c__1, (char *)&nm, (ftnlen)sizeof(integer));
    e_rsle();
    if (nm < 1) {
	s_wsfe(&io___10);
	do_fio(&c__1, " NM ", (ftnlen)4);
	do_fio(&c__1, (char *)&nm, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&c__1, (ftnlen)sizeof(integer));
	e_wsfe();
	nm = 0;
	fatal = TRUE_;
    } else if (nm > 12) {
	s_wsfe(&io___11);
	do_fio(&c__1, " NM ", (ftnlen)4);
	do_fio(&c__1, (char *)&nm, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&c__12, (ftnlen)sizeof(integer));
	e_wsfe();
	nm = 0;
	fatal = TRUE_;
    }
    s_rsle(&io___12);
    i__1 = nm;
    for (i__ = 1; i__ <= i__1; ++i__) {
	do_lio(&c__3, &c__1, (char *)&mval[i__ - 1], (ftnlen)sizeof(integer));
    }
    e_rsle();
    i__1 = nm;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (mval[i__ - 1] < 0) {
	    s_wsfe(&io___15);
	    do_fio(&c__1, " M  ", (ftnlen)4);
	    do_fio(&c__1, (char *)&mval[i__ - 1], (ftnlen)sizeof(integer));
	    do_fio(&c__1, (char *)&c__0, (ftnlen)sizeof(integer));
	    e_wsfe();
	    fatal = TRUE_;
	} else if (mval[i__ - 1] > 132) {
	    s_wsfe(&io___16);
	    do_fio(&c__1, " M  ", (ftnlen)4);
	    do_fio(&c__1, (char *)&mval[i__ - 1], (ftnlen)sizeof(integer));
	    do_fio(&c__1, (char *)&c__132, (ftnlen)sizeof(integer));
	    e_wsfe();
	    fatal = TRUE_;
	}
/* L10: */
    }
    if (nm > 0) {
	s_wsfe(&io___17);
	do_fio(&c__1, "M   ", (ftnlen)4);
	i__1 = nm;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    do_fio(&c__1, (char *)&mval[i__ - 1], (ftnlen)sizeof(integer));
	}
	e_wsfe();
    }

/*     Read the values of N */

    s_rsle(&io___18);
    do_lio(&c__3, &c__1, (char *)&nn, (ftnlen)sizeof(integer));
    e_rsle();
    if (nn < 1) {
	s_wsfe(&io___20);
	do_fio(&c__1, " NN ", (ftnlen)4);
	do_fio(&c__1, (char *)&nn, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&c__1, (ftnlen)sizeof(integer));
	e_wsfe();
	nn = 0;
	fatal = TRUE_;
    } else if (nn > 12) {
	s_wsfe(&io___21);
	do_fio(&c__1, " NN ", (ftnlen)4);
	do_fio(&c__1, (char *)&nn, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&c__12, (ftnlen)sizeof(integer));
	e_wsfe();
	nn = 0;
	fatal = TRUE_;
    }
    s_rsle(&io___22);
    i__1 = nn;
    for (i__ = 1; i__ <= i__1; ++i__) {
	do_lio(&c__3, &c__1, (char *)&nval[i__ - 1], (ftnlen)sizeof(integer));
    }
    e_rsle();
    i__1 = nn;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (nval[i__ - 1] < 0) {
	    s_wsfe(&io___24);
	    do_fio(&c__1, " N  ", (ftnlen)4);
	    do_fio(&c__1, (char *)&nval[i__ - 1], (ftnlen)sizeof(integer));
	    do_fio(&c__1, (char *)&c__0, (ftnlen)sizeof(integer));
	    e_wsfe();
	    fatal = TRUE_;
	} else if (nval[i__ - 1] > 132) {
	    s_wsfe(&io___25);
	    do_fio(&c__1, " N  ", (ftnlen)4);
	    do_fio(&c__1, (char *)&nval[i__ - 1], (ftnlen)sizeof(integer));
	    do_fio(&c__1, (char *)&c__132, (ftnlen)sizeof(integer));
	    e_wsfe();
	    fatal = TRUE_;
	}
/* L20: */
    }
    if (nn > 0) {
	s_wsfe(&io___26);
	do_fio(&c__1, "N   ", (ftnlen)4);
	i__1 = nn;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    do_fio(&c__1, (char *)&nval[i__ - 1], (ftnlen)sizeof(integer));
	}
	e_wsfe();
    }

/*     Read the values of NRHS */

    s_rsle(&io___27);
    do_lio(&c__3, &c__1, (char *)&nns, (ftnlen)sizeof(integer));
    e_rsle();
    if (nns < 1) {
	s_wsfe(&io___29);
	do_fio(&c__1, " NNS", (ftnlen)4);
	do_fio(&c__1, (char *)&nns, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&c__1, (ftnlen)sizeof(integer));
	e_wsfe();
	nns = 0;
	fatal = TRUE_;
    } else if (nns > 12) {
	s_wsfe(&io___30);
	do_fio(&c__1, " NNS", (ftnlen)4);
	do_fio(&c__1, (char *)&nns, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&c__12, (ftnlen)sizeof(integer));
	e_wsfe();
	nns = 0;
	fatal = TRUE_;
    }
    s_rsle(&io___31);
    i__1 = nns;
    for (i__ = 1; i__ <= i__1; ++i__) {
	do_lio(&c__3, &c__1, (char *)&nsval[i__ - 1], (ftnlen)sizeof(integer))
		;
    }
    e_rsle();
    i__1 = nns;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (nsval[i__ - 1] < 0) {
	    s_wsfe(&io___33);
	    do_fio(&c__1, "NRHS", (ftnlen)4);
	    do_fio(&c__1, (char *)&nsval[i__ - 1], (ftnlen)sizeof(integer));
	    do_fio(&c__1, (char *)&c__0, (ftnlen)sizeof(integer));
	    e_wsfe();
	    fatal = TRUE_;
	} else if (nsval[i__ - 1] > 16) {
	    s_wsfe(&io___34);
	    do_fio(&c__1, "NRHS", (ftnlen)4);
	    do_fio(&c__1, (char *)&nsval[i__ - 1], (ftnlen)sizeof(integer));
	    do_fio(&c__1, (char *)&c__16, (ftnlen)sizeof(integer));
	    e_wsfe();
	    fatal = TRUE_;
	}
/* L30: */
    }
    if (nns > 0) {
	s_wsfe(&io___35);
	do_fio(&c__1, "NRHS", (ftnlen)4);
	i__1 = nns;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    do_fio(&c__1, (char *)&nsval[i__ - 1], (ftnlen)sizeof(integer));
	}
	e_wsfe();
    }

/*     Read the values of NB */

    s_rsle(&io___36);
    do_lio(&c__3, &c__1, (char *)&nnb, (ftnlen)sizeof(integer));
    e_rsle();
    if (nnb < 1) {
	s_wsfe(&io___38);
	do_fio(&c__1, "NNB ", (ftnlen)4);
	do_fio(&c__1, (char *)&nnb, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&c__1, (ftnlen)sizeof(integer));
	e_wsfe();
	nnb = 0;
	fatal = TRUE_;
    } else if (nnb > 12) {
	s_wsfe(&io___39);
	do_fio(&c__1, "NNB ", (ftnlen)4);
	do_fio(&c__1, (char *)&nnb, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&c__12, (ftnlen)sizeof(integer));
	e_wsfe();
	nnb = 0;
	fatal = TRUE_;
    }
    s_rsle(&io___40);
    i__1 = nnb;
    for (i__ = 1; i__ <= i__1; ++i__) {
	do_lio(&c__3, &c__1, (char *)&nbval[i__ - 1], (ftnlen)sizeof(integer))
		;
    }
    e_rsle();
    i__1 = nnb;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (nbval[i__ - 1] < 0) {
	    s_wsfe(&io___42);
	    do_fio(&c__1, " NB ", (ftnlen)4);
	    do_fio(&c__1, (char *)&nbval[i__ - 1], (ftnlen)sizeof(integer));
	    do_fio(&c__1, (char *)&c__0, (ftnlen)sizeof(integer));
	    e_wsfe();
	    fatal = TRUE_;
	}
/* L40: */
    }
    if (nnb > 0) {
	s_wsfe(&io___43);
	do_fio(&c__1, "NB  ", (ftnlen)4);
	i__1 = nnb;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    do_fio(&c__1, (char *)&nbval[i__ - 1], (ftnlen)sizeof(integer));
	}
	e_wsfe();
    }

/*     Set NBVAL2 to be the set of unique values of NB */

    nnb2 = 0;
    i__1 = nnb;
    for (i__ = 1; i__ <= i__1; ++i__) {
	nb = nbval[i__ - 1];
	i__2 = nnb2;
	for (j = 1; j <= i__2; ++j) {
	    if (nb == nbval2[j - 1]) {
		goto L60;
	    }
/* L50: */
	}
	++nnb2;
	nbval2[nnb2 - 1] = nb;
L60:
	;
    }

/*     Read the values of NX */

    s_rsle(&io___48);
    i__1 = nnb;
    for (i__ = 1; i__ <= i__1; ++i__) {
	do_lio(&c__3, &c__1, (char *)&nxval[i__ - 1], (ftnlen)sizeof(integer))
		;
    }
    e_rsle();
    i__1 = nnb;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (nxval[i__ - 1] < 0) {
	    s_wsfe(&io___50);
	    do_fio(&c__1, " NX ", (ftnlen)4);
	    do_fio(&c__1, (char *)&nxval[i__ - 1], (ftnlen)sizeof(integer));
	    do_fio(&c__1, (char *)&c__0, (ftnlen)sizeof(integer));
	    e_wsfe();
	    fatal = TRUE_;
	}
/* L70: */
    }
    if (nnb > 0) {
	s_wsfe(&io___51);
	do_fio(&c__1, "NX  ", (ftnlen)4);
	i__1 = nnb;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    do_fio(&c__1, (char *)&nxval[i__ - 1], (ftnlen)sizeof(integer));
	}
	e_wsfe();
    }

/*     Read the threshold value for the test ratios. */

    s_rsle(&io___52);
    do_lio(&c__4, &c__1, (char *)&thresh, (ftnlen)sizeof(real));
    e_rsle();
    s_wsfe(&io___54);
    do_fio(&c__1, (char *)&thresh, (ftnlen)sizeof(real));
    e_wsfe();

/*     Read the flag that indicates whether to test the LAPACK routines. */

    s_rsle(&io___55);
    do_lio(&c__8, &c__1, (char *)&tstchk, (ftnlen)sizeof(logical));
    e_rsle();

/*     Read the flag that indicates whether to test the driver routines. */

    s_rsle(&io___57);
    do_lio(&c__8, &c__1, (char *)&tstdrv, (ftnlen)sizeof(logical));
    e_rsle();

/*     Read the flag that indicates whether to test the error exits. */

    s_rsle(&io___59);
    do_lio(&c__8, &c__1, (char *)&tsterr, (ftnlen)sizeof(logical));
    e_rsle();

    if (fatal) {
	s_wsfe(&io___61);
	e_wsfe();
	s_stop("", (ftnlen)0);
    }

/*     Calculate and print the machine dependent constants. */

    eps = slamch_("Underflow threshold");
    s_wsfe(&io___63);
    do_fio(&c__1, "underflow", (ftnlen)9);
    do_fio(&c__1, (char *)&eps, (ftnlen)sizeof(real));
    e_wsfe();
    eps = slamch_("Overflow threshold");
    s_wsfe(&io___64);
    do_fio(&c__1, "overflow ", (ftnlen)9);
    do_fio(&c__1, (char *)&eps, (ftnlen)sizeof(real));
    e_wsfe();
    eps = slamch_("Epsilon");
    s_wsfe(&io___65);
    do_fio(&c__1, "precision", (ftnlen)9);
    do_fio(&c__1, (char *)&eps, (ftnlen)sizeof(real));
    e_wsfe();
    s_wsle(&io___66);
    e_wsle();
    nrhs = nsval[0];

L80:

/*     Read a test path and the number of matrix types to use. */

    i__1 = s_rsfe(&io___68);
    if (i__1 != 0) {
	goto L140;
    }
    i__1 = do_fio(&c__1, aline, (ftnlen)72);
    if (i__1 != 0) {
	goto L140;
    }
    i__1 = e_rsfe();
    if (i__1 != 0) {
	goto L140;
    }
    s_copy(path, aline, (ftnlen)3, (ftnlen)3);
    nmats = 30;
    i__ = 3;
L90:
    ++i__;
    if (i__ > 72) {
	goto L130;
    }
    if (*(unsigned char *)&aline[i__ - 1] == ' ') {
	goto L90;
    }
    nmats = 0;
L100:
    *(unsigned char *)c1 = *(unsigned char *)&aline[i__ - 1];
    for (k = 1; k <= 10; ++k) {
	if (*(unsigned char *)c1 == *(unsigned char *)&intstr[k - 1]) {
	    ic = k - 1;
	    goto L120;
	}
/* L110: */
    }
    goto L130;
L120:
    nmats = nmats * 10 + ic;
    ++i__;
    if (i__ > 72) {
	goto L130;
    }
    goto L100;
L130:
    *(unsigned char *)c1 = *(unsigned char *)path;
    s_copy(c2, path + 1, (ftnlen)2, (ftnlen)2);

/*     Check first character for correct precision. */

    if (! lsame_(c1, "Complex precision")) {
	s_wsfe(&io___76);
	do_fio(&c__1, path, (ftnlen)3);
	e_wsfe();

    } else if (nmats <= 0) {

/*        Check for a positive number of tests requested. */

	s_wsfe(&io___77);
	do_fio(&c__1, path, (ftnlen)3);
	e_wsfe();

    } else if (lsamen_(&c__2, c2, "GE")) {

/*        GE:  general matrices */

	ntypes = 11;
	alareq_(path, &nmats, dotype, &ntypes, &c__5, &c__6);

	if (tstchk) {
	    cchkge_(dotype, &nm, mval, &nn, nval, &nnb2, nbval2, &nns, nsval, 
		    &thresh, &tsterr, &lda, &a_ref(1, 1), &a_ref(1, 2), &
		    a_ref(1, 3), &b_ref(1, 1), &b_ref(1, 2), &b_ref(1, 3), 
		    work, rwork, iwork, &c__6);
	} else {
	    s_wsfe(&io___85);
	    do_fio(&c__1, path, (ftnlen)3);
	    e_wsfe();
	}

	if (tstdrv) {
	    cdrvge_(dotype, &nn, nval, &nrhs, &thresh, &tsterr, &lda, &a_ref(
		    1, 1), &a_ref(1, 2), &a_ref(1, 3), &b_ref(1, 1), &b_ref(1,
		     2), &b_ref(1, 3), &b_ref(1, 4), s, work, rwork, iwork, &
		    c__6);
	} else {
	    s_wsfe(&io___87);
	    do_fio(&c__1, path, (ftnlen)3);
	    e_wsfe();
	}

    } else if (lsamen_(&c__2, c2, "GB")) {

/*        GB:  general banded matrices */

	la = 43692;
	lafac = 65472;
	ntypes = 8;
	alareq_(path, &nmats, dotype, &ntypes, &c__5, &c__6);

	if (tstchk) {
	    cchkgb_(dotype, &nm, mval, &nn, nval, &nnb2, nbval2, &nns, nsval, 
		    &thresh, &tsterr, &a_ref(1, 1), &la, &a_ref(1, 3), &lafac,
		     &b_ref(1, 1), &b_ref(1, 2), &b_ref(1, 3), work, rwork, 
		    iwork, &c__6);
	} else {
	    s_wsfe(&io___90);
	    do_fio(&c__1, path, (ftnlen)3);
	    e_wsfe();
	}

	if (tstdrv) {
	    cdrvgb_(dotype, &nn, nval, &nrhs, &thresh, &tsterr, &a_ref(1, 1), 
		    &la, &a_ref(1, 3), &lafac, &a_ref(1, 6), &b_ref(1, 1), &
		    b_ref(1, 2), &b_ref(1, 3), &b_ref(1, 4), s, work, rwork, 
		    iwork, &c__6);
	} else {
	    s_wsfe(&io___91);
	    do_fio(&c__1, path, (ftnlen)3);
	    e_wsfe();
	}

    } else if (lsamen_(&c__2, c2, "GT")) {

/*        GT:  general tridiagonal matrices */

	ntypes = 12;
	alareq_(path, &nmats, dotype, &ntypes, &c__5, &c__6);

	if (tstchk) {
	    cchkgt_(dotype, &nn, nval, &nns, nsval, &thresh, &tsterr, &a_ref(
		    1, 1), &a_ref(1, 2), &b_ref(1, 1), &b_ref(1, 2), &b_ref(1,
		     3), work, rwork, iwork, &c__6);
	} else {
	    s_wsfe(&io___92);
	    do_fio(&c__1, path, (ftnlen)3);
	    e_wsfe();
	}

	if (tstdrv) {
	    cdrvgt_(dotype, &nn, nval, &nrhs, &thresh, &tsterr, &a_ref(1, 1), 
		    &a_ref(1, 2), &b_ref(1, 1), &b_ref(1, 2), &b_ref(1, 3), 
		    work, rwork, iwork, &c__6);
	} else {
	    s_wsfe(&io___93);
	    do_fio(&c__1, path, (ftnlen)3);
	    e_wsfe();
	}

    } else if (lsamen_(&c__2, c2, "PO")) {

/*        PO:  positive definite matrices */

	ntypes = 9;
	alareq_(path, &nmats, dotype, &ntypes, &c__5, &c__6);

	if (tstchk) {
	    cchkpo_(dotype, &nn, nval, &nnb2, nbval2, &nns, nsval, &thresh, &
		    tsterr, &lda, &a_ref(1, 1), &a_ref(1, 2), &a_ref(1, 3), &
		    b_ref(1, 1), &b_ref(1, 2), &b_ref(1, 3), work, rwork, &
		    c__6);
	} else {
	    s_wsfe(&io___94);
	    do_fio(&c__1, path, (ftnlen)3);
	    e_wsfe();
	}

	if (tstdrv) {
	    cdrvpo_(dotype, &nn, nval, &nrhs, &thresh, &tsterr, &lda, &a_ref(
		    1, 1), &a_ref(1, 2), &a_ref(1, 3), &b_ref(1, 1), &b_ref(1,
		     2), &b_ref(1, 3), &b_ref(1, 4), s, work, rwork, &c__6);
	} else {
	    s_wsfe(&io___95);
	    do_fio(&c__1, path, (ftnlen)3);
	    e_wsfe();
	}

    } else if (lsamen_(&c__2, c2, "PP")) {

/*        PP:  positive definite packed matrices */

	ntypes = 9;
	alareq_(path, &nmats, dotype, &ntypes, &c__5, &c__6);

	if (tstchk) {
	    cchkpp_(dotype, &nn, nval, &nns, nsval, &thresh, &tsterr, &lda, &
		    a_ref(1, 1), &a_ref(1, 2), &a_ref(1, 3), &b_ref(1, 1), &
		    b_ref(1, 2), &b_ref(1, 3), work, rwork, &c__6);
	} else {
	    s_wsfe(&io___96);
	    do_fio(&c__1, path, (ftnlen)3);
	    e_wsfe();
	}

	if (tstdrv) {
	    cdrvpp_(dotype, &nn, nval, &nrhs, &thresh, &tsterr, &lda, &a_ref(
		    1, 1), &a_ref(1, 2), &a_ref(1, 3), &b_ref(1, 1), &b_ref(1,
		     2), &b_ref(1, 3), &b_ref(1, 4), s, work, rwork, &c__6);
	} else {
	    s_wsfe(&io___97);
	    do_fio(&c__1, path, (ftnlen)3);
	    e_wsfe();
	}

    } else if (lsamen_(&c__2, c2, "PB")) {

/*        PB:  positive definite banded matrices */

	ntypes = 8;
	alareq_(path, &nmats, dotype, &ntypes, &c__5, &c__6);

	if (tstchk) {
	    cchkpb_(dotype, &nn, nval, &nnb2, nbval2, &nns, nsval, &thresh, &
		    tsterr, &lda, &a_ref(1, 1), &a_ref(1, 2), &a_ref(1, 3), &
		    b_ref(1, 1), &b_ref(1, 2), &b_ref(1, 3), work, rwork, &
		    c__6);
	} else {
	    s_wsfe(&io___98);
	    do_fio(&c__1, path, (ftnlen)3);
	    e_wsfe();
	}

	if (tstdrv) {
	    cdrvpb_(dotype, &nn, nval, &nrhs, &thresh, &tsterr, &lda, &a_ref(
		    1, 1), &a_ref(1, 2), &a_ref(1, 3), &b_ref(1, 1), &b_ref(1,
		     2), &b_ref(1, 3), &b_ref(1, 4), s, work, rwork, &c__6);
	} else {
	    s_wsfe(&io___99);
	    do_fio(&c__1, path, (ftnlen)3);
	    e_wsfe();
	}

    } else if (lsamen_(&c__2, c2, "PT")) {

/*        PT:  positive definite tridiagonal matrices */

	ntypes = 12;
	alareq_(path, &nmats, dotype, &ntypes, &c__5, &c__6);

	if (tstchk) {
	    cchkpt_(dotype, &nn, nval, &nns, nsval, &thresh, &tsterr, &a_ref(
		    1, 1), s, &a_ref(1, 2), &b_ref(1, 1), &b_ref(1, 2), &
		    b_ref(1, 3), work, rwork, &c__6);
	} else {
	    s_wsfe(&io___100);
	    do_fio(&c__1, path, (ftnlen)3);
	    e_wsfe();
	}

	if (tstdrv) {
	    cdrvpt_(dotype, &nn, nval, &nrhs, &thresh, &tsterr, &a_ref(1, 1), 
		    s, &a_ref(1, 2), &b_ref(1, 1), &b_ref(1, 2), &b_ref(1, 3),
		     work, rwork, &c__6);
	} else {
	    s_wsfe(&io___101);
	    do_fio(&c__1, path, (ftnlen)3);
	    e_wsfe();
	}

    } else if (lsamen_(&c__2, c2, "HE")) {

/*        HE:  Hermitian indefinite matrices */

	ntypes = 10;
	alareq_(path, &nmats, dotype, &ntypes, &c__5, &c__6);

	if (tstchk) {
	    cchkhe_(dotype, &nn, nval, &nnb2, nbval2, &nns, nsval, &thresh, &
		    tsterr, &lda, &a_ref(1, 1), &a_ref(1, 2), &a_ref(1, 3), &
		    b_ref(1, 1), &b_ref(1, 2), &b_ref(1, 3), work, rwork, 
		    iwork, &c__6);
	} else {
	    s_wsfe(&io___102);
	    do_fio(&c__1, path, (ftnlen)3);
	    e_wsfe();
	}

	if (tstdrv) {
	    cdrvhe_(dotype, &nn, nval, &nrhs, &thresh, &tsterr, &lda, &a_ref(
		    1, 1), &a_ref(1, 2), &a_ref(1, 3), &b_ref(1, 1), &b_ref(1,
		     2), &b_ref(1, 3), work, rwork, iwork, &c__6);
	} else {
	    s_wsfe(&io___103);
	    do_fio(&c__1, path, (ftnlen)3);
	    e_wsfe();
	}

    } else if (lsamen_(&c__2, c2, "HP")) {

/*        HP:  Hermitian indefinite packed matrices */

	ntypes = 10;
	alareq_(path, &nmats, dotype, &ntypes, &c__5, &c__6);

	if (tstchk) {
	    cchkhp_(dotype, &nn, nval, &nns, nsval, &thresh, &tsterr, &lda, &
		    a_ref(1, 1), &a_ref(1, 2), &a_ref(1, 3), &b_ref(1, 1), &
		    b_ref(1, 2), &b_ref(1, 3), work, rwork, iwork, &c__6);
	} else {
	    s_wsfe(&io___104);
	    do_fio(&c__1, path, (ftnlen)3);
	    e_wsfe();
	}

	if (tstdrv) {
	    cdrvhp_(dotype, &nn, nval, &nrhs, &thresh, &tsterr, &lda, &a_ref(
		    1, 1), &a_ref(1, 2), &a_ref(1, 3), &b_ref(1, 1), &b_ref(1,
		     2), &b_ref(1, 3), work, rwork, iwork, &c__6);
	} else {
	    s_wsfe(&io___105);
	    do_fio(&c__1, path, (ftnlen)3);
	    e_wsfe();
	}

    } else if (lsamen_(&c__2, c2, "SY")) {

/*        SY:  symmetric indefinite matrices */

	ntypes = 11;
	alareq_(path, &nmats, dotype, &ntypes, &c__5, &c__6);

	if (tstchk) {
	    cchksy_(dotype, &nn, nval, &nnb2, nbval2, &nns, nsval, &thresh, &
		    tsterr, &lda, &a_ref(1, 1), &a_ref(1, 2), &a_ref(1, 3), &
		    b_ref(1, 1), &b_ref(1, 2), &b_ref(1, 3), work, rwork, 
		    iwork, &c__6);
	} else {
	    s_wsfe(&io___106);
	    do_fio(&c__1, path, (ftnlen)3);
	    e_wsfe();
	}

	if (tstdrv) {
	    cdrvsy_(dotype, &nn, nval, &nrhs, &thresh, &tsterr, &lda, &a_ref(
		    1, 1), &a_ref(1, 2), &a_ref(1, 3), &b_ref(1, 1), &b_ref(1,
		     2), &b_ref(1, 3), work, rwork, iwork, &c__6);
	} else {
	    s_wsfe(&io___107);
	    do_fio(&c__1, path, (ftnlen)3);
	    e_wsfe();
	}

    } else if (lsamen_(&c__2, c2, "SP")) {

/*        SP:  symmetric indefinite packed matrices */

	ntypes = 11;
	alareq_(path, &nmats, dotype, &ntypes, &c__5, &c__6);

	if (tstchk) {
	    cchksp_(dotype, &nn, nval, &nns, nsval, &thresh, &tsterr, &lda, &
		    a_ref(1, 1), &a_ref(1, 2), &a_ref(1, 3), &b_ref(1, 1), &
		    b_ref(1, 2), &b_ref(1, 3), work, rwork, iwork, &c__6);
	} else {
	    s_wsfe(&io___108);
	    do_fio(&c__1, path, (ftnlen)3);
	    e_wsfe();
	}

	if (tstdrv) {
	    cdrvsp_(dotype, &nn, nval, &nrhs, &thresh, &tsterr, &lda, &a_ref(
		    1, 1), &a_ref(1, 2), &a_ref(1, 3), &b_ref(1, 1), &b_ref(1,
		     2), &b_ref(1, 3), work, rwork, iwork, &c__6);
	} else {
	    s_wsfe(&io___109);
	    do_fio(&c__1, path, (ftnlen)3);
	    e_wsfe();
	}

    } else if (lsamen_(&c__2, c2, "TR")) {

/*        TR:  triangular matrices */

	ntypes = 18;
	alareq_(path, &nmats, dotype, &ntypes, &c__5, &c__6);

	if (tstchk) {
	    cchktr_(dotype, &nn, nval, &nnb2, nbval2, &nns, nsval, &thresh, &
		    tsterr, &lda, &a_ref(1, 1), &a_ref(1, 2), &b_ref(1, 1), &
		    b_ref(1, 2), &b_ref(1, 3), work, rwork, &c__6);
	} else {
	    s_wsfe(&io___110);
	    do_fio(&c__1, path, (ftnlen)3);
	    e_wsfe();
	}

    } else if (lsamen_(&c__2, c2, "TP")) {

/*        TP:  triangular packed matrices */

	ntypes = 18;
	alareq_(path, &nmats, dotype, &ntypes, &c__5, &c__6);

	if (tstchk) {
	    cchktp_(dotype, &nn, nval, &nns, nsval, &thresh, &tsterr, &lda, &
		    a_ref(1, 1), &a_ref(1, 2), &b_ref(1, 1), &b_ref(1, 2), &
		    b_ref(1, 3), work, rwork, &c__6);
	} else {
	    s_wsfe(&io___111);
	    do_fio(&c__1, path, (ftnlen)3);
	    e_wsfe();
	}

    } else if (lsamen_(&c__2, c2, "TB")) {

/*        TB:  triangular banded matrices */

	ntypes = 17;
	alareq_(path, &nmats, dotype, &ntypes, &c__5, &c__6);

	if (tstchk) {
	    cchktb_(dotype, &nn, nval, &nns, nsval, &thresh, &tsterr, &lda, &
		    a_ref(1, 1), &a_ref(1, 2), &b_ref(1, 1), &b_ref(1, 2), &
		    b_ref(1, 3), work, rwork, &c__6);
	} else {
	    s_wsfe(&io___112);
	    do_fio(&c__1, path, (ftnlen)3);
	    e_wsfe();
	}

    } else if (lsamen_(&c__2, c2, "QR")) {

/*        QR:  QR factorization */

	ntypes = 8;
	alareq_(path, &nmats, dotype, &ntypes, &c__5, &c__6);

	if (tstchk) {
	    cchkqr_(dotype, &nm, mval, &nn, nval, &nnb, nbval, nxval, &nrhs, &
		    thresh, &tsterr, &c__132, &a_ref(1, 1), &a_ref(1, 2), &
		    a_ref(1, 3), &a_ref(1, 4), &a_ref(1, 5), &b_ref(1, 1), &
		    b_ref(1, 2), &b_ref(1, 3), &b_ref(1, 4), work, rwork, 
		    iwork, &c__6);
	} else {
	    s_wsfe(&io___113);
	    do_fio(&c__1, path, (ftnlen)3);
	    e_wsfe();
	}

    } else if (lsamen_(&c__2, c2, "LQ")) {

/*        LQ:  LQ factorization */

	ntypes = 8;
	alareq_(path, &nmats, dotype, &ntypes, &c__5, &c__6);

	if (tstchk) {
	    cchklq_(dotype, &nm, mval, &nn, nval, &nnb, nbval, nxval, &nrhs, &
		    thresh, &tsterr, &c__132, &a_ref(1, 1), &a_ref(1, 2), &
		    a_ref(1, 3), &a_ref(1, 4), &a_ref(1, 5), &b_ref(1, 1), &
		    b_ref(1, 2), &b_ref(1, 3), &b_ref(1, 4), work, rwork, 
		    iwork, &c__6);
	} else {
	    s_wsfe(&io___114);
	    do_fio(&c__1, path, (ftnlen)3);
	    e_wsfe();
	}

    } else if (lsamen_(&c__2, c2, "QL")) {

/*        QL:  QL factorization */

	ntypes = 8;
	alareq_(path, &nmats, dotype, &ntypes, &c__5, &c__6);

	if (tstchk) {
	    cchkql_(dotype, &nm, mval, &nn, nval, &nnb, nbval, nxval, &nrhs, &
		    thresh, &tsterr, &c__132, &a_ref(1, 1), &a_ref(1, 2), &
		    a_ref(1, 3), &a_ref(1, 4), &a_ref(1, 5), &b_ref(1, 1), &
		    b_ref(1, 2), &b_ref(1, 3), &b_ref(1, 4), work, rwork, 
		    iwork, &c__6);
	} else {
	    s_wsfe(&io___115);
	    do_fio(&c__1, path, (ftnlen)3);
	    e_wsfe();
	}

    } else if (lsamen_(&c__2, c2, "RQ")) {

/*        RQ:  RQ factorization */

	ntypes = 8;
	alareq_(path, &nmats, dotype, &ntypes, &c__5, &c__6);

	if (tstchk) {
	    cchkrq_(dotype, &nm, mval, &nn, nval, &nnb, nbval, nxval, &nrhs, &
		    thresh, &tsterr, &c__132, &a_ref(1, 1), &a_ref(1, 2), &
		    a_ref(1, 3), &a_ref(1, 4), &a_ref(1, 5), &b_ref(1, 1), &
		    b_ref(1, 2), &b_ref(1, 3), &b_ref(1, 4), work, rwork, 
		    iwork, &c__6);
	} else {
	    s_wsfe(&io___116);
	    do_fio(&c__1, path, (ftnlen)3);
	    e_wsfe();
	}

    } else if (lsamen_(&c__2, c2, "EQ")) {

/*        EQ:  Equilibration routines for general and positive definite   
               matrices (THREQ should be between 2 and 10) */

	if (tstchk) {
	    cchkeq_(&threq, &c__6);
	} else {
	    s_wsfe(&io___117);
	    do_fio(&c__1, path, (ftnlen)3);
	    e_wsfe();
	}

    } else if (lsamen_(&c__2, c2, "TZ")) {

/*        TZ:  Trapezoidal matrix */

	ntypes = 3;
	alareq_(path, &nmats, dotype, &ntypes, &c__5, &c__6);

	if (tstchk) {
	    cchktz_(dotype, &nm, mval, &nn, nval, &thresh, &tsterr, &a_ref(1, 
		    1), &a_ref(1, 2), s, &s[132], &b_ref(1, 1), work, rwork, &
		    c__6);
	} else {
	    s_wsfe(&io___118);
	    do_fio(&c__1, path, (ftnlen)3);
	    e_wsfe();
	}

    } else if (lsamen_(&c__2, c2, "QP")) {

/*        QP:  QR factorization with pivoting */

	ntypes = 6;
	alareq_(path, &nmats, dotype, &ntypes, &c__5, &c__6);

	if (tstchk) {
	    cchkqp_(dotype, &nm, mval, &nn, nval, &thresh, &tsterr, &a_ref(1, 
		    1), &a_ref(1, 2), s, &s[132], &b_ref(1, 1), work, rwork, 
		    iwork, &c__6);
	    cchkq3_(dotype, &nm, mval, &nn, nval, &nnb, nbval, nxval, &thresh,
		     &a_ref(1, 1), &a_ref(1, 2), s, &s[132], &b_ref(1, 1), 
		    work, rwork, iwork, &c__6);
	} else {
	    s_wsfe(&io___119);
	    do_fio(&c__1, path, (ftnlen)3);
	    e_wsfe();
	}

    } else if (lsamen_(&c__2, c2, "LS")) {

/*        LS:  Least squares drivers */

	ntypes = 6;
	alareq_(path, &nmats, dotype, &ntypes, &c__5, &c__6);

	if (tstdrv) {
	    cdrvls_(dotype, &nm, mval, &nn, nval, &nns, nsval, &nnb, nbval, 
		    nxval, &thresh, &tsterr, &a_ref(1, 1), &a_ref(1, 2), &
		    a_ref(1, 3), &a_ref(1, 4), &a_ref(1, 5), s, &s[132], work,
		     rwork, iwork, &c__6);
	} else {
	    s_wsfe(&io___120);
	    do_fio(&c__1, path, (ftnlen)3);
	    e_wsfe();
	}

    } else {

	s_wsfe(&io___121);
	do_fio(&c__1, path, (ftnlen)3);
	e_wsfe();
    }

/*     Go back to get another input line. */

    goto L80;

/*     Branch to this line when the last record is read. */

L140:
    cl__1.cerr = 0;
    cl__1.cunit = 5;
    cl__1.csta = 0;
    f_clos(&cl__1);
    s2 = second_();
    s_wsfe(&io___123);
    e_wsfe();
    s_wsfe(&io___124);
    r__1 = s2 - s1;
    do_fio(&c__1, (char *)&r__1, (ftnlen)sizeof(real));
    e_wsfe();


/*     End of CCHKAA */

    return 0;
} /* MAIN__ */

#undef b_ref
#undef b_subscr
#undef a_ref
#undef a_subscr


/* Main program alias */ int cchkaa_ () { MAIN__ (); return 0; }
