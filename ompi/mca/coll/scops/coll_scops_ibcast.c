/*
 * Copyright (c) 2019 Siberian state university of telecommunications and information sciences. All rights reserved.
 * $COPYRIGHT$
 *
 * $HEADER$
 */

#include "ompi_config.h"
#include "coll_scops.h"

#include "mpi.h"
#include "ompi/constants.h"
#include "opal/util/bit_ops.h"
#include "ompi/datatype/ompi_datatype.h"
#include "ompi/communicator/communicator.h"
#include "ompi/mca/coll/coll.h"
#include "ompi/mca/coll/base/coll_base_functions.h"
#include "ompi/mca/coll/base/coll_tags.h"
#include "ompi/mca/coll/base/coll_base_util.h"
#include "ompi/mca/pml/pml.h"
#include "ompi/op/op.h"

/* returns a no-operation request (e.g. for one process barrier) */
static inline int nbc_get_noop_request(bool persistent, ompi_request_t **request) {
  if (persistent) {
    return ompi_request_persistent_noop_create(request);
  } else {
    *request = &ompi_request_empty;
    return OMPI_SUCCESS;
  }
}

static inline int create_request1(ompi_request_t **request, bool persistent)
{
    int res = nbc_get_noop_request(persistent, request);
    if (OMPI_SUCCESS != res) {
        return OMPI_ERR_OUT_OF_RESOURCE;
    }

    ompi_request_t *handle = *request;
    handle->req_state = OMPI_REQUEST_INACTIVE;
    //handle->req_status.MPI_ERROR = res;
    handle->req_complete = (persistent) ? REQUEST_COMPLETED : REQUEST_PENDING;
    
    //ompi_request_complete_fn_t req_complete_cb; /**< Called when the request is MPI completed */

    return res;
}

static inline int create_request2(ompi_request_t **request, bool persistent)    // segfault
{
    ompi_request_t *handle = calloc(1, sizeof(ompi_request_t));
    if (NULL == handle) {
        return OMPI_ERR_OUT_OF_RESOURCE;
    }
    OMPI_REQUEST_INIT(handle, persistent);
    *request = handle;

    return OMPI_SUCCESS;
}

static inline int create_request3(ompi_request_t **request, bool persistent)    // segfault
{
    ompi_request_t *req;
    req = OBJ_NEW(ompi_request_t);
    if (NULL == req) {
        return OMPI_ERR_OUT_OF_RESOURCE;
    }
    OMPI_REQUEST_INIT(req, persistent);
    *request = req;

    return OMPI_SUCCESS;
}

#define OMPI_COLL_LIBNBC_REQUEST_RETURN(req)                            \
    do {                                                                \
        OMPI_REQUEST_FINI(req);                               \
        opal_free_list_return (&mca_coll_scops_component.requests,     \
                               (opal_free_list_item_t*) (req));         \
    } while (0)

static inline int create_request4(ompi_request_t **request, bool persistent)     // segfault
{
    ompi_request_t *req;
    req = (ompi_request_t *)opal_free_list_wait(&mca_coll_scops_component.requests);
    //OMPI_COLL_SCOPS_REQUEST_ALLOC(persistent, req);
    if (NULL == req) return OMPI_ERR_OUT_OF_RESOURCE;
    OMPI_REQUEST_INIT(req, persistent);
    //req->req_free = ompi_request_empty_free;
    *request = req;

    return OMPI_SUCCESS;
}

static void NBC_Return_handle(ompi_request_t *request) {
  //NBC_Free (request);
  OMPI_COLL_LIBNBC_REQUEST_RETURN(request);
}

static inline int complete_request(ompi_request_t **request) {
    ompi_request_t *handle = *request;
    handle->req_complete = REQUEST_COMPLETED;

    return OMPI_SUCCESS;
}





static int request_return(ompi_request_t **request) {
    ompi_request_t *req = *request;

    OMPI_REQUEST_FINI(req);
    opal_free_list_return(&mca_coll_scops_component.requests, (opal_free_list_item_t*) (req));
    //req = &ompi_request_null.request;

    return OMPI_SUCCESS;
}

static int request_alloc(ompi_request_t **request, bool persistent, struct ompi_communicator_t *comm) {
    ompi_request_t *req;
    
    req = (ompi_request_t *)opal_free_list_wait(&mca_coll_scops_component.requests);
    if (NULL == req) { return OMPI_ERR_OUT_OF_RESOURCE; }
    OMPI_REQUEST_INIT(req, persistent);
    req->req_state = OMPI_REQUEST_ACTIVE;
    req->req_mpi_object.comm = comm;
    req->req_complete = (persistent) ? REQUEST_COMPLETED : REQUEST_PENDING;
    *request = req;

    return OMPI_SUCCESS;
}

static int request_free(ompi_request_t **request) {
    int res = OMPI_SUCCESS;
    if (!REQUEST_COMPLETE(*request)) {
        return MPI_ERR_REQUEST;
    }

    res = request_return(request);

    *request = MPI_REQUEST_NULL;
    return res;
}

static int request_cancel(struct ompi_request_t *request, int complete) {
    return MPI_ERR_REQUEST;
}

static int request_init(ompi_request_t **request, bool persistent, struct ompi_communicator_t *comm) {
    // ompi_request_t *req = *request;
    int res = OMPI_SUCCESS;

    // if (req == &ompi_request_empty) {
    //     return OMPI_SUCCESS;
    // }

    /* request allocation */
    //if (NULL == req || MPI_REQUEST_NULL == req) {   // (request may not be equal NULL)
        if (persistent) {
            res = ompi_request_persistent_noop_create(request);
        } else {
            res = request_alloc(request, persistent, comm);
        }
    //}

    ompi_request_t *req = *request;
    // req->req_state = OMPI_REQUEST_ACTIVE;
    req->req_status.MPI_ERROR = OMPI_SUCCESS;
    req->req_type = OMPI_REQUEST_COLL;
    req->req_free = request_free;
    req->req_cancel = request_cancel;
    // req->req_mpi_object.comm = comm;
    // req->req_complete = (persistent) ? REQUEST_COMPLETED : REQUEST_PENDING;

    return res;
}

static int request_complete(ompi_request_t **request) {
    ompi_request_t *req = *request;
    int res = OMPI_SUCCESS;

    res = ompi_request_complete(req, false);     // just mark request as complete
    // req->req_complete = REQUEST_COMPLETED;

    return OMPI_SUCCESS;
}

static int request_fini(ompi_request_t **request) {
    ompi_request_t *req = *request;

    if (REQUEST_COMPLETE(req)) {
        request_return(request);
        //req = &ompi_request_null.request;
    }

    return OMPI_SUCCESS;
}





int scops_ibcast_intra(
    void *buf, int count, struct ompi_datatype_t *dtype, int root,
    struct ompi_communicator_t *comm, ompi_request_t **request, mca_coll_base_module_t *module)
{
    opal_output_verbose(30, mca_coll_scops_stream, "coll:scops:ibcast:, proc %d/%d count %d",
                        ompi_comm_rank(comm), ompi_comm_size(comm), count);
    printf("  [%d] scops_ibcast_intra\n", ompi_comm_rank(comm));

    bool persistent = false;

    int res = OMPI_SUCCESS;

    res = request_init(request, persistent, comm);
    res = ompi_request_complete(*request, false);
    // res = request_return(request);








    // res = create_request4(request, persistent);
    // ompi_request_complete(*request, false);
    // if (REQUEST_COMPLETE(*request)) {
    //     OMPI_COLL_LIBNBC_REQUEST_RETURN(*request);
    //     *request = &ompi_request_null.request;
    // }


    // MCA_PML_CALL(isend(buf, count, dtype, root, 123,
    //                              MCA_PML_BASE_SEND_STANDARD, comm,
    //                              request));
    // if(!REQUEST_COMPLETE(*request)) {
    //   ompi_request_complete(*request, false);
    // }
    // ompi_request_t *handle = *request;
    // handle->req_status.MPI_ERROR = OMPI_SUCCESS;

    //OMPI_COLL_LIBNBC_REQUEST_RETURN(*request);
    //*request = &ompi_request_null.request;

    // if(!request->super.req_persistent || !REQUEST_COMPLETE(&request->super)) {
    //   ompi_request_complete(&request->super, true);
    // }

    // if (REQUEST_COMPLETE(*request)) {
    //     printf("complete\n");
    // } else {
    //     printf("not complete\n");
    // }
    // OMPI_REQUEST_FINI(*request);

    // ompi_request_t *handle = *request;
    // if (handle->req_state == OMPI_REQUEST_INACTIVE) {
    //     printf("OMPI_REQUEST_INACTIVE\n");
    // } else {
    //     printf("OMPI_REQUEST_ACTIVE\n");
    // }

    // OPAL_THREAD_LOCK(&mca_coll_libnbc_component.lock);
    // opal_list_append(&mca_coll_libnbc_component.active_requests, &(handle->super.super.super));
    // OPAL_THREAD_UNLOCK(&mca_coll_libnbc_component.lock);





    //int ret;

    //ret = nbc_get_noop_request(persistent, request);
    //if (OMPI_SUCCESS != ret) { return OMPI_ERR_OUT_OF_RESOURCE; }
    // ompi_coll_scops_request_t *handle;
    // OMPI_COLL_SCOPS_REQUEST_ALLOC(comm, persistent, handle);
    // handle->comm = comm;
    // *request = (ompi_request_t *) handle;


    //*request = calloc(1, sizeof(ompi_request_t));
    //OBJ_CONSTRUCT(*request, ompi_request_t);
    //ompi_request_construct(*request);
    //ompi_request_t *req = calloc(1, sizeof(ompi_request_t));

    // MCA_PML_CALL(isend(buf, count, dtype, root, 123,
    //                              MCA_PML_BASE_SEND_STANDARD, comm,
    //                              request));

    //OMPI_REQUEST_INIT(req, persistent);
    // *request = req;
    //OMPI_REQUEST_FINI(&(req)->super);
    //ompi_request_complete(*request, true);
    //req->req_complete = REQUEST_COMPLETED;
    //*request = req;

    return res;
}
