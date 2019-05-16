#
# Copyright (c) 2019      Siberian state university of telecommunications and information sciences. All rights reserved.
# $COPYRIGHT$
#
# Additional copyrights may follow
#
# $HEADER$
#

sources = \
        coll_scops.h \
        coll_scops.c \
        coll_scops_component.c \
        coll_scops_module.c \
        coll_scops_ibcast.c

# Make the output library in this directory, and name it either
# mca_<type>_<name>.la (for DSO builds) or libmca_<type>_<name>.la
# (for static builds).

if MCA_BUILD_ompi_coll_scops_DSO
component_noinst =
component_install = mca_coll_scops.la
else
component_noinst = libmca_coll_scops.la
component_install =
endif

mcacomponentdir = $(ompilibdir)
mcacomponent_LTLIBRARIES = $(component_install)
mca_coll_scops_la_SOURCES = $(sources)
mca_coll_scops_la_LDFLAGS = -module -avoid-version
mca_coll_scops_la_LIBADD = $(top_builddir)/ompi/lib@OMPI_LIBMPI_NAME@.la

noinst_LTLIBRARIES = $(component_noinst)
libmca_coll_scops_la_SOURCES =$(sources)
libmca_coll_scops_la_LDFLAGS = -module -avoid-version
