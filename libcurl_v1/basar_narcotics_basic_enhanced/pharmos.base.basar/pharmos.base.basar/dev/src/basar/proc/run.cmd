REM runs unit tests
REM %1 : working directory
REM %2 : target
REM %3 : target options

cd %1
%2 %3
if ERRORLEVEL 1 exit 1
