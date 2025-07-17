@echo off

set proc_dir=%CD%\proc

call %proc_dir%\build-common.inc.bat
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

pushd %CD%\qtbase*
    set base_src_dir=%CD%
popd
set base_build_dir=%CD%\build\qtbase

rem if exist %install_dir%\bin\qmake.exe goto qtbase_end
:qtbase
mkdir %base_build_dir% >NUL 2>&1
pushd %base_build_dir%

echo configuring qtbase

cmd /c %base_src_dir%\configure ^
    -prefix %install_dir% ^
    -platform win32-msvc2015 ^
    -opengl desktop ^
    -nomake examples ^
    -nomake tests ^
    -debug-and-release ^
    -qt-libjpeg ^
    -no-icu ^
    -plugin-sql-sqlite ^
    -plugin-sql-odbc ^
    -opensource ^
    -confirm-license ^
    -mp
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

echo done
echo.

echo building qtbase

nmake
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

echo done
echo.

echo installing qtbase

nmake install
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

echo done
echo.

xcopy /Q /Y /D %src_dir%\conf\qt.conf %install_dir%\bin\

popd
:qtbase_end
