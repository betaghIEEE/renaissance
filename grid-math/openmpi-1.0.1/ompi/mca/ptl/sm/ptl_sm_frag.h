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
/**
 * @file
 */

#ifndef MCA_PTL_SM_RECV_FRAG_H
#define MCA_PTL_SM_RECV_FRAG_H

#include <string.h>
#include <sys/types.h>
#include "include/sys/atomic.h"
#include "mca/ptl/ptl.h"
#include "mca/ptl/base/ptl_base_recvfrag.h"
#include "ptl_sm.h"


#if defined(c_plusplus) || defined(__cplusplus)
extern "C" {
#endif
OBJ_CLASS_DECLARATION(mca_ptl_sm_frag_t);

OBJ_CLASS_DECLARATION(mca_ptl_sm_second_frag_t);


/**
 *  shared memory received fragment derived type - because of
 *  the way lists are initialized in Open MPI, this is good
 *  only for the first fragment.
 */
struct mca_ptl_sm_frag_t {
    mca_ptl_base_recv_frag_t super; /**< base receive fragment descriptor */
    size_t buff_length;   /**< size of buffer */
    int queue_index;      /**< local process index, cached for fast
                            acking */
    struct mca_ptl_base_module_t* send_ptl;   /**< PTL that is selected for first fragment */
    struct mca_ptl_base_send_request_t* send_req;
    size_t send_offset;
    void *buff;           /**< pointer to buffer */
};
typedef struct mca_ptl_sm_frag_t mca_ptl_sm_frag_t;

/**
 *  shared memory received fragment derived type - because of
 *  the way lists are initialized in Open MPI, this is good
 *  only for the second and beyond fragments.
 */
typedef struct mca_ptl_sm_frag_t mca_ptl_sm_second_frag_t;
#if defined(c_plusplus) || defined(__cplusplus)
}
#endif

#endif
