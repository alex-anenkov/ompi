/*
 * Copyright (c) 2019 Siberian state university of telecommunications and information sciences. All rights reserved.
 * $COPYRIGHT$
 *
 * $HEADER$
 */

#include "coll_scops_request.h"

int request_alloc(ompi_request_t **request, bool persistent, struct ompi_communicator_t *comm) {
    ompi_request_t *req;
    
    req = (ompi_request_t *)opal_free_list_wait(&mca_coll_scops_component.requests);
    if (NULL == req) { return OMPI_ERR_OUT_OF_RESOURCE; }
    OMPI_REQUEST_INIT(req, persistent);
    req->req_state = OMPI_REQUEST_ACTIVE;
    req->req_mpi_object.comm = comm;
    req->req_complete = (persistent) ? REQUEST_COMPLETED : REQUEST_PENDING;
    req->req_persistent = persistent;
    *request = req;

    return OMPI_SUCCESS;
}

int request_return(ompi_request_t **request) {
    ompi_request_t *req = *request;

    OMPI_REQUEST_FINI(req);
    opal_free_list_return(&mca_coll_scops_component.requests, (opal_free_list_item_t*) (req));
    //req = &ompi_request_null.request;

    return OMPI_SUCCESS;
}

int request_free(ompi_request_t **request) {
    int res = OMPI_SUCCESS;
    if (!REQUEST_COMPLETE(*request)) {
        return MPI_ERR_REQUEST;
    }

    res = request_return(request);

    //*request = MPI_REQUEST_NULL;
    *request = &ompi_request_null.request;
    return res;
}

int request_cancel(struct ompi_request_t *request, int complete) {
    return MPI_ERR_REQUEST;
}

int request_init(ompi_request_t **request, bool persistent, struct ompi_communicator_t *comm) {
    int res = OMPI_SUCCESS;

    res = request_alloc(request, persistent, comm);

    ompi_request_t *req = *request;
    req->req_status.MPI_ERROR = OMPI_SUCCESS;
    req->req_type = OMPI_REQUEST_COLL;
    req->req_free = request_free;
    req->req_cancel = request_cancel;

    return res;
}

// int request_complete(ompi_request_t **request) {
//     ompi_request_t *req = *request;
//     int res = OMPI_SUCCESS;

//     res = ompi_request_complete(req, false);     // just mark request as complete
//     // req->req_complete = REQUEST_COMPLETED;

//     return OMPI_SUCCESS;
// }
