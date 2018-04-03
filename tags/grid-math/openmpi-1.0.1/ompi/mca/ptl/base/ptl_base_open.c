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

#include "mca/mca.h"
#include "mca/base/base.h"
#include "mca/base/mca_base_param.h"
#include "mca/pml/pml.h"
#include "mca/ptl/ptl.h"
#include "mca/ptl/base/base.h"


/*
 * The following file was created by configure.  It contains extern
 * statements and the definition of an array of pointers to each
 * component's public mca_base_component_t struct.
 */

#include "ompi/mca/ptl/base/static-components.h"


/*
 * Global variables
 */
int mca_ptl_base_output = -1;
char* mca_ptl_base_include = NULL;
char* mca_ptl_base_exclude = NULL;
opal_list_t mca_ptl_base_components_opened;
opal_list_t mca_ptl_base_components_initialized;
opal_list_t mca_ptl_base_modules_initialized;
int mca_ptl_base_open_called = 0;

/**
 * Function for finding and opening either all MCA components, or the one
 * that was specifically requested via a MCA parameter.
 */
int mca_ptl_base_open(void)
{

    if( 0 != mca_ptl_base_open_called ) return OMPI_SUCCESS;
    mca_ptl_base_open_called = 1;

    /* Open up all available components */
    if (OMPI_SUCCESS != 
        mca_base_components_open("ptl", 0, mca_ptl_base_static_components, 
                                 &mca_ptl_base_components_opened, true)) {
        return OMPI_ERROR;
    }

    /* Initialize the list containing all the PTL's where the init function has been called */
    OBJ_CONSTRUCT( &mca_ptl_base_components_initialized, opal_list_t );

    /* Initialize the list so that in mca_ptl_base_close(), we can
       iterate over it (even if it's empty, as in the case of
       ompi_info) */

    OBJ_CONSTRUCT(&mca_ptl_base_modules_initialized, opal_list_t);

    /* register parameters */
    mca_base_param_lookup_string(
                                 mca_base_param_register_string("ptl","base","include",NULL,NULL), &mca_ptl_base_include);
    mca_base_param_lookup_string(
                                 mca_base_param_register_string("ptl","base","exclude",NULL,NULL), &mca_ptl_base_exclude);

    /* All done */
    return OMPI_SUCCESS;
}