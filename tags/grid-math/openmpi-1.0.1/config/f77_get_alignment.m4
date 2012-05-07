dnl -*- shell-script -*-
dnl
dnl Copyright (c) 2004-2005 The Trustees of Indiana University and Indiana
dnl                         University Research and Technology
dnl                         Corporation.  All rights reserved.
dnl Copyright (c) 2004-2005 The University of Tennessee and The University
dnl                         of Tennessee Research Foundation.  All rights
dnl                         reserved.
dnl Copyright (c) 2004-2005 High Performance Computing Center Stuttgart, 
dnl                         University of Stuttgart.  All rights reserved.
dnl Copyright (c) 2004-2005 The Regents of the University of California.
dnl                         All rights reserved.
dnl $COPYRIGHT$
dnl 
dnl Additional copyrights may follow
dnl 
dnl $HEADER$
dnl

AC_DEFUN([OMPI_F77_GET_ALIGNMENT],[
# Determine FORTRAN datatype size.
# First arg is type, 2nd arg is config var to define.
AC_MSG_CHECKING(alignment of FORTRAN $1)
ompi_ac_align_fn=
if test "x$ompi_ac_doubleunder" = xy || test "x$ompi_ac_singleunder" = xy; then
    ompi_ac_align_fn=align_
else
    if test "x$ompi_ac_nounder" = xy; then
	ompi_ac_align_fn=align
    else
	if test "x$ompi_ac_caps" = xy; then
	    ompi_ac_align_fn=ALIGN
	else
	    AC_MSG_WARN([*** FORTRAN external naming convention undefined])
	    AC_MSG_ERROR([*** Cannot continue.])
	fi
    fi
fi

#
# Fortran module
#

cat > conftestf.f <<EOF
      program falign
      external ALIGN
      $1  w,x,y,z
      CHARACTER a,b,c
      common /foo/a,w,b,x,y,c,z
      call ALIGN(w,x,y,z)
      end
EOF

#
# C module
#

if test -f conftest.h; then
    ompi_conftest_h="#include \"conftest.h\""
else
    ompi_conftest_h=""
fi
cat > conftest.c <<EOF
#include <stdio.h>
#include <stdlib.h>
$conftest

#ifdef __cplusplus
extern "C" {
#endif
void $ompi_ac_align_fn(char *w, char *x, char *y, char *z)
{   unsigned long aw, ax, ay, az;
    FILE *f=fopen("conftestval", "w");
    if (!f) exit(1);
    aw = (unsigned long) w;
    ax = (unsigned long) x;
    ay = (unsigned long) y;
    az = (unsigned long) z;
    if (! ((aw%16)||(ax%16)||(ay%16)||(az%16))) fprintf(f, "%d\n", 16);
    else if (! ((aw%12)||(ax%12)||(ay%12)||(az%12))) fprintf(f, "%d\n", 12);
    else if (! ((aw%8)||(ax%8)||(ay%8)||(az%8))) fprintf(f, "%d\n", 8);
    else if (! ((aw%4)||(ax%4)||(ay%4)||(az%4))) fprintf(f, "%d\n", 4);
    else if (! ((aw%2)||(ax%2)||(ay%2)||(az%2))) fprintf(f, "%d\n", 2);
    else fprintf(f, "%d\n", 1); 
    fclose(f);
}
#ifdef __cplusplus
}
#endif
EOF

#
# Try the compilation and run.  Can't use AC_TRY_RUN because it's two
# module files.
#

OMPI_LOG_COMMAND([$CC $CFLAGS -I. -c conftest.c],
    OMPI_LOG_COMMAND([$F77 $FFLAGS conftestf.f conftest.o -o conftest $LDFLAGS $LIBS],
	OMPI_LOG_COMMAND([./conftest],[HAPPY=1],[HAPPY=0]),
	[HAPPY=0]),
    [HAPPY=0])

if test "$HAPPY" = "1" -a -f conftestval; then
    ompi_ac_align=`cat conftestval`
    AC_MSG_RESULT([$ompi_ac_align])
    if test -n "$2"; then
	eval "$2=$ompi_ac_align"
    fi
else
    AC_MSG_RESULT([unknown])

    OMPI_LOG_MSG([here is the C program:], 1)
    OMPI_LOG_FILE([conftest.c])
    if test -f conftest.h; then
	OMPI_LOG_MSG([here is contest.h:], 1)
	OMPI_LOG_FILE([conftest.h])
    fi
    OMPI_LOG_MSG([here is the fortran program:], 1)
    OMPI_LOG_FILE([conftestf.f])

    AC_MSG_WARN([*** Problem running configure test!])
    AC_MSG_WARN([*** See config.log for details.])
    AC_MSG_ERROR([*** Cannot continue.])
fi
str="$2=$ompi_ac_align"
eval $str

unset ompi_ac_align HAPPY ompi_conftest_h
/bin/rm -f conftest*])dnl