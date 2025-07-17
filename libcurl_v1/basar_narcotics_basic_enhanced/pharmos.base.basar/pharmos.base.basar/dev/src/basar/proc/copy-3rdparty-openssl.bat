@echo off

rem echo %1 %2 %3

set Platform=%1
set Configuration=%2
set SolutionDir=%3

set openssl_dir=%SolutionDir%\..\..\ext\openssl
set install_dir=%SolutionDir%\..\sharedlib

echo -----------------------------------------------------
echo  copy openssl files to sharedlib directory 
echo       from %openssl_dir%
echo       to   %install_dir%
echo -----------------------------------------------------

REM dll files
xcopy /Y /D /Q %openssl_dir%\bin\libssl-1_1*.*      %install_dir%\%Configuration%\
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
xcopy /Y /D /Q %openssl_dir%\bin\libcrypto-1_1*.*   %install_dir%\%Configuration%\
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

echo -----------------------------------------------------
echo  done
echo -----------------------------------------------------
echo.
