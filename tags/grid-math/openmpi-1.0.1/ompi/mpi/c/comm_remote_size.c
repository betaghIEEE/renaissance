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
#include <stdio.h>

#include "mpi/c/bindings.h"

#if OMPI_HAVE_WEAK_SYMBOLS && OMPI_PROFILING_DEFINES
#pragma weak MPI_Comm_remote_size = PMPI_Comm_remote_size
#endif

#if OMPI_PROFILING_DEFINES
#include "mpi/c/profile/defines.h"
#endif


static const char FUNC_NAME[] = "MPI_Comm_remote_size";


int MPI_Comm_remote_size(MPI_Comm comm, int *size) {

    if ( MPI_PARAM_CHECK ) {
        OMPI_ERR_INIT_FINALIZE(FUNC_NAME);

        if (MPI_COMM_NULL == comm || ompi_comm_invalid (comm)) {
            return OMPI_ERRHANDLER_INVOKE(MPI_COMM_WORLD, MPI_ERR_COMM, 
                                          FUNC_NAME);
        }

        if ( NULL == size ) {
            return OMPI_ERRHANDLER_INVOKE(comm, MPI_ERR_ARG, FUNC_NAME);
        }
    }

    *size = ompi_comm_remote_size ((ompi_communicator_t*)comm);
    return MPI_SUCCESS;
}