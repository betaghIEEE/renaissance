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
#include "ptl_mx.h"
#include "ptl_mx_recvfrag.h"


static void mca_ptl_mx_recv_frag_construct(mca_ptl_mx_recv_frag_t* frag);
static void mca_ptl_mx_recv_frag_destruct(mca_ptl_mx_recv_frag_t* frag);


OBJ_CLASS_INSTANCE(
    mca_ptl_mx_recv_frag_t,
    mca_ptl_base_recv_frag_t,
    mca_ptl_mx_recv_frag_construct,
    mca_ptl_mx_recv_frag_destruct);
                                                                                                           
/*
 * MX recv fragment constructor
 */

static void mca_ptl_mx_recv_frag_construct(mca_ptl_mx_recv_frag_t* frag)
{
    /* one time initialization */
    frag->frag_segments[0].segment_ptr = &frag->frag_recv.frag_base.frag_header;
    frag->frag_segments[0].segment_length = sizeof(frag->frag_recv.frag_base.frag_header);
}


/*
 * MX recv fragment destructor
 */

static void mca_ptl_mx_recv_frag_destruct(mca_ptl_mx_recv_frag_t* frag)
{
}
