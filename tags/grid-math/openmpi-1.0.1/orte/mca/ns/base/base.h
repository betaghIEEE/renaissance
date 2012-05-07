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
/** @file:
 */

#ifndef MCA_NS_BASE_H
#define MCA_NS_BASE_H

/*
 * includes
 */
#include "orte_config.h"
#include "include/orte_constants.h"

#include "opal/class/opal_list.h"
#include "mca/mca.h"
#include "dps/dps_types.h"

#include "mca/ns/ns.h"


/*
 * Global functions for MCA overall collective open and close
 */
#if defined(c_plusplus) || defined(__cplusplus)
extern "C" {
#endif

/* default limits */
#define ORTE_NS_ARRAY_MAX_SIZE  INT_MAX
#define ORTE_NS_ARRAY_BLOCK_SIZE    512
/*
 * Internal definitions
 */
typedef uint8_t orte_ns_cmd_bitmask_t;

/*
 * packing type definitions
 */
/* CAUTION - any changes here must also change corresponding
 * typedefs above
 */
#define ORTE_NS_CMD     ORTE_INT8

/*
 * define flag values for remote commands - only used internally
 */
#define ORTE_NS_CREATE_CELLID_CMD       (int8_t)0x01
#define ORTE_NS_CREATE_JOBID_CMD        (int8_t)0x02
#define ORTE_NS_RESERVE_RANGE_CMD       (int8_t)0x04
#define ORTE_NS_ASSIGN_OOB_TAG_CMD      (int8_t)0x08
#define ORTE_NS_GET_JOB_PEERS_CMD       (int8_t)0x0A
#define ORTE_NS_DEFINE_DATA_TYPE_CMD    (int8_t)0x10
#define ORTE_NS_CREATE_MY_NAME_CMD      (int8_t)0x20
#define ORTE_NS_DUMP_CELLS_CMD          (int8_t)0x21
#define ORTE_NS_DUMP_JOBIDS_CMD         (int8_t)0x22
#define ORTE_NS_DUMP_TAGS_CMD           (int8_t)0x23
#define ORTE_NS_DUMP_DATATYPES_CMD      (int8_t)0x24


/*
 * function definitions
 */
OMPI_DECLSPEC    int orte_ns_base_open(void);
OMPI_DECLSPEC    int orte_ns_base_select(void);
OMPI_DECLSPEC    int orte_ns_base_close(void);

    /*
     * Base functions that are common to all implementations - can be overridden
     */

OMPI_DECLSPEC    int orte_ns_base_assign_cellid_to_process(orte_process_name_t* name);

OMPI_DECLSPEC    int orte_ns_base_create_process_name(orte_process_name_t **name,
                                  orte_cellid_t cell,
                                  orte_jobid_t job,
                                  orte_vpid_t vpid);

OMPI_DECLSPEC    int orte_ns_base_copy_process_name(orte_process_name_t **dest,
                                orte_process_name_t* src);

OMPI_DECLSPEC    int orte_ns_base_convert_string_to_process_name(orte_process_name_t **name,
                                             const char* name_string);

OMPI_DECLSPEC    int orte_ns_base_get_proc_name_string(char **name_string,
                                   const orte_process_name_t* name);

OMPI_DECLSPEC    int orte_ns_base_get_vpid_string(char **vpid_string, const orte_process_name_t* name);

OMPI_DECLSPEC    int orte_ns_base_convert_vpid_to_string(char **vpid_string, const orte_vpid_t vpid);

OMPI_DECLSPEC    int orte_ns_base_convert_string_to_vpid(orte_vpid_t *vpid, const char* vpidstring);

OMPI_DECLSPEC    int orte_ns_base_get_jobid_string(char **jobid_string, const orte_process_name_t* name);

OMPI_DECLSPEC    int orte_ns_base_convert_jobid_to_string(char **jobid_string, const orte_jobid_t jobid);

OMPI_DECLSPEC    int orte_ns_base_convert_string_to_jobid(orte_jobid_t *jobid, const char* jobidstring);

OMPI_DECLSPEC    int orte_ns_base_get_cellid_string(char **cellid_string, const orte_process_name_t* name);

OMPI_DECLSPEC    int orte_ns_base_convert_string_to_cellid(orte_cellid_t *cellid, const char *cellidstring);

OMPI_DECLSPEC    int orte_ns_base_convert_cellid_to_string(char **cellid_string, const orte_cellid_t cellid);

OMPI_DECLSPEC    int orte_ns_base_get_vpid(orte_vpid_t *vpid, const orte_process_name_t* name);

OMPI_DECLSPEC    int orte_ns_base_get_jobid(orte_jobid_t *jobid, const orte_process_name_t* name);

OMPI_DECLSPEC    int orte_ns_base_get_cellid(orte_cellid_t *cellid, const orte_process_name_t* name);

OMPI_DECLSPEC    int orte_ns_base_compare(orte_ns_cmp_bitmask_t fields,
			const orte_process_name_t* name1,
			const orte_process_name_t* name2);

OMPI_DECLSPEC    int orte_ns_base_free_name(orte_process_name_t **name);

OMPI_DECLSPEC    int orte_ns_base_print_dump(orte_buffer_t *buffer, int output_id);


/* not available functions */
OMPI_DECLSPEC    int orte_ns_base_module_init_not_available(void);

OMPI_DECLSPEC    int orte_ns_base_create_cellid_not_available(orte_cellid_t *cellid,
                                    char *site, char *resource);

OMPI_DECLSPEC    int orte_ns_base_get_cell_info_not_available(orte_cellid_t cellid,
                                char **site, char **resource);
                                
OMPI_DECLSPEC    int orte_ns_base_create_jobid_not_available(orte_jobid_t *jobid);

OMPI_DECLSPEC    int orte_ns_base_get_vpid_range_not_available(orte_jobid_t job,
                                                               orte_vpid_t range,
                                                               orte_vpid_t *startvpid);

OMPI_DECLSPEC    int orte_ns_base_derive_vpid(orte_vpid_t *vpid,
                                             orte_vpid_t base_vpid,
                                             int offset);

OMPI_DECLSPEC    int orte_ns_base_assign_rml_tag_not_available(orte_rml_tag_t *tag, char *name);

OMPI_DECLSPEC    int orte_ns_base_define_data_type_not_available(
                                  const char *name,
                                  orte_data_type_t *type);

OMPI_DECLSPEC    int orte_ns_base_create_my_name_not_available(void);

OMPI_DECLSPEC    int orte_ns_base_get_job_peers_not_available(orte_process_name_t **procs, 
                                  size_t *num_procs, orte_jobid_t job);

OMPI_DECLSPEC    int orte_ns_base_dump_cells_not_available(int output_id);
OMPI_DECLSPEC    int orte_ns_base_dump_jobs_not_available(int output_id);
OMPI_DECLSPEC    int orte_ns_base_dump_tags_not_available(int output_id);
OMPI_DECLSPEC    int orte_ns_base_dump_datatypes_not_available(int output_id);

/* Base functions used everywhere */
OMPI_DECLSPEC    int orte_ns_base_get_peers(orte_process_name_t **procs, 
                                  size_t *num_procs, size_t *self);

OMPI_DECLSPEC    int orte_ns_base_pack_name(orte_buffer_t *buffer, void *src,
                       size_t num_vals, orte_data_type_t type);

OMPI_DECLSPEC    int orte_ns_base_pack_cellid(orte_buffer_t *buffer, void *src,
                       size_t num_vals, orte_data_type_t type);

OMPI_DECLSPEC    int orte_ns_base_pack_jobid(orte_buffer_t *buffer, void *src,
                       size_t num_vals, orte_data_type_t type);

OMPI_DECLSPEC    int orte_ns_base_pack_vpid(orte_buffer_t *buffer, void *src,
                       size_t num_vals, orte_data_type_t type);

OMPI_DECLSPEC    int orte_ns_base_unpack_name(orte_buffer_t *buffer, void *dest,
                       size_t *num_vals, orte_data_type_t type);

OMPI_DECLSPEC    int orte_ns_base_unpack_cellid(orte_buffer_t *buffer, void *dest,
                       size_t *num_vals, orte_data_type_t type);

OMPI_DECLSPEC    int orte_ns_base_unpack_jobid(orte_buffer_t *buffer, void *dest,
                       size_t *num_vals, orte_data_type_t type);

OMPI_DECLSPEC    int orte_ns_base_unpack_vpid(orte_buffer_t *buffer, void *dest,
                       size_t *num_vals, orte_data_type_t type);
/*
 * globals that might be needed
 */

OMPI_DECLSPEC extern int mca_ns_base_output;
OMPI_DECLSPEC extern bool mca_ns_base_selected;
OMPI_DECLSPEC extern opal_list_t mca_ns_base_components_available;
OMPI_DECLSPEC extern mca_ns_base_component_t mca_ns_base_selected_component;

/*
 * external API functions will be documented in the mca/ns/ns.h file
 */

#if defined(c_plusplus) || defined(__cplusplus)
}
#endif
#endif