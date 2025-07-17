
@echo off
setlocal EnableDelayedExpansion

echo [INFO] Building BASAR Framework with libcurl Integration
echo =====================================================

REM Configuration
set BASAR_ROOT=%CD%\basar_narcotics_basic_enhanced\pharmos.base.basar\pharmos.base.basar
set BUILD_CONFIG=Release
set VS_VERSION=2022

echo [INFO] BASAR Root: %BASAR_ROOT%
echo [INFO] Build Configuration: %BUILD_CONFIG%
echo [INFO] Visual Studio: %VS_VERSION%
echo.

REM Check if BASAR source exists
if not exist "%BASAR_ROOT%\dev\src\basar" (
    echo [ERROR] BASAR source directory not found
    echo [ERROR] Expected: %BASAR_ROOT%\dev\src\basar
    pause
    exit /b 1
)

REM Check if libcurl is available
if not exist "%BASAR_ROOT%\dev\ext\libcurl" (
    echo [ERROR] libcurl not found in ext directory
    echo [ERROR] Expected: %BASAR_ROOT%\dev\ext\libcurl
    echo [INFO] Please run export.bat first to copy libcurl artifacts
    pause
    exit /b 1
)

REM Verify libcurl installation
if not exist "%BASAR_ROOT%\dev\ext\libcurl\include\curl\curl.h" (
    echo [ERROR] libcurl headers not found
    echo [ERROR] Expected: %BASAR_ROOT%\dev\ext\libcurl\include\curl\curl.h
    pause
    exit /b 1
)

if not exist "%BASAR_ROOT%\dev\ext\libcurl\lib\libcurl.lib" (
    echo [ERROR] libcurl library not found
    echo [ERROR] Expected: %BASAR_ROOT%\dev\ext\libcurl\lib\libcurl.lib
    pause
    exit /b 1
)

echo [SUCCESS] libcurl integration verified
echo.

REM Check for Visual Studio
where cl >nul 2>&1
if %errorlevel% neq 0 (
    echo [ERROR] Visual Studio compiler not found
    echo [INFO] Please run from Visual Studio Developer Command Prompt
    pause
    exit /b 1
)

echo [INFO] Building BASAR Framework...
cd /d "%BASAR_ROOT%\dev\src\basar"

REM Check for solution file
if exist "basar.sln" (
    echo [INFO] Using Visual Studio solution: basar.sln
    
    echo [INFO] Building libbasarcmnutil (contains FTP client)...
    msbuild basar.sln /p:Configuration=%BUILD_CONFIG% /p:Platform=x64 /t:libbasarcmnutil /m
    
    if %errorlevel% neq 0 (
        echo [ERROR] libbasarcmnutil build failed
        goto :build_error
    )
    
    echo [SUCCESS] libbasarcmnutil built successfully
    echo.
    
    echo [INFO] Building complete BASAR solution...
    msbuild basar.sln /p:Configuration=%BUILD_CONFIG% /p:Platform=x64 /m
    
    if %errorlevel% neq 0 (
        echo [ERROR] BASAR solution build failed
        goto :build_error
    )
    
) else if exist "makefile" (
    echo [INFO] Using Makefile build
    
    REM Set environment for libcurl
    set LIBCURL_ROOT=%BASAR_ROOT%\dev\ext\libcurl
    set LIBCURL_INCLUDE=%LIBCURL_ROOT%\include
    set LIBCURL_LIB=%LIBCURL_ROOT%\lib
    
    make clean
    make all
    
    if %errorlevel% neq 0 (
        echo [ERROR] Makefile build failed
        goto :build_error
    )
    
) else (
    echo [ERROR] No build system found (basar.sln or makefile)
    pause
    exit /b 1
)

echo.
echo [SUCCESS] BASAR Framework build completed successfully!
echo.
echo [INFO] Build Summary:
echo   - libcurl integration: ✓ Verified
echo   - libbasarcmnutil: ✓ Built (contains FTP client)
echo   - BASAR Framework: ✓ Complete
echo.
echo [INFO] Next step: Build narcotics project
echo   Run: scripts\build_narcotics_with_libcurl.bat
echo.
goto :end

:build_error
echo.
echo [ERROR] Build failed!
echo.
echo [INFO] Troubleshooting tips:
echo   1. Ensure Visual Studio 2022 is properly installed
echo   2. Run from Visual Studio Developer Command Prompt
echo   3. Check libcurl integration in ext/libcurl
echo   4. Verify all dependencies are available
echo.
pause
exit /b 1

:end
pause
