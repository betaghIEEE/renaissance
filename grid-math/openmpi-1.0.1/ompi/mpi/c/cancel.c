/*
 * Copyright (c) 2004-2005 The Trustees of Indiana University and Indiana
 *                         University Research and Technology
 *                         Corporation.  All rights reserved.
 * Copyright (c) 2004-2005 The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
 * Copyright (c) 2004-2005 High Performance Computing Center Stuttgart, 
 *                         University of Stuttgart.  All rights reserved.
 *
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
#include "ompi/mca/pml/pml.h"
#include "ompi/communicator/communicator.h"
#include "ompi/errhandler/errhandler.h"
#include "ompi/request/request.h"

#if OMPI_HAVE_WEAK_SYMBOLS && OMPI_PROFILING_DEFINES
#pragma weak MPI_Cancel = PMPI_Cancel
#endif

#if OMPI_PROFILING_DEFINES
#include "mpi/c/profile/defines.h"
#endif

static const char FUNC_NAME[] = "MPI_Cancel";


int MPI_Cancel(MPI_Request *request) 
{
    int rc;
    if ( MPI_PARAM_CHECK ) {
        rc = MPI_SUCCESS;
        OMPI_ERR_INIT_FINALIZE(FUNC_NAME);
        if (request == NULL) {
            rc = MPI_ERR_REQUEST;
        }
        /* JMS: Tim will fix to invoke on the communicator/window/file
           on the request (i.e., not COMM_WORLD), if the request is
           available/valid */
        OMPI_ERRHANDLER_CHECK(rc, MPI_COMM_WORLD, rc, "MPI_Cancel");
    }

    if (NULL == *request) {
        return MPI_SUCCESS;
    }
    rc = ompi_request_cancel(*request);
    OMPI_ERRHANDLER_RETURN(rc, MPI_COMM_WORLD, rc, "MPI_Cancel");
}
