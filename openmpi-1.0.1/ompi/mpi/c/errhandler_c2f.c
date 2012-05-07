/*
 * Copyright (c) 2004-2005 The Trustees of Indiana University and Indiana
 *                         University Research and Technology
 *                         Corporation.  All rights reserved.
 * Copyright (c) 2004-2005 The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
 * Copyright (c) 2004-2005 High Performance Computing Center Stuttgart, 
 *                         University of Stuttgart.  All rights reserved.
 * Copyright (c) 2004-2005 The Regents of the University of California.
 *                         All rights reserved.
 * $COPYRIGHT$
 * 
 * Additional copyrights may follow
 * 
 * $HEADER$
 */

#include "ompi_config.h"

#include "mpi/c/bindings.h"
#include "mpi/f77/fint_2_int.h"

#if OMPI_HAVE_WEAK_SYMBOLS && OMPI_PROFILING_DEFINES
#pragma weak MPI_Errhandler_c2f = PMPI_Errhandler_c2f
#endif

#if OMPI_PROFILING_DEFINES
#include "mpi/c/profile/defines.h"
#endif

static const char FUNC_NAME[] = "MPI_Errhandler_c2f";


MPI_Fint MPI_Errhandler_c2f(MPI_Errhandler errhandler)
{
  /* Error checking */

  if (MPI_PARAM_CHECK) {
    OMPI_ERR_INIT_FINALIZE(FUNC_NAME);

	/* mapping an invalid handle to a null handle */
	/* also checks errhandler type matches */
	/* not invoking an error handler */
    if (NULL == errhandler ||
        OMPI_ERRHANDLER_TYPE_COMM != errhandler->eh_mpi_object_type) {
        errhandler = MPI_ERRHANDLER_NULL;
    }
  }


  return OMPI_INT_2_FINT(errhandler->eh_f_to_c_index);
}
