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
TARGET_NAME = libopensslwrap.so

#-------------------------------------------------------------
#   flags
#-------------------------------------------------------------
REL_VALUE_PROJ    =
DBG_VALUE_PROJ    =

#-------------------------------------------------------------
#   libraries, lib path, include path
#-------------------------------------------------------------

INCS_OPENSSL = $(OPENSSLDIR)/include

# shared library paths
LIBS_DIR            = -L$(LIBS_BASAR_DIR) -L$(LIBS_OPENSSL_DIR)

# shared libraries
LIBS               = -lssl

# include paths
INCS               = -I. -I$(INCS_GLOBAL) -I$(INCS_OPENSSL)

#-------------------------------------------------------------
#    object files
#-------------------------------------------------------------

PROJ_SRC_LIB =

PROJ_SRC_CPP = openssl.cpp

PROJ_OBJS    = $(PROJ_SRC_CPP:%.cpp=$(RELSUBDIR)/%.o)
PROJ_LIBS    = $(PROJ_SRC_LIB)

#-------------------------------------------------------------
#   include common makefile and dependency file
#-------------------------------------------------------------
include ../makefile.inc
include ./deps

