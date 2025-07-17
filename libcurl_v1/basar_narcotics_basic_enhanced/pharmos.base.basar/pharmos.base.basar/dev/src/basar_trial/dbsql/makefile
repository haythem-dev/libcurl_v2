##############################################################
#                                                            #
#   testdbsql  makefile (Solaris)                            #
#	Michael Eichenlaub                                       #
#	18.07.2005                                               #
#                                                            #
##############################################################

#-------------------------------------------------------------
#    target
#-------------------------------------------------------------
TARGET_NAME = testdbsql

#-------------------------------------------------------------
#   flags
#-------------------------------------------------------------
REL_VALUE    = -library=stlport4 -m64 -DNDEBUG -O
DBG_VALUE    = -library=stlport4 -m64 -g
GCCREL_VALUE = -DNDEBUG -O -ansi -Wall -pedantic -fPIC
GCCDBG_VALUE = -g -ansi -Wall -pedantic -fPIC
DBGFLAGS     = $(DBG_VALUE)

RELDIR_VALUE     = release
DBGDIR_VALUE     = debug
GCCRELDIR_VALUE  = release/gcc
GCCDBGDIR_VALUE  = debug/gcc
DBGDIR           = $(DBGDIR_VALUE)

LDFLAGS     = -m64 -library=stlport4
CPPFLAGS    = $(INCS) $(OUTDIR) $(DBGFLAGS)

#-------------------------------------------------------------
#   libraries, lib path, include path
#-------------------------------------------------------------

# shared library paths
LIBS_BASAR_DIR_BASE     = ../../basar/library
LIBS_BASAR_DIR          = $(LIBS_BASAR_DIR_BASE)/lib/$(DBGDIR)

LIBS_LOG4CPLUS_DIR_BASE = ${DEVLIB_PATH}/marathon/lib/log4cplus/1.0.4_2
LIBS_LOG4CPLUS_DIR      = $(LIBS_LOG4CPLUS_DIR_BASE)/lib/$(DBGDIR)

LIBS_BOOST_DIR_BASE     = ${DEVLIB_PATH}/marathon/lib/boost/1.41.0_2
LIBS_BOOST_DIR          = $(LIBS_BOOST_DIR_BASE)/lib

LIBS_CICS_DIR_BASE      = ${DEVLIB_PATH}/marathon/lib/tcaccess/8.0.6.3
LIBS_CICS_DIR           = $(LIBS_CICS_DIR_BASE)/lib

LIBS_DIR                = -L$(LIBS_BASAR_DIR) \
                          -L$(LIBS_LOG4CPLUS_DIR) \
                          -L$(LIBS_BOOST_DIR) \
                          -L$(LIBS_CICS_DIR)

# shared libraries
LIBS_SYSTEM        = -lc
LIBS               = -lbasarcmnutil -llog4cplus -lbasardbsql $(LIBS_SYSTEM)

# include paths
INCS_BASAR         = $(LIBS_BASAR_DIR_BASE)/include
INCS_BOOST         = $(LIBS_BOOST_DIR_BASE)
INCS_LOG4CPLUS     = $(LIBS_LOG4CPLUS_DIR_BASE)/include
INCS               = -I. -I$(INCS_BASAR) -I$(INCS_BOOST) -I$(INCS_LOG4CPLUS)

OUTDIR             = -o ./$@
CMD_PATH           = LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:$(LIBS_BASAR_DIR):$(LIBS_LOG4CPLUS_DIR):$(LIBS_BOOST_DIR):$(LIBS_CICS_DIR); \
                     export LD_LIBRARY_PATH
CMD_TARGET         = ./$(TARGET_NAME)
CMD_START          = $(CMD_PATH); $(CMD_TARGET)

#-------------------------------------------------------------
#    object files 
#-------------------------------------------------------------
PROJ_SRC_LIB = 

PROJ_SRC_CPP = testdbsql.cpp \
               misc.cpp \
               connection.cpp \
               statement_infx.cpp \
               statement_odbc.cpp \
               version.cpp \
               prepstmt.cpp

PROJ_OBJS    = $(PROJ_SRC_CPP:.cpp=.o)
PROJ_LIBS    = $(PROJ_SRC_LIB)

#-------------------------------------------------------------
#   tools
#-------------------------------------------------------------
# CC        : Forte C++ compiler/linker
# strip     : strip symbol table, debugging and line number information from an object file

GCC_CC   = gcc
GCC_CPP  = g++
GCC_LINK = g++

LINK  = CC
CPP   = CC
STRIP = strip

#-------------------------------------------------------------
#   default goal
#-------------------------------------------------------------
$(TARGET_NAME): $(PROJ_OBJS) $(PROJ_LIBS)
	@echo "  linking $(TARGET_NAME) ...";
	$(LINK) -o $(TARGET_NAME) $(LIBS_DIR) $(LDFLAGS) $(PROJ_OBJS) $(PROJ_LIBS) $(LIBS);
	@echo "";
	ls -l $(TARGET_NAME);
	@echo "";
	@if [ "$(DBGFLAGS)" = "$(REL_VALUE)" ]; \
	then $(STRIP) $(TARGET_NAME); \
	    echo "$(STRIP) $(TARGET_NAME)"; \
	    ls -l $(TARGET_NAME); \
	    echo ""; \
	fi;     
	@echo "";

#-------------------------------------------------------------
#   C++ sources
#-------------------------------------------------------------
.cpp.o:
	@echo "  compiling $< ...";
	$(CPP) $(CPPFLAGS) -c $<;
	@echo "";

#-------------------------------------------------------------
#   build release version
#-------------------------------------------------------------
release: FORCE
	@$(MAKE) DBGFLAGS='$(REL_VALUE)' DBGDIR='$(RELDIR_VALUE)' $(TARGET_NAME);

#-------------------------------------------------------------
#   build debug version: 
#-------------------------------------------------------------
debug: FORCE
	@$(MAKE) $(TARGET_NAME);

#-------------------------------------------------------------
#   build gcc release version
#-------------------------------------------------------------
gccrel: FORCE
	@$(MAKE) DBGFLAGS='$(GCCREL_VALUE)' DBGDIR='$(GCCRELDIR_VALUE)' LINK='$(GCC_LINK)' CPP='$(GCC_CPP)' CC='$(GCC_CC)' TEMP_FILES='' $(TARGET_NAME);

#-------------------------------------------------------------
#   build gcc debug version
#-------------------------------------------------------------
gccdbg: FORCE
	@$(MAKE) DBGFLAGS='$(GCCDBG_VALUE)' DBGDIR='$(GCCDBGDIR_VALUE)' LINK='$(GCC_LINK)' CPP='$(GCC_CPP)' CC='$(GCC_CC)' TEMP_FILES='' $(TARGET_NAME);

#-------------------------------------------------------------
#   remove remporary files
#-------------------------------------------------------------
clean: FORCE
	@if rm $(PROJ_OBJS) $(TARGET_NAME) 2>/dev/null; \
	then echo "  temporary files for $(TARGET_NAME) removed"; \
	else echo "  temporary files for $(TARGET_NAME) already removed"; \
	fi;
	
#-------------------------------------------------------------
#   start process
#-------------------------------------------------------------
start: FORCE
	@echo "  start $(CMD_TARGET) ...";
	@($(CMD_START));

#-------------------------------------------------------------
#   Usage
#-------------------------------------------------------------
help: FORCE
	@echo "  make           : update (compile and link) $(TARGET_NAME) (debug)";
	@echo "  make release   : create release version";
	@echo "  make debug     : create debug version";
	@echo "  make gccrel    : create gcc release version";
	@echo "  make gccdbg    : create gcc debug version";
	@echo "  make clean     : delete $(TARGET_NAME), *.o files";
	@echo "  make start     : start $(TARGET_NAME)";
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

#-------------------------------------------------------------
#   dependencies between object and header files
#-------------------------------------------------------------
# DEP_START

testdbsql.o      : $(INCS_BASAR)/libbasardbsql.h testdbsql.h
misc.o           : $(INCS_BASAR)/libbasardbsql.h testdbsql.h
connection.o     : $(INCS_BASAR)/libbasardbsql.h testdbsql.h
statement_infx.o : $(INCS_BASAR)/libbasardbsql.h $(INCS_BASAR)/libbasarcmnutil_decimal.h $(INCS_BASAR)/libbasarcmnutil_i18nstring.h testdbsql.h 
statement_odbc.o : $(INCS_BASAR)/libbasardbsql.h $(INCS_BASAR)/libbasarcmnutil_decimal.h $(INCS_BASAR)/libbasarcmnutil_i18nstring.h testdbsql.h 
version.o        : $(INCS_BASAR)/libbasardbsql.h testdbsql.h

# DEP_END
