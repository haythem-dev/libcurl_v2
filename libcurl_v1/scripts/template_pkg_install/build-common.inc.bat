
set src_dir=%CD%
set install_dir=%CD%\pkg-install

rem force 64bit build if 64bit requested (we need to call vsvars32.bat before to get VSINSTALLDIR)
call "%VS140COMNTOOLS%vsvars32.bat"
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

rem Parse parameter set by ant script. Defaults to 64-bit build
if "%~1"=="win32" (
    echo. >NUL
) else (
    call "%VSINSTALLDIR%VC\Auxiliary\Build\vcvars64.bat"
    if %ERRORLEVEL% neq 0 exit %ERRORLEVEL%
)

echo Looking for the latest installed Windows 10 SDK...

setlocal enabledelayedexpansion
set "kitsRoot=C:\Program Files (x86)\Windows Kits\10\Include"

REM Detect latest SDK version
for /f "tokens=*" %%i in ('dir /b /ad "%kitsRoot%" ^| sort /r') do (
    set "sdkVer=%%i"
    goto :foundSDK
)

echo No Windows 10 SDK found in %kitsRoot%
exit /b 1

:foundSDK
echo Found Windows SDK version: !sdkVer!

REM Add include paths
set "INCLUDE=%INCLUDE%;%kitsRoot%\!sdkVer!\ucrt"
set "INCLUDE=%INCLUDE%;%kitsRoot%\!sdkVer!\shared"
set "INCLUDE=%INCLUDE%;%kitsRoot%\!sdkVer!\um"
set "INCLUDE=%INCLUDE%;%kitsRoot%\!sdkVer!\winrt"
set "INCLUDE=%INCLUDE%;%kitsRoot%\!sdkVer!\cppwinrt"

REM Add lib paths
set "LIB=%LIB%;C:\Program Files (x86)\Windows Kits\10\Lib\!sdkVer!\ucrt\x64"
set "LIB=%LIB%;C:\Program Files (x86)\Windows Kits\10\Lib\!sdkVer!\um\x64"

endlocal & (
    set "INCLUDE=%INCLUDE%"
    set "LIB=%LIB%"
)

set QMAKE=%install_dir%\bin\qmake.exe
