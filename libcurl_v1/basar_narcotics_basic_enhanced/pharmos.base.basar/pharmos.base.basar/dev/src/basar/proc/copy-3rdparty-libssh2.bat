@echo off

rem echo %1 %2 %3

set Platform=%1
set Configuration=%2
set SolutionDir=%3

set libssh2_dir=%SolutionDir%\..\..\ext\libssh2
set install_dir=%SolutionDir%\..\sharedlib

echo -----------------------------------------------------
echo  copy libssh2 files to sharedlib directory 
echo       from %libssh2_dir%
echo       to   %install_dir%
echo -----------------------------------------------------

REM dll files
xcopy /Y /D /Q %libssh2_dir%\bin\%Configuration%\libssh2*.*	%install_dir%\%Configuration%\
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

echo -----------------------------------------------------
echo  done
echo -----------------------------------------------------
echo.
