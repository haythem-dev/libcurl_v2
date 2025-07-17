
@echo off
setlocal EnableDelayedExpansion

echo [INFO] Building Narcotics Project with libcurl Integration
echo ======================================================

REM Configuration
set NARCOTICS_ROOT=%CD%\basar_narcotics_basic_enhanced\pharmos.outbound.narcotics\pharmos.outbound.narcotics
set BASAR_ROOT=%CD%\basar_narcotics_basic_enhanced\pharmos.base.basar\pharmos.base.basar
set BUILD_CONFIG=Release

echo [INFO] Narcotics Root: %NARCOTICS_ROOT%
echo [INFO] BASAR Root: %BASAR_ROOT%
echo [INFO] Build Configuration: %BUILD_CONFIG%
echo.

REM Check if narcotics source exists
if not exist "%NARCOTICS_ROOT%\dev\src\narcotic" (
    echo [ERROR] Narcotics source directory not found
    echo [ERROR] Expected: %NARCOTICS_ROOT%\dev\src\narcotic
    pause
    exit /b 1
)

REM Check if BASAR is built
if not exist "%BASAR_ROOT%\dev\src\basar" (
    echo [ERROR] BASAR source directory not found
    echo [ERROR] Please build BASAR first with build_basar_with_libcurl.bat
    pause
    exit /b 1
)

REM Verify BasarFtpFileTransfer exists with libcurl integration
if not exist "%NARCOTICS_ROOT%\dev\src\narcotic\basarftpfiletransfer.cpp" (
    echo [ERROR] BasarFtpFileTransfer implementation not found
    echo [ERROR] Expected: %NARCOTICS_ROOT%\dev\src\narcotic\basarftpfiletransfer.cpp
    pause
    exit /b 1
)

REM Check for Visual Studio
where cl >nul 2>&1
if %errorlevel% neq 0 (
    echo [ERROR] Visual Studio compiler not found
    echo [INFO] Please run from Visual Studio Developer Command Prompt
    pause
    exit /b 1
)

echo [INFO] Verifying libcurl integration...

REM Check if basarftpfiletransfer.cpp contains libcurl integration
findstr /C:"basar::cmnutil::createFtpClient" "%NARCOTICS_ROOT%\dev\src\narcotic\basarftpfiletransfer.cpp" >nul
if %errorlevel% neq 0 (
    echo [ERROR] BasarFtpFileTransfer not updated for libcurl
    echo [ERROR] Please ensure QtFTP has been replaced with BASAR FTP client
    pause
    exit /b 1
)

echo [SUCCESS] libcurl integration verified in BasarFtpFileTransfer
echo.

echo [INFO] Building Narcotics Project...
cd /d "%NARCOTICS_ROOT%\dev\src"

REM Check for solution file
if exist "narcotics.sln" (
    echo [INFO] Using Visual Studio solution: narcotics.sln
    
    REM Set BASAR paths for linking
    set BASAR_INCLUDE=%BASAR_ROOT%\dev\src
    set BASAR_LIB=%BASAR_ROOT%\dev\src\basar\%BUILD_CONFIG%
    set LIBCURL_ROOT=%BASAR_ROOT%\dev\ext\libcurl
    
    echo [INFO] Building libnarcotics...
    msbuild narcotics.sln /p:Configuration=%BUILD_CONFIG% /p:Platform=x64 /t:libnarcotics /m
    
    if %errorlevel% neq 0 (
        echo [ERROR] libnarcotics build failed
        goto :build_error
    )
    
    echo [SUCCESS] libnarcotics built successfully
    echo.
    
    echo [INFO] Building narcotic module...
    msbuild narcotics.sln /p:Configuration=%BUILD_CONFIG% /p:Platform=x64 /t:narcotic /m
    
    if %errorlevel% neq 0 (
        echo [ERROR] narcotic module build failed
        goto :build_error
    )
    
    echo [SUCCESS] narcotic module built successfully
    echo.
    
    echo [INFO] Building complete narcotics solution...
    msbuild narcotics.sln /p:Configuration=%BUILD_CONFIG% /p:Platform=x64 /m
    
    if %errorlevel% neq 0 (
        echo [ERROR] Narcotics solution build failed
        goto :build_error
    )
    
) else if exist "makefile" (
    echo [INFO] Using Makefile build
    
    REM Set environment variables for BASAR and libcurl
    set BASAR_ROOT=%BASAR_ROOT%
    set LIBCURL_ROOT=%BASAR_ROOT%\dev\ext\libcurl
    
    make clean
    make all
    
    if %errorlevel% neq 0 (
        echo [ERROR] Makefile build failed
        goto :build_error
    )
    
) else (
    echo [ERROR] No build system found (narcotics.sln or makefile)
    pause
    exit /b 1
)

echo.
echo [SUCCESS] Narcotics Project build completed successfully!
echo.
echo [INFO] Build Summary:
echo   - BASAR integration: ✓ Verified
echo   - libcurl FTP client: ✓ Integrated
echo   - QtFTP replacement: ✓ Complete
echo   - BasarFtpFileTransfer: ✓ Updated
echo   - libnarcotics: ✓ Built
echo   - narcotic module: ✓ Built
echo   - Complete solution: ✓ Built
echo.
echo [INFO] Integration Test Recommendations:
echo   1. Test FTP connection establishment
echo   2. Verify file upload/download operations
echo   3. Check error handling and logging
echo   4. Validate backward compatibility
echo.
goto :end

:build_error
echo.
echo [ERROR] Build failed!
echo.
echo [INFO] Troubleshooting tips:
echo   1. Ensure BASAR framework is built first
echo   2. Check BasarFtpFileTransfer libcurl integration
echo   3. Verify BASAR library paths are accessible
echo   4. Ensure libcurl is properly integrated in BASAR
echo   5. Check for missing dependencies or headers
echo.
pause
exit /b 1

:end
pause
