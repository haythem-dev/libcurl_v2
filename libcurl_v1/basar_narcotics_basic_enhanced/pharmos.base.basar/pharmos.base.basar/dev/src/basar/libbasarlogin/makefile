##############################################################
#                                                            #
#   libbasarlogin  makefile (Solaris)                        #
#	Thomas Hörath	                                         #
#	27.02.2006                                               #
#                                                            #
##############################################################

#-------------------------------------------------------------
#    target
#-------------------------------------------------------------
TARGET_NAME = libbasarlogin.so

#-------------------------------------------------------------
#   flags
#-------------------------------------------------------------
REL_VALUE_PROJ    =
DBG_VALUE_PROJ    =

#-------------------------------------------------------------
#   libraries, lib path, include path
#-------------------------------------------------------------

# shared library paths
LIBS_DIR = -L$(LIBS_BASAR_DIR) \
           -L$(LIBS_OPENLDAP_DIR) \
           -L$(LIBS_OPENSSL_DIR) \
           -L$(LIBS_LOG4CPLUS_DIR) \
           -L$(LIBS_BOOST_DIR)


# shared libraries
LIBS_BASAR         = -lbasarcmnutil -lbasarproperty -lbasardbsql -lbasardbaspect
LIBS_OPENLDAP      = -lldap -llber
LIBS_OPENSSL       = -lcrypto -lssl
LIBS_BOOST         = -lboost_system -lboost_filesystem
LIBS               = -llog4cplus \
                     $(LIBS_BOOST) \
                     $(LIBS_BASAR) \
                     $(LIBS_SYSTEM) \
                     $(LIBS_OPENSSL) \
                     $(LIBS_OPENLDAP)


# include paths
INCS_UNX_SRC = ./unix
INCS         = -I. \
               -I$(INCS_UNX_SRC) \
               -I$(INCS_GLOBAL) \
               -I$(INCS_OPENLDAP) \
               -I$(INCS_BASAR) \
               -I$(INCS_BOOST) \
               -I$(INCS_LOG4CPLUS)

#-------------------------------------------------------------
#    object files 
#-------------------------------------------------------------
PROJ_SRC_LIB =

PROJ_DIR_UNX = unix

PROJ_SRC_CPP =                  libbasarlogin.cpp \
                                libbasarlogin_manager.cpp \
                                logindata.cpp \
                                loginappl.cpp \
                                $(PROJ_DIR_UNX)/loginapplunix.cpp \
                                changepwuc.cpp \
                                checkloginbaseuc.cpp \
                                checkadloginuc.cpp \
                                checkloginuc.cpp \
                                checkloginucfactory.cpp \
                                adusercheckerunix.cpp \
                                usecasecontroller.cpp \
								baseusermgmtdm.cpp \
								baseapplmgmtdm.cpp \
								adloginusermgmtdm.cpp \
								adloginapplmgmtdm.cpp \
								adloginapplacc.cpp \
								adloginlanguageacc.cpp \
								adloginpreviousacc.cpp \
								adloginparaacc.cpp \
								adloginareaacc.cpp \
								adloginuseracc.cpp \
								adloginapprightacc.cpp \
                                usermanagementdm.cpp \
				usermgmtdmfactory.cpp \
                                acc_user.cpp \
                                acc_appright.cpp \
                                applmanagementdm.cpp \
				applmgmtdmfactory.cpp \
                                acc_appl.cpp \
                                acc_language.cpp \
                                acc_area.cpp \
                                acc_previous.cpp \
                                acc_para.cpp \
                                textmanagementdm.cpp \
                                acc_text.cpp \
                                loggerpool.cpp \
                                crypt.cpp \
                                connection.cpp \
                                configuration.cpp \
                                init.cpp

PROJ_OBJS    = $(PROJ_SRC_CPP:%.cpp=$(RELSUBDIR)/%.o)
PROJ_LIBS    = $(PROJ_SRC_LIB)

PROJ_RELSUBDIRS = $(RELSUBDIR) $(RELSUBDIR)/$(PROJ_DIR_UNX)

#-------------------------------------------------------------
#   include common makefile and dependency file
#-------------------------------------------------------------
include ../makefile.inc
include ./deps
