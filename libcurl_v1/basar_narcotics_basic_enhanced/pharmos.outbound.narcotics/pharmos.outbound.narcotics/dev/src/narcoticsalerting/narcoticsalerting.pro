######################################################################
# narctransfer project file
# Frank Naumann
# 08.02.2012
######################################################################

!include( ../definitions.pri ) {
	message("definitions.pri not found")
}

######################################################################

TEMPLATE	= app
TARGET		= narcoticsalerting.bin
INCLUDEPATH	+= $${INCS_BASAR}
LIBS		+= $${LIBS_BASAR} $${LIBS_LOG4CPLUS} $${LIBS_TCACCESS}

######################################################################
# in release mode, add install target
######################################################################

build_pass:CONFIG(release, debug|release) {

	target.path	= $$(WSS)/de/narcotics/bin
	INSTALLS	+= target

	ini.files	= ini/narcoticsalerting.cfg
	ini.path	= $$(WSS)/de/narcotics/ini
	INSTALLS	+= ini

	proc.files	= proc/narcoticsalerting.sh \
				  proc/narcoticsalerting.option
	proc.path	= $$(WSS)/de/narcotics/proc
	INSTALLS	+= proc
}

######################################################################
# Input
######################################################################

GitVersionInfo.target = gitversioninfo.cpp
GitVersionInfo.depends = FORCE
GitVersionInfo.commands = gitversioninfo.sh
PRE_TARGETDEPS += gitversioninfo.cpp
QMAKE_EXTRA_TARGETS += GitVersionInfo


HEADERS += ../transfer2bfarm/bfarmservice.h \
   ../transfer2bfarm/bgabranch.h \
   ../transfer2bfarm/bgabranch_acc.h \
   ../transfer2bfarm/bgabranchptr.h \
   ../transfer2bfarm/bgabranchrepo.h \
   ../transfer2bfarm/bgafolder.h \
   ../transfer2bfarm/bgafolderptr.h \
   ../transfer2bfarm/bgafolderrepo.h \
   commonheader.h \
   ../transfer2bfarm/configuration.h \
   ../transfer2bfarm/contributionvoucherentry.h \
   ../transfer2bfarm/contributionvoucherentryptr.h \
   ../transfer2bfarm/contributionvoucherentryrepo.h \
   ../transfer2bfarm/contributionvoucherfile.h \
   ../transfer2bfarm/contributionvoucherfileptr.h \
   ../transfer2bfarm/convert.h \
   ../transfer2bfarm/crosscheckcontributionvouchersuc.h \
   ../transfer2bfarm/crosschecksvc.h \
   ../transfer2bfarm/crosschecksvcfactory.h \
   ../transfer2bfarm/dateinfo.h \
   ../transfer2bfarm/definitions.h \
   ../transfer2bfarm/exbgabranchrepo.h \
   ../transfer2bfarm/exbgafolder.h \
   ../transfer2bfarm/exconfiguration.h \
   ../transfer2bfarm/excrosschecksvc.h \
   ../transfer2bfarm/excrosscheckuc.h \
   ../transfer2bfarm/exservice.h \
   ../transfer2bfarm/exstartdatecalculator.h \
   ../transfer2bfarm/extransfer.h \
   ../transfer2bfarm/exvoucherentryrepo.h \
   ../transfer2bfarm/exvoucherfile.h \
   ../transfer2bfarm/iconfiguration.h \
   ../transfer2bfarm/infxconnection.h \
   ../transfer2bfarm/loggerpool.h \
   narcoticsalertingapp.h \
   ../transfer2bfarm/serviceconfiguration.h \
   ../transfer2bfarm/servicefactory.h \
   ../transfer2bfarm/startenddatecalculator.h \
   usecasecontroler.h \
   version.h \
   ../transfer2bfarm/voucherentry_acc.h

SOURCES += ../transfer2bfarm/bfarmservice.cpp \
   ../transfer2bfarm/bgabranch.cpp \
   ../transfer2bfarm/bgabranch_acc.cpp \
   ../transfer2bfarm/bgabranchrepo.cpp \
   ../transfer2bfarm/bgafolder.cpp \
   ../transfer2bfarm/bgafolderrepo.cpp \
   ../transfer2bfarm/configuration.cpp \
   ../transfer2bfarm/contributionvoucherentry.cpp \
   ../transfer2bfarm/contributionvoucherentryrepo.cpp \
   ../transfer2bfarm/contributionvoucherfile.cpp \
   ../transfer2bfarm/convert.cpp \
   ../transfer2bfarm/crosscheckcontributionvouchersuc.cpp \
   ../transfer2bfarm/crosschecksvc.cpp \
   ../transfer2bfarm/crosschecksvcfactory.cpp \
   ../transfer2bfarm/dateinfo.cpp \
   ../transfer2bfarm/exbgabranchrepo.cpp \
   ../transfer2bfarm/exbgafolder.cpp \
   ../transfer2bfarm/exconfiguration.cpp \
   ../transfer2bfarm/excrosschecksvc.cpp \
   ../transfer2bfarm/excrosscheckuc.cpp \
   ../transfer2bfarm/exservice.cpp \
   ../transfer2bfarm/exstartdatecalculator.cpp \
   ../transfer2bfarm/extransfer.cpp \
   ../transfer2bfarm/exvoucherentryrepo.cpp \
   ../transfer2bfarm/exvoucherfile.cpp \
   ../transfer2bfarm/infxconnection.cpp \
   ../transfer2bfarm/loggerpool.cpp \
   main.cpp \
   narcoticsalertingapp.cpp \
   ../transfer2bfarm/servicefactory.cpp \
   ../transfer2bfarm/startenddatecalculator.cpp \
   usecasecontroler.cpp \
   ../transfer2bfarm/voucherentry_acc.cpp \
   version.c \
   gitversioninfo.cpp



