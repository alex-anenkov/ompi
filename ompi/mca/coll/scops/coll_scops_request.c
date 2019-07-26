/*
 * Copyright (c) 2019 Siberian state university of telecommunications and information sciences. All rights reserved.
 * $COPYRIGHT$
 *
 * $HEADER$
 */

#include "coll_scops_request.h"

int scops_request_start(size_t count, ompi_request_t ** requests) {
    // int res;
    // size_t i;

    // for (i = 0; i < count; i++) {
        //SCOPS_Handle *handle = (SCOPS_Handle *) requests[i];
        //NBC_Schedule *schedule = handle->schedule;
        //handle->super.req_complete = REQUEST_PENDING;
        //handle->nbc_complete = false;

        //res = NBC_Start(handle);
        // if (OPAL_UNLIKELY(OMPI_SUCCESS != res)) {
        //     return res;
        // }
    // }

    return OMPI_SUCCESS;
}

int scops_request_cancel(struct ompi_request_t *request, int complete) {
    return MPI_ERR_REQUEST;
}


int scops_request_free(struct ompi_request_t **ompi_req) {
    ompi_coll_scops_request_t *request = (ompi_coll_scops_request_t*) *ompi_req;

    if (!REQUEST_COMPLETE(&request->super)) {
        return MPI_ERR_REQUEST;
    }

    OMPI_COLL_SCOPS_REQUEST_RETURN(request);
    *ompi_req = MPI_REQUEST_NULL;

    return OMPI_SUCCESS;
}

void scops_request_construct(ompi_coll_scops_request_t *request) {
    request->super.req_type = OMPI_REQUEST_COLL;
    request->super.req_status._cancelled = 0;
    request->super.req_start = scops_request_start;
    request->super.req_free = scops_request_free;
    request->super.req_cancel = scops_request_cancel;
}
