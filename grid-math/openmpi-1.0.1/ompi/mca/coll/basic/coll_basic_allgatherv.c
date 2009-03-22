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
#include "coll_basic.h"

#include "mpi.h"
#include "ompi/datatype/datatype.h"
#include "ompi/include/constants.h"
#include "ompi/mca/coll/coll.h"
#include "ompi/mca/coll/base/coll_tags.h"
#include "coll_basic.h"


/*
 *	allgatherv_intra
 *
 *	Function:	- allgatherv using other MPI collectives
 *	Accepts:	- same as MPI_Allgatherv()
 *	Returns:	- MPI_SUCCESS or error code
 */
int
mca_coll_basic_allgatherv_intra(void *sbuf, int scount,
                                struct ompi_datatype_t *sdtype,
                                void *rbuf, int *rcounts, int *disps,
                                struct ompi_datatype_t *rdtype,
                                struct ompi_communicator_t *comm)
{
    int i, size, rank;
    int err;
    MPI_Aint extent;
    MPI_Aint lb;
    char *send_buf = NULL;

    /* Collect all values at each process, one at a time. */

    size = ompi_comm_size(comm);
    rank = ompi_comm_rank(comm);

    if (MPI_IN_PLACE == sbuf) {
        ompi_ddt_get_extent(rdtype, &lb, &extent);
        send_buf = rbuf;
        for (i = 0; i < rank; ++i) {
            send_buf += (rcounts[i] * extent);
        }
    }
    for (i = 0; i < size; ++i) {
        if (MPI_IN_PLACE == sbuf) {
            /* MPI-2 7.3.3 description of MPI_Allgatherv is wrong --
               can't just have all processes call
               MPI_Gatherv(MPI_IN_PLACE...) because IN_PLACE is only
               allowed to be used at the root.  Non-root processes
               must use their receive buf as the send buf. */
            if (i == rank) {
                err = comm->c_coll.coll_gatherv(MPI_IN_PLACE, -1, 
                                                MPI_DATATYPE_NULL, rbuf,
                                                rcounts, disps, rdtype, i,
                                                comm);
            } else {
                err = comm->c_coll.coll_gatherv(send_buf, 
                                                rcounts[rank], rdtype, 
                                                NULL, NULL, NULL, 
                                                MPI_DATATYPE_NULL,
                                                i, comm);
            }
        } else {
            err = comm->c_coll.coll_gatherv(sbuf, scount, sdtype, rbuf,
                                            rcounts, disps, rdtype, i, comm);
        }
        if (MPI_SUCCESS != err) {
            return err;
        }
    }

    return MPI_SUCCESS;
}


/*
 *	allgatherv_inter
 *
 *	Function:	- allgatherv using other MPI collectives
 *	Accepts:	- same as MPI_Allgatherv()
 *	Returns:	- MPI_SUCCESS or error code
 */
int
mca_coll_basic_allgatherv_inter(void *sbuf, int scount,
                                struct ompi_datatype_t *sdtype,
                                void *rbuf, int *rcounts, int *disps,
                                struct ompi_datatype_t *rdtype,
                                struct ompi_communicator_t *comm)
{
    int size, rsize;
    int err, i;
    int *scounts = NULL;
    int *sdisps = NULL;

    rsize = ompi_comm_remote_size(comm);
    size = ompi_comm_size(comm);

    scounts = (int *) malloc(rsize * sizeof(int));
    sdisps = (int *) calloc(rsize, sizeof(int));
    if (NULL == scounts || NULL == sdisps) {
        return OMPI_ERR_OUT_OF_RESOURCE;
    }

    for (i = 0; i < rsize; i++) {
        scounts[i] = scount;
    }

    err = comm->c_coll.coll_alltoallv(sbuf, scounts, sdisps, sdtype,
                                      rbuf, rcounts, disps, rdtype, comm);

    if (NULL != sdisps) {
        free(sdisps);
    }
    if (NULL != scounts) {
        free(scounts);
    }

    return err;
}