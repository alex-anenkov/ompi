/*
 * Copyright (c) 2019 Siberian state university of telecommunications and information sciences. All rights reserved.
 * $COPYRIGHT$
 *
 * $HEADER$
 */

#include "ompi_config.h"

#include "mpi.h"
#include "ompi/mca/coll/coll.h"

#include "coll_scops.h"

/*
 * Public string showing the coll ompi_scops component version number
 */
const char *mca_coll_scops_component_version_string =
    "Open MPI SCOPS collective MCA component version " OMPI_VERSION;

/*
 * Global variable
 */
int mca_coll_scops_priority = 5;
int mca_coll_scops_stream = -1;
int mca_coll_scops_verbose = 0;

static int scops_in_progress = false;

/*
 * Local function
 */
static int scops_register(void);
static int scops_open(void);
static int scops_close(void);
static int ompi_coll_scops_progress(void);

/*
 * Instantiate the public struct with all of our public information
 * and pointers to our public functions in it
 */
mca_coll_scops_component_t mca_coll_scops_component = {
    /* First, fill in the super */
    {
        /* First, the mca_component_t struct containing meta information
           about the component itself */
        .collm_version = {
            MCA_COLL_BASE_VERSION_2_0_0,

            /* Component name and version */
            .mca_component_name = "scops",
            MCA_BASE_MAKE_VERSION(component, OMPI_MAJOR_VERSION, OMPI_MINOR_VERSION,
                                  OMPI_RELEASE_VERSION),

            /* Component open and close functions */
            .mca_open_component = scops_open,
            .mca_close_component = scops_close,
            .mca_register_component_params = scops_register,
        },
        .collm_data = {
            /* The component is not checkpoint ready */
            MCA_BASE_METADATA_PARAM_NONE
        },

        /* Initialization / querying functions */
        .collm_init_query = mca_coll_scops_init_query,
        .collm_comm_query = mca_coll_scops_comm_query,
    }
};

static int scops_register(void)
{
    /* Priority must be greater than priority of basic component! */
    mca_coll_scops_priority = 80;
    (void)mca_base_component_var_register(&mca_coll_scops_component.super.collm_version,
                                          "priority", "Priority of the scops coll component",
                                          MCA_BASE_VAR_TYPE_INT, NULL, 0, 0,
                                          OPAL_INFO_LVL_9,
                                          MCA_BASE_VAR_SCOPE_READONLY,
                                          &mca_coll_scops_priority);

    (void)mca_base_component_var_register(&mca_coll_scops_component.super.collm_version,
                                          "verbose", "Verbose level of the scops coll component",
                                          MCA_BASE_VAR_TYPE_INT, NULL, 0, 0,
                                          OPAL_INFO_LVL_9,
                                          MCA_BASE_VAR_SCOPE_READONLY,
                                          &mca_coll_scops_verbose);

    return OMPI_SUCCESS;
}

static int scops_open(void)
{
    mca_coll_scops_stream = opal_output_open(NULL);
    opal_output_set_verbosity(mca_coll_scops_stream, mca_coll_scops_verbose);

    int res;
    OBJ_CONSTRUCT(&mca_coll_scops_component.requests, opal_free_list_t);
    OBJ_CONSTRUCT(&mca_coll_scops_component.active_requests, opal_list_t);
    OBJ_CONSTRUCT(&mca_coll_scops_component.lock, opal_mutex_t);
    res = opal_free_list_init(&mca_coll_scops_component.requests,
                               sizeof(ompi_coll_scops_request_t), 8,
                               OBJ_CLASS(ompi_coll_scops_request_t),
                               0, 0, 0, -1, 8, NULL, 0, NULL, NULL, NULL);
    if (OMPI_SUCCESS != res) { return res; }

    res = opal_progress_register(ompi_coll_scops_progress);
    if (OPAL_SUCCESS != res) { return res; }

    opal_output_verbose(30, mca_coll_scops_stream, "coll:scops:component_open: done");
    return OMPI_SUCCESS;
}

static int scops_close(void)
{
    opal_progress_unregister(ompi_coll_scops_progress);
    
    OBJ_DESTRUCT(&mca_coll_scops_component.lock);
    OBJ_DESTRUCT(&mca_coll_scops_component.active_requests);
    OBJ_DESTRUCT(&mca_coll_scops_component.requests);

    opal_output_verbose(30, mca_coll_scops_stream, "coll:scops:component_close: done");
    return OMPI_SUCCESS;
}

static int ompi_coll_scops_progress(void)
{
    ompi_coll_scops_request_t *request, *next;

    OPAL_THREAD_LOCK(&mca_coll_scops_component.lock);

    if (!scops_in_progress) {
        scops_in_progress = true;

        OPAL_LIST_FOREACH_SAFE(request, next, &mca_coll_scops_component.active_requests, ompi_coll_scops_request_t) {
            OPAL_THREAD_UNLOCK(&mca_coll_scops_component.lock);

            scops_progress(request);

            if (REQUEST_COMPLETE(&request->super)) {
                OPAL_THREAD_LOCK(&mca_coll_scops_component.lock);
                opal_list_remove_item(&mca_coll_scops_component.active_requests, &request->super.super.super);
                OPAL_THREAD_UNLOCK(&mca_coll_scops_component.lock);
            }

            OPAL_THREAD_LOCK(&mca_coll_scops_component.lock);
        }

        scops_in_progress = false;
    }

    OPAL_THREAD_UNLOCK(&mca_coll_scops_component.lock);

    return 0;
}


OBJ_CLASS_INSTANCE(ompi_coll_scops_request_t, ompi_request_t, scops_request_construct, NULL);
