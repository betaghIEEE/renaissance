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
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>  /* for mkfifo */

#include "ompi/include/constants.h"
#include "include/sys/cache.h"
#include "opal/event/event.h"
#include "opal/util/if.h"
#include "opal/util/argv.h"
#include "opal/util/output.h"
#include "util/sys_info.h"
#include "util/proc_info.h"
#include "mca/pml/pml.h"
#include "mca/base/mca_base_param.h"
#include "btl_self.h"
#include "btl_self_frag.h"



/*
 * Shared Memory (SELF) component instance. 
 */

mca_btl_self_component_t mca_btl_self_component = {
    {  /* super is being filled in */
        /* First, the mca_base_component_t struct containing meta information
          about the component itself */
        {
            /* Indicate that we are a pml v1.0.0 component (which also implies a
               specific MCA version) */
            MCA_BTL_BASE_VERSION_1_0_0,
            "self", /* MCA component name */
            OMPI_MAJOR_VERSION,  /* MCA component major version */
            OMPI_MINOR_VERSION,  /* MCA component minor version */
            OMPI_RELEASE_VERSION,  /* MCA component release version */
            mca_btl_self_component_open,  /* component open */
            mca_btl_self_component_close  /* component close */
        },

        /* Next the MCA v1.0.0 component meta data */
        {
            /* Whether the component is checkpointable or not */
            false
        },

        mca_btl_self_component_init,  
        NULL,
    }  /* end super */
};


/*
 * utility routines for parameter registration
 */

static inline char* mca_btl_self_param_register_string(
    const char* param_name, 
    const char* default_value)
{
    char *param_value;
    int id = mca_base_param_register_string("btl","self",param_name,NULL,default_value);
    mca_base_param_lookup_string(id, &param_value);
    return param_value;
}
                                                                                                                            
static inline int mca_btl_self_param_register_int(
    const char* param_name, 
    int default_value)
{
    int id = mca_base_param_register_int("btl","self",param_name,NULL,default_value);
    int param_value = default_value;
    mca_base_param_lookup_int(id,&param_value);
    return param_value;
}


/*
 *  Called by MCA framework to open the component, registers
 *  component parameters.
 */

int mca_btl_self_component_open(void)
{
    /* register SELF component parameters */
    mca_btl_self_component.free_list_num =
        mca_btl_self_param_register_int("free_list_num", 0);
    mca_btl_self_component.free_list_max =
        mca_btl_self_param_register_int("free_list_max", -1);
    mca_btl_self_component.free_list_inc =
        mca_btl_self_param_register_int("free_list_inc", 32);
    mca_btl_self.btl_eager_limit = 
        mca_btl_self_param_register_int("eager_limit", 128*1024);
    mca_btl_self.btl_min_send_size = 
    mca_btl_self.btl_max_send_size = 
        mca_btl_self_param_register_int("max_send_size", 256*1024);
    mca_btl_self.btl_min_rdma_size = 
    mca_btl_self.btl_max_rdma_size =
        mca_btl_self_param_register_int("max_rdma_size", INT_MAX);
    mca_btl_self.btl_exclusivity = 
        mca_btl_self_param_register_int("exclusivity", 64*1024);
    mca_btl_self.btl_flags =
        mca_btl_self_param_register_int("flags", MCA_BTL_FLAGS_PUT);

    /* initialize objects */
    OBJ_CONSTRUCT(&mca_btl_self_component.self_lock, opal_mutex_t);
    OBJ_CONSTRUCT(&mca_btl_self_component.self_frags_eager, ompi_free_list_t);
    OBJ_CONSTRUCT(&mca_btl_self_component.self_frags_send, ompi_free_list_t);
    OBJ_CONSTRUCT(&mca_btl_self_component.self_frags_rdma, ompi_free_list_t);
    return OMPI_SUCCESS;
}


/*
 * component cleanup - sanity checking of queue lengths
 */

int mca_btl_self_component_close(void)
{
    OBJ_DESTRUCT(&mca_btl_self_component.self_lock);
    OBJ_DESTRUCT(&mca_btl_self_component.self_frags_eager);
    OBJ_DESTRUCT(&mca_btl_self_component.self_frags_send);
    OBJ_DESTRUCT(&mca_btl_self_component.self_frags_rdma);
    return OMPI_SUCCESS;
}


/*
 *  SELF component initialization
 */
mca_btl_base_module_t** mca_btl_self_component_init(
    int *num_btls, 
    bool enable_progress_threads,
    bool enable_mpi_threads)
{
    mca_btl_base_module_t **btls = NULL;
    *num_btls = 0;

    /* allocate the Shared Memory PTL */
    *num_btls = 1;
    btls = malloc((*num_btls)*sizeof(mca_btl_base_module_t*));
    if (NULL == btls) {
        return NULL;
    }

    /* initialize free lists */
    ompi_free_list_init(&mca_btl_self_component.self_frags_eager, 
        sizeof(mca_btl_self_frag_eager_t) + mca_btl_self.btl_eager_limit,
        OBJ_CLASS(mca_btl_self_frag_eager_t),
        mca_btl_self_component.free_list_num,
        mca_btl_self_component.free_list_max,
        mca_btl_self_component.free_list_inc,
        NULL);
    ompi_free_list_init(&mca_btl_self_component.self_frags_send, 
        sizeof(mca_btl_self_frag_send_t) + mca_btl_self.btl_max_send_size,
        OBJ_CLASS(mca_btl_self_frag_send_t),
        mca_btl_self_component.free_list_num,
        mca_btl_self_component.free_list_max,
        mca_btl_self_component.free_list_inc,
        NULL);
    ompi_free_list_init(&mca_btl_self_component.self_frags_rdma, 
        sizeof(mca_btl_self_frag_rdma_t),
        OBJ_CLASS(mca_btl_self_frag_rdma_t),
        mca_btl_self_component.free_list_num,
        mca_btl_self_component.free_list_max,
        mca_btl_self_component.free_list_inc,
        NULL);

    /* get pointer to the btls */
    btls[0] = (mca_btl_base_module_t *)(&mca_btl_self);
    return btls;
}
