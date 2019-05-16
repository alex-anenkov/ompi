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

int request_alloc(ompi_request_t **request, bool persistent, struct ompi_communicator_t *comm);
int request_return(ompi_request_t **request);
int request_free(ompi_request_t **request);
int request_cancel(struct ompi_request_t *request, int complete);
int request_init(ompi_request_t **request, bool persistent, struct ompi_communicator_t *comm);
//int request_complete(ompi_request_t **request);

#endif
