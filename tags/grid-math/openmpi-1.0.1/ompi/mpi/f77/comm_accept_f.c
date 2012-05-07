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

#include "mpi/f77/bindings.h"

#if OMPI_HAVE_WEAK_SYMBOLS && OMPI_PROFILE_LAYER
#pragma weak PMPI_COMM_ACCEPT = mpi_comm_accept_f
#pragma weak pmpi_comm_accept = mpi_comm_accept_f
#pragma weak pmpi_comm_accept_ = mpi_comm_accept_f
#pragma weak pmpi_comm_accept__ = mpi_comm_accept_f
#elif OMPI_PROFILE_LAYER
OMPI_GENERATE_F77_BINDINGS (PMPI_COMM_ACCEPT,
                           pmpi_comm_accept,
                           pmpi_comm_accept_,
                           pmpi_comm_accept__,
                           pmpi_comm_accept_f,
                           (char *port_name, MPI_Fint *info, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *newcomm, MPI_Fint *ierr),
                           (port_name, info, root, comm, newcomm, ierr) )
#endif

#if OMPI_HAVE_WEAK_SYMBOLS
#pragma weak MPI_COMM_ACCEPT = mpi_comm_accept_f
#pragma weak mpi_comm_accept = mpi_comm_accept_f
#pragma weak mpi_comm_accept_ = mpi_comm_accept_f
#pragma weak mpi_comm_accept__ = mpi_comm_accept_f
#endif

#if ! OMPI_HAVE_WEAK_SYMBOLS && ! OMPI_PROFILE_LAYER
OMPI_GENERATE_F77_BINDINGS (MPI_COMM_ACCEPT,
                           mpi_comm_accept,
                           mpi_comm_accept_,
                           mpi_comm_accept__,
                           mpi_comm_accept_f,
                           (char *port_name, MPI_Fint *info, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *newcomm, MPI_Fint *ierr),
                           (port_name, info, root, comm, newcomm, ierr) )
#endif


#if OMPI_PROFILE_LAYER && ! OMPI_HAVE_WEAK_SYMBOLS
#include "mpi/f77/profile/defines.h"
#endif

void mpi_comm_accept_f(char *port_name, MPI_Fint *info, MPI_Fint *root, 
		       MPI_Fint *comm, MPI_Fint *newcomm, MPI_Fint *ierr)
{
    MPI_Comm c_comm, c_new_comm;
    MPI_Info c_info;

    c_comm = MPI_Comm_f2c(*comm);
    c_info = MPI_Info_f2c(*info);

    *ierr = OMPI_INT_2_FINT(MPI_Comm_accept(port_name, c_info, 
					    OMPI_FINT_2_INT(*root), 
					    c_comm, &c_new_comm));
    if (MPI_SUCCESS == OMPI_FINT_2_INT(*ierr)) {
        *newcomm = MPI_Comm_c2f(c_new_comm);
    }
}