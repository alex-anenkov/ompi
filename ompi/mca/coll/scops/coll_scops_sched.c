/*
 * Copyright (c) 2019 Siberian state university of telecommunications and information sciences. All rights reserved.
 * $COPYRIGHT$
 *
 * $HEADER$
 */

#include "coll_scops_sched.h"

int scops_push_to_schedule(SCOPS_Handle handle, void *data) {
    int err = OMPI_SUCCESS;
    
    return err;
}

int scops_sched_send(SCOPS_Handle handle, const void *buf, int count,
                     MPI_Datatype datatype, int dest, int tag, MPI_Comm) {
    int err = OMPI_SUCCESS;

    struct scops_sched_send send;
    send.buf = buf;
    send.count = count;
    send.datatype = datatype;
    send.dest = dest;
    send.tag = tag;

    scops_push_to_schedule(handle, (void*)&send);

    return err;
}
