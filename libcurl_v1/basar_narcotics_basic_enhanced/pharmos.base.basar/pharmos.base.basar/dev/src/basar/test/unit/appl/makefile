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
TARGET_NAME = utappl

#-------------------------------------------------------------
#    object files 
#-------------------------------------------------------------
PROJ_SRC_CPP  =	main.cpp \
                eventreturnstruct.cpp

PROJ_OBJS     = $(PROJ_SRC_CPP:%.cpp=$(RELSUBDIR)/%.o)

PROJ_LIBS_ARG = \
    -lboost_system -lboost_filesystem \
    -lbasardebug \
    -lbasarcmnutil \
    -lbasarappl

#-------------------------------------------------------------
#   include common makefile and dependency file
#-------------------------------------------------------------
include ../makefile_ut.inc

LIBS_DIR += -L$(LIBS_ZLIB_DIR)

include ./deps

