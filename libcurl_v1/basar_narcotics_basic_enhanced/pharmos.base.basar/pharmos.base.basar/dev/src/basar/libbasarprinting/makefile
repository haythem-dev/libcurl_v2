##############################################################
#                                                            #
#   libbasarprinting (AIX)				     #
#                                                            #
##############################################################

#-------------------------------------------------------------
#    target
#-------------------------------------------------------------
TARGET_NAME = libbasarprinting.so

#-------------------------------------------------------------
#   flags
#-------------------------------------------------------------
REL_VALUE_PROJ    =
DBG_VALUE_PROJ    =

#-------------------------------------------------------------
#   libraries, lib path, include path
#-------------------------------------------------------------

# shared library paths
LIBS_DIR            = -L$(LIBS_BASAR_DIR) -L$(LIBS_LOG4CPLUS_DIR) -L$(LIBS_BOOST_DIR)

# shared libraries
LIBS_BASAR         = -lbasarcmnutil -lbasarproperty -lbasarlogin -lpthread
LIBS               = -llog4cplus -lboost_filesystem -lboost_system -lboost_thread $(LIBS_BASAR) $(LIBS_SYSTEM)

# include paths
INCS               = -I. -I$(INCS_GLOBAL) -I$(INCS_BASAR) -I../libbasarcmnutil -I$(INCS_BOOST) -I$(INCS_LOG4CPLUS) 

#-------------------------------------------------------------
#    object files 
#-------------------------------------------------------------

PROJ_SRC_LIB =

#PROJ_SRC_CPP = $(wildcard **/*.cpp)

PROJ_SRC_CPP = 	loggerpool.cpp \
				printutils.cpp \
				print/interpretprintol.cpp \
				print/psinterpreterol.cpp

PROJ_RELSUBDIRS = $(RELSUBDIR)\
                  $(RELSUBDIR)/print

PROJ_OBJS    = $(PROJ_SRC_CPP:%.cpp=$(RELSUBDIR)/%.o)
PROJ_LIBS    = $(PROJ_SRC_LIB)

#-------------------------------------------------------------
#   include common makefile and dependency file
#-------------------------------------------------------------
include ../makefile.inc
include ./deps


