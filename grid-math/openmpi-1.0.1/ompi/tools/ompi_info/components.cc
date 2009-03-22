//
// Copyright (c) 2004-2005 The Trustees of Indiana University and Indiana
//                         University Research and Technology
//                         Corporation.  All rights reserved.
// Copyright (c) 2004-2005 The University of Tennessee and The University
//                         of Tennessee Research Foundation.  All rights
//                         reserved.
// Copyright (c) 2004-2005 High Performance Computing Center Stuttgart, 
//                         University of Stuttgart.  All rights reserved.
// Copyright (c) 2004-2005 The Regents of the University of California.
//                         All rights reserved.
// $COPYRIGHT$
// 
// Additional copyrights may follow
// 
// $HEADER$
//

#include "ompi_config.h"

#include <iostream>
#include <string>

#include <stdlib.h>
#include <string.h>

#include "ompi/runtime/params.h"
#include "orte/runtime/runtime.h"
#include "ompi/tools/ompi_info/ompi_info.h"

#include "opal/mca/base/base.h"
#include "opal/mca/memory/memory.h"
#include "opal/mca/paffinity/paffinity.h"
#include "opal/mca/paffinity/base/base.h"
#include "opal/mca/paffinity/base/internal.h"
#include "opal/mca/maffinity/maffinity.h"
#include "opal/mca/maffinity/base/base.h"
#include "opal/mca/maffinity/base/internal.h"
#include "opal/mca/memory/memory.h"
#include "opal/mca/memory/base/base.h"
#include "opal/mca/timer/timer.h"
#include "opal/mca/timer/base/base.h"

#include "ompi/mca/allocator/allocator.h"
#include "ompi/mca/allocator/base/base.h"
#include "ompi/mca/coll/coll.h"
#include "ompi/mca/coll/base/base.h"
#include "ompi/mca/io/io.h"
#include "ompi/mca/io/base/base.h"
#include "ompi/mca/mpool/mpool.h"
#include "ompi/mca/mpool/base/base.h"
#include "ompi/mca/pml/pml.h"
#include "ompi/mca/pml/base/base.h"
#include "ompi/mca/ptl/ptl.h"
#include "ompi/mca/ptl/base/base.h"
#include "ompi/mca/btl/btl.h"
#include "ompi/mca/btl/base/base.h"
#include "ompi/mca/topo/topo.h"
#include "ompi/mca/topo/base/base.h"

#include "orte/mca/errmgr/errmgr.h"
#include "orte/mca/errmgr/base/base.h"
#include "orte/mca/gpr/gpr.h"
#include "orte/mca/gpr/base/base.h"
#include "orte/mca/iof/iof.h"
#include "orte/mca/iof/base/base.h"
#include "orte/mca/ns/ns.h"
#include "orte/mca/ns/base/base.h"
#include "orte/mca/oob/oob.h"
#include "orte/mca/oob/base/base.h"
#include "orte/mca/ras/ras.h"
#include "orte/mca/ras/base/base.h"
#include "orte/mca/rds/rds.h"
#include "orte/mca/rds/base/base.h"
#include "orte/mca/rmaps/rmaps.h"
#include "orte/mca/rmaps/base/base.h"
#include "orte/mca/rmgr/rmgr.h"
#include "orte/mca/rmgr/base/base.h"
#include "orte/mca/rml/rml.h"
#include "orte/mca/rml/base/base.h"
#include "orte/mca/pls/pls.h"
#include "orte/mca/pls/base/base.h"
#include "orte/mca/soh/soh.h"
#include "orte/mca/soh/base/base.h"
#include "orte/mca/sds/sds.h"
#include "orte/mca/sds/base/base.h"

using namespace std;
using namespace ompi_info;


//
// Public variables
//

ompi_info::component_map_t ompi_info::component_map;


//
// Private variables
//

static bool opened_components = false;


//
// Open all MCA components so that they can register their MCA
// parameters.  Take a shotgun approach here and indiscriminately open
// all components -- don't be selective.  To this end, we need to clear
// out the environment of all OMPI_MCA_<type> variables to ensure
// that the open algorithms don't try to only open one component.
//
void ompi_info::open_components()
{
  ompi_info::type_vector_t::size_type i;
  string env;
  char *target;

  if (opened_components)
    return;

  // Clear out the environment.  Use strdup() to orphan the resulting
  // strings because items are placed in the environment by reference,
  // not by value.

  for (i = 0; i < mca_types.size(); ++i) {
    env = "OMPI_MCA_" + mca_types[i];
    if (NULL != getenv(env.c_str())) {
      env += "=";
      target = strdup(env.c_str());
      putenv(target);
    }
  }

  // Open the DPS

  if (ORTE_SUCCESS != orte_dps_open()) {
     printf( "Unable to initialize the DPS\n" );
     return;
  }
    
  // Open up the MCA

  mca_base_open();

  // Register the ORTE layer's MCA parameters

  orte_register_params(false);

  // Register the MPI layer's MCA parameters

  ompi_mpi_register_params();

  // Find / open all components

  component_map["base"] = NULL;

  // OPAL frameworks

  opal_memory_base_open();
  component_map["memory"] = &opal_memory_base_components_opened;

  opal_paffinity_base_open();
  component_map["paffinity"] = &opal_paffinity_base_components_opened;

  opal_maffinity_base_open();
  component_map["maffinity"] = &opal_maffinity_base_components_opened;

  opal_timer_base_open();
  component_map["timer"] = &opal_timer_base_components_opened;

  // ORTE frameworks

  mca_oob_base_open();
  component_map["oob"] = &mca_oob_base_components;

  orte_errmgr_base_open();
  component_map["errmgr"] = &orte_errmgr_base_components_available;

  orte_gpr_base_open();
  component_map["gpr"] = &orte_gpr_base_components_available;

  orte_iof_base_open();
  component_map["iof"] = &orte_iof_base.iof_components_opened;

  orte_ns_base_open();
  component_map["ns"] = &mca_ns_base_components_available;

  orte_ras_base_open();
  component_map["ras"] = &orte_ras_base.ras_opened;

  orte_rds_base_open();
  component_map["rds"] = &orte_rds_base.rds_components;

  orte_rmaps_base_open();
  component_map["rmaps"] = &orte_rmaps_base.rmaps_opened;

  orte_rmgr_base_open();
  component_map["rmgr"] = &orte_rmgr_base.rmgr_components;

  orte_rml_base_open();
  component_map["rml"] = &orte_rml_base.rml_components;

  orte_pls_base_open();
  component_map["pls"] = &orte_pls_base.pls_opened;

  orte_sds_base_open();
  component_map["sds"] = &orte_sds_base_components_available;

  orte_soh_base_open();
  component_map["soh"] = &orte_soh_base.soh_components;

  // MPI frameworks

  mca_allocator_base_open();
  component_map["allocator"] = &mca_allocator_base_components;

  mca_coll_base_open();
  component_map["coll"] = &mca_coll_base_components_opened;

  mca_io_base_open();
  component_map["io"] = &mca_io_base_components_opened;

  mca_mpool_base_open();
  component_map["mpool"] = &mca_mpool_base_components;

  mca_pml_base_open();
  component_map["pml"] = &mca_pml_base_components_available;

  /* mca_ptl_base_open() should not be called directly. This call is performed
   * in the PML base open.
   */
  component_map["ptl"] = &mca_ptl_base_components_opened;

  mca_btl_base_open();
  component_map["btl"] = &mca_btl_base_components_opened;

  mca_topo_base_open();
  component_map["topo"] = &mca_topo_base_components_opened;

  // All done

  opened_components = true;
}


void ompi_info::close_components()
{
    if (opened_components) {

        // Note that the order of shutdown here doesn't matter because
        // we aren't *using* any components -- none were selected, so
        // there are no dependencies between the frameworks.  We list
        // them generally "in order", but it doesn't really matter.

        mca_topo_base_close();
        // the PML has to call the base PTL close function.
        mca_btl_base_close();
        mca_pml_base_close();
        mca_mpool_base_close();
        mca_io_base_close();
        mca_coll_base_close();
        mca_allocator_base_close();

        orte_iof_base_close();
        orte_sds_base_close();
        orte_soh_base_close();
        orte_pls_base_close();
        orte_rmgr_base_close();
        orte_rmaps_base_close();
        orte_rds_base_close();
        orte_ras_base_close();
        orte_ns_base_close();
        orte_gpr_base_close();
        orte_errmgr_base_close();
        orte_rml_base_close();
        mca_oob_base_close();
    
        opal_memory_base_close();
        opal_paffinity_base_close();
        opal_maffinity_base_close();
        opal_timer_base_close();

        component_map.clear();
    }

    opened_components = false;
}