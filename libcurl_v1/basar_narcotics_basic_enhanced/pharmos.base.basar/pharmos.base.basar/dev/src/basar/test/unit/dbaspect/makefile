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
TARGET_NAME = utdbaspect

#-------------------------------------------------------------
#    object files 
#-------------------------------------------------------------
PROJ_SRC_CPP = \
          loggerpool.cpp \
          testconnection.cpp \
          butprocs.cpp \
          buttablereader.cpp \
          cicsvsamdkpar_ut.cpp \
          testdelete.cpp \
          testfixedstrings.cpp \
          testi18n.cpp \
		  testpreparedsqlbuilder.cpp \
          testsqlbuilder.cpp \
          testsqlbuilderensureparameters.cpp \
          testsqlinsertupdate.cpp \
          testsqlresolvequeries.cpp \
          testaccessorholder.cpp \
          testaccessorexecutelistener.cpp \
          accessordefinitionlist.cpp \
          accessorpropertytableref.cpp \
          accinstlist.cpp \
          accmethod.cpp \
          columnnames.cpp \
          exceptions.cpp \
          fixedstrings.cpp \
          i18n.cpp \
		  infxint8.cpp \
		  infxint8_builder.cpp \
		  infxsimpleblob.cpp \
		  infxsimpleblob_builder.cpp \
          iterator.cpp \
          manager.cpp \
          odbcfix.cpp \
          prepstmt.cpp \
          stmttype.cpp \
          storedproc.cpp \
          successivestmts.cpp \
          transaction.cpp \
          accessorinstanceref.cpp \
          accessorinstancerefdefaultparameterusage.cpp \
          accessorinstancerefdeletetest.cpp \
          accessorinstancerefensureparametertest.cpp \
          accessorinstancerefensureparametertest2.cpp \
          accessorinstancerefsqlinsertupdatetest.cpp \
          accessorinstancerefsqlresolvtest.cpp \
          yiterator.cpp \
          testfixturebase.cpp \
          accessorpropertytablerefbeginfixture.cpp \
          main.cpp \
          accessornames.cpp \
          accessmethodnames.cpp \
          float.cpp \
          float_builder.cpp \
		  cachingpolicy.cpp

PROJ_OBJS     =  $(PROJ_SRC_CPP:%.cpp=$(RELSUBDIR)/%.o)
PROJ_LIBS_ARG = \
                -lboost_system -lboost_filesystem \
                -lbasardebug -lbasarcmnutil -lbasarproperty -lbasardbsql -lbasardbaspect

#-------------------------------------------------------------
#   include common makefile and dependency file
#-------------------------------------------------------------
include ../makefile_ut.inc
include ./deps

#-------------------------------------------------------------

CMD_PATH     += LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:$(LIBS_CICS_DIR); \
                export LD_LIBRARY_PATH;

