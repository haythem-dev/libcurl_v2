@echo off

rem echo %1 %2 %3 %4 %5 %6

set ProjectName=%1
set Platform=%2
set Configuration=%3
set SolutionDir=%4
set OutDir=%5
set QTDIR=%6

echo --------------------------------------------------------------------------
echo  copy headers and librarys of "%ProjectName%" for %Platform% and %Configuration%
echo    SolutionDir = "%SolutionDir%"
echo    OutDir      = "%OutDir%"
echo --------------------------------------------------------------------------
echo.

REM header files
xcopy /Y /D .\libbasar*.h     %SolutionDir%library\include\
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
xcopy /Y /D ..\libbasar*.h    %SolutionDir%library\include\
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
xcopy /Y /D .\libbasar*.hpp   %SolutionDir%library\include\
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
xcopy /Y /D ..\libbasar*.hpp  %SolutionDir%library\include\
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

mkdir %SolutionDir%library\lib\%Platform%\%Configuration% >NUL 2>NUL

xcopy /Y /D %OutDir%%ProjectName%.lib %SolutionDir%library\lib\%Platform%\%Configuration%\
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

if exist %OutDir%%ProjectName%.dll (
    xcopy /Y /D %OutDir%%ProjectName%.dll %SolutionDir%..\sharedlib\%Configuration%\
    if %ERRORLEVEL% neq 0 exit %ERRORLEVEL%
    xcopy /Y /D %OutDir%%ProjectName%.pdb %SolutionDir%..\sharedlib\%Configuration%\
    if %ERRORLEVEL% neq 0 exit %ERRORLEVEL%
)

REM multi-language qm files (Qt)
xcopy /Y /D %OutDir%*.qm %SolutionDir%..\sharedlib\multilang\
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

REM basar xml config file
xcopy /Y /D %SolutionDir%data\basar.config.xml %SolutionDir%..\sharedlib\%Configuration%\
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

echo.

REM copy all necessary 3rdparty files
cmd /c %SolutionDir%\proc\copy-3rdparty.bat %Platform% %Configuration% %SolutionDir%

echo --------------------------------------------------------------------------
echo  done
echo --------------------------------------------------------------------------
echo.
