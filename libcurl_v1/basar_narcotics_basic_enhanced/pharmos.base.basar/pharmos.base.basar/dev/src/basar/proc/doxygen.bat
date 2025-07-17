@echo off

set src_dir=%CD%\..
set ext_dir=%src_dir%\..\ext
set doc_dir=%src_dir%\doxygen

set DOXYGEN_TOOLS_DIR=%ext_dir%\doxygen
set DOXYGEN_INCLUDE_DIR=%src_dir%\basar\doxygen
set DOXYGEN_INPUT_DIR=%src_dir%\basar
set DOXYGEN_OUTPUT_DIR=%doc_dir%

rmdir /S /Q %doc_dir% >NUL 2>NUL
mkdir %doc_dir% >NUL 2>NUL

echo Generating Documentation

set PATH=%DOXYGEN_TOOLS_DIR%;%PATH%

doxygen %src_dir%\basar\doxygen\doxyfile >%doc_dir%\doxygen_log.txt
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

del *.tmp >NUL 2>NUL
