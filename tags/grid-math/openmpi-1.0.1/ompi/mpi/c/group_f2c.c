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
#include "ompi/group/group.h"

#if OMPI_HAVE_WEAK_SYMBOLS && OMPI_PROFILING_DEFINES
#pragma weak MPI_Group_f2c = PMPI_Group_f2c
#endif

#if OMPI_PROFILING_DEFINES
#include "mpi/c/profile/defines.h"
#endif

static const char FUNC_NAME[] = "MPI_Group_f2c";


MPI_Group MPI_Group_f2c(MPI_Fint group_f)
{
    int group_index = OMPI_FINT_2_INT(group_f);

    if (MPI_PARAM_CHECK) {
        OMPI_ERR_INIT_FINALIZE(FUNC_NAME);
    }

    /* Per MPI-2:4.12.4, do not invoke an error handler if we get an
       invalid fortran handle */

    if (group_index < 0 ||
        group_index >=
        ompi_pointer_array_get_size(ompi_group_f_to_c_table)) {
        return MPI_GROUP_NULL;
    }

    return ompi_pointer_array_get_item(ompi_group_f_to_c_table, group_index);
}