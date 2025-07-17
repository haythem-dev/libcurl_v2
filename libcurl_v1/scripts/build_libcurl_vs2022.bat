@echo off
REM libcurl Build Script for Visual Studio 2022
REM Optimized for Windows with minimal external dependencies

REM Default configuration
set BUILD_TYPE=Release
set INSTALL_PREFIX=C:\libcurl
set CURL_VERSION=8.14.1

echo [INFO] Starting libcurl build for Visual Studio 2022
echo [INFO] Build type: %BUILD_TYPE%
echo [INFO] Install prefix: %INSTALL_PREFIX%
echo [INFO] Target curl version: %CURL_VERSION%
echo.

REM Find source directory
set SOURCE_DIR=
for /d %%i in (curl-*) do (
    if exist "%%i\CMakeLists.txt" (
        set SOURCE_DIR=%%i
        goto :source_found
    )
)

if exist "libcurl-build" (
    for /d %%i in (libcurl-build\curl-*) do (
        if exist "%%i\CMakeLists.txt" (
            set SOURCE_DIR=%%i
            goto :source_found
        )
    )
)

echo [ERROR] No libcurl source directory found
echo [INFO] Please run download_libcurl_simple.bat first
pause
exit /b 1

:source_found
echo [INFO] Using source directory: %SOURCE_DIR%

REM Check for CMake
cmake --version >nul 2>&1
if %errorlevel% neq 0 (
    echo [ERROR] CMake not found
    echo [INFO] Please install CMake from https://cmake.org/download/
    pause
    exit /b 1
)

REM Check for Visual Studio 2022
where cl >nul 2>&1
if %errorlevel% neq 0 (
    echo [ERROR] Visual Studio 2022 compiler not found
    echo [INFO] Please install Visual Studio 2022 or run from Developer Command Prompt
    pause
    exit /b 1
)

REM Build with CMake
cd /d "%SOURCE_DIR%"
if exist "build" rmdir /s /q build
mkdir build
cd build

echo [INFO] Configuring with CMake for Visual Studio 2022...
echo [INFO] Using Windows native SSL (Schannel) - no OpenSSL required
echo [INFO] Disabling optional dependencies to avoid errors
echo.

REM Configure with minimal dependencies and Windows-native features
cmake .. ^
    -G "Visual Studio 17 2022" ^
    -A x64 ^
    -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ^
    -DCMAKE_INSTALL_PREFIX="%INSTALL_PREFIX%" ^
    -DCURL_USE_SCHANNEL=ON ^
    -DCURL_USE_WINSSL=ON ^
    -DCURL_USE_OPENSSL=OFF ^
    -DCURL_USE_LIBSSH2=OFF ^
    -DCURL_USE_NGHTTP2=OFF ^
    -DCURL_USE_LIBIDN2=OFF ^
    -DCURL_USE_LIBPSL=OFF ^
    -DCURL_ZLIB=OFF ^
    -DCURL_BROTLI=OFF ^
    -DCURL_ZSTD=OFF ^
    -DBUILD_SHARED_LIBS=ON ^
    -DBUILD_STATIC_LIBS=ON ^
    -DCURL_STATIC_CRT=OFF ^
    -DENABLE_MANUAL=OFF ^
    -DCURL_DISABLE_TESTS=ON ^
    -DCURL_CA_BUNDLE=none ^
    -DCURL_CA_PATH=none

if %errorlevel% neq 0 (
    echo [ERROR] CMake configuration failed
    echo [INFO] Common solutions:
    echo   1. Run from Visual Studio 2022 Developer Command Prompt
    echo   2. Ensure Visual Studio 2022 is properly installed
    echo   3. Check Windows SDK is installed
    pause
    exit /b 1
)

echo [SUCCESS] Configuration completed successfully
echo.

echo [INFO] Building libcurl with Visual Studio 2022...
cmake --build . --config %BUILD_TYPE% --parallel

if %errorlevel% neq 0 (
    echo [ERROR] Build failed
    echo [INFO] Check the error messages above for details
    pause
    exit /b 1
)

echo [SUCCESS] Build completed successfully
echo.

REM Install to pkg-install directory one level up from build (following template pattern)
set PKG_INSTALL_DIR=%CD%\..\pkg-install
echo [INFO] Installing libcurl to pkg-install directory: %PKG_INSTALL_DIR%
cmake --install . --config %BUILD_TYPE% --prefix "%PKG_INSTALL_DIR%"
if %errorlevel% neq 0 (
    echo [ERROR] Installation failed
    pause
    exit /b 1
)
echo [SUCCESS] Installation completed to pkg-install
echo.
echo [INFO] Installation details:
echo   Binaries: %PKG_INSTALL_DIR%\bin\
echo   Libraries: %PKG_INSTALL_DIR%\lib\
echo   Headers: %PKG_INSTALL_DIR%\include\curl\
echo.

if exist "%PKG_INSTALL_DIR%\bin\curl.exe" (
    echo [INFO] Testing installation:
    "%PKG_INSTALL_DIR%\bin\curl.exe" --version
)

echo.
echo [SUCCESS] libcurl build for Visual Studio 2022 completed successfully
echo.
echo [INFO] Build Summary:
echo   - Visual Studio 2022 (x64)
echo   - Windows native SSL (Schannel)
echo   - Both shared (.dll) and static (.lib) libraries
echo   - No external dependencies required
echo.
echo [INFO] To use in your projects:
echo   Include: %INSTALL_PREFIX%\include\curl\curl.h
echo   Library: %INSTALL_PREFIX%\lib\libcurl.lib
echo   DLL: %INSTALL_PREFIX%\bin\libcurl.dll
echo.
pause