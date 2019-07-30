/*
 * Copyright (c) 2019 Siberian state university of telecommunications and information sciences. All rights reserved.
 * $COPYRIGHT$
 *
 * $HEADER$
 */

#include "ompi_config.h"

#include "mpi.h"
#include "ompi/communicator/communicator.h"
#include "ompi/mca/coll/base/base.h"
#include "ompi/mca/coll/base/coll_base_functions.h"
#include "ompi/mca/coll/coll.h"
#include "ompi/mca/bml/base/base.h"
#include "coll_scops.h"

static int scops_module_enable(mca_coll_base_module_t *module,
                               struct ompi_communicator_t *comm);

/*
 * Initial query function that is invoked during MPI_INIT, allowing
 * this component to disqualify itself if it doesn't support the
 * required level of thread support.
 */
int mca_coll_scops_init_query(bool enable_progress_threads,
                              bool enable_mpi_threads)
{
    return OMPI_SUCCESS;
}

/*
 * Invoked when there's a new communicator that has been created.
 * Look at the communicator and decide which set of functions and
 * priority we want to return.
 */
mca_coll_base_module_t *mca_coll_scops_comm_query(
    struct ompi_communicator_t *comm, int *priority)
{
    *priority = mca_coll_scops_priority;

    if (mca_coll_scops_priority <= 0) {
        opal_output_verbose(20, mca_coll_scops_stream, "coll:scops:module_comm_query: too low priority %d",
                            mca_coll_scops_priority);
        return NULL;
    }

    if (OMPI_COMM_IS_INTER(comm)) {
        opal_output_verbose(20, mca_coll_scops_stream,
                            "coll:scops:module_comm_query: scops does not support inter-communicators");
        *priority = 0;
        return NULL;
    }

    mca_coll_scops_module_t *scops_module = OBJ_NEW(mca_coll_scops_module_t);
    if (NULL == scops_module) {
        return NULL;
    }

    scops_module->super.coll_module_enable = scops_module_enable;
    scops_module->super.ft_event = NULL;

    scops_module->super.coll_ibcast      = scops_ibcast_intra;

    scops_module->comm = comm;

    return &(scops_module->super);
}

/*
 * Init module on the communicator
 */
static int scops_module_enable(mca_coll_base_module_t *module,
                               struct ompi_communicator_t *comm)
{
    mca_coll_scops_module_t *scops_module = (mca_coll_scops_module_t *)module;

    /* prepare the placeholder for the array of request* */
    mca_coll_base_comm_t *data = OBJ_NEW(mca_coll_base_comm_t);
    if (NULL == data) {
        return OMPI_ERROR;
    }
    scops_module->super.base_data = data;

    return OMPI_SUCCESS;
}

static void mca_coll_scops_module_construct(mca_coll_scops_module_t *module)
{
    module->comm = MPI_COMM_NULL;
}

static void mca_coll_scops_module_destruct(mca_coll_scops_module_t *module)
{
    /* cleanup */
}

OBJ_CLASS_INSTANCE(mca_coll_scops_module_t, mca_coll_base_module_t,
                   mca_coll_scops_module_construct,
                   mca_coll_scops_module_destruct);
