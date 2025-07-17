##############################################################
#                                                            #
#   common makefile include for basar                        #
#	Michael Eichenlaub                                       #
#	18.07.2005                                               #
#                                                            #
##############################################################


##############################################################
#   variables                                                #
##############################################################

#-------------------------------------------------------------
#   directories
#-------------------------------------------------------------
BASARDIR            = ..

BASE_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

include $(BASARDIR)/data/version.inc

INFORMIXDIR         = /informix/informix12.10FC10

BOOSTDIR            = $(BASE_DIR)../../ext/boost
CICSDIR             = ${DEVLIB_PATH}/marathon/lib/tcaccess/$(TCACCESSVERSION)
LOG4CPLUSDIR        = $(BASE_DIR)../../ext/log4cplus
LIBSSH2DIR          = $(BASE_DIR)../../ext/libssh2
OPENLDAPDIR         = $(BASE_DIR)../../ext/openldap
OPENSSLDIR          = $(BASE_DIR)../../ext/openssl
GHOSTSCRIPTDIR      = $(BASE_DIR)../../ext/ghostscript

INCS_GLOBAL         = $(BASARDIR)
INCS_BASAR          = $(BASARDIR)/library/include
INCS_BOOST          = $(BOOSTDIR)/include
INCS_LOG4CPLUS      = $(LOG4CPLUSDIR)/include
INCS_LIBSSH2        = $(LIBSSH2DIR)/include
INCS_OPENLDAP       = $(OPENLDAPDIR)/include
INCS_GHOSTSCRIPT    = $(GHOSTSCRIPTDIR)/include

LIBS_BASAR_DIR_BASE = $(BASARDIR)/library/lib
LIBS_BASAR_DIR      = $(LIBS_BASAR_DIR_BASE)/$(DBGDIR)
LIBS_LOG4CPLUS_DIR  = $(LOG4CPLUSDIR)/lib/$(DBGDIR)
LIBS_BOOST_DIR      = $(BOOSTDIR)/lib
LIBS_LIBSSH2_DIR    = $(LIBSSH2DIR)/lib/$(DBGDIR)
LIBS_OPENLDAP_DIR   = $(OPENLDAPDIR)/lib
LIBS_OPENSSL_DIR    = $(OPENSSLDIR)/lib
LIBS_GHOSTSCRIPT_DIR= $(GHOSTSCRIPTDIR)/lib

#-------------------------------------------------------------
#   misc
#-------------------------------------------------------------
HEADER_FILE_PREFIX = libbasar

TARGET_NAME_STATIC = $(TARGET_NAME:%.so=%.a)

#-------------------------------------------------------------
#   flags
#-------------------------------------------------------------

REL_VALUE = -DNDEBUG -O2
DBG_VALUE = -g
PROF_VALUE= -pg
DBGFLAGS  = $(DBG_VALUE) $(DBG_VALUE_PROJ)

RELDIR_VALUE     = release
DBGDIR_VALUE     = debug
PROFDIR_VALUE    = profile
DBGDIR           = $(DBGDIR_VALUE)

RELSUBDIR_RELVALUE = release
RELSUBDIR_DBGVALUE = debug
RELSUBDIR_PROFVALUE= profile
RELSUBDIR          = $(RELSUBDIR_DBGVALUE)

PROJ_RELSUBDIRS   += $(RELSUBDIR)

WARNLEVEL  = -pedantic \
             -Wall \
             -Wextra \
             -Werror

MISCFLAGS  = -maix64 \
             -pthread \
             $(WARNLEVEL)

LDFLAGS    = -Wl,-brtl \
             -shared \
             -maix64 \
             -lpthread \
             -pthread

CPPFLAGS   = $(MISCFLAGS) \
             -ansi \
             -std=c++98

ESQLCFLAGS = $(MISCFLAGS)

# we have to get and reset the libpath manually, otherwise ld will add all -L paths 
# (which are absolute and point into some user/build dependent directories) to the binaries, which could lead into weird bugs!
# it's a pity that we can't use $ORIGIN like on standard linux
LDFLAGS += -Wl,-blibpath:"$(shell dump -H -X64 $(LIBS_LOG4CPLUS_DIR)/liblog4cplus.so 2>/dev/null | egrep "^0[ \t]" | awk -F ' ' '{print $$2}')"

#-------------------------------------------------------------
#   tools
#-------------------------------------------------------------

CPP  = g++
LINK = g++

ESQL     = /software/ae/marathon/bin/esql
STRIP    = strip -X64
AWK      = awk
AR       = ar -X64 -v -q

MKDIR    = mkdir -p

##############################################################
#    rules                                                   #
##############################################################

#-------------------------------------------------------------
#   default goal
#-------------------------------------------------------------
$(RELSUBDIR)/$(TARGET_NAME): $(PROJ_OBJS) $(PROJ_LIBS)
	@echo "  linking $(RELSUBDIR)/$(TARGET_NAME) ...";
	$(LINK) -o $(RELSUBDIR)/$(TARGET_NAME) $(LIBS_DIR) $(LDFLAGS) $(LIBS) $(PROJ_LIBS) $(PROJ_OBJS);
	@echo "";
	ls -l $(RELSUBDIR)/$(TARGET_NAME);
	@echo "";
	@if [ "$(DBGDIR)" = "$(RELDIR_VALUE)" ]; \
	then $(STRIP) $(RELSUBDIR)/$(TARGET_NAME); \
	    echo "$(STRIP) $(RELSUBDIR)/$(TARGET_NAME)"; \
	    ls -l $(RELSUBDIR)/$(TARGET_NAME); \
	    echo ""; \
	fi;
	@$(MKDIR) $(LIBS_BASAR_DIR);
	@cp -f $(RELSUBDIR)/$(TARGET_NAME) $(LIBS_BASAR_DIR)/; \
	echo "  copying $(RELSUBDIR)/$(TARGET_NAME) to library dir $(LIBS_BASAR_DIR) ...";
	@$(MKDIR) $(INCS_BASAR);
	@for hdrfile in $(INCS_GLOBAL)/$(HEADER_FILE_PREFIX)*.h $(HEADER_FILE_PREFIX)*.h*; \
	do \
	    DIR_NAME=`dirname $$hdrfile`; \
	    BASE_NAME=`basename $$hdrfile`; \
	    if [ -f $(INCS_BASAR)/$$BASE_NAME ]; \
	    then FOUND=`find $$DIR_NAME/ -name $$BASE_NAME -newer $(INCS_BASAR)/$$BASE_NAME`; \
	    else FOUND="not existing"; \
	    fi; \
	    if [ -n "$$FOUND" ]; \
	    then cp $$hdrfile $(INCS_BASAR); \
	         echo "  copying $$hdrfile to library dir $(INCS_BASAR)/ ..."; \
	    fi; \
	done;
	@echo "";

#-------------------------------------------------------------
#   default goal - static profile
#-------------------------------------------------------------
$(RELSUBDIR)/$(TARGET_NAME_STATIC): $(PROJ_OBJS)
	@echo "  linking $(RELSUBDIR)/$(TARGET_NAME) ...";
	$(AR) $(RELSUBDIR)/$(TARGET_NAME_STATIC) $(PROJ_OBJS);
	@echo "";
	ls -l $(RELSUBDIR)/$(TARGET_NAME_STATIC);
	@echo "";
	@$(MKDIR) $(LIBS_BASAR_DIR);
	@cp -f $(RELSUBDIR)/$(TARGET_NAME_STATIC) $(LIBS_BASAR_DIR)/; \
	echo "  copying $(RELSUBDIR)/$(TARGET_NAME_STATIC) to library dir $(LIBS_BASAR_DIR) ...";
	@$(MKDIR) $(INCS_BASAR);
	@for hdrfile in $(INCS_GLOBAL)/$(HEADER_FILE_PREFIX)*.h $(HEADER_FILE_PREFIX)*.h*; \
	do \
	    DIR_NAME=`dirname $$hdrfile`; \
	    BASE_NAME=`basename $$hdrfile`; \
	    if [ -f $(INCS_BASAR)/$$BASE_NAME ]; \
	    then FOUND=`find $$DIR_NAME/ -name $$BASE_NAME -newer $(INCS_BASAR)/$$BASE_NAME`; \
	    else FOUND="not existing"; \
	    fi; \
	    if [ -n "$$FOUND" ]; \
	    then cp $$hdrfile $(INCS_BASAR); \
	         echo "  copying $$hdrfile to library dir $(INCS_BASAR)/ ..."; \
	    fi; \
	done;
	@echo "";


#-------------------------------------------------------------
#   ESQL sources
#-------------------------------------------------------------
$(RELSUBDIR)/%.o : %.ec
	@$(MKDIR) $(PROJ_RELSUBDIRS)
	$(ESQL) -I$(INCS_ESQL_SRC) $(ESQLCFLAGS) $(DBGFLAGS) -o $@ -c $<
	@rm -f `basename $*`.c 

#-------------------------------------------------------------
#   C++ sources
#-------------------------------------------------------------
$(RELSUBDIR)/%.o : %.cpp
	@$(MKDIR) $(PROJ_RELSUBDIRS);
	$(CPP) $(INCS) $(CPPFLAGS) $(DBGFLAGS) -o $@ -c $<

#-------------------------------------------------------------
#   build release version
#-------------------------------------------------------------
rel: FORCE
	@$(MAKE) ECDBGFLAGS='$(ECREL_VALUE)' \
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
#   build static profiling version
#-------------------------------------------------------------


prof: FORCE
	@echo $(TARGET_NAME_STATIC);
	@$(MAKE) DBGFLAGS='$(PROF_VAL)' \
			 DBGDIR='$(PROFDIR_VALUE)' \
			 RELSUBDIR='$(RELSUBDIR_PROFVALUE)' \
			 $(RELSUBDIR_PROFVALUE)/$(TARGET_NAME_STATIC);

#-------------------------------------------------------------
#   build all
#   create dependencies file first (needed by build server)
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
	for FILE in $$FILES; \
	do \
		OBJNAME='$$(RELSUBDIR)/'`echo "$$FILE" | sed -e "s/\.[cep]\{1,3\}$$/\.o/"`; \
		EXT=`echo "$$FILE" | $(AWK) -F. '{print $$NF}'`; \
		case $$EXT in \
			ec) LANGUAGE="-x c";; \
			* ) LANGUAGE="";; \
		esac; \
		echo "gcc -MM -MT $$OBJNAME $(INCS) $$LANGUAGE $$FILE 1>>deps;"; \
		gcc -MM -MT $$OBJNAME $(INCS) $$LANGUAGE $$FILE 1>>deps; \
	done;

#-------------------------------------------------------------
# check if file "deps" exists
#-------------------------------------------------------------
deps:
	@touch deps;

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

clean_prof: FORCE
	@$(MAKE) .clean RELSUBDIR='$(RELSUBDIR_PROFVALUE)'

clean: clean_rel clean_dbg clean_prof

#-------------------------------------------------------------
#   Usage
#-------------------------------------------------------------
help: FORCE
	@echo "  make              : update (compile and link) $(TARGET_NAME) (debug)";
	@echo "  make all          : update (compile and link) $(TARGET_NAME) (all versions)";
	@echo "  make rel          : create release version  ($(RELSUBDIR_RELVALUE)/$(TARGET_NAME))";
	@echo "  make dbg          : create debug version ($(RELSUBDIR_RELVALUE)/$(TARGET_NAME))";
	@echo "";
	@echo "  make clean        : delete all versions";
	@echo "  make clean_rel    : delete release version ($(RELSUBDIR_RELVALUE))";
	@echo "  make clean_dbg    : delete debug version ($(RELSUBDIR_DBGVALUE))";
	@echo "";
	@echo "  make dep          : scan source files to create dependencies in file \"deps\"";
	@echo "";

#-------------------------------------------------------------
#   Dummy
#-------------------------------------------------------------
FORCE:


##############################################################
#    suffices                                                #
##############################################################

#-------------------------------------------------------------
#   Undefined
#-------------------------------------------------------------
.c:

#-------------------------------------------------------------
#   File Suffixes
#-------------------------------------------------------------
.SUFFIXES:
.SUFFIXES: .o .ec .cpp
