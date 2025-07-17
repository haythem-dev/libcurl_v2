##############################################################
#                                                            #
#   libbasarcmnutil makefile (Solaris)                       #
#	Michael Eichenlaub                                       #
#	19.07.2005                                               #
#                                                            #
##############################################################

#-------------------------------------------------------------
#    target
#-------------------------------------------------------------
TARGET_NAME = libbasarcmnutil.so

#-------------------------------------------------------------
#   flags
#-------------------------------------------------------------
REL_VALUE_PROJ    =
DBG_VALUE_PROJ    =

#-------------------------------------------------------------
#   libraries, lib path, include path
#-------------------------------------------------------------

# shared library paths
LIBS_INFX_ESQL_DIR  = $(INFORMIXDIR)/lib/esql
LIBS_INFX_DIR       = $(INFORMIXDIR)/lib

LIBS_DIR            = -L$(LIBS_BASAR_DIR) \
					  -L$(LIBS_BOOST_DIR) \
					  -L$(LIBS_LOG4CPLUS_DIR) \
					  -L$(LIBS_INFX_ESQL_DIR) \
					  -L$(LIBS_INFX_DIR) \
					  -L$(LIBS_LIBSSH2_DIR) \
                                          -L$(LIBS_LIBCURL_DIR) \
					  -L$(LIBS_GHOSTSCRIPT_DIR)

# shared libraries
LIBS_INFX_ESQL     = -lifgen -lifos -lifsql -lifgls $(LIBS_INFX_ESQL_DIR)/checkapi.o
LIBS_INFX          = -lifasf
LIBS_SYSTEM        = -liconv
LIBS_BOOST         = -lboost_system -lboost_filesystem -lboost_regex
LIBS_LIBSSH2       = -lssh2
LIBS_LIBCURL       = -lcurl
LIBS_GHOSTSCRIPT   = -lgs
LIBS               = \
                     -llog4cplus \
                     $(LIBS_BOOST) \
                     -lbasardebug \
                     -lopensslwrap \
                     $(LIBS_INFX_ESQL) $(LIBS_INFX) $(LIBS_LIBSSH2) $(LIBS_GHOSTSCRIPT) $(LIBS_SYSTEM)
                     $(LIBS_LIBCURL) \

# include paths
INCS_INFX          = $(INFORMIXDIR)/incl/esql

INCS               = -I. \
                     -I./ps \
                     -I$(INCS_GLOBAL) \
                     -I$(INCS_BASAR) \
                     -I$(INCS_BOOST) \
                     -I$(INCS_INFX) \
                     -I$(INCS_LOG4CPLUS) \
                     -I$(INCS_LIBSSH2) \
                     -I$(INCS_LIBCURL) \
                     -I$(INCS_GHOSTSCRIPT)

#-------------------------------------------------------------
#    object files 
#-------------------------------------------------------------

PROJ_SRC_LIB =

PROJ_SRC_CPP = \
               libbasarcmnutil.cpp \
               libbasarcmnutil_locale.cpp \
               libbasarcmnutil_bstring.cpp \
               libbasarcmnutil_cmdline.cpp \
               libbasarcmnutil_credentialbase.cpp \
               libbasarcmnutil_credentialsref.cpp \
               libbasarcmnutil_datetime.cpp \
               libbasarcmnutil_date.cpp \
               libbasarcmnutil_datetimetools.cpp \
               libbasarcmnutil_timespan.cpp \
               libbasarcmnutil_decimal.cpp \
               libbasarcmnutil_exceptions.cpp \
               libbasarcmnutil_i18nstring.cpp \
               libbasarcmnutil_logging.cpp \
               libbasarcmnutil_remotetaskref.cpp \
               libbasarcmnutil_remotetaskresultref.cpp \
               libbasarcmnutil_remotetasksubmitterbase.cpp \
               libbasarcmnutil_parameterlist.cpp \
               libbasarcmnutil_parameterlist_iterator.cpp \
               libbasarcmnutil_userpasscredential.cpp \
               attachment.cpp \
               mailhelper.cpp \
               timestamp.cpp \
               printtools.cpp \
               rawprintprocdataprov.cpp \
               rawprint.cpp \
               remoteendpoint.cpp \
               remotetask.cpp \
               remotetaskexecution.cpp \
               remotetaskexitstatus.cpp \
               remotetaskresult.cpp \
               remotetasksubmitterimpl.cpp \
               lpr.cpp \
               portprint.cpp \
               tcpsocket.cpp \
               smtp.cpp \
               sshchannel.cpp \
               sshsession.cpp \
               quotedprint.cpp \
               base64.cpp \
               mimechar.cpp \
               mailmessage.cpp \
               pipe.cpp \
               pipedprocess.cpp \
               loggerpool.cpp \
               config.cpp \
               getexecname.cpp \
               getlibpath.cpp \
               ps/collargs.cpp \
               ps/encodedargs.cpp \
               ps/interpretprint.cpp \
               ps/psinterpreter.cpp \
               init.cpp
               libbasarcmnutil_ftpclient.cpp \

PROJ_RELSUBDIRS = $(RELSUBDIR)\
                  $(RELSUBDIR)/ps

PROJ_OBJS    = $(PROJ_SRC_CPP:%.cpp=$(RELSUBDIR)/%.o)
PROJ_LIBS    = $(PROJ_SRC_LIB)

#-------------------------------------------------------------
#   include common makefile and dependency file
#-------------------------------------------------------------
include ../makefile.inc
include ./deps

