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
TARGET_NAME = utlogin

#-------------------------------------------------------------
#    object files 
#-------------------------------------------------------------
PROJ_SRC_CPP  =	backup.cpp \
				config.cpp \
				nodlg.cpp \
                main.cpp
				
PROJ_DEP      = 

PROJ_OBJS     = $(PROJ_SRC_CPP:%.cpp=$(RELSUBDIR)/%.o)
PROJ_LIBS_ARG = \
                -lboost_system -lboost_filesystem \
                -lbasardebug \
                -lbasarcmnutil \
                -lbasardbsql \
                -lbasarlogin

#-------------------------------------------------------------
#   include common makefile and dependency file
#-------------------------------------------------------------
include ../makefile_ut.inc
include ./deps

#-------------------------------------------------------------
