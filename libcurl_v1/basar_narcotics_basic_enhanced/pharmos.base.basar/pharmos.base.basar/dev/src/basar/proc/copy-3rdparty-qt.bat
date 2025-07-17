@echo off

rem echo %1 %2 %3

set Platform=%1
set Configuration=%2
set SolutionDir=%3

set qt_dir=%SolutionDir%\..\..\ext\qt5
set install_dir=%SolutionDir%\..\sharedlib

set suffix=
if "%Configuration%" == "Debug" (
    set suffix=d
    goto :SUFFIX_SET
)

:SUFFIX_SET

echo -----------------------------------------------------
echo  copy QT files to sharedlib directory
echo       from %qt_dir%
echo       to   %install_dir%
echo -----------------------------------------------------

REM dll files
REM xcopy /Y /D /Q %qt_dir%\bin\libGLESv2%suffix%.*								%install_dir%\%Configuration%\
REM if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
REM xcopy /Y /D /Q %qt_dir%\bin\libEGL%suffix%.*  								%install_dir%\%Configuration%\
REM if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
xcopy /Y /D /Q %qt_dir%\bin\qt5gui%suffix%.*									%install_dir%\%Configuration%\
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
xcopy /Y /D /Q %qt_dir%\bin\qt5widgets%suffix%.*								%install_dir%\%Configuration%\
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
xcopy /Y /D /Q %qt_dir%\bin\qt5sql%suffix%.*									%install_dir%\%Configuration%\
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
xcopy /Y /D /Q %qt_dir%\bin\qt5core%suffix%.*									%install_dir%\%Configuration%\
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
REM xcopy /Y /D /Q %qt_dir%\bin\qt5script%suffix%.*								%install_dir%\%Configuration%\
REM if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
REM xcopy /Y /D /Q %qt_dir%\bin\qt5scripttools%suffix%.*						%install_dir%\%Configuration%\
REM if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
xcopy /Y /D /Q %qt_dir%\bin\qt5test%suffix%.*									%install_dir%\%Configuration%\
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
xcopy /Y /D /Q %qt_dir%\bin\qt5opengl%suffix%.*									%install_dir%\%Configuration%\
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
xcopy /Y /D /Q %qt_dir%\bin\qt5xml%suffix%.*									%install_dir%\%Configuration%\
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
REM xcopy /Y /D /Q %qt_dir%\bin\qt5xmlpatterns%suffix%.*						%install_dir%\%Configuration%\
REM if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
REM xcopy /Y /D /Q %qt_dir%\bin\qt5svg%suffix%.*								%install_dir%\%Configuration%\
REM if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
xcopy /Y /D /Q %qt_dir%\bin\qt5network%suffix%.*								%install_dir%\%Configuration%\
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
xcopy /Y /D /Q %qt_dir%\bin\qt5printsupport%suffix%.*							%install_dir%\%Configuration%\
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
xcopy /Y /D /Q %qt_dir%\bin\qt5serialport%suffix%.*								%install_dir%\%Configuration%\
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
xcopy /Y /D /Q %qt_dir%\plugins\sqldrivers\qsqlodbc%suffix%.*					%install_dir%\%Configuration%\sqldrivers\
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
xcopy /Y /D /Q %qt_dir%\plugins\platforms\qwindows%suffix%.*					%install_dir%\%Configuration%\platforms\
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
xcopy /Y /D /Q %qt_dir%\plugins\printsupport\windowsprintersupport%suffix%.*	%install_dir%\%Configuration%\printsupport\
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
xcopy /Y /D /Q %qt_dir%\plugins\imageformats\qico%suffix%.*						%install_dir%\%Configuration%\imageformats\
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
REM xcopy /Y /D /Q %qt_dir%\plugins\imageformats\qsvg%suffix%.*					%install_dir%\%Configuration%\imageformats\
REM if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

REM translation files
xcopy /Y /D /Q %qt_dir%\translations\qt*.qm										%install_dir%\multilang\
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

echo -----------------------------------------------------
echo  done
echo -----------------------------------------------------
echo.
