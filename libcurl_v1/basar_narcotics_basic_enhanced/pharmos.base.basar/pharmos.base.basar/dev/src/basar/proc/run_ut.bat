@echo off

set Configuration=Debug
REM set Configuration=Release

set Platform=x64
REM set Platform=

set Params=%*
REM set Params=-short

set proc_dir=%CD%
if exist %proc_dir%\basar.sln (
    set proc_dir=%CD%\proc
    goto procDirSet
)
if exist %proc_dir%\ivy.xml (
    set proc_dir=%CD%\basar\proc
    goto procDirSet
)

:procDirSet

set test_dir=%proc_dir%\..\test\unit
set sharedlib_dir=%proc_dir%\..\..\sharedlib\%Configuration%

REM TODO: start unittest cics automagically

echo Running Tests with %Configuration% configuration
echo.

set TestsFailed=0
set TestsSuccessful=0

REM loginui is excluded because its not an automatic test
for %%v in (appl cmnutil dbaspect dbsql guiqt guitie login property qtwidget) do (call :runTest %%v || exit %ERRORLEVEL%)

echo.
echo Finished running tests
call :echoYellow Result: %TestsSuccessful% Tests ran successful and %TestsFailed% failed.
echo.

pause
goto :eof

:runTest
    set test=%1

    call :echoYellow Running ut%test% %Params%

    pushd %sharedlib_dir% >NUL 2>NUL
    
    xcopy /Y /D %test_dir%\%test%\data\ut%test%.cfg %MARATHON_LOG_CONFIGDIR%\
    if %ERRORLEVEL% neq 0 (
        call :echoRed Copying test configuration failed: %ERRORLEVEL%
        exit 1
    )
    
    %proc_dir%\..\%Platform%\%Configuration%\ut%test%.exe %Params%
    if %ERRORLEVEL% neq 0 (
        call :echoRed Test failed: Exitcode %ERRORLEVEL%
        set /A TestsFailed=TestsFailed+1
    ) else (
        call :echoGreen Test successful
        set /A TestsSuccessful=TestsSuccessful+1
    )

    popd

    echo.
goto :eof

:echoRed
powershell -Command Write-Host "%*" -foreground "Red"
goto :eof

:echoYellow
powershell -Command Write-Host "%*" -foreground "Yellow"
goto :eof

:echoGreen
powershell -Command Write-Host "%*" -foreground "Green"
goto :eof
