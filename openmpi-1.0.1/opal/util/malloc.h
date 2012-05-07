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

/** @file */

#ifndef OPAL_MALLOC_H
#define OPAL_MALLOC_H

#include <stdlib.h>

/*
 * THIS FILE CANNOT INCLUDE ANY OTHER OPAL HEADER FILES!!!
 *
 * It is included via <ompi_config_bottom.h>.  Hence, it should not
 * include ANY other files, nor should it include "ompi_config.h".
 *
 */

/*
 * Set OPAL_MALLOC_DEBUG_LEVEL to
 * 0 for no checking
 * 1 for basic error checking
 * 2 for more error checking
 */

#ifndef OPAL_MALLOC_DEBUG_LEVEL
#define OPAL_MALLOC_DEBUG_LEVEL 2
#endif

#if defined(c_plusplus) || defined(__cplusplus)
extern "C" {
#endif
  /**
   * Initialize malloc debug output.
   *
   * This function is invoked to setup a dedicated output stream for
   * malloc debug functions.  It does \em not (currently) do anything
   * other than that (i.e., no internal accounting for tracking
   * malloc/free statements, etc.).
   *
   * It is invoked as part of opal_init().  Although this function is
   * not \em necessary for OPAL_MALLOC() and OPAL_FREE(), it is strong
   * recommended because no output messages -- regardless of the
   * malloc debug level set by opal_malloc_debug() -- will be displayed
   * unless this function is invoked first.
   */
void opal_malloc_init(void);

  /**
   * Shut down malloc debug output.
   *
   * This function is invoked as part of opal_finalize() to shut down the
   * output stream for malloc debug messages.
   */
void opal_malloc_finalize(void);

  /**
   * \internal
   *
   * Back-end error-checking malloc function for OPAL (you should use
   * the normal malloc() instead of this function).
   *
   * @param size The number of bytes to allocate
   * @param file Typically the __FILE__ macro
   * @param line Typically the __LINE__ macro
   *
   * This function is only used when --enable-mem-debug was specified to
   * configure (or by default if you're building in a SVN checkout).
   */
OMPI_DECLSPEC void *opal_malloc(size_t size, const char *file, int line);

  /**
   * \internal
   *
   * Back-end error-checking calloc function for OPAL (you should use
   * the normal calloc() instead of this function).
   *
   * @param nmembers Number of elements to malloc
   * @param size Size of each elements
   * @param file Typically the __FILE__ macro
   * @param line Typically the __LINE__ macro
   *
   * This function is only used when --enable-mem-debug was specified to
   * configure (or by default if you're building in a SVN checkout).
   */
OMPI_DECLSPEC void *opal_calloc(size_t nmembers, size_t size, const char *file, int line);

  /**
   * \internal
   *
   * Back-end error-checking realloc function for OPAL (you should use
   * the normal realloc() instead of this function).
   *
   * @param ptr Pointer to reallocate
   * @param size The number of bytes to allocate
   * @param file Typically the __FILE__ macro
   * @param line Typically the __LINE__ macro
   *
   * This function is only used when --enable-mem-debug was specified to
   * configure (or by default if you're building in a SVN checkout).
   */
OMPI_DECLSPEC void *opal_realloc(void *ptr, size_t size, const char *file, int line);

  /**
   * \internal
   *
   * Back-end error-checking free function for OPAL (you should use
   * free() instead of this function).
   *
   * @param addr Address on the heap to free()
   * @param file Typically the __FILE__ macro
   * @param line Typically the __LINE__ macro
   *
   * This function is only used when --enable-mem-debug was specified
   * to configure (or by default if you're building in a SVN
   * checkout).
   */
OMPI_DECLSPEC void opal_free(void *addr, const char *file, int line);

OMPI_DECLSPEC extern int opal_malloc_debug_level;
OMPI_DECLSPEC extern int opal_malloc_output;

static inline void opal_malloc_debug(int level);


/**
 * Used to set the debug level for malloc debug.
 *
 * @param level The level of debugging (0 = none, 1 = some, 2 = more)
 *
 * This value defaults to the OPAL_MALLOC_DEBUG_LEVEL.
 */
static inline void opal_malloc_debug(int level)
{
  opal_malloc_debug_level = level;
}
#if defined(c_plusplus) || defined(__cplusplus)
}
#endif

#endif /* OPAL_MALLOC_H */
