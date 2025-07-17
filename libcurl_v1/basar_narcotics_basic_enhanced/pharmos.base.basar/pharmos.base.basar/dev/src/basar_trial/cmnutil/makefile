##############################################################
#                                                            #
#   testcmnutil makefile (Solaris)                         #
#	Thomas Hörath                                            #
#	01.08.2005                                               #
#                                                            #
##############################################################

#-------------------------------------------------------------
#    target
#-------------------------------------------------------------
TARGET_NAME = testcmnutil

#-------------------------------------------------------------
#    misc
#-------------------------------------------------------------

# Sun WorkShop cache directory
SUNWS_FILES         = SunWS_cache ir.out

#-------------------------------------------------------------
#   flags
#-------------------------------------------------------------
REL_VALUE    = -DNDEBUG -O
DBG_VALUE    = -g
GCCREL_VALUE = -DNDEBUG -O -ansi -Wall -pedantic -fPIC
GCCDBG_VALUE = -g -ansi -Wall -pedantic -fPIC
DBGFLAGS     = $(DBG_VALUE)

RELDIR_VALUE     = release
DBGDIR_VALUE     = debug
GCCRELDIR_VALUE  = release/gcc
GCCDBGDIR_VALUE  = debug/gcc
DBGDIR           = $(DBGDIR_VALUE)

LDFLAGS     = -m64
CPPFLAGS    = $(INCS) $(OUTDIR) $(DBGFLAGS) -m64

#-------------------------------------------------------------
#   libraries, lib path, include path
#-------------------------------------------------------------

# shared library paths
LIBS_BASAR_DIR_BASE     = ../../../library/lib
LIBS_BASAR_DIR          = $(LIBS_BASAR_DIR_BASE)/$(DBGDIR)
LIBS_LOG4CPLUS_DIR_BASE = ../../../../log4cplus/lib
LIBS_LOG4CPLUS_DIR      = $(LIBS_LOG4CPLUS_DIR_BASE)/$(DBGDIR)
LIBS_DIR                = -L$(LIBS_BASAR_DIR) -L$(LIBS_LOG4CPLUS_DIR)

# shared libraries
LIBS_SYSTEM        = -lc
LIBS               = -llog4cplus -lbasarcmnutil $(LIBS_SYSTEM)

# include paths
INCS_BASAR         = ../../../library/include
INCS_BOOST         = ${DEVLIB_PATH}/marathon/lib/boost_1.32.0
INCS_LOG4CPLUS     = ../../../../log4cplus/include
INCS               = -I. -I$(INCS_BASAR) -I$(INCS_BOOST) -I$(INCS_LOG4CPLUS)

OUTDIR             = -o ./$@
CMD_PATH           = LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:$(LIBS_BASAR_DIR):$(LIBS_LOG4CPLUS_DIR); \
                     export LD_LIBRARY_PATH
CMD_TARGET         = ./$(TARGET_NAME)
CMD_START          = $(CMD_PATH); $(CMD_TARGET)

#-------------------------------------------------------------
#    object files 
#-------------------------------------------------------------
PROJ_SRC_LIB =
PROJ_SRC_CPP =	testcmnutil.cpp \
				exception.cpp \
				logging.cpp \
				print.cpp \
				printdata.cpp \
				process.cpp \
				email.cpp \
				codec.cpp \
				parameterlist.cpp \
				decimal.cpp \
				string.cpp \
				datetime.cpp

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
	@rm -r $(SUNWS_FILES) 2>/dev/null || true; \
	echo "  SunWS cached files $(SUNWS_FILES) for $(TARGET_NAME) removed";

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

testcmnutil.o:	$(INCS_BASAR)/libbasarcmnutil.h \
				testcmnutil.h

logging.o:      $(INCS_BASAR)/libbasarcmnutil.h \
				testcmnutil.h

print.o:		$(INCS_BASAR)/libbasarcmnutil.h \
				testcmnutil.h \
				printdata.h

printdata.o:    $(INCS_BASAR)/libbasarcmnutil.h \
				testcmnutil.h \
				printdata.h

process.o:      $(INCS_BASAR)/libbasarcmnutil.h \
				testcmnutil.h

email.o:        $(INCS_BASAR)/libbasarcmnutil.h \
				testcmnutil.h \
				$(INCS_BASAR)/libbasarcmnutil_email.h

codec.o:        $(INCS_BASAR)/libbasarcmnutil.h \
				testcmnutil.h \
				$(INCS_BASAR)/libbasarcmnutil_codec.h

exception.o:	$(INCS_BASAR)/libbasarcmnutil.h \
				testcmnutil.h

decimal.o:	    $(INCS_BASAR)/libbasarcmnutil.h \
				testcmnutil.h

string.o:	    $(INCS_BASAR)/libbasarcmnutil.h \
				testcmnutil.h

datetime.o:	    $(INCS_BASAR)/libbasarcmnutil.h \
				testcmnutil.h

parameterlist.o: $(INCS_BASAR)/libbasarcmnutil.h \
				 testcmnutil.h


# DEP_END
