######################################################################
# narcpep project file
# Frank Naumann
# 08.02.2012
######################################################################

!include( ../definitions.pri ) {
	message("definitions.pri not found")
}

######################################################################

TEMPLATE		= app
TARGET			= narcpep.bin

INCLUDEPATH		+= $${INCS_BASAR}

#LIBS			+= $${LIBS_DIR_BASAR} -lboost_system -lbasarcmnutil -lbasardbsql -lbasardebug $${LIBS_LOG4CPLUS} 
LIBS			+=  $${LIBS_BASAR} $${LIBS_LOG4CPLUS} $${LIBS_BOOST}
#$${LIBS_TCACCESS}

######################################################################
# in release mode, add install target
######################################################################

build_pass:CONFIG(release, debug|release) {

	target.path	= $$(WSS)/de/narcotics/bin
	INSTALLS	+= target

	ini.files	= narcpep.cfg
	ini.path	= $$(WSS)/de/narcotics/ini
	INSTALLS	+= ini

	proc.files	= narcpep.sh \
				  narcpep.option
	proc.path	= $$(WSS)/de/narcotics/proc
	INSTALLS	+= proc
}

######################################################################
# Input
######################################################################

HEADERS +=
SOURCES += main.cpp \
           narcpep.cpp \
           abbapep.cpp
