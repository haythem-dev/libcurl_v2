@echo off

rem echo %1 %2 %3

set Platform=%1
set Configuration=%2
set SolutionDir=%3

set ghostscript_dir=%SolutionDir%\..\..\ext\ghostscript
set install_dir=%SolutionDir%\..\sharedlib

echo -----------------------------------------------------
echo  copy ghostscript files to sharedlib directory 
echo       from %ghostscript_dir%
echo       to   %install_dir%
echo -----------------------------------------------------

REM dll files
xcopy /Y /D /Q %ghostscript_dir%\bin\gsdll*.dll %install_dir%\%Configuration%\
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

REM fonts, lib, resource files
xcopy /Y /S /D /Q %ghostscript_dir%\lib %install_dir%\ghostscript\lib\
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

rmdir /S /Q %install_dir%\ghostscript\lib\cmake

echo -----------------------------------------------------
echo  done
echo -----------------------------------------------------
echo.
