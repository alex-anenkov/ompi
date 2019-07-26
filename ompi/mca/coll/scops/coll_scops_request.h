/*
 * Copyright (c) 2019 Siberian state university of telecommunications and information sciences. All rights reserved.
 * $COPYRIGHT$
 *
 * $HEADER$
 */

#ifndef MCA_COLL_SCOPS_REQUEST_EXPORT_H
#define MCA_COLL_SCOPS_REQUEST_EXPORT_H

#include "coll_scops.h"
#include "ompi/request/request.h"

enum ompi_coll_scops_request_type_t {
    OMPI_COLL_SCOPS_TYPE_BARRIER,
    OMPI_COLL_SCOPS_TYPE_BCAST
};
typedef enum ompi_coll_scops_request_type_t ompi_coll_scops_request_type_t;

struct ompi_coll_scops_request_t {
    ompi_request_t super;
    ompi_coll_scops_request_type_t type;
};
typedef struct ompi_coll_scops_request_t ompi_coll_scops_request_t;

OBJ_CLASS_DECLARATION(ompi_coll_scops_request_t);


#define OMPI_COLL_SCOPS_REQUEST_ALLOC(comm, req)                       \
    do {                                                                  \
        opal_free_list_item_t *item;                                      \
        item = opal_free_list_get(&mca_coll_scops_component.requests); \
                req = (ompi_coll_scops_request_t*) item;               \
                OMPI_REQUEST_INIT(&req->super, false);                    \
                req->super.req_mpi_object.comm = comm;                    \
                req->super.req_complete = false;                          \
                req->super.req_state = OMPI_REQUEST_ACTIVE;               \
    } while (0)

#define OMPI_COLL_SCOPS_REQUEST_RETURN(req)                       \
    do {                                                             \
        OMPI_REQUEST_FINI(&request->super);                          \
        opal_free_list_return(&mca_coll_scops_component.requests, \
                (opal_free_list_item_t*) req);                       \
    } while (0)

















int scops_request_alloc(ompi_request_t **request, bool persistent, struct ompi_communicator_t *comm);
int scops_request_return(ompi_request_t **request);
// int scops_request_free(ompi_request_t **request);
// int scops_request_cancel(struct ompi_request_t *request, int complete);
int scops_request_init(ompi_request_t **request, bool persistent, struct ompi_communicator_t *comm);
//int scops_request_complete(ompi_request_t **request);

#endif
