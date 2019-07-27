/*
 * Copyright (c) 2019 Siberian state university of telecommunications and information sciences. All rights reserved.
 * $COPYRIGHT$
 *
 * $HEADER$
 */

#ifndef MCA_COLL_SCOPS_EXPORT_H
#define MCA_COLL_SCOPS_EXPORT_H

struct scops_sched_send {
    const void *buf;
    int count;
    MPI_Datatype datatype;
    int dest;
    int tag;
    MPI_Comm comm;
    struct ompi_request *sreq;
};

struct scops_sched_recv {
    void *buf;
    int count;
    MPI_Datatype datatype;
    int source;
    int tag;
    MPI_Comm comm;
    struct ompi_request *rreq;
};



struct scops_sched_entry {
    bool is_barrier;
    union {
        struct scops_sched_send send;
        struct scops_sched_recv recv;
    } op;
    struct scops_sched_entry *next, *prev;
};




#endif