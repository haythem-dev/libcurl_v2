######################################################################
# libnarcotics project file
# Frank Naumann
# 08.02.2012
######################################################################

!include( ../definitions.pri ) {
	message("definitions.pri not found")
}

######################################################################

TEMPLATE	= lib
TARGET		= libnarcotics
CONFIG		+= staticlib
INCLUDEPATH	+= $${INCS_BASAR}

######################################################################
# Input
######################################################################

HEADERS += lockmanager/branchlockmanager.h \
           commonheader.h \
           lockmanager/istatements.h \
           lockmanager/lockmanager.h \
           lockmanager/statements.h \
           loggerpool/loggerpool.h \
           numberassignmentdm/numberacc.h \
           numberassignmentdm/numberassignmentdm.h \
           numberassignmentdm/numberassignmentdm_definitions.h

SOURCES += lockmanager/branchlockmanager.cpp \
           lockmanager/lockmanager.cpp \
           lockmanager/statements.cpp \
           loggerpool/loggerpool.cpp \
           numberassignmentdm/numberacc.cpp \
           numberassignmentdm/numberassignmentdm.cpp
