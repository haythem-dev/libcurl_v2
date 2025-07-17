##############################################################
#                                                            #
#   makefile (Solaris)                                       #
#	Michael Eichenlaub                                       #
#	09.05.2007                                               #
#                                                            #
##############################################################

#-------------------------------------------------------------
#    target
#-------------------------------------------------------------
TARGET_NAME = utproperty

#-------------------------------------------------------------
#    object files 
#-------------------------------------------------------------
PROJ_SRC_CPP  = algorithm.cpp \
                iterator.cpp \
                propany.cpp \
                propertydescription.cpp \
                propertydescriptionlistref.cpp \
                propertystateset.cpp \
                propertytable.cpp \
                propertytableyiterator.cpp \
                propertydescriptionlist_iterator.cpp \
                loggerpool.cpp \
                main.cpp

PROJ_OBJS     = $(PROJ_SRC_CPP:%.cpp=$(RELSUBDIR)/%.o)
PROJ_LIBS_ARG = \
                -lboost_system -lboost_filesystem \
                -lbasardebug \
                -lbasarcmnutil \
                -lbasarproperty

PROJ_INCS_ARG = -I../../../libbasarproperty

#-------------------------------------------------------------
#   include common makefile and dependency file
#-------------------------------------------------------------
include ../makefile_ut.inc

LIBS_DIR += -L$(LIBS_ZLIB_DIR)

include ./deps

