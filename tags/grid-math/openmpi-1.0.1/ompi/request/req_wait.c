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
#include "request/request.h"

int ompi_request_poll_iterations = 20000;

int ompi_request_wait_any(
    size_t count,
    ompi_request_t ** requests,
    int *index, 
    ompi_status_public_t * status)
{
#if OMPI_ENABLE_PROGRESS_THREADS
    int c;
#endif
    size_t i=0, num_requests_null_inactive=0;
    int rc = OMPI_SUCCESS;
    int completed = -1;
    ompi_request_t **rptr=NULL;
    ompi_request_t *request=NULL;

#if OMPI_ENABLE_PROGRESS_THREADS
    /* poll for completion */
    opal_atomic_mb();
    for (c = 0; completed < 0 && c < ompi_request_poll_iterations; c++) {
        rptr = requests;
        num_requests_null_inactive = 0;
        for (i = 0; i < count; i++, rptr++) {
            request = *rptr;
            /* check for null or completed persistent request */
            if (MPI_REQUEST_NULL == request || 
                request->req_state == OMPI_REQUEST_INACTIVE) { 
                if(++num_requests_null_inactive == count)
                    goto finished;
                continue;
            }
            if (true == request->req_complete) {
                completed = i;
                goto finished;
            } 
        }
    }
#endif

    /* give up and sleep until completion */
    OPAL_THREAD_LOCK(&ompi_request_lock);
    ompi_request_waiting++;
    do {
        rptr = requests;
        num_requests_null_inactive = 0;
        for (i = 0; i < count; i++, rptr++) {
            request = *rptr;
            /* check for null or completed persistent request */
            if (MPI_REQUEST_NULL == request || 
                request->req_state == OMPI_REQUEST_INACTIVE) {
                num_requests_null_inactive++;
                continue;
            }
            if (request->req_complete == true) {
                completed = i;
                break;
            }
        }
        if(num_requests_null_inactive == count)
            break;
        if (completed < 0) {
            opal_condition_wait(&ompi_request_cond, &ompi_request_lock);
        }
    } while (completed < 0);
    ompi_request_waiting--;
    OPAL_THREAD_UNLOCK(&ompi_request_lock);

#if OMPI_ENABLE_PROGRESS_THREADS
finished:
#endif  /* OMPI_ENABLE_PROGRESS_THREADS */

    if(num_requests_null_inactive == count) {
        *index = MPI_UNDEFINED;
        if (MPI_STATUS_IGNORE != status) {
            *status = ompi_status_empty;
        }
    } else {

        /* return status */
        if (MPI_STATUS_IGNORE != status) {
            *status = request->req_status;
        }
        /* return request to pool */ 
        rc = request->req_fini(rptr);
        *index = completed;
    }
    return rc;
}


int ompi_request_wait_all(
    size_t count,
    ompi_request_t ** requests,
    ompi_status_public_t * statuses)
{
    size_t completed = 0, i;
    ompi_request_t **rptr;
    ompi_request_t *request;

    rptr = requests;
    for (i = 0; i < count; i++) {
        request = *rptr++;
        if (request->req_complete == true) {
            completed++;
        }
    }

    /* if all requests have not completed -- defer acquiring lock
     * unless required 
     */
    if (completed != count) {

        /*
         * acquire lock and test for completion - if all requests are
         * not completed pend on condition variable until a request
         * completes
         */
        OPAL_THREAD_LOCK(&ompi_request_lock);
        ompi_request_waiting++;

        do {
            /* check number of pending requests */
            size_t start = ompi_request_completed;
            size_t pending = count - completed;

            /*
             * wait until at least pending requests complete 
            */
            while (pending > ompi_request_completed - start) {
                opal_condition_wait(&ompi_request_cond, &ompi_request_lock);
            }

            /*
             * confirm that all pending operations have completed
            */
            completed = 0;
            rptr = requests;
            for (i = 0; i < count; i++) {
                request = *rptr++;
                if (request->req_complete == true) {
                    completed++;
                }
            }
        } while (completed != count);
        ompi_request_waiting--;
        OPAL_THREAD_UNLOCK(&ompi_request_lock);
    }

    if (MPI_STATUSES_IGNORE != statuses) {
        /* fill out status and free request if required */
        rptr = requests;
        for (i = 0; i < count; i++) {
            int rc;
            request = *rptr;
            if(request == MPI_REQUEST_NULL ||
               request->req_state == OMPI_REQUEST_INACTIVE) {
                statuses[i] = ompi_status_empty;
            } else {
                statuses[i] = request->req_status;
            }
            rc = request->req_fini(rptr);
            rptr++;
        }
    } else {
        /* free request if required */
        rptr = requests;
        for (i = 0; i < count; i++) {
            int rc;
            request = *rptr;
            rc = request->req_fini(rptr);
            rptr++;
        }
    }
    return OMPI_SUCCESS;
}
