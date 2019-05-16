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

int scops_ibcast_intra(
    void *buf, int count, struct ompi_datatype_t *dtype, int root,
    struct ompi_communicator_t *comm, ompi_request_t **request, mca_coll_base_module_t *module)
{
    opal_output_verbose(30, mca_coll_scops_stream, "coll:scops:ibcast:, proc %d/%d count %d",
                        ompi_comm_rank(comm), ompi_comm_size(comm), count);
    printf("  [%d] scops_ibcast_intra\n", ompi_comm_rank(comm));

    bool persistent = false;

    int res = OMPI_SUCCESS;

    res = scops_request_init(request, persistent, comm);
    res = ompi_request_complete(*request, true);
    if (OMPI_SUCCESS != res) {
        scops_request_return(request);
    }

    return res;
}
