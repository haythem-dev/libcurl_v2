##############################################################
#                                                            #
#   libbasardebug makefile (Solaris)                         #
#	Michael Eichenlaub                                       #
#	10.10.2005                                               #
#                                                            #
##############################################################

#-------------------------------------------------------------
#    target
#-------------------------------------------------------------
TARGET_NAME = libbasardebug.so

#-------------------------------------------------------------
#   flags
#-------------------------------------------------------------
REL_VALUE_PROJ    =
DBG_VALUE_PROJ    =

#-------------------------------------------------------------
#   libraries, lib path, include path
#-------------------------------------------------------------

# shared library paths
LIBS_DIR           =

# shared libraries
LIBS               = $(LIBS_SYSTEM)

# include paths
INCS               = -I. -I$(INCS_GLOBAL)

#-------------------------------------------------------------
#    object files 
#-------------------------------------------------------------

PROJ_SRC_LIB =

PROJ_SRC_CPP = libbasardebug.cpp \
               checks.cpp \
               init.cpp


PROJ_OBJS    = $(PROJ_SRC_CPP:%.cpp=$(RELSUBDIR)/%.o)
PROJ_LIBS    = $(PROJ_SRC_LIB)

#-------------------------------------------------------------
#   include common makefile and dependency file
#-------------------------------------------------------------
include ../makefile.inc
include ./deps
