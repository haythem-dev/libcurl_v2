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
TARGET_NAME = utcmnutil


#-------------------------------------------------------------
#    object files 
#-------------------------------------------------------------
#				remotetasktest.cpp \

PROJ_SRC_CPP  =	config.cpp \
				date.cpp \
				datetime.cpp \
				datetimetools.cpp \
				decimal.cpp \
				endian.cpp \
				exception.cpp \
				exceptionfixture.cpp \
				gsprint.cpp \
				locale.cpp \
				logging.cpp \
				main.cpp \
				parameterlist.cpp \
				remotetasktest.cpp \
				singleton.cpp \
				socket.cpp \
				smtp.cpp \
				string.cpp \
				stringappender.cpp \
				testfixturebase.cpp \
				timespan.cpp \
				utils.cpp

PROJ_OBJS     = $(PROJ_SRC_CPP:%.cpp=$(RELSUBDIR)/%.o)
PROJ_LIBS_ARG = \
                -lboost_system -lboost_filesystem \
                -lbasardebug \
                -lbasarcmnutil

#-------------------------------------------------------------
#   include common makefile and dependency file
#-------------------------------------------------------------
include ../makefile_ut.inc

LIBS_DIR += -L$(LIBS_ZLIB_DIR)

include ./deps

