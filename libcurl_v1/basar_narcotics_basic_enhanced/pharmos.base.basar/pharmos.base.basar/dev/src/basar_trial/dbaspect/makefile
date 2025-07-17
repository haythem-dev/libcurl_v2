##############################################################
#                                                            #
#   testdbaspect makefile (Solaris)                         #
#	Roland Kiefert                                           #
#	20.07.2005                                               #
#                                                            #
##############################################################

#-------------------------------------------------------------
#    target
#-------------------------------------------------------------
TARGET_NAME = testdbaspect

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

LDFLAGS     = 
CPPFLAGS    = $(INCS) $(OUTDIR) $(DBGFLAGS)

#-------------------------------------------------------------
#   libraries, lib path, include path
#-------------------------------------------------------------

# shared library paths
LIBS_BASAR_DIR_BASE     = ../../../library/lib
LIBS_BASAR_DIR		    = $(LIBS_BASAR_DIR_BASE)/$(DBGDIR)
LIBS_LOG4CPLUS_DIR_BASE = ../../../../log4cplus/lib
LIBS_LOG4CPLUS_DIR      = $(LIBS_LOG4CPLUS_DIR_BASE)/$(DBGDIR)
LIBS_DIR                = -L$(LIBS_BASAR_DIR) -L$(LIBS_LOG4CPLUS_DIR)

# shared libraries
LIBS_SYSTEM        = -lc
LIBS               = -lbasarcmnutil -llog4cplus -lbasarproperty -lbasardbsql -lbasardbaspect $(LIBS_SYSTEM)

# include paths
INCS_BASAR         = ../../../library/include
INCS_BOOST         = ${DEVLIB_PATH}/marathon/lib/boost_1.32.0
INCS_DBASPECT      = ../../../libbasardbaspect
INCS_PROPERTY      = ../../../libbasarproperty
INCS_LOG4CPLUS     = ../../../../log4cplus/include
INCS_MISC          = ../../..
INCS               = -I. -I$(INCS_BASAR) -I$(INCS_MISC) -I$(INCS_PROPERTY) -I$(INCS_DBASPECT) -I$(INCS_BOOST) -I$(INCS_LOG4CPLUS)

OUTDIR             = -o ./$@
CMD_PATH           = LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:$(LIBS_BASAR_DIR):$(LIBS_LOG4CPLUS_DIR); \
                     export LD_LIBRARY_PATH
CMD_TARGET         = ./$(TARGET_NAME)
CMD_START          = $(CMD_PATH); $(CMD_TARGET)

#-------------------------------------------------------------
#    object files 
#-------------------------------------------------------------
PROJ_SRC_LIB =
PROJ_SRC_CPP = testlibbasardbaspect.cpp \
               classdefinition.cpp \
               check_accessor.cpp \
               check_filtered_iteration_erase.cpp \
               check_inserts.cpp \
               check_praepost_indecrement.cpp \
               check_transaction.cpp \
               check_yiterator_xiterator.cpp \
               datetime.cpp \
               check_policies.cpp\
               check_states.cpp

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

testlibbasardbaspect.o: $(INCS_DBASPECT)/libbasardbaspect.h \
                         $(INCS_BASAR)/libbasarcmnutil_pattern.h \
                         $(INCS_BASAR)/libbasardbaspect.h \
                         $(INCS_DBASPECT)/sqlstringbuilderimpl.h \
                         $(INCS_BASAR)/libbasardbaspect_sqlstringbuilder.h \
                         $(INCS_BASAR)/libbasardbaspect_accessmethod.h \
                         $(INCS_BASAR)/libbasardbaspect_accessor.h \
                         $(INCS_BASAR)/libbasardbaspect_manager.h \
                         usings.h \
                         classdefinition.h
classdefinition.o: $(INCS_DBASPECT)/libbasardbaspect.h \
                   $(INCS_BASAR)/libbasarcmnutil_pattern.h \
                   $(INCS_BASAR)/libbasardbaspect.h \
                   $(INCS_DBASPECT)/sqlstringbuilderimpl.h \
                   $(INCS_BASAR)/libbasardbaspect_sqlstringbuilder.h \
                   $(INCS_BASAR)/libbasardbaspect_accessmethod.h \
                   $(INCS_BASAR)/libbasardbaspect_accessor.h \
                   $(INCS_BASAR)/libbasardbaspect_manager.h \
                   usings.h \
                   classdefinition.h
                   
check_accessor.o: \
                   $(INCS_DBASPECT)/libbasardbaspect.h \
                   $(INCS_BASAR)/libbasarcmnutil_pattern.h \
                   $(INCS_BASAR)/libbasardbaspect.h \
                   $(INCS_DBASPECT)/sqlstringbuilderimpl.h \
                   $(INCS_BASAR)/libbasardbaspect_sqlstringbuilder.h \
                   $(INCS_BASAR)/libbasardbaspect_accessmethod.h \
                   $(INCS_BASAR)/libbasardbaspect_accessor.h \
                   $(INCS_BASAR)/libbasardbaspect_manager.h \
                   usings.h \
                   classdefinition.h
                              
check_filtered_iteration_erase.o: \
                   $(INCS_DBASPECT)/libbasardbaspect.h \
                   $(INCS_BASAR)/libbasarcmnutil_pattern.h \
                   $(INCS_BASAR)/libbasardbaspect.h \
                   $(INCS_DBASPECT)/sqlstringbuilderimpl.h \
                   $(INCS_BASAR)/libbasardbaspect_sqlstringbuilder.h \
                   $(INCS_BASAR)/libbasardbaspect_accessmethod.h \
                   $(INCS_BASAR)/libbasardbaspect_accessor.h \
                   $(INCS_BASAR)/libbasardbaspect_manager.h \
                   usings.h \
                   classdefinition.h

check_inserts.o: \
                   $(INCS_DBASPECT)/libbasardbaspect.h \
                   $(INCS_BASAR)/libbasarcmnutil_pattern.h \
                   $(INCS_BASAR)/libbasardbaspect.h \
                   $(INCS_DBASPECT)/sqlstringbuilderimpl.h \
                   $(INCS_BASAR)/libbasardbaspect_sqlstringbuilder.h \
                   $(INCS_BASAR)/libbasardbaspect_accessmethod.h \
                   $(INCS_BASAR)/libbasardbaspect_accessor.h \
                   $(INCS_BASAR)/libbasardbaspect_manager.h \
                   usings.h \
                   classdefinition.h

check_praepost_indecrement.o: \
                   $(INCS_DBASPECT)/libbasardbaspect.h \
                   $(INCS_BASAR)/libbasarcmnutil_pattern.h \
                   $(INCS_BASAR)/libbasardbaspect.h \
                   $(INCS_DBASPECT)/sqlstringbuilderimpl.h \
                   $(INCS_BASAR)/libbasardbaspect_sqlstringbuilder.h \
                   $(INCS_BASAR)/libbasardbaspect_accessmethod.h \
                   $(INCS_BASAR)/libbasardbaspect_accessor.h \
                   $(INCS_BASAR)/libbasardbaspect_manager.h \
                   usings.h \
                   classdefinition.h

check_transaction.o: \
                   $(INCS_DBASPECT)/libbasardbaspect.h \
                   $(INCS_BASAR)/libbasarcmnutil_pattern.h \
                   $(INCS_BASAR)/libbasardbaspect.h \
                   $(INCS_DBASPECT)/sqlstringbuilderimpl.h \
                   $(INCS_BASAR)/libbasardbaspect_sqlstringbuilder.h \
                   $(INCS_BASAR)/libbasardbaspect_accessmethod.h \
                   $(INCS_BASAR)/libbasardbaspect_accessor.h \
                   $(INCS_BASAR)/libbasardbaspect_manager.h \
                   usings.h \
                   classdefinition.h

check_yiterator_xiterator.o: \
                   $(INCS_DBASPECT)/libbasardbaspect.h \
                   $(INCS_BASAR)/libbasarcmnutil_pattern.h \
                   $(INCS_BASAR)/libbasardbaspect.h \
                   $(INCS_DBASPECT)/sqlstringbuilderimpl.h \
                   $(INCS_BASAR)/libbasardbaspect_sqlstringbuilder.h \
                   $(INCS_BASAR)/libbasardbaspect_accessmethod.h \
                   $(INCS_BASAR)/libbasardbaspect_accessor.h \
                   $(INCS_BASAR)/libbasardbaspect_manager.h \
                   usings.h \
                   classdefinition.h

datetime.o: \
                   $(INCS_DBASPECT)/libbasardbaspect.h \
                   $(INCS_BASAR)/libbasarcmnutil_pattern.h \
                   $(INCS_BASAR)/libbasardbaspect.h \
                   $(INCS_DBASPECT)/sqlstringbuilderimpl.h \
                   $(INCS_BASAR)/libbasardbaspect_sqlstringbuilder.h \
                   $(INCS_BASAR)/libbasardbaspect_accessmethod.h \
                   $(INCS_BASAR)/libbasardbaspect_accessor.h \
                   $(INCS_BASAR)/libbasardbaspect_manager.h \
                   usings.h \
                   classdefinition.h

check_policies.o: classdefinition.h

# DEP_END
