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
 *
 * The Open MPI general purpose registry - unit test
 *
 */

/*
 * includes
 */

#include "orte_config.h"
#include <stdio.h>
#include <string.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "include/orte_constants.h"
#include "mca/schema/schema.h"
#include "mca/schema/base/base.h"
#include "mca/ns/base/base.h"
#include "mca/soh/base/base.h"
#include "mca/rmgr/base/base.h"

#include "support.h"

#include "class/orte_pointer_array.h"
#include "dps/dps.h"
#include "runtime/runtime.h"
#include "util/proc_info.h"
#include "util/sys_info.h"
#include "opal/util/malloc.h"
#include "opal/util/output.h"

#include "mca/gpr/base/base.h"
#include "mca/gpr/replica/api_layer/gpr_replica_api.h"
#include "mca/gpr/replica/functional_layer/gpr_replica_fn.h"
#include "mca/gpr/replica/communications/gpr_replica_comm.h"
#include "mca/gpr/replica/transition_layer/gpr_replica_tl.h"

/* output files needed by the test */
static FILE *test_out=NULL;

int main(int argc, char **argv)
{
    int rc;
    size_t i;
    char *tokens[5], *keys[5];
    orte_data_type_t types[5];
    orte_gpr_value_union_t value, values[5];
    
    if (getenv("TEST_WRITE_TO_FILE") != NULL) {
        test_out = fopen( "test_gpr_quick_put", "w+" );
    } else {
        test_out = stderr;
    }
    if( test_out == NULL ) {
      test_failure("gpr_test couldn't open test file failed");
      test_finalize();
      exit(1);
    } 

    orte_init();
    
    tokens[0] = strdup("test-token-1");
    tokens[1] = strdup("test-token-2");
    tokens[2] = NULL;
    value.i32 = 123456;
    fprintf(stderr, "quick-put one value with single keyval\n");
    if (ORTE_SUCCESS != (rc = orte_gpr.put_1(ORTE_GPR_TOKENS_AND,
                            "test-put-segment", tokens,
                            "test-key", ORTE_INT32, value))) {
        fprintf(test_out, "gpr_test: put of 1 value/1 keyval failed with error code %s\n",
                    ORTE_ERROR_NAME(rc));
        return rc;
    } else {
        fprintf(test_out, "gpr_test: quick-put of 1 value/1 keyval passed\n");
    }
    free(tokens[0]);
    free(tokens[1]);
    
    for (i=0; i < 4; i++) {
        asprintf(&tokens[i], "test-token-%lu", (unsigned long)i);
        asprintf(&keys[i], "test-keys-%lu", (unsigned long)i);
        types[i] = ORTE_INT16;
        values[i].i16 = i * 1000;
    }
    tokens[4] = NULL;
    keys[4] = NULL;
    fprintf(stderr, "quick-put one value with multiple keyvals\n");
    if (ORTE_SUCCESS != (rc = orte_gpr.put_N(ORTE_GPR_TOKENS_AND,
                            "test-put-segment23", tokens, 4,
                            keys, types, values))) {
        fprintf(test_out, "gpr_test: put 1 value/multiple keyval failed with error code %s\n",
                    ORTE_ERROR_NAME(rc));
        return rc;
    } else {
        fprintf(test_out, "gpr_test: put 1 value/multiple keyval passed\n");
    }
    
    orte_gpr.dump_segment(NULL, 0);
    
    fprintf(stderr, "now finalize and see if all memory cleared\n");
    orte_finalize();

    fclose( test_out );
/*    result = system( cmd_str );
    if( result == 0 ) {
        test_success();
    }
    else {
      test_failure( "test_gpr_replica failed");
    }
*/
    unlink("test_gpr_quick_put");

    return(0);
}