//
// Copyright (c) 2004-2005 The Trustees of Indiana University and Indiana
//                         University Research and Technology
//                         Corporation.  All rights reserved.
// Copyright (c) 2004-2005 The University of Tennessee and The University
//                         of Tennessee Research Foundation.  All rights
//                         reserved.
// Copyright (c) 2004-2005 High Performance Computing Center Stuttgart, 
//                         University of Stuttgart.  All rights reserved.
// Copyright (c) 2004-2005 The Regents of the University of California.
//                         All rights reserved.
// $COPYRIGHT$
// 
// Additional copyrights may follow
// 
// $HEADER$
//

#include "ompi_config.h"
#include "tools/wrappers/ompi_wrap.h"


int
main(int argc, char *argv[])
{
    // The four wrapper compilers are extremely similar.  So similar,
    // in fact, that they can be parameterized on what is different.
    // Hence, we call the "wrapper compiler engine" to do all the
    // work, and pass in just a few arguments to customize for the
    // language of this wrapper compiler.

    ompi_sv_t compiler;
    ompi_sv_t cxxcppflags;
    ompi_sv_t cxxflags;
    ompi_sv_t ldflags;
    ompi_sv_t libs;

    compiler.clear();
    compiler.push_back("OMPI_MPICXX");

    cxxcppflags.clear();
    cxxcppflags.push_back("OMPI_MPICXX_CXXCPPFLAGS");
    cxxcppflags.push_back("OMPI_CXXCPPFLAGS");

    cxxflags.clear();
    cxxflags.push_back("OMPI_MPICXX_CXXFLAGS");
    cxxflags.push_back("OMPI_CXXFLAGS");

    ldflags.clear();
    ldflags.push_back("OMPI_MPICXX_LDFLAGS");
    ldflags.push_back("OMPI_LDFLAGS");

    libs.clear();
    libs.push_back("OMPI_MPICXX_LIBS");
    libs.push_back("OMPI_LIBS");

    return ompi_wrap_engine(argc, argv, compiler, cxxcppflags, cxxflags,
			    ldflags, libs, OMPI_CXX,
			    WRAPPER_EXTRA_CXXFLAGS, true, false, false);
}