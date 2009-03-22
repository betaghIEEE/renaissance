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

#include "include/orte_constants.h"
#include "mca/mca.h"
#include "mca/base/base.h"
#include "mca/pls/base/base.h"
#include "opal/util/output.h"


int orte_pls_base_finalize(void)
{
    /* Finalize all available modules */
    if (orte_pls_base.pls_available_valid) {
        opal_list_item_t* item;
        while (NULL != 
               (item = opal_list_remove_first(&orte_pls_base.pls_available))) {
            orte_pls_base_cmp_t* cmp = (orte_pls_base_cmp_t*) item;
            opal_output(orte_pls_base.pls_output,
                        "orte:base:close: finalizing module %s",
                        cmp->component->pls_version.mca_component_name);
            if (NULL != cmp->module->finalize) {
                cmp->module->finalize();
            }
            OBJ_RELEASE(cmp);
        }
    }
    orte_pls_base.pls_available_valid = false;
    return ORTE_SUCCESS;
}


int orte_pls_base_close(void)
{
    /* Close all remaining open components */
    if (orte_pls_base.pls_opened_valid) {
        orte_pls_base.pls_opened_valid = false;
        mca_base_components_close(orte_pls_base.pls_output, 
                                  &orte_pls_base.pls_opened, NULL);
        OBJ_DESTRUCT(&orte_pls_base.pls_opened);
    }
    return ORTE_SUCCESS;
}
