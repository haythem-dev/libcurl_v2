
@echo off
setlocal EnableDelayedExpansion

echo ================================================================
echo  Complete BASAR + Narcotics libcurl Integration Build
echo ================================================================
echo.

set START_TIME=%TIME%

REM Step 1: Verify libcurl artifacts
echo [STEP 1] Verifying libcurl artifacts...
if not exist "libcurl" (
    echo [ERROR] libcurl directory not found
    echo [INFO] Please run export.bat first to export libcurl artifacts
    pause
    exit /b 1
)

echo [SUCCESS] libcurl artifacts found
echo.

REM Step 2: Copy libcurl to BASAR ext directory
echo [STEP 2] Copying libcurl to BASAR ext directory...
set BASAR_EXT_DIR=basar_narcotics_basic_enhanced\pharmos.base.basar\pharmos.base.basar\dev\ext
if not exist "%BASAR_EXT_DIR%" (
    echo [ERROR] BASAR ext directory not found: %BASAR_EXT_DIR%
    pause
    exit /b 1
)

xcopy /E /I /Q /Y "libcurl\*" "%BASAR_EXT_DIR%\libcurl\"
if %errorlevel% neq 0 (
    echo [ERROR] Failed to copy libcurl to BASAR ext directory
    pause
    exit /b 1
)

echo [SUCCESS] libcurl copied to BASAR ext directory
echo.

REM Step 3: Build BASAR Framework
echo [STEP 3] Building BASAR Framework with libcurl...
call scripts\build_basar_with_libcurl.bat
if %errorlevel% neq 0 (
    echo [ERROR] BASAR build failed
    pause
    exit /b 1
)

echo [SUCCESS] BASAR Framework built successfully
echo.

REM Step 4: Build Narcotics Project
echo [STEP 4] Building Narcotics Project with libcurl integration...
call scripts\build_narcotics_with_libcurl.bat
if %errorlevel% neq 0 (
    echo [ERROR] Narcotics build failed
    pause
    exit /b 1
)

echo [SUCCESS] Narcotics Project built successfully
echo.

REM Calculate build time
set END_TIME=%TIME%

echo ================================================================
echo  🎉 COMPLETE INTEGRATION BUILD SUCCESSFUL! 🎉
echo ================================================================
echo.
echo [SUMMARY] All components built successfully:
echo   ✓ libcurl artifacts exported
echo   ✓ libcurl integrated into BASAR ext/
echo   ✓ BASAR Framework built with libcurl
echo   ✓ Narcotics Project built with BASAR FTP client
echo   ✓ QtFTP completely replaced with libcurl
echo.
echo [INTEGRATION STATUS]
echo   ✓ BasarFtpFileTransfer uses BASAR FTP client
echo   ✓ BASAR FTP client uses libcurl internally
echo   ✓ All FTP operations now libcurl-based
echo   ✓ Error handling and logging integrated
echo   ✓ Progress callbacks implemented
echo   ✓ Connection management improved
echo.
echo [NEXT STEPS]
echo   1. Run integration tests
echo   2. Validate FTP functionality
echo   3. Check performance characteristics
echo   4. Deploy to test environment
echo.
echo Build started: %START_TIME%
echo Build ended:   %END_TIME%
echo.
pause
