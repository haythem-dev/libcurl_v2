##############################################################
#                                                            #
#   libbasardbaspect makefile (Solaris)                     #
#	Roland Kiefert                                           #
#	20.07.2005                                               #
#                                                            #
##############################################################

#-------------------------------------------------------------
#    target
#-------------------------------------------------------------
TARGET_NAME = libbasardbaspect.so

#-------------------------------------------------------------
#   flags
#-------------------------------------------------------------
REL_VALUE_PROJ    =
DBG_VALUE_PROJ    =

#-------------------------------------------------------------
#   libraries, lib path, include path
#-------------------------------------------------------------

# shared library paths
LIBS_DIR            = -L$(LIBS_BASAR_DIR) \
                      -L$(LIBS_LOG4CPLUS_DIR)

# shared libraries
LIBS_BASAR         = \
                     -lbasardebug \
                     -lbasarcmnutil \
                     -lbasarproperty \
                     -lbasardbsql
LIBS               = -llog4cplus $(LIBS_BASAR)

# include paths
INCS               = -I. -I$(INCS_GLOBAL) \
                     -I../libbasarproperty \
                     -I$(INCS_BASAR) \
                     -I$(INCS_BOOST) \
                     -I$(INCS_LOG4CPLUS) 

#-------------------------------------------------------------
#    object files 
#-------------------------------------------------------------

PROJ_SRC_LIB =

PROJ_SRC_CPP = accessmethod.cpp \
               accessmethod_iterator.cpp \
               accessmethodlistref.cpp \
               accessmethodref.cpp \
               accessor.cpp \
               accessordefinitionlistref.cpp \
               accessordefinitionlist_iterator.cpp \
               accessordefinitionref.cpp \
               accessorexecuteevent.cpp \
               accessorexecutelistenerref.cpp \
               accessorinstancelistref.cpp \
               accessorinstancelist_iterator.cpp \
               accessorinstanceref.cpp \
               accessorpropertytable_yiterator.cpp \
               accessorpropertytableref.cpp \
               cachecontroller.cpp \
               connection.cpp \
               connectionref.cpp \
               libbasardbaspect.cpp \
               loggerpool.cpp \
               manager.cpp \
               managerimpl.cpp \
			   sqlstringbuilderbaseimpl.cpp \
			   sqlpreparedstringbuilderimpl.cpp \
               sqlstringbuilder.cpp \
               sqlstringbuilderimpl.cpp \
               sqlstringbuilderlistref.cpp \
               sqlstringbuilderlist_iterator.cpp \
               sqlstringbuilderref.cpp \
               cachingpolicy.cpp \
               fullcaching.cpp \
               singlerowcaching.cpp \
               ondemandcaching.cpp \
               nocaching.cpp \
               init.cpp

PROJ_OBJS    = $(PROJ_SRC_CPP:%.cpp=$(RELSUBDIR)/%.o)
PROJ_LIBS    = $(PROJ_SRC_LIB)

#-------------------------------------------------------------
#   include common makefile and dependency file
#-------------------------------------------------------------
include ../makefile.inc
include ./deps
