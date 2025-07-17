@echo off

set arch=%1
set project=ssuk
set src_dir=%CD%\..
set pkginstall_dir=%src_dir%\pkg-install
set export_dir=%src_dir%\..\..\export\%project%

set arch_dir=Win32
if "%arch%" == "win64" (
    set arch_dir=x64
    goto :install
)

:install

rem ----

rmdir /S /Q %pkginstall_dir% >NUL
mkdir %pkginstall_dir% >NUL

xcopy /Y /E /S /D /EXCLUDE:%src_dir%\basar\proc\export.%arch%.excl %src_dir%\basar\library\include %pkginstall_dir%\basar\include\
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

xcopy /Y /E /S /D /EXCLUDE:%src_dir%\basar\proc\export.%arch%.excl %src_dir%\basar\library\lib\%arch_dir% %pkginstall_dir%\basar\lib\
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

xcopy /Y /E /S /D /EXCLUDE:%src_dir%\basar\proc\export.%arch%.excl %src_dir%\sharedlib %pkginstall_dir%\sharedlib\
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

xcopy /Y /D %src_dir%\basar\changelog.txt %pkginstall_dir%\doc\
xcopy /Y /D %src_dir%\..\docs\*.docx      %pkginstall_dir%\doc\

xcopy /Y /D %src_dir%\doxygen\basar.chm %pkginstall_dir%\doc\
xcopy /Y /D %src_dir%\doxygen\*.pdf     %pkginstall_dir%\doc\

rem ----

rmdir /S /Q %export_dir% >NUL
mkdir %export_dir% >NUL

echo zipping %pkginstall_dir%
pushd %pkginstall_dir% >NUL
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

7z.exe a %export_dir%\%project%.zip .
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

popd >NUL

copy %src_dir%\ivy.xml %export_dir%
copy %src_dir%\build.xml %export_dir%
