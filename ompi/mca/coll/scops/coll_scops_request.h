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

int scops_request_alloc(ompi_request_t **request, bool persistent, struct ompi_communicator_t *comm);
int scops_request_return(ompi_request_t **request);
// int scops_request_free(ompi_request_t **request);
// int scops_request_cancel(struct ompi_request_t *request, int complete);
int scops_request_init(ompi_request_t **request, bool persistent, struct ompi_communicator_t *comm);
//int scops_request_complete(ompi_request_t **request);

#endif
