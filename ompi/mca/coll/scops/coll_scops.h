/*
 * Copyright (c) 2019 Siberian state university of telecommunications and information sciences. All rights reserved.
 * $COPYRIGHT$
 *
 * $HEADER$
 */

#ifndef MCA_COLL_SCOPS_EXPORT_H
#define MCA_COLL_SCOPS_EXPORT_H

#include "ompi_config.h"

#include "mpi.h"
#include "ompi/mca/coll/coll.h"
#include "ompi/request/request.h"

BEGIN_C_DECLS

/* Globally exported variables */
extern int mca_coll_scops_stream;
extern int mca_coll_scops_priority;
extern int mca_coll_scops_verbose;

/* API functions */

int mca_coll_scops_init_query(bool enable_progress_threads,
                              bool enable_mpi_threads);
mca_coll_base_module_t
*mca_coll_scops_comm_query(struct ompi_communicator_t *comm, int *priority);

int mca_coll_scops_module_enable(mca_coll_base_module_t *module,
                                 struct ompi_communicator_t *comm);

/*
 * coll API functions
 */

/* API functions */

int mca_coll_scops_init_query(bool enable_progress_threads,
                              bool enable_mpi_threads);

mca_coll_base_module_t *
mca_coll_scops_comm_query(struct ompi_communicator_t *comm, int *priority);

struct mca_coll_scops_component_t {
    /* Base coll component */
    mca_coll_base_component_2_0_0_t super;

    /* MCA parameter: priority of this component */
    int scops_priority;

    /* global stuff that I need the component to store */

    /* MCA parameters first */

    opal_free_list_t requests;
};

/*
 * Convenience typedef
 */
typedef struct mca_coll_scops_component_t mca_coll_scops_component_t;

/*
 * Global component instance
 */
OMPI_MODULE_DECLSPEC extern mca_coll_scops_component_t mca_coll_scops_component;

struct mca_coll_scops_module_t {
    mca_coll_base_module_t super;

    /* Module communicator */
    struct ompi_communicator_t *comm;
};
typedef struct mca_coll_scops_module_t mca_coll_scops_module_t;
OBJ_CLASS_DECLARATION(mca_coll_scops_module_t);

int scops_ibcast_intra(
    void *buf, int count, struct ompi_datatype_t *dtype, int root,
    struct ompi_communicator_t *comm, ompi_request_t **request, mca_coll_base_module_t *module);


#endif  /* MCA_COLL_SCOPS_EXPORT_H */
