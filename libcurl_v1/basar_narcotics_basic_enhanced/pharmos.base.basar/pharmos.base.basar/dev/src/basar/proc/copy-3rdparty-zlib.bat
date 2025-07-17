@echo off

rem echo %1 %2 %3

set Platform=%1
set Configuration=%2
set SolutionDir=%3

set zlib_dir=%SolutionDir%\..\..\ext\zlib
set install_dir=%SolutionDir%\..\sharedlib

set suffix=
if "%Configuration%" == "Debug" (
    set suffix=d
    goto :SUFFIX_SET
)

:SUFFIX_SET

echo -----------------------------------------------------
echo  copy zlib files to sharedlib directory 
echo       from %zlib_dir%
echo       to   %install_dir%
echo -----------------------------------------------------

REM dll files
xcopy /Y /D /Q %zlib_dir%\bin\%Configuration%\zlib%suffix%.dll %install_dir%\%Configuration%\
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

echo -----------------------------------------------------
echo  done
echo -----------------------------------------------------
echo.
