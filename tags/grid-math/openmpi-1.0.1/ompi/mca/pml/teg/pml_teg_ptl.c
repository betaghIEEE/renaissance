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

#include "pml_teg_ptl.h"


static void mca_pml_base_ptl_construct(mca_pml_base_ptl_t* ptl)
{
    OBJ_CONSTRUCT(&ptl->ptl_cache, opal_list_t);
    OBJ_CONSTRUCT(&ptl->ptl_cache_lock, opal_mutex_t);
    ptl->ptl = NULL;
    ptl->ptl_cache_size = 0;
    ptl->ptl_cache_alloc = 0;
}

static void mca_pml_base_ptl_destruct(mca_pml_base_ptl_t* ptl)
{
    OBJ_DESTRUCT(&ptl->ptl_cache);
    OBJ_DESTRUCT(&ptl->ptl_cache_lock);
}

OBJ_CLASS_INSTANCE(
    mca_pml_base_ptl_t,
    opal_list_t,
    mca_pml_base_ptl_construct,
    mca_pml_base_ptl_destruct
);
