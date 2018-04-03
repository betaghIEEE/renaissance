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
 * @file:
 * Part of the rsh launcher. See pls_rsh.h for an overview of how it works.
 */

#ifndef ORTE_PLS_RSH_EXPORT_H
#define ORTE_PLS_RSH_EXPORT_H

#include "ompi_config.h"

#include "opal/threads/condition.h"
#include "mca/mca.h"
#include "mca/pls/pls.h"

#if defined(c_plusplus) || defined(__cplusplus)
extern "C" {
#endif

/*
 * Module open / close
 */
int orte_pls_rsh_component_open(void);
int orte_pls_rsh_component_close(void);
orte_pls_base_module_t* orte_pls_rsh_component_init(int *priority);

/*
 * Startup / Shutdown
 */
int orte_pls_rsh_finalize(void);

/*
 * Interface
 */
int orte_pls_rsh_launch(orte_jobid_t);
int orte_pls_rsh_terminate_job(orte_jobid_t);
int orte_pls_rsh_terminate_proc(const orte_process_name_t* proc_name);
 
/**
 * PLS Component
 */
struct orte_pls_rsh_component_t {
    orte_pls_base_component_t super;
    bool debug;
    bool reap;
    bool assume_same_shell;
    int delay;
    int priority;
    char** argv;
    int argc;
    char* orted;
    char* path;
    size_t num_children;
    size_t num_concurrent;
    opal_mutex_t lock;
    opal_condition_t cond;
};
typedef struct orte_pls_rsh_component_t orte_pls_rsh_component_t;

ORTE_DECLSPEC extern orte_pls_rsh_component_t mca_pls_rsh_component;
ORTE_DECLSPEC extern orte_pls_base_module_t orte_pls_rsh_module;

#if defined(c_plusplus) || defined(__cplusplus)
}
#endif
#endif /* ORTE_PLS_RSH_EXPORT_H */