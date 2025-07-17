#####################################################################
#                                                                   #
#	libbasarsqlinf makefile for informix esqlc sources (Windows)    #
#	Michael Eichenlaub                                              #
#	15.07.2005                                                      #
#                                                                   #
#####################################################################

#-------------------------------------------------------------
#   flags
#-------------------------------------------------------------
CFLAGS_COMMON  = /D_LIB /D_CRT_SECURE_NO_WARNINGS /D_WIN32 /W3 /WX -thread /TC
CFLAGS_DEBUG   = /Od /D_DEBUG /MDd /Zi
CFLAGS_RELEASE = /O2 /DNDEBUG /MD

LFLAGS_COMMON  = /SUBSYSTEM:CONSOLE
LFLAGS_X86     = /MACHINE:X86
LFLAGS_X64     = /MACHINE:X64

MAKEFLAGS_COMMON = \
    /nologo \
    IntDir=$(IntDir) \
    OutDir=$(OutDir) \
    TargetName=$(TargetName) \
    INFORMIXDIR="$(INFORMIXDIR)"

#-------------------------------------------------------------
#   libraries, lib path, include path
#-------------------------------------------------------------
LIBS           = isqlt09a.lib
LIBPATH        = /LIBPATH:"$(INFORMIXDIR)\lib"
PATH           = $(PATH);$(INFORMIXDIR)\bin

#-------------------------------------------------------------
#   object files
#-------------------------------------------------------------
TargetObjs = $(IntDir)\connect.obj \
             $(IntDir)\exception.obj \
             $(IntDir)\statement.obj \
             $(IntDir)\cursor.obj \
             $(IntDir)\descriptor.obj \
             $(IntDir)\profiling.obj

#-------------------------------------------------------------
#   tools (Microsoft)
#-------------------------------------------------------------
LINK     = lib
ESQL     = "$(INFORMIXDIR)\bin\esql"

#-------------------------------------------------------------
#   target rules
#-------------------------------------------------------------

$(IntDir):
	-mkdir $(IntDir) 2>NUL

$(OutDir):
	-mkdir $(OutDir) 2>NUL

$(OutDir)\$(TargetName).lib: $(IntDir) $(OutDir) $(TargetObjs)
    $(LINK) /nologo \
        /OUT:$@ \
        $(LIBPATH) \
        $(LFLAGS) \
        $(TargetObjs) \
        $(LIBS)

clean:
    @echo Cleaning $(OutDir)
    -del /Q /S $(IntDir)
    -del /Q $(OutDir)\$(TargetName).*

Release-x64-clean: clean

Release-x64-build:
    @$(MAKE) $(MAKEFLAGS_COMMON) \
        CFLAGS="$(CFLAGS_COMMON) $(CFLAGS_RELEASE)" \
        LFLAGS="$(LFLAGS_COMMON) $(LFLAGS_X64)" \
        $(OutDir)\$(TargetName).lib

Release-x64-rebuild: Release-x64-clean Release-x64-build

Debug-x64-clean: clean

Debug-x64-build:
    @$(MAKE) $(MAKEFLAGS_COMMON) \
        CFLAGS="$(CFLAGS_COMMON) $(CFLAGS_DEBUG)" \
        LFLAGS="$(LFLAGS_COMMON) $(LFLAGS_X64)" \
        $(OutDir)\$(TargetName).lib

Debug-x64-rebuild: Debug-x64-clean Debug-x64-build

Release-Win32-clean: clean

Release-Win32-build:
    @$(MAKE) $(MAKEFLAGS_COMMON) \
        CFLAGS="$(CFLAGS_COMMON) $(CFLAGS_RELEASE)" \
        LFLAGS="$(LFLAGS_COMMON) $(LFLAGS_X86)" \
        $(OutDir)\$(TargetName).lib

Release-Win32-rebuild: Release-Win32-clean Release-Win32-build

Debug-Win32-clean: clean

Debug-Win32-build:
    @$(MAKE) $(MAKEFLAGS_COMMON) \
        CFLAGS="$(CFLAGS_COMMON) $(CFLAGS_DEBUG)" \
        LFLAGS="$(LFLAGS_COMMON) $(LFLAGS_X86)" \
        $(OutDir)\$(TargetName).lib

Debug-Win32-rebuild: Debug-Win32-clean Debug-Win32-build

#-------------------------------------------------------------
#   suffix rules
#-------------------------------------------------------------

.ec{$(IntDir)}.obj:
    @for %%f in ($?) do @xcopy /Y /D %%f $(IntDir)
    (cd $(IntDir) && \
	$(ESQL) -dcmdl -mserr /nologo $(CFLAGS) /Fo$(@F) /Fd$(OutDir) -c $? )

#-------------------------------------------------------------
#   suffix
#-------------------------------------------------------------
.SUFFIXES:
.SUFFIXES: .ec 

#-------------------------------------------------------------
#   dependencies between object and header files
#-------------------------------------------------------------
# DEP_START

$(IntDir)\connect.obj: connect.ec esqlc.h
$(IntDir)\exception.obj: exception.ec esqlc.h
$(IntDir)\statement.obj: statement.ec esqlc.h
$(IntDir)\cursor.obj: cursor.ec esqlc.h profiling.h
$(IntDir)\descriptor.obj: descriptor.ec esqlc.h
$(IntDir)\profiling.obj: profiling.ec profiling.h

# DEP_END
