rem @echo off
rem get vs2010 environment

rem %1 working directory
rem %2 dll to instrument
rem %3 test binary
rem %4 options?


call "%VS100COMNTOOLS%\vsvars32.bat"

set PATH=%INFORMIXDIR%\bin;%ProgramFiles%\tcACCESS;%PATH%

set PWD=%CD%

set DLL=libbasar%2.dll

rem delete old coverages? move to old?
rem instrument dll

cd %1

rem how to remove error: file already instrumented? - reinstate former file
rem how to use only one .bat file for instrumentation of all unit tests? - by configuration
rem how to remove output from system & 3rd party libs?
vsinstr.exe /coverage %DLL%



rem start vsperfmom /coverage /output:path_to_coverage
vsperfcmd /start:coverage /output:basar.%2

rem run unit test
%3 %4

rem shutdown vsperfmon
vsperfcmd /shutdown

rem how to display results convenient(?)

basar.%2.coverage

cd %PWD%