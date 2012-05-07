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
#include "include/constants.h"
#include "mca/mca.h"
#include "mca/base/base.h"
#include "mca/base/mca_base_param.h"
#include "mca/oob/oob.h"


/*
 * The following file was created by configure.  It contains extern
 * statements and the definition of an array of pointers to each
 * component's public mca_base_component_t struct.
 */

#include "orte/mca/oob/base/static-components.h"


/*
 * Global variables
 */
mca_oob_t mca_oob;
int mca_oob_base_output = -1;
char* mca_oob_base_include = NULL;
char* mca_oob_base_exclude = NULL;
opal_list_t mca_oob_base_components;
opal_list_t mca_oob_base_modules;
opal_list_t mca_oob_base_exception_handlers;

/**
 * Function for finding and opening either all MCA components, or the one
 * that was specifically requested via a MCA parameter.
 */
int mca_oob_base_open(void)
{
  /* Open up all available components */

  OBJ_CONSTRUCT(&mca_oob_base_components, opal_list_t);
  OBJ_CONSTRUCT(&mca_oob_base_modules, opal_list_t);
  OBJ_CONSTRUCT(&mca_oob_base_exception_handlers, opal_list_t);

  if (OMPI_SUCCESS != 
      mca_base_components_open("oob", mca_oob_base_output,
                               mca_oob_base_static_components, 
                               &mca_oob_base_components, true)) {
    return ORTE_ERROR;
  }

  /* register parameters */
  mca_base_param_reg_string_name("oob_base", "include",
                                 "Components to include for oob framework selection",
                                 false, false, NULL, &mca_oob_base_include);
  mca_base_param_reg_string_name("oob_base", "exclude",
                                 "Components to exclude for oob framework selection",
                                 false, false, NULL, &mca_oob_base_exclude);

  /* All done */
  return ORTE_SUCCESS;
}
