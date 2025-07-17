@echo off

rem echo %1 %2 %3

set Platform=%1
set Configuration=%2
set SolutionDir=%3

set log4cplus_dir=%SolutionDir%\..\..\ext\log4cplus
set install_dir=%SolutionDir%\..\sharedlib

echo -----------------------------------------------------
echo  copy log4cplus files to sharedlib directory 
echo       from %log4cplus_dir%
echo       to   %install_dir%
echo -----------------------------------------------------

REM dll files
xcopy /Y /D /Q %log4cplus_dir%\bin\%Configuration%\log4cplus*.*	%install_dir%\%Configuration%\
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

echo -----------------------------------------------------
echo  done
echo -----------------------------------------------------
echo.
