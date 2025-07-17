@echo off
REM libcurl Build Script using vcpkg for dependency management
REM This script uses vcpkg to automatically handle dependencies

set INSTALL_PREFIX=C:\libcurl
set VCPKG_ROOT=C:\vcpkg

echo [INFO] libcurl build using vcpkg for Visual Studio 2022
echo [INFO] Install prefix: %INSTALL_PREFIX%
echo.

REM Check if vcpkg is installed
if not exist "%VCPKG_ROOT%\vcpkg.exe" (
    echo [INFO] vcpkg not found, installing...
    echo [INFO] This will take a few minutes...
    
    REM Install vcpkg
    cd /d C:\
    git clone https://github.com/Microsoft/vcpkg.git
    cd vcpkg
    call bootstrap-vcpkg.bat
    
    if %errorlevel% neq 0 (
        echo [ERROR] Failed to install vcpkg
        pause
        exit /b 1
    )
    
    REM Integrate with Visual Studio
    vcpkg integrate install
    echo [SUCCESS] vcpkg installed successfully
)

echo [INFO] Installing curl using vcpkg...
cd /d "%VCPKG_ROOT%"

REM Install curl with all features
vcpkg install curl[tool]:x64-windows

if %errorlevel% neq 0 (
    echo [ERROR] Failed to install curl with vcpkg
    pause
    exit /b 1
)

echo [SUCCESS] libcurl installed successfully using vcpkg
echo.
echo [INFO] Installation locations:
echo   Headers: %VCPKG_ROOT%\installed\x64-windows\include\curl\
echo   Libraries: %VCPKG_ROOT%\installed\x64-windows\lib\
echo   Binaries: %VCPKG_ROOT%\installed\x64-windows\bin\
echo   Tools: %VCPKG_ROOT%\installed\x64-windows\tools\curl\
echo.

REM Test installation
if exist "%VCPKG_ROOT%\installed\x64-windows\tools\curl\curl.exe" (
    echo [INFO] Testing curl installation:
    "%VCPKG_ROOT%\installed\x64-windows\tools\curl\curl.exe" --version
    echo.
)

echo [INFO] To use in your Visual Studio projects:
echo   1. vcpkg is already integrated with Visual Studio
echo   2. Simply #include ^<curl/curl.h^> in your code
echo   3. No additional configuration needed
echo.

echo [SUCCESS] vcpkg installation completed
pause