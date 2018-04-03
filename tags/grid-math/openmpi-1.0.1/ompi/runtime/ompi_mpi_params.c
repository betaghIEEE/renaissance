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

#include "ompi/include/constants.h"
#include "opal/util/output.h"
#include "opal/util/show_help.h"
#include "opal/mca/base/mca_base_param.h"
#include "ompi/runtime/mpiruntime.h"
#include "ompi/runtime/params.h"
#include <time.h>

/*
 * Global variables
 *
 * As a deviation from the norm, ompi_mpi_param_check is also
 * extern'ed in src/mpi/interface/c/bindings.h because it is already
 * included in all MPI function imlementation files
 *
 * The values below are the default values.
 */
bool ompi_mpi_param_check = true;
bool ompi_debug_show_handle_leaks = false;
bool ompi_debug_no_free_handles = false;
bool ompi_mpi_show_mca_params = false;
char *ompi_mpi_show_mca_params_file = NULL;
bool ompi_mpi_paffinity_alone = false;


int ompi_mpi_register_params(void)
{
    int value;

    /* Whether we want MPI API function parameter checking or not */

    mca_base_param_reg_int_name("mpi", "param_check", 
                                "Whether you want MPI API parameters checked at run-time or not.  Possible values are 0 (no checking) and 1 (perform checking at run-time)",
                                false, false, MPI_PARAM_CHECK, &value);
    ompi_mpi_param_check = (bool) value;
    if (ompi_mpi_param_check) {
        value = 0;
        if (MPI_PARAM_CHECK) {
            value = 1;
        }
        if (0 == value) {
            opal_show_help("help-ompi-runtime.txt", 
                           "mpi-param-check-enabled-but-compiled-out",
                           true);
            ompi_mpi_param_check = false;
        }
    }

    /*
     * This string is going to be used in opal/util/stacktrace.c
     */
    mca_base_param_reg_string_name("mpi", "signal", 
                                   "If a signal is received, display the stack trace frame",
                                   false, false, NULL, NULL);
    
    /*
     * opal_progress: decide whether to yield and the event library
     * tick rate
     */
    /* JMS: Need ORTE data here -- set this to 0 when
       exactly/under-subscribed, or 1 when oversubscribed */
    mca_base_param_reg_int_name("mpi", "yield_when_idle", 
                                "Yield the processor when waiting for MPI communication (for MPI processes, will default to 1 when oversubscribing nodes)",
                                false, false, 0, NULL);
    mca_base_param_reg_int_name("mpi", "event_tick_rate", 
                                "How often to progress TCP communications (0 = never, otherwise specified in microseconds)",
                                false, false, -1, NULL);

    /* Whether or not to show MPI handle leaks */
    
    mca_base_param_reg_int_name("mpi", "show_handle_leaks",
                                "Whether MPI_FINALIZE shows all MPI handles that were not freed or not",
                                false, false, 
                                (int) ompi_debug_show_handle_leaks, &value);
    ompi_debug_show_handle_leaks = (bool) value;
    
    /* Whether or not to free MPI handles.  Useless without run-time
       param checking, so implicitly set that to true if we don't want
       to free the handles. */
    
    mca_base_param_reg_int_name("mpi", "no_free_handles", 
                                "Whether to actually free MPI objects when their handles are freed",
                                false, false, 
                                (int) ompi_debug_no_free_handles, &value);
    ompi_debug_no_free_handles = (bool) value;
    if (ompi_debug_no_free_handles) {
        ompi_mpi_param_check = true;
        value = 0;
        if (MPI_PARAM_CHECK) {
            value = 1;
        }
        if (0 == value) {
            opal_output(0, "WARNING: MCA parameter mpi_no_free_handles set to true, but MPI");
            opal_output(0, "WARNING: parameter checking has been compiled out of Open MPI.");
            opal_output(0, "WARNING: mpi_no_free_handles is therefore only partially effective!");
        }
    }

    /* Whether or not to print all MCA parameters in MPI_INIT */
    mca_base_param_reg_int_name("mpi", "show_mca_params",
                                "Whether to show all MCA parameter value during MPI_INIT or not (good for reproducability of MPI jobs)",
                                false, false, 
                                (int) ompi_mpi_show_mca_params, &value);
    ompi_mpi_show_mca_params = (bool) value;

    /* File to use when dumping the parameters */
    mca_base_param_reg_string_name("mpi", "show_mca_params_file",
                                   "If mpi_show_mca_params is true, setting this string to a valid filename tells Open MPI to dump all the MCA parameter values into a file suitable for reading via the mca_param_files parameter (good for reproducability of MPI jobs)",
                                   false, false,
                                   "", &ompi_mpi_show_mca_params_file);
    
    /* User-level process pinning controls */
    mca_base_param_reg_int_name("mpi", "paffinity_alone",
                                "If nonzero, assume that this job is the only (set of) process(es) running on each node and bind processes to processors, starting with processor ID 0",
                                false, false, 
                                (int) ompi_mpi_paffinity_alone, &value);
    ompi_mpi_paffinity_alone = (bool) value;

    mca_base_param_reg_int_name("mpi", "paffinity_processor",
                                "If set, pin this process to the processor number indicated by the value",
                                true, false, 
                                -1, NULL);

    /* All done */

    return OMPI_SUCCESS;
}

int ompi_show_all_mca_params(int32_t rank, int requested, char *nodename) {
   opal_list_t *info;
   opal_list_item_t *i;
   mca_base_param_info_t *item;
   char *value_string;
   int value_int;
   FILE *fp = NULL;
   time_t timestamp;

   if (rank != 0) {
      return OMPI_SUCCESS;
   }

   timestamp = time(NULL);

   /* Open the file if one is specified */
   if (0 != strlen(ompi_mpi_show_mca_params_file)) {
      if ( NULL == (fp = fopen(ompi_mpi_show_mca_params_file, "w")) ) {
         opal_output(0, "Unable to open file <%s> to write MCA parameters", ompi_mpi_show_mca_params_file);
         return OMPI_ERR_FILE_OPEN_FAILURE;
      }
      fprintf(fp, "#\n");
      fprintf(fp, "# This file was automatically generated on %s", ctime(&timestamp));
      fprintf(fp, "# by MPI_COMM_WORLD rank %d (out of a total of %d) on %s\n", rank, requested, nodename );
      fprintf(fp, "#\n");
   }

   mca_base_param_dump(&info, false);
   for (i =  opal_list_get_first(info); 
        i != opal_list_get_last(info);
        i =  opal_list_get_next(i)) {
      item = (mca_base_param_info_t*) i;

      /* Get the parameter name, and convert it to a printable string */
      if (MCA_BASE_PARAM_TYPE_STRING == item->mbpp_type) {
         mca_base_param_lookup_string(item->mbpp_index, &value_string);
         if (NULL == value_string) {
            value_string = strdup("");
         }
      } else {
         mca_base_param_lookup_int(item->mbpp_index, &value_int);
         asprintf(&value_string, "%d", value_int);
      }

      /* Print the parameter */
      if (0 != strlen(ompi_mpi_show_mca_params_file)) {
         fprintf(fp, "%s=%s\n", item->mbpp_full_name, value_string);
      } else {
         opal_output(0, "%s=%s", item->mbpp_full_name, value_string);
      }

      free(value_string);
   }

   /* Close file, cleanup allocated memory*/
   if (0 != strlen(ompi_mpi_show_mca_params_file)) {
      fclose(fp);
   }
   mca_base_param_dump_release(info);

   return OMPI_SUCCESS;
}