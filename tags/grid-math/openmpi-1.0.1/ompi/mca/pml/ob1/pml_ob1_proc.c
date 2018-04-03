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

#include "include/sys/atomic.h"
#include "pml_ob1.h"
#include "pml_ob1_proc.h"


static void mca_pml_ob1_proc_construct(mca_pml_ob1_proc_t* proc)
{
    proc->base.proc_ompi = NULL;
    OBJ_CONSTRUCT(&proc->base.proc_lock, opal_mutex_t);
}


static void mca_pml_ob1_proc_destruct(mca_pml_ob1_proc_t* proc)
{
    OBJ_DESTRUCT(&proc->base.proc_lock);
}


OBJ_CLASS_INSTANCE(
    mca_pml_ob1_proc_t,
    opal_list_item_t,
    mca_pml_ob1_proc_construct,
    mca_pml_ob1_proc_destruct
);
