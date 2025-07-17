@echo off

set PWD=%CD%
set PROJECT=narcotics

set BASEDIR=%PWD%\..\..

set SRCDIR=%BASEDIR%\dev\src
set EXTDIR=%BASEDIR%\dev\ext
set DESTDIR=%BASEDIR%\export\%PROJECT%

echo -----------------------------------------------------
echo  copy %PROJECT% files to export directory 
echo       from %SRCDIR%
echo       to   %DESTDIR%
echo  compress export files to %DESTDIR%\built.zip
echo -----------------------------------------------------

rmdir /S /Q %DESTDIR% 2>nul
mkdir %DESTDIR%
mkdir %DESTDIR%\multilang
mkdir %DESTDIR%\print

echo copy binaries ...
copy /y %SRCDIR%\narcotic\release\*.exe  			%DESTDIR%
copy /y %SRCDIR%\narcotic\release\multilang\*.qm 	%DESTDIR%\multilang\
copy /y %SRCDIR%\print\*	             			%DESTDIR%\print\

cd %DESTDIR%\print
del *.pp*

cd %DESTDIR%
echo zip %DESTDIR%
7z a -sdel .\%PROJECT%.zip  *.exe .\multilang .\print

copy /y %SRCDIR%\build.xml %DESTDIR%
copy /y %SRCDIR%\ivy.xml %DESTDIR%

cd /D %PWD%
