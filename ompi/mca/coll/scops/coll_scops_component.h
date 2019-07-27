/*
 * Copyright (c) 2019 Siberian state university of telecommunications and information sciences. All rights reserved.
 * $COPYRIGHT$
 *
 * $HEADER$
 */

#ifndef MCA_COLL_SCOPS_COMPONENT_EXPORT_H
#define MCA_COLL_SCOPS_COMPONENT_EXPORT_H

#include "ompi_config.h"

#include "mpi.h"
#include "ompi/mca/coll/coll.h"

struct mca_coll_scops_component_t {
    /* Base coll component */
    mca_coll_base_component_2_0_0_t super;

    /* MCA parameter: priority of this component */
    int scops_priority;

    /* global stuff that I need the component to store */

    /* MCA parameters first */

    opal_free_list_t requests;
    opal_list_t active_requests;
    opal_mutex_t lock;              /* protect access to the active_requests list */
};

/*
 * Convenience typedef
 */
typedef struct mca_coll_scops_component_t mca_coll_scops_component_t;

/*
 * Global component instance
 */
OMPI_MODULE_DECLSPEC extern mca_coll_scops_component_t mca_coll_scops_component;

#endif
