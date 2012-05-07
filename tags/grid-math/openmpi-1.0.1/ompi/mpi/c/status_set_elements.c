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
#include "ompi/datatype/datatype.h"

#if OMPI_HAVE_WEAK_SYMBOLS && OMPI_PROFILING_DEFINES
#pragma weak MPI_Status_set_elements = PMPI_Status_set_elements
#endif

#if OMPI_PROFILING_DEFINES
#include "mpi/c/profile/defines.h"
#endif

static const char FUNC_NAME[] = "MPI_Status_set_elements";


int MPI_Status_set_elements(MPI_Status *status, MPI_Datatype datatype,
                            int count) 
{
    int rc = MPI_SUCCESS;
    int32_t size;

    if (MPI_PARAM_CHECK) {
        OMPI_ERR_INIT_FINALIZE(FUNC_NAME);
        if (NULL == datatype || MPI_DATATYPE_NULL == datatype) {
            rc = MPI_ERR_TYPE;
        } else if (count < 0) {
            rc = MPI_ERR_COUNT;
        }
        OMPI_ERRHANDLER_CHECK(rc, MPI_COMM_WORLD, rc, FUNC_NAME);
    }

    if (status != MPI_STATUS_IGNORE) {
        ompi_ddt_type_size( datatype, &size );
        status->_count = count * size;
    }
    return OMPI_SUCCESS;
}