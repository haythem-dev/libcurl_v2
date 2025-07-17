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
TARGET_NAME = utdbsql

#-------------------------------------------------------------
#    object files 
#-------------------------------------------------------------
PROJ_SRC_CPP = \
               buttable.cpp \
               defaultmasterbuttable.cpp \
               testcicsconnection.cpp \
               testconnection.cpp \
               testinfxconnection.cpp \
               testmssqlodbcconnection.cpp \
               testodbcconnection.cpp \
               utdbsqlhelper.cpp  \
               cics.cpp \
               cicsmt.cpp \
               cicsconnect.cpp \
               columnnames.cpp \
               connection.cpp \
               connectionmt.cpp \
               databaseinfo.cpp \
               datetime.cpp \
               decimal.cpp \
               fixedstrings.cpp \
               infxbigint.cpp \
               infxbigserial.cpp \
               infxint8.cpp \
               infxsimpleblob.cpp \
               infxsqlfunc.cpp \
               i18n.cpp \
               managerimpl.cpp \
               odbcfix.cpp \
               preparedstatement.cpp \
               resultset.cpp \
               resultsetclose.cpp \
               statement.cpp \
               stmttype.cpp \
               storedproc.cpp \
               successivestmts.cpp \
               timespan.cpp \
               main.cpp

PROJ_OBJS     = $(PROJ_SRC_CPP:%.cpp=$(RELSUBDIR)/%.o)

PROJ_LIBS_ARG = \
    -lboost_system -lboost_filesystem \
    -lbasardebug -lbasarcmnutil -lbasardbsql \
    $(LIBS_QT)
 	
PROJ_FLAGS = -Wno-long-long


#-------------------------------------------------------------
#   include common makefile and dependency file
#-------------------------------------------------------------
include ../makefile_ut.inc
include ./deps

#-------------------------------------------------------------

CMD_PATH     += LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:$(LIBS_CICS_DIR):$(LIBS_QT_DIR); \
                export LD_LIBRARY_PATH;

