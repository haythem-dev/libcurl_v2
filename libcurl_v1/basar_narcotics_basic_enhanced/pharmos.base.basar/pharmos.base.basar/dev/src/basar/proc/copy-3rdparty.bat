@echo off

rem echo %1 %2 %3

set Platform=%1
set Configuration=%2
set SolutionDir=%3

echo ##########################################################################
echo  copy all necessary 3rdparty files
echo.

cmd /c %SolutionDir%\proc\copy-3rdparty-ghostscript.bat %Platform% %Configuration% %SolutionDir%
cmd /c %SolutionDir%\proc\copy-3rdparty-log4cplus.bat %Platform% %Configuration% %SolutionDir%
cmd /c %SolutionDir%\proc\copy-3rdparty-qt.bat %Platform% %Configuration% %SolutionDir%
cmd /c %SolutionDir%\proc\copy-3rdparty-zlib.bat %Platform% %Configuration% %SolutionDir%
cmd /c %SolutionDir%\proc\copy-3rdparty-openssl.bat %Platform% %Configuration% %SolutionDir%
cmd /c %SolutionDir%\proc\copy-3rdparty-libssh2.bat %Platform% %Configuration% %SolutionDir%

echo  done
echo ##########################################################################
echo.
