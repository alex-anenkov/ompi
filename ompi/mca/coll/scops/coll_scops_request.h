/*
 * Copyright (c) 2019 Siberian state university of telecommunications and information sciences. All rights reserved.
 * $COPYRIGHT$
 *
 * $HEADER$
 */

#ifndef MCA_COLL_SCOPS_REQUEST_EXPORT_H
#define MCA_COLL_SCOPS_REQUEST_EXPORT_H

#include "coll_scops_component.h"
#include "ompi/request/request.h"
#include "coll_scops_sched.h"

struct ompi_coll_scops_request_t {
    ompi_request_t super;
    struct scops_sched_entry *entries;
};
typedef struct ompi_coll_scops_request_t ompi_coll_scops_request_t;
typedef ompi_coll_scops_request_t SCOPS_Handle;

OBJ_CLASS_DECLARATION(ompi_coll_scops_request_t);

#define OMPI_COLL_SCOPS_REQUEST_ALLOC(comm, persistent, req)           \
    do {                                                                \
        opal_free_list_item_t *item;                                    \
        item = opal_free_list_wait(&mca_coll_scops_component.requests); \
        req = (ompi_coll_scops_request_t*) item;                       \
        OMPI_REQUEST_INIT(&req->super, persistent);                     \
        req->super.req_mpi_object.comm = comm;                          \
        req->super.req_state = OMPI_REQUEST_ACTIVE;                           \
        req->super.req_complete = (persistent) ? REQUEST_COMPLETED : REQUEST_PENDING; \
        req->super.req_persistent = persistent;                         \
        req->entries = NULL;                                            \
    } while (0)

#define OMPI_COLL_SCOPS_REQUEST_RETURN(req)                            \
    do {                                                                \
        OMPI_REQUEST_FINI(&(req)->super);                               \
        opal_free_list_return(&mca_coll_scops_component.requests,     \
                               (opal_free_list_item_t*) (req));         \
    } while (0)



int scops_request_start(size_t count, ompi_request_t **reqs);
int scops_request_cancel(struct ompi_request_t *req, int complete);
int scops_request_free(struct ompi_request_t **req);
void scops_request_construct(ompi_coll_scops_request_t *req);

#endif
