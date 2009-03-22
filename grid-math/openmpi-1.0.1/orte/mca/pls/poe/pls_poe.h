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

#ifndef ORTE_PLS_POE_EXPORT_H
#define ORTE_PLS_POE_EXPORT_H

#include "ompi_config.h"

#include "mca/mca.h"
#include "mca/pls/pls.h"

#if defined(c_plusplus) || defined(__cplusplus)
extern "C" {
#endif

/*
 * Module open / close
 */
int orte_pls_poe_component_open(void);
int orte_pls_poe_component_close(void);
orte_pls_base_module_t* orte_pls_poe_component_init(int *priority);

/**
 * PLS Component
 */
struct orte_pls_poe_component_t {
    orte_pls_base_component_t super;
    orte_jobid_t jobid;
    int priority;
    int verbose;
    char* path;
    char* env;
    char** argv;
    int argc;
    int debug;
    char* orted;
    char* class;
    char* hostfile;
    char* cmdfile; 
    char* mp_stdoutmode;
    char* mp_labelio;
    int mp_retry;
    int mp_retrycount; 
    int mp_infolevel;
};
typedef struct orte_pls_poe_component_t orte_pls_poe_component_t;


ORTE_DECLSPEC extern orte_pls_poe_component_t mca_pls_poe_component;
ORTE_DECLSPEC extern orte_pls_base_module_t orte_pls_poe_module;



#if defined(c_plusplus) || defined(__cplusplus)
}
#endif
#endif /* ORTE_PLS_POE_EXPORT_H */