##############################################################
#                                                            #
#   libbasarproperty makefile (Solaris)                      #
#	Bjoern Bischof                                           #
#	19.07.2005                                               #
#                                                            #
##############################################################

#-------------------------------------------------------------
#    target
#-------------------------------------------------------------
TARGET_NAME = libbasarproperty.so

#-------------------------------------------------------------
#   flags
#-------------------------------------------------------------
REL_VALUE_PROJ    =
DBG_VALUE_PROJ    =

#-------------------------------------------------------------
#   libraries, lib path, include path
#-------------------------------------------------------------

# shared library paths
LIBS_DIR = -L$(LIBS_BASAR_DIR) \
           -L$(LIBS_LOG4CPLUS_DIR)

# shared libraries
LIBS_BASAR = -lbasardebug -lbasarcmnutil 
LIBS       = -llog4cplus $(LIBS_BASAR) $(LIBS_SYSTEM)

# include paths
INCS = -I. \
       -I$(INCS_GLOBAL) \
       -I$(INCS_BASAR) \
       -I$(INCS_BOOST) \
       -I$(INCS_LOG4CPLUS)

#-------------------------------------------------------------
#    object files 
#-------------------------------------------------------------

PROJ_SRC_LIB =

PROJ_SRC_CPP = algorithm.cpp \
               libbasarproperty.cpp \
               propertydescriptionlist.cpp \
               propertydescriptionlist_iterator.cpp \
               propertydescriptionlistref.cpp \
               propertyinternal.cpp \
               propertystate.cpp \
               propertystateset.cpp \
               propertytable.cpp \
               propertytable_xiterator.cpp \
               propertytable_yiterator.cpp \
               propertytableref.cpp \
               propertytype.cpp \
               propertytablesnapshot.cpp \
               propertytablesnapshotref.cpp \
               propertytable_yiteratorsnapshot.cpp \
               loggerpool.cpp \
               propertydescription.cpp \
               propertydescriptionvector.cpp \
               init.cpp

PROJ_OBJS    = $(PROJ_SRC_CPP:%.cpp=$(RELSUBDIR)/%.o)
PROJ_LIBS    = $(PROJ_SRC_LIB)

#-------------------------------------------------------------
#   include common makefile and dependency file
#-------------------------------------------------------------
include ../makefile.inc
include ./deps
