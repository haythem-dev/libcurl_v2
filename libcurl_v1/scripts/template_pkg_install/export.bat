@echo off

set project=qt5
set src_dir=%CD%
set export_dir=%CD%\..\..\export\%project%

mkdir %export_dir%

echo zipping %export_dir%
pushd %src_dir%\pkg-install
7z.exe a %export_dir%\%project%.zip .
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
popd

copy %src_dir%\ivy.xml %export_dir%
copy %src_dir%\build.xml %export_dir%
