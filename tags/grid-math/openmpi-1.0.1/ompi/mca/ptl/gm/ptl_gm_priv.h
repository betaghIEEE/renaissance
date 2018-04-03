/* -*- Mode: C; c-basic-offset:4 ; -*- */

/*
 * Copyright (c) 2004-2005 The Trustees of Indiana University and Indiana
 *                         University Research and Technology
 *                         Corporation.  All rights reserved.
 * Copyright (c) 2004-2005 The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
 * Copyright (c) 2004 The Ohio State University.
 *                    All rights reserved.
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
#include "gm.h"

struct mca_ptl_gm_send_frag_t;
struct mca_ptl_gm_peer_t;
struct mca_ptl_gm_module_t;

/* Pinning down memory pages is a costly operation. We can avoid it by using a LRU list
 * of pinned down memory, managed inside the GM PTL.
 */
gm_status_t mca_ptl_gm_register_memory( struct gm_port *port, void *ptr, unsigned len );
gm_status_t mca_ptl_gm_deregister_memory( struct gm_port *port, void *ptr, unsigned len );
#if OMPI_MCA_PTL_GM_CACHE_ENABLE
void mca_ptl_gm_regcache_init(void);
unsigned int gmpi_use_interval(struct gm_port *gmpi_gm_port, gm_up_t start, unsigned int length);
gm_status_t gmpi_unuse_interval(struct gm_port *gmpi_gm_port, gm_up_t start, unsigned int length);
void gmpi_clear_interval(struct gm_port *gmpi_gm_port, gm_up_t start, unsigned int length);
void gmpi_clear_all_intervals(void);
#endif  /* OMPI_MCA_PTL_GM_CACHE_ENABLE */

/* Some flags that have to go in the header hdr_common.hdr_flags field */
#define PTL_FLAG_GM_HAS_FRAGMENT    0x04
#define PTL_FLAG_GM_LAST_FRAGMENT   0x08
#define PTL_FLAG_GM_REQUIRE_LOCK    0x10

/* Internal flags for handling long messages */
#define GM_PTL_REGISTER_MEMORY      0x01
#define GM_PTL_SEND_MESSAGE         0x02

int mca_ptl_gm_analyze_recv_event( struct mca_ptl_gm_module_t* ptl, gm_recv_event_t* event );

void mca_ptl_gm_outstanding_recv( struct mca_ptl_gm_module_t *ptl);

int mca_ptl_gm_peer_send( struct mca_ptl_base_module_t* ptl,
                          struct mca_ptl_base_peer_t* ptl_base_peer,
                          struct mca_ptl_base_send_request_t *sendreq,
                          size_t offset,
                          size_t size,
                          int flags );

int
mca_ptl_gm_peer_send_continue( struct mca_ptl_gm_peer_t *ptl_peer,
			       struct mca_ptl_gm_send_frag_t *fragment,
			       struct mca_ptl_base_send_request_t *sendreq,
			       size_t offset,
			       size_t *size,
			       int flags );