/*
 * Copyright (c) 2019 Siberian state university of telecommunications and information sciences. All rights reserved.
 * $COPYRIGHT$
 *
 * $HEADER$
 */

#include "coll_scops_request.h"

int scops_request_start(size_t count, ompi_request_t **reqs) {
    int res = OMPI_SUCCESS;

    // for(size_t i = 0; i < count; i++) {
        //SCOPS_Handle *handle = (SCOPS_Handle *) reqs[i];
        //NBC_Schedule *schedule = handle->schedule;
        //handle->super.req_complete = REQUEST_PENDING;
        //handle->nbc_complete = false;

        //res = NBC_Start(handle);
        // if (OPAL_UNLIKELY(OMPI_SUCCESS != res)) {
        //     return res;
        // }
    // }

    return res;
}

int scops_request_cancel(struct ompi_request_t *req, int complete) {
    return MPI_ERR_REQUEST;
}

int scops_request_free(struct ompi_request_t **req) {
    ompi_coll_scops_request_t *scops_request = (ompi_coll_scops_request_t*) *req;

    if (!REQUEST_COMPLETE(&scops_request->super)) {
        return MPI_ERR_REQUEST;
    }

    OMPI_COLL_SCOPS_REQUEST_RETURN(scops_request);
    *req = MPI_REQUEST_NULL;

    return OMPI_SUCCESS;
}

void scops_request_construct(ompi_coll_scops_request_t *req) {
    req->super.req_type = OMPI_REQUEST_COLL;
    req->super.req_status._cancelled = 0;
    req->super.req_start = scops_request_start;
    req->super.req_free = scops_request_free;
    req->super.req_cancel = scops_request_cancel;
}
