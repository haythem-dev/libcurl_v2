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
TARGET		= transfer2bfarm.bin
INCLUDEPATH	+= $${INCS_BASAR}
INCLUDEPATH	+= ../libnarcotics
LIBS		+= ../libnarcotics/$${DBGDIR}/liblibnarcotics.a
LIBS		+= $${LIBS_BASAR} $${LIBS_LOG4CPLUS} $${LIBS_TCACCESS}

######################################################################
# in release mode, add install target
######################################################################

build_pass:CONFIG(release, debug|release) {

	target.path	= $$(WSS)/de/narcotics/bin
	INSTALLS	+= target

	ini.files	= ini/transfer2bfarm.cfg
	ini.path	= $$(WSS)/de/narcotics/ini
	INSTALLS	+= ini

	proc.files	= proc/transfer2bfarm.sh \
				  proc/transfer2bfarm.option
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


HEADERS += archivesvc.h \
   archivesvcfactory.h \
   archivevouchersuc.h \
   bfarmservice.h \
   bgabranch.h \
   bgabranch_acc.h \
   bgabranchptr.h \
   bgabranchrepo.h \
   bgafolder.h \
   bgafolderptr.h \
   bgafolderrepo.h \
   coldmetadata.h \
   coldmetadatawriter.h \
   commonheader.h \
   configuration.h \
   contributionvoucherentry.h \
   contributionvoucherentryptr.h \
   contributionvoucherentryrepo.h \
   contributionvoucherfile.h \
   contributionvoucherfileptr.h \
   convert.h \
   crosscheckcontributionvouchersuc.h \
   crosschecksvc.h \
   crosschecksvcfactory.h \
   dateinfo.h \
   definitions.h \
   exbgabranchrepo.h \
   exbgafolder.h \
   exconfiguration.h \
   excrosschecksvc.h \
   excrosscheckuc.h \
   exservice.h \
   exstartdatecalculator.h \
   extransfer.h \
   exvoucherentryrepo.h \
   exvoucherfile.h \
   iconfiguration.h \
   infxconnection.h \
   loggerpool.h \
   metadata.h \
   metadatawriter.h \
   metadatawriterptr.h \
   repobase.h \
   serviceconfiguration.h \
   servicefactory.h \
   startenddatecalculator.h \
   transferbfarmappl.h \
   transfertobfarmuc.h \
   usecasecontroler.h \
   version.h \
   voucherentry_acc.h

SOURCES += archivesvc.cpp \
   archivesvcfactory.cpp \
   archivevouchersuc.cpp \
   bfarmservice.cpp \
   bgabranch.cpp \
   bgabranch_acc.cpp \
   bgabranchrepo.cpp \
   bgafolder.cpp \
   bgafolderrepo.cpp \
   coldmetadata.cpp \
   coldmetadatawriter.cpp \
   configuration.cpp \
   contributionvoucherentry.cpp \
   contributionvoucherentryrepo.cpp \
   contributionvoucherfile.cpp \
   convert.cpp \
   crosscheckcontributionvouchersuc.cpp \
   crosschecksvc.cpp \
   crosschecksvcfactory.cpp \
   dateinfo.cpp \
   exbgabranchrepo.cpp \
   exbgafolder.cpp \
   exconfiguration.cpp \
   excrosschecksvc.cpp \
   excrosscheckuc.cpp \
   exservice.cpp \
   exstartdatecalculator.cpp \
   extransfer.cpp \
   exvoucherentryrepo.cpp \
   exvoucherfile.cpp \
   infxconnection.cpp \
   loggerpool.cpp \
   main.cpp \
   metadata.cpp \
   metadatawriter.cpp \
   servicefactory.cpp \
   startenddatecalculator.cpp \
   transferbfarmappl.cpp \
   transfertobfarmuc.cpp \
   usecasecontroler.cpp \
   voucherentry_acc.cpp \
   version.c \
   gitversioninfo.cpp



