##############################################################
#                                                            #
#   libbasarappl makefile (Solaris)							 #
#	Michael Eichenlaub                                       #
#	19.08.2006                                               #
#                                                            #
##############################################################

#-------------------------------------------------------------
#    target
#-------------------------------------------------------------
TARGET_NAME = libbasarappl.so

#-------------------------------------------------------------
#   flags
#-------------------------------------------------------------
REL_VALUE_PROJ    =
DBG_VALUE_PROJ    =

#-------------------------------------------------------------
#   libraries, lib path, include path
#-------------------------------------------------------------

# shared library paths
LIBS_DIR            = -L$(LIBS_BASAR_DIR) -L$(LIBS_LOG4CPLUS_DIR)

# shared libraries
LIBS_BASAR         = -lbasarcmnutil -lbasarproperty -lbasardbsql -lbasardbaspect
LIBS               = -llog4cplus $(LIBS_BASAR) $(LIBS_SYSTEM)

# include paths
INCS               = -I. -I$(INCS_GLOBAL) -I$(INCS_BASAR) -I$(INCS_BOOST) -I$(INCS_LOG4CPLUS) 

#-------------------------------------------------------------
#    object files 
#-------------------------------------------------------------

PROJ_SRC_LIB =

PROJ_SRC_CPP = libbasarappl.cpp \
               systemeventmanager.cpp \
               loggerpool.cpp \
               init.cpp

PROJ_OBJS    = $(PROJ_SRC_CPP:%.cpp=$(RELSUBDIR)/%.o)
PROJ_LIBS    = $(PROJ_SRC_LIB)

#-------------------------------------------------------------
#   include common makefile and dependency file
#-------------------------------------------------------------
include ../makefile.inc
include ./deps

