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
dnl OMPI_SETUP_F90
dnl
dnl sets:
dnl  F90                   : full pathname to compiler
dnl  BASEF90               : compiler name (no path)
dnl  OMPI_WANT_F90_BINDINGS : (actually set by ompi_configure_options, may be
dnl                          redefined here)
dnl  FC                    : Same as F90.  Side effect of AC_PROG_FC.  Should
dnl                          not be used
dnl defines:
dnl  OMPI_F90               : same as F90
dnl  OMPI_WANT_F90_BINDINGS :
dnl am_conditional:
dnl  OMPI_WANT_F90_BINDINGS :

AC_DEFUN([OMPI_SETUP_F90],[

# Modularize this setup so that sub-configure.in scripts can use this
# same setup code.

ompi_show_subtitle "Fortran 90/95 compiler" 

if test "$OMPI_WANT_F77_BINDINGS" = "0" ; then
    AC_MSG_WARN([*** Fortran 90/95 bindings implicitly disabled (because])
    AC_MSG_WARN([*** Fortran 77 bindings were disabled)])

    OMPI_WANT_F90_BINDINGS=0
    OMPI_F90="none"
    BASEF90="none"
    OMPI_F90_ABSOLUTE="none"
    if test "$enable_f90" = "yes"; then
        AC_MSG_WARN([*** but --enable-f90 was explicitly specified])
        AC_MSG_ERROR([Cannot continue])
    fi
elif test "$OMPI_WANT_F90_BINDINGS" = "0" ; then
    AC_MSG_WARN([*** Fortran 90/95 bindings disabled by user])
    OMPI_WANT_F90_BINDINGS=0
    OMPI_F90="none"
    BASEF90="none"
    OMPI_F90_ABSOLUTE="none"
else

    #
    # Check for the compiler
    #
    # Note that we don't actually *use* the fortran compiler to build
    # anything in OMPI; it's only used here in configure to find out
    # symbol conventions, type sizes, etc.  We also pass it down to
    # the wrapper compiler mpif90.
    #
    # Note that AC_PROG_FC will look for *any* fortran compiler, and
    # we don't want it to find an F77 compiler.  The AC docs don't
    # recommend using the "dialect" feature of AC_PROG_FC (and you can
    # only look for one of Fortran 90 or 95 -- not both), so instead
    # use the optional first parameter and steal the list of Fortran
    # compilers (excluding the f77 compiler names) from AC's default
    # list of 95 and 90 compilers and use it here.
    #

    ompi_fcflags_save="$FCFLAGS"
    AC_PROG_FC([gfortran f95 fort xlf95 ifort ifc efc pgf95 lf95 f90 xlf90 pgf90 epcf90])
    FCFLAGS="$ompi_fcflags_save"
    if test -z "$FC"; then
        AC_MSG_WARN([*** Fortran 90/95 bindings disabled (could not find compiler)])
        OMPI_WANT_F90_BINDINGS=0
        OMPI_F90="none"
        BASEF90="none"
        OMPI_F90_ABSOLUTE="none"
    else
        OMPI_F90="$FC"
        BASEF90="`basename $FC`"
        OMPI_F90_ABSOLUTE="`which $FC`"

        AC_LANG_PUSH(Fortran)
        AC_FC_SRCEXT(f)
        AC_FC_SRCEXT(f90)
        AC_FC_SRCEXT(f95)
        AC_LANG_POP(Fortran)

        AC_MSG_CHECKING([whether $OMPI_F77 and $OMPI_F90 compilers are compatible])
        OMPI_INTL_F90_F77_INTERACTION(fortran_goodness=1, fortran_goodness=0)
        if test "$fortran_goodness" = "0" ; then
            AC_MSG_RESULT([no])
            AC_MSG_WARN([*** Fortran 77 and Fortran 90 compilers are not link compatible])
            AC_MSG_WARN([*** Disabling Fortran 90/95 bindings])
            OMPI_WANT_F90_BINDINGS=0
        else
            AC_MSG_RESULT([yes])
            OMPI_WANT_F90_BINDINGS=1
        fi
    fi
fi

AC_DEFINE_UNQUOTED(OMPI_WANT_F90_BINDINGS, $OMPI_WANT_F90_BINDINGS,
    [Whether we want the MPI f90 bindings or not])
AC_DEFINE_UNQUOTED(OMPI_F90, "$OMPI_F90", [OMPI underlying F90 compiler])
AM_CONDITIONAL(OMPI_WANT_F90_BINDINGS, test "$OMPI_WANT_F90_BINDINGS" = "1")
AC_SUBST(OMPI_F90_ABSOLUTE)
unset fortran_goodness
])


AC_DEFUN([OMPI_INTL_F90_F77_INTERACTION], [
# make sure that we can combine F90 and F77 code
AC_LANG_PUSH(Fortran)
# Fortran module
cat > conftestf77.f <<EOF
        subroutine Conf1_test()
        end
EOF
cat > conftestf90.f <<EOF
        program main
        call Conf1_test()
        end
EOF

# Try the compile
OMPI_LOG_COMMAND(
    [$OMPI_F90 $FCFLAGS $FCFLAGS_f -c conftestf90.f],
    OMPI_LOG_COMMAND(
        [$OMPI_F77 $FFLAGS -c conftestf77.f],
        OMPI_LOG_COMMAND(
            [$OMPI_F90 $FCFLAGS -o conftest conftestf90.o conftestf77.o $LIBS],
            [HAPPY=1],
            [HAPPY=0]),
	[HAPPY=0]),
    [HAPPY=0])

if test "$HAPPY" = "1"; then
   $1
else
    OMPI_LOG_MSG([here is the F77 program:], 1)
    OMPI_LOG_FILE([conftestf77.f])
    OMPI_LOG_MSG([here is the F90 program:], 1)
    OMPI_LOG_FILE([conftestf90.f])
    $2
fi

unset HAPPY ompi_conftest_h
/bin/rm -f conftest*

AC_LANG_POP(Fortran)
])dnl