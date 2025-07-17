######################################################################
# narctransfer project file
# Frank Naumann
# 08.02.2012
######################################################################

!include( ../definitions.pri ) {
	message("definitions.pri not found")
}

######################################################################

TEMPLATE		= app
TARGET			= narctransfer.bin

INCLUDEPATH		+= $${INCS_BASAR} ../libnarcotics

LIBS			+= ../libnarcotics/$${DBGDIR}/liblibnarcotics.a
LIBS			+=  $${LIBS_BASAR} $${LIBS_LOG4CPLUS} $${LIBS_TCACCESS} $${LIBS_BOOST}

######################################################################
# in release mode, add install target
######################################################################

build_pass:CONFIG(release, debug|release) {

	target.path	= $$(WSS)/de/narcotics/bin
	INSTALLS	+= target

	ini.files	= narctransfer.cfg
	ini.path	= $$(WSS)/de/narcotics/ini
	INSTALLS	+= ini

	solaris-cc-64-stlport4 {
		xml.files	= basar.config.xml
	}
	aix-g++-64-uf {
		xml.files	=
		xml.extra	= cp -f basar.config.xml_UF $$(WSS)/de/narcotics/bin/basar.config.xml
	}
	xml.path	= $$(WSS)/de/narcotics/bin
	INSTALLS	+= xml

	proc.files	= proc/narctransfer.sh \
				  proc/narctransfer.option \
                  proc/narcreorg.sh \
                  proc/narcreorg.option \
                  proc/diffqtyinstock.sh \
                  proc/diffqtyinstock.option \
                  proc/hyreport.sh \
                  proc/hyreport.option
	proc.path	= $$(WSS)/de/narcotics/proc
	INSTALLS	+= proc

	vars.files	= proc/narcotics_global.vars
	vars.path	= $$(WSS)/de/narcotics/vars
	INSTALLS	+= vars
}

######################################################################
# Input
######################################################################

HEADERS += acc_catalog.h \
           acc_customernomapping.h \
           acc_customersupplier.h \
           acc_narcgroup.h \
           acc_original.h \
           acc_supplier.h \
           acc_transfer.h \
           commonheader.h \
           connection.h \
           loggerpool.h \
           narctransfer_definitions.h \
           narctransferappl.h \
           parameter.h \
           transferdm.h \
           transferuc.h \
           usecasecontroller.h \
           version.h
SOURCES += acc_catalog.cpp \
           acc_customernomapping.cpp \
           acc_customersupplier.cpp \
           acc_narcgroup.cpp \
           acc_original.cpp \
           acc_supplier.cpp \
           acc_transfer.cpp \
           connection.cpp \
           loggerpool.cpp \
           main.cpp \
           narctransferappl.cpp \
           parameter.cpp \
           transferdm.cpp \
           transferuc.cpp \
           usecasecontroller.cpp
