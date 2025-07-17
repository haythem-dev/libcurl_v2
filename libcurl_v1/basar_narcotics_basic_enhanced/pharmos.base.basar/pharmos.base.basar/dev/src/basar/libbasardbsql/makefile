##############################################################
#                                                            #
#   libbasardbsql makefile (Solaris)                         #
#   Michael Eichenlaub                                       #
#   18.07.2005                                               #
#                                                            #
##############################################################

#-------------------------------------------------------------
#    target
#-------------------------------------------------------------
TARGET_NAME = libbasardbsql.so

#-------------------------------------------------------------
#   flags
#-------------------------------------------------------------
# -DLIBBASARDBSQL_PROFILING
REL_VALUE_PROJ    =
DBG_VALUE_PROJ    =

#-------------------------------------------------------------
#   libraries, lib path, include path
#-------------------------------------------------------------

# shared library paths
LIBS_INFX_ESQL_DIR  = $(INFORMIXDIR)/lib/esql
LIBS_INFX_DIR       = $(INFORMIXDIR)/lib

LIBS_CICS_DIR       = $(CICSDIR)/lib
LIBS_DIR            = -L$(LIBS_BASAR_DIR) \
                      -L$(LIBS_LOG4CPLUS_DIR) \
                      -L$(LIBS_BOOST_DIR) \
                      -L$(LIBS_INFX_ESQL_DIR) \
                      -L$(LIBS_INFX_DIR) \
                      -L$(LIBS_CICS_DIR) 

# shared libraries
LIBS_BASAR         = -lbasardebug -lbasarcmnutil
LIBS_BOOST         = -lboost_system -lboost_filesystem
LIBS_INFX_ESQL     = -lifgen -lifos -lifsql -lifgls
LIBS_INFX          = -lifasf
LIBS_CICS          = -lBml \
                     -lbmlApi \
                     -lBmlCh 
LIBS_SYSTEM        = 
# regard: lazy load is valid for all following libs!
LIBS = \
	-llog4cplus \
	$(LIBS_BOOST) \
	$(LIBS_BASAR) \
	$(LIBS_SYSTEM) \
	$(LIBS_INFX_ESQL) $(LIBS_INFX) \
	$(LIBS_CICS)

# include paths
INCS_CICS          = $(CICSDIR)/include
INCS_ESQL_SRC      = ./esqlc
INCS_INFORMIX      = $(INFORMIXDIR)/incl/esql
INCS_INFX_SRC      = ./infx
INCS_CICS_SRC      = ./cics
INCS               = -I. \
                     -I$(INCS_INFORMIX) \
                     -I$(INCS_ESQL_SRC) \
                     -I$(INCS_INFX_SRC) \
                     -I$(INCS_CICS_SRC) \
                     -I$(INCS_GLOBAL) \
                     -I$(INCS_BASAR) \
                     -I$(INCS_BOOST) \
                     -I$(INCS_LOG4CPLUS) \
                     -I$(INCS_CICS) 

#-------------------------------------------------------------
#    object files 
#-------------------------------------------------------------

PROJ_SRC_LIB =

PROJ_DIR_EC   = esqlc
PROJ_DIR_INFX = infx
PROJ_DIR_CICS = cics

PROJ_SRC_EC  = $(PROJ_DIR_EC)/connect.ec \
               $(PROJ_DIR_EC)/cursor.ec \
               $(PROJ_DIR_EC)/descriptor.ec \
               $(PROJ_DIR_EC)/exception.ec \
               $(PROJ_DIR_EC)/profiling.ec \
               $(PROJ_DIR_EC)/statement.ec

PROJ_SRC_CPP_DIRECT = \
               connection.cpp \
               connectionlist_iterator.cpp \
               connectionref.cpp \
               database.cpp \
               databaseinfo.cpp \
               dbexception.cpp \
               insertset.cpp \
               insertsetref.cpp \
               libbasardbsql.cpp \
               loggerpool.cpp \
               managerimpl.cpp \
               preparedstatement.cpp \
               preparedstatementref.cpp \
               resultset.cpp \
               resultsetref.cpp \
               sqldiagnostics.cpp \
               sqldiagnosticsref.cpp \
               sqlexcepthandler.cpp \
               sqloperative.cpp \
               statement.cpp \
               statementref.cpp \
               init.cpp

PROJ_SRC_CPP_INFX =  \
               $(PROJ_DIR_INFX)/connectioninfx.cpp \
               $(PROJ_DIR_INFX)/dbexceptioninfx.cpp \
               $(PROJ_DIR_INFX)/insertsetinfx.cpp \
               $(PROJ_DIR_INFX)/preparedstatementinfx.cpp \
               $(PROJ_DIR_INFX)/resultsetinfx.cpp \
               $(PROJ_DIR_INFX)/resultsetinfxmulti.cpp \
               $(PROJ_DIR_INFX)/resultsetinfxsingle.cpp \
               $(PROJ_DIR_INFX)/sqldiagnosticsinfx.cpp \
               $(PROJ_DIR_INFX)/sqlexcepthandlerinfx.cpp \
               $(PROJ_DIR_INFX)/sqlhostsinfx.cpp \
               $(PROJ_DIR_INFX)/statementinfx.cpp \
               $(PROJ_DIR_INFX)/dblocaleinfx.cpp \
               $(PROJ_DIR_INFX)/localeenvinfx.cpp \
               $(PROJ_DIR_INFX)/setenvinfx.cpp \
               $(PROJ_DIR_INFX)/infxstring.cpp

PROJ_SRC_CPP_CICS =  \
               $(PROJ_DIR_CICS)/cicsdefinitions.cpp \
               $(PROJ_DIR_CICS)/connectioncics.cpp \
               $(PROJ_DIR_CICS)/dbexceptioncics.cpp \
               $(PROJ_DIR_CICS)/resultsetcics.cpp \
               $(PROJ_DIR_CICS)/sqldiagnosticscics.cpp \
               $(PROJ_DIR_CICS)/sqlexcepthandlercics.cpp \
               $(PROJ_DIR_CICS)/statementcics.cpp \

PROJ_SRC_CPP = $(PROJ_SRC_CPP_DIRECT) \
               $(PROJ_SRC_CPP_INFX) \
               $(PROJ_SRC_CPP_CICS) 

PROJ_OBJS    = $(PROJ_SRC_EC:%.ec=$(RELSUBDIR)/%.o) \
               $(PROJ_SRC_CPP:%.cpp=$(RELSUBDIR)/%.o)

PROJ_LIBS    = $(PROJ_SRC_LIB)

PROJ_RELSUBDIRS = $(RELSUBDIR) \
                  $(RELSUBDIR)/$(PROJ_DIR_EC) \
                  $(RELSUBDIR)/$(PROJ_DIR_INFX) \
                  $(RELSUBDIR)/$(PROJ_DIR_CICS)

#-------------------------------------------------------------
#   include common makefile and dependency file
#-------------------------------------------------------------
include ../makefile.inc
include ./deps
