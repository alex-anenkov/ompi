/*
 * Copyright (c) 2019 Siberian state university of telecommunications and information sciences. All rights reserved.
 * $COPYRIGHT$
 *
 * $HEADER$
 */

#ifndef MCA_COLL_SCOPS_EXPORT_H
#define MCA_COLL_SCOPS_EXPORT_H

struct scops_sched_entry {
    bool is_barrier;
    // union {
    //     struct scops_sched_send;
    // } op;
    struct scops_sched_entry *next, *prev;
};
typedef struct scops_sched_entry scops_sched_entry;




#endif