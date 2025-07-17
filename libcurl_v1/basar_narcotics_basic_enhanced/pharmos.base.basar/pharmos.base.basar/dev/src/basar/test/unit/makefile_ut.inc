##############################################################
#                                                            #
#   makefile_ut.inc (Solaris)                                #
#	Anke Klink / Michael Eichenlaub                          #
#	27.09.2007                                               #
#                                                            #
##############################################################


#-------------------------------------------------------------
#   flags
#-------------------------------------------------------------

REL_VALUE    = -DNDEBUG -O2
DBG_VALUE    = -g
DBGFLAGS     = $(DBG_VALUE)

RELDIR_VALUE     = release
DBGDIR_VALUE     = debug
DBGDIR           = $(DBGDIR_VALUE)

RELSUBDIR_RELVALUE    = release
RELSUBDIR_DBGVALUE    = debug
RELSUBDIR             = $(RELSUBDIR_DBGVALUE)

PROJ_RELSUBDIRS      += $(RELSUBDIR)

LDFLAGS  = -Wl,-brtl,-bexpfull \
           -maix64 \
           -lpthread -pthread
CPPFLAGS = $(INCS_ARG) \
           $(PROJ_INCS_ARG) \
           -maix64 \
           -pthread \
           -ansi -pedantic \
           -std=c++98 \
           -Wall -Wextra -Werror $(PROJ_FLAGS)

#-------------------------------------------------------------
#   libraries, lib path, include path
#-------------------------------------------------------------

BASARDIR = ../../..

BASE_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

include $(BASARDIR)/data/version.inc

INFORMIXDIR         = /informix/informix12.10FC10

BOOSTDIR            = $(BASE_DIR)../../../../ext/boost
CICSDIR             = ${DEVLIB_PATH}/marathon/lib/tcaccess/$(TCACCESSVERSION)
LOG4CPLUSDIR        = $(BASE_DIR)../../../../ext/log4cplus
QTDIR               = $(BASE_DIR)../../../../ext/qt5
UNITTESTDIR         = $(BASE_DIR)../../../../ext/unittestpp
OPENSSLDIR          = $(BASE_DIR)../../../../ext/openssl
GHOSTSCRIPTDIR      = $(BASE_DIR)../../../../ext/ghostscript
ZLIBDIR             = $(BASE_DIR)../../../../ext/zlib

# shared library paths
LIBS_INFORMIX           = $(INFORMIXDIR)/lib
LIBS_INFORMIX_ESQL      = $(INFORMIXDIR)/lib/esql
LIBS_BASAR_DIR_BASE     = $(BASARDIR)/library/lib
LIBS_BASAR_DIR          = $(LIBS_BASAR_DIR_BASE)/$(DBGDIR)
LIBS_LOG4CPLUS_DIR      = $(LOG4CPLUSDIR)/lib/$(DBGDIR)
LIBS_CICS_DIR           = $(CICSDIR)/lib
LIBS_QT_DIR             = $(QTDIR)/lib
LIBS_BOOST_DIR          = $(BOOSTDIR)/lib
LIBS_UNITTEST_DIR       = $(UNITTESTDIR)/lib/$(DBGDIR)
LIBS_OPENSSL_DIR        = $(OPENSSLDIR)/lib
LIBS_GHOSTSCRIPT_DIR    = $(GHOSTSCRIPTDIR)/lib
LIBS_ZLIB_DIR           = $(ZLIBDIR)/lib/$(DBGDIR)
LIBS_DIR                = -L$(LIBS_BASAR_DIR) \
						  -L$(LIBS_BOOST_DIR) \
						  -L$(LIBS_LOG4CPLUS_DIR) \
						  -L$(LIBS_OPENSSL_DIR) \
						  -L$(LIBS_CICS_DIR) \
						  -L$(LIBS_QT_DIR) \
						  -L$(LIBS_UNITTEST_DIR) \
						  -L$(LIBS_INFORMIX) \
						  -L$(LIBS_INFORMIX_ESQL)

#-L$(LIBS_OPENSSL_DIR) \

# shared libraries

# we have to get and reset the libpath manually, otherwise ld will add all -L paths 
# (which are absolute and point into some user/build dependent directories) to the binaries, which could lead into weird bugs!
# it's a pity that we can't use $ORIGIN like on standard linux
LDFLAGS += -Wl,-blibpath:"$(shell dump -H -X64 $(LIBS_LOG4CPLUS_DIR)/liblog4cplus.so 2>/dev/null | egrep "^0[ \t]" | awk -F ' ' '{print $$2}')"

QT_SUFFIX          = d

LIBS_SYSTEM        = 
LIBS_QT            = -lQt5Core$(QT_SUFFIX)
LIBS_ARG           = -llog4cplus -lUnitTest++ \
                     $(LIBS_SYSTEM)

# include paths
INCS_BASAR         = $(BASARDIR)/library/include
INCS_BOOST         = $(BOOSTDIR)/include
INCS_LOG4CPLUS     = $(LOG4CPLUSDIR)/include
INCS_UNITTEST      = $(UNITTESTDIR)/include
INCS_QT            = $(QTDIR)/include
INCS_ARG           = -I. \
					 -I$(INCS_UNITTEST) \
					 -I$(INCS_BASAR) \
					 -I$(INCS_BOOST) \
					 -I$(INCS_LOG4CPLUS) \
					 -I$(INCS_QT) 

CMD_PATH           = LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:$(LIBS_BASAR_DIR):$(LIBS_BOOST_DIR):$(LIBS_LOG4CPLUS_DIR):$(LIBS_OPENSSL_DIR):$(LIBS_GHOSTSCRIPT_DIR)
RUN_ARGS           =

#-------------------------------------------------------------
#   tools
#-------------------------------------------------------------
# CC        : Forte C++ compiler/linker
# strip     : strip symbol table, debugging and line number information from an object file

CC   = gcc
CPP  = g++
LINK = g++

STRIP    = strip -X64
MKDIR    = mkdir -p
DEBUGGER = gdb

###############################################################
#    rules                                                   #
##############################################################

#-------------------------------------------------------------
#   default goal
#-------------------------------------------------------------
$(RELSUBDIR)/$(TARGET_NAME): $(PROJ_OBJS) $(PROJ_DEP) ./basar.config.xml
	@echo "  linking $(RELSUBDIR)/$(TARGET_NAME) ...";
	$(LINK) -o $(RELSUBDIR)/$(TARGET_NAME) \
	           $(LIBS_DIR) \
	           $(LDFLAGS) \
	           $(LIBS_ARG) \
	           $(PROJ_LIBS_ARG) \
	           $(PROJ_OBJS)

	@echo "";
	ls -l $(RELSUBDIR)/$(TARGET_NAME);

	@echo "";
	@if [ "$(DBGFLAGS)" = "$(REL_VALUE)" ]; \
	then $(STRIP) $(RELSUBDIR)/$(TARGET_NAME); \
	    echo "$(STRIP) $(RELSUBDIR)/$(TARGET_NAME)"; \
	    ls -l $(RELSUBDIR)/$(TARGET_NAME); \
	    echo ""; \
	fi;     

	@echo "";
	@echo "copy $(TARGET_NAME).cfg to $$MARATHON_LOG_CONFIGDIR/";
	@cp data/$(TARGET_NAME).cfg $$MARATHON_LOG_CONFIGDIR/;

	@echo ""; # no test run after build
	@if false; \
    then \
        if [ -z "$$CISERVER" ]; \
        then \
            $(MAKE) start DBGDIR='$(DBGDIR)' RELSUBDIR='$(RELSUBDIR)' RUN_ARGS='-short'; \
        else \
            case "$(TARGET_NAME)" in \
                "utappl" | "utcmnutil" | "utproperty") \
                    $(MAKE) start DBGDIR='$(DBGDIR)' \
                                  RELSUBDIR='$(RELSUBDIR)' \
                                  RUN_ARGS='-short' ;; \
                *) true;; \
            esac; \
        fi; \
    fi;

#-------------------------------------------------------------
#   basar config file in current working dir
#-------------------------------------------------------------
basar.config.xml: $(BASARDIR)/data/basar.config.xml
	@echo "  copying $(BASARDIR)/data/basar.config.xml to $@ ...";
	@cp $(BASARDIR)/data/basar.config.xml $@;

#-------------------------------------------------------------
#   C++ sources
#-------------------------------------------------------------

$(RELSUBDIR)/%.o : %.cpp
	@$(MKDIR) $(PROJ_RELSUBDIRS);
	$(CPP) -o $@ $(CPPFLAGS) $(DBGFLAGS) -c $< 

#-------------------------------------------------------------
#   build release version
#-------------------------------------------------------------
rel: FORCE
	@$(MAKE) QT_SUFFIX='' \
	         ECDBGFLAGS='$(ECREL_VALUE)' \
	         DBGFLAGS='$(REL_VALUE) $(REL_VALUE_PROJ)' \
	         DBGDIR='$(RELDIR_VALUE)' \
	         RELSUBDIR='$(RELSUBDIR_RELVALUE)' \
	         $(RELSUBDIR_RELVALUE)/$(TARGET_NAME);

#-------------------------------------------------------------
#   build debug version
#-------------------------------------------------------------
dbg: FORCE
	@$(MAKE) $(RELSUBDIR_DBGVALUE)/$(TARGET_NAME);

#-------------------------------------------------------------
#   build all
#-------------------------------------------------------------
all: dep dbg rel

#-------------------------------------------------------------
#   create dependencies
#-------------------------------------------------------------
dep: FORCE
	@echo "create dependencies ...";
	@rm deps;
	@touch deps;
	@FILES="$(PROJ_SRC_EC) $(PROJ_SRC_CPP)"; \
	INCS_UT="$(INCS_ARG) $(PROJ_INCS_ARG)"; \
	for FILE in $$FILES; \
	do \
		OBJNAME='$$(RELSUBDIR)/'`echo "$$FILE" | sed -e "s/\.[cep]\{1,3\}$$/\.o/"`; \
		EXT=`echo "$$FILE" | awk -F. '{print $$NF}'`; \
		case $$EXT in \
			ec) LANGUAGE="-x c";; \
			* ) LANGUAGE="";; \
		esac; \
		echo "gcc -MM -MT $$OBJNAME $$INCS_UT $$LANGUAGE $$FILE 1>>deps;"; \
		gcc -MM -MT $$OBJNAME $$INCS_UT $$LANGUAGE $$FILE 1>>deps; \
	done;

#-------------------------------------------------------------
# check if file "deps" exists
#-------------------------------------------------------------
deps:
	@touch deps;

#-------------------------------------------------------------
#   start process
#-------------------------------------------------------------
start: FORCE
	@echo "  start $(RELSUBDIR)/$(TARGET_NAME) ($(DBGDIR)) ...";
	@$(CMD_PATH) $(RELSUBDIR)/$(TARGET_NAME) $(RUN_ARGS);

#-------------------------------------------------------------
#   start debug process
#-------------------------------------------------------------
stdbg: FORCE
	@$(MAKE) start DBGDIR='$(DBGDIR_VALUE)'  RELSUBDIR='$(RELSUBDIR_DBGVALUE)'

#-------------------------------------------------------------
#   start release process
#-------------------------------------------------------------
strel: FORCE
	@$(MAKE) start DBGDIR='$(RELDIR_VALUE)'  RELSUBDIR='$(RELSUBDIR_RELVALUE)'

#-------------------------------------------------------------
#   remove temporary files
#-------------------------------------------------------------
.clean: FORCE
	@echo "cleaning ...";
	@if rm -r $(RELSUBDIR) 2>/dev/null; \
	then echo "  directory $(RELSUBDIR) removed"; \
	else echo "  directory $(RELSUBDIR) already removed"; \
	fi;
	@if rm $(PROJ_OBJS) $(RELSUBDIR)/$(TARGET_NAME) 2>/dev/null; \
	then echo " additional object and binary files for $(RELSUBDIR)/$(TARGET_NAME) removed"; \
	fi;
	
#-------------------------------------------------------------
#   remove temporary files
#-------------------------------------------------------------
clean_rel: FORCE
	@$(MAKE) .clean  RELSUBDIR='$(RELSUBDIR_RELVALUE)'

clean_dbg: FORCE
	@$(MAKE) .clean  RELSUBDIR='$(RELSUBDIR_DBGVALUE)'

clean: clean_rel clean_dbg 

#-------------------------------------------------------------
#   start $(DEBUGGER) debug session
#-------------------------------------------------------------
gdb: FORCE
	$(CMD_PATH) $(DEBUGGER) $(RELSUBDIR_DBGVALUE)/$(TARGET_NAME);

core: FORCE
	$(CMD_PATH) $(DEBUGGER) - core;

#-------------------------------------------------------------
#   Usage
#-------------------------------------------------------------
help: FORCE
	@echo "  make              : update (compile and link) $(TARGET_NAME) (debug)";
	@echo "  make all          : update (compile and link) $(TARGET_NAME) (all versions)";
	@echo "  make dbg          : compile and link debug version ($(RELSUBDIR_DBGVALUE)/$(TARGET_NAME))";
	@echo "  make rel          : compile and link release version  ($(RELSUBDIR_RELVALUE)/$(TARGET_NAME))";
	@echo "  make clean        : delete all versions";
	@echo "  make clean_dbg    : delete debug version ($(RELSUBDIR_DBGVALUE))";
	@echo "  make clean_rel    : delete release version ($(RELSUBDIR_RELVALUE))";
	@echo "  make stdbg        : start debug       ($(RELSUBDIR_DBGVALUE)/$(TARGET_NAME))";
	@echo "  make strel        : start release     ($(RELSUBDIR_RELVALUE)/$(TARGET_NAME))";
	@echo "  make gdb          : start $(DEBUGGER) debug session";
	@echo "  make core         : start $(DEBUGGER) debug session with core file";
	@echo "";
	@echo "  make dep          : scan source files to create dependencies in file \"deps\"";
	@echo "";

#-------------------------------------------------------------
#   Dummy
#-------------------------------------------------------------
FORCE:

#-------------------------------------------------------------
#   Undefined
#-------------------------------------------------------------
.c:

#-------------------------------------------------------------
#   File Suffixes
#-------------------------------------------------------------
.SUFFIXES:
.SUFFIXES: .o .cpp
