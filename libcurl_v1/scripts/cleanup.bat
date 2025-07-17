@echo off
REM libcurl Cleanup Script for Windows
REM Removes build artifacts, source code, and optionally installed files

setlocal EnableDelayedExpansion

REM Configuration
if "%INSTALL_PREFIX%"=="" set INSTALL_PREFIX=C:\libcurl
if "%DOWNLOAD_DIR%"=="" set DOWNLOAD_DIR=.\libcurl-build
if "%REMOVE_INSTALLED%"=="" set REMOVE_INSTALLED=no
if "%REMOVE_SOURCE%"=="" set REMOVE_SOURCE=yes
if "%REMOVE_ARCHIVES%"=="" set REMOVE_ARCHIVES=yes

echo [INFO] Starting libcurl cleanup process for Windows
echo Configuration:
echo   Download directory: %DOWNLOAD_DIR%
echo   Install prefix: %INSTALL_PREFIX%
echo   Remove source: %REMOVE_SOURCE%
echo   Remove archives: %REMOVE_ARCHIVES%
echo   Remove installed: %REMOVE_INSTALLED%
echo.

REM Confirm dangerous operations
if "%REMOVE_INSTALLED%"=="yes" (
    echo [WARNING] WARNING: This will remove libcurl from your system!
    echo [WARNING] This may break other software that depends on libcurl.
    echo.
    set /p "REPLY=Are you sure you want to continue? [y/N]: "
    if /i not "!REPLY!"=="y" (
        echo [INFO] Operation cancelled
        exit /b 0
    )
)

echo ================================================
echo [INFO] Cleaning up build artifacts...
echo ================================================

REM Find and clean source directories
set FOUND_DIRS=0

REM Check current directory
for /d %%i in (curl-*) do (
    if exist "%%i\CMakeLists.txt" (
        set /a FOUND_DIRS+=1
        echo [INFO] Found source directory: %%i
        if exist "%%i\build" (
            echo [INFO] Removing build directory: %%i\build
            rmdir /s /q "%%i\build" 2>nul
            if %errorlevel%==0 (
                echo [SUCCESS] Removed build directory
            ) else (
                echo [ERROR] Failed to remove build directory
            )
        )
        
        if "%REMOVE_SOURCE%"=="yes" (
            echo [INFO] Removing source directory: %%i
            rmdir /s /q "%%i" 2>nul
            if %errorlevel%==0 (
                echo [SUCCESS] Removed source directory
            ) else (
                echo [ERROR] Failed to remove source directory
            )
        ) else (
            echo [INFO] Keeping source directory: %%i
        )
    )
)

REM Check download directory
if exist "%DOWNLOAD_DIR%" (
    for /d %%i in (%DOWNLOAD_DIR%\curl-*) do (
        if exist "%%i\CMakeLists.txt" (
            set /a FOUND_DIRS+=1
            echo [INFO] Found source directory: %%i
            if exist "%%i\build" (
                echo [INFO] Removing build directory: %%i\build
                rmdir /s /q "%%i\build" 2>nul
                if %errorlevel%==0 (
                    echo [SUCCESS] Removed build directory
                ) else (
                    echo [ERROR] Failed to remove build directory
                )
            )
            
            if "%REMOVE_SOURCE%"=="yes" (
                echo [INFO] Removing source directory: %%i
                rmdir /s /q "%%i" 2>nul
                if %errorlevel%==0 (
                    echo [SUCCESS] Removed source directory
                ) else (
                    echo [ERROR] Failed to remove source directory
                )
            ) else (
                echo [INFO] Keeping source directory: %%i
            )
        )
    )
)

if %FOUND_DIRS%==0 (
    echo [INFO] No source directories found
)

echo.
echo ================================================
echo [INFO] Cleaning up downloaded archives...
echo ================================================

REM Remove archive files
set ARCHIVE_PATTERNS=curl-*.tar.gz curl-*.zip curl-*.tar.bz2 curl-*.tar.xz

for %%p in (%ARCHIVE_PATTERNS%) do (
    if exist "%%p" (
        if "%REMOVE_ARCHIVES%"=="yes" (
            echo [INFO] Removing archive: %%p
            del "%%p" 2>nul
            if %errorlevel%==0 (
                echo [SUCCESS] Removed archive
            ) else (
                echo [ERROR] Failed to remove archive
            )
        ) else (
            echo [INFO] Keeping archive: %%p
        )
    )
)

REM Check download directory for archives
if exist "%DOWNLOAD_DIR%" (
    for %%p in (%ARCHIVE_PATTERNS%) do (
        if exist "%DOWNLOAD_DIR%\%%p" (
            if "%REMOVE_ARCHIVES%"=="yes" (
                echo [INFO] Removing archive: %DOWNLOAD_DIR%\%%p
                del "%DOWNLOAD_DIR%\%%p" 2>nul
                if %errorlevel%==0 (
                    echo [SUCCESS] Removed archive
                ) else (
                    echo [ERROR] Failed to remove archive
                )
            ) else (
                echo [INFO] Keeping archive: %DOWNLOAD_DIR%\%%p
            )
        )
    )
)

echo.
echo ================================================
echo [INFO] Cleaning up download directory...
echo ================================================

if exist "%DOWNLOAD_DIR%" (
    REM Check if directory is empty
    dir /b "%DOWNLOAD_DIR%" 2>nul | findstr . >nul
    if %errorlevel% neq 0 (
        echo [INFO] Removing empty download directory: %DOWNLOAD_DIR%
        rmdir "%DOWNLOAD_DIR%" 2>nul
        if %errorlevel%==0 (
            echo [SUCCESS] Removed download directory
        ) else (
            echo [ERROR] Failed to remove download directory
        )
    ) else (
        echo [INFO] Download directory not empty, keeping: %DOWNLOAD_DIR%
    )
) else (
    echo [INFO] Download directory not found: %DOWNLOAD_DIR%
)

echo.
echo ================================================
echo [INFO] Cleaning up temporary files...
echo ================================================

REM Remove temporary test files
if exist "%TEMP%\libcurl_test_*" (
    echo [INFO] Removing temporary test files...
    for /d %%i in ("%TEMP%\libcurl_test_*") do (
        rmdir /s /q "%%i" 2>nul
    )
    echo [SUCCESS] Temporary files cleaned up
) else (
    echo [INFO] No temporary files found
)

if "%REMOVE_INSTALLED%"=="yes" (
    echo.
    echo ================================================
    echo [WARNING] Removing installed files...
    echo ================================================
    
    if exist "%INSTALL_PREFIX%" (
        echo [INFO] Removing installation directory: %INSTALL_PREFIX%
        rmdir /s /q "%INSTALL_PREFIX%" 2>nul
        if %errorlevel%==0 (
            echo [SUCCESS] Removed installation directory
        ) else (
            echo [ERROR] Failed to remove installation directory
            echo [INFO] You may need to run as Administrator or manually remove:
            echo [INFO] %INSTALL_PREFIX%
        )
    ) else (
        echo [INFO] Installation directory not found: %INSTALL_PREFIX%
    )
    
    REM Remove from system PATH if present
    echo [INFO] Checking system PATH for %INSTALL_PREFIX%\bin
    echo %PATH% | findstr /C:"%INSTALL_PREFIX%\bin" >nul
    if %errorlevel%==0 (
        echo [WARNING] %INSTALL_PREFIX%\bin found in PATH
        echo [INFO] Please manually remove it from your PATH environment variable
    )
)

echo.
echo ================================================
echo [INFO] Cleanup Summary
echo ================================================

if "%REMOVE_SOURCE%"=="yes" (
    echo ✓ Source directories removed
) else (
    echo - Source directories kept
)

if "%REMOVE_ARCHIVES%"=="yes" (
    echo ✓ Archive files removed
) else (
    echo - Archive files kept
)

if "%REMOVE_INSTALLED%"=="yes" (
    echo ✓ Installed files removed
) else (
    echo - Installed files kept
)

echo ✓ Build artifacts cleaned
echo ✓ Temporary files cleaned

echo.
echo [INFO] Cleanup locations:
echo   Download directory: %DOWNLOAD_DIR%
echo   Install prefix: %INSTALL_PREFIX%

if not "%REMOVE_INSTALLED%"=="yes" (
    echo.
    echo [INFO] To remove installed files, run:
    echo   %~nx0 /remove-installed
)

echo.
echo [SUCCESS] Cleanup process completed successfully!
pause