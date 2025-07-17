@echo off
REM Simple libcurl Build Script for Windows using CMake

REM Default configuration
set BUILD_TYPE=Release
set INSTALL_PREFIX=C:\libcurl

echo [INFO] Starting libcurl build process for Windows
echo [INFO] Build type: %BUILD_TYPE%
echo [INFO] Install prefix: %INSTALL_PREFIX%
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
echo [INFO] Please run download_libcurl.bat first
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

REM Check for Visual Studio compiler
where cl >nul 2>&1
if %errorlevel% neq 0 (
    echo [ERROR] Visual Studio compiler not found
    echo [INFO] Please install Visual Studio Build Tools or Visual Studio Community
    pause
    exit /b 1
)

REM Build with CMake
cd /d "%SOURCE_DIR%"
if exist "build" rmdir /s /q build
mkdir build
cd build

echo [INFO] Configuring with CMake for Visual Studio 2022...
echo [INFO] Using Windows native SSL - no external dependencies
cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_INSTALL_PREFIX="%INSTALL_PREFIX%" -DCURL_USE_SCHANNEL=ON -DCURL_USE_WINSSL=ON -DCURL_USE_OPENSSL=OFF -DCURL_USE_LIBSSH2=OFF -DCURL_USE_NGHTTP2=OFF -DCURL_USE_LIBIDN2=OFF -DCURL_USE_LIBPSL=OFF -DCURL_ZLIB=OFF -DCURL_BROTLI=OFF -DCURL_ZSTD=OFF -DBUILD_SHARED_LIBS=ON -DENABLE_MANUAL=OFF

if %errorlevel% neq 0 (
    echo [ERROR] CMake configuration failed
    pause
    exit /b 1
)

echo [INFO] Building libcurl...
cmake --build . --config %BUILD_TYPE% --parallel

if %errorlevel% neq 0 (
    echo [ERROR] Build failed
    pause
    exit /b 1
)

echo [SUCCESS] Build completed successfully

set /p "INSTALL_NOW=Install libcurl to %INSTALL_PREFIX%? [Y/n]: "
if /i not "%INSTALL_NOW%"=="n" (
    echo [INFO] Installing libcurl...
    cmake --install . --config %BUILD_TYPE%
    if %errorlevel% neq 0 (
        echo [ERROR] Installation failed
        pause
        exit /b 1
    )
    echo [SUCCESS] Installation completed
)

echo.
echo [SUCCESS] libcurl build process completed successfully
echo [INFO] Install location: %INSTALL_PREFIX%
echo.
echo [INFO] To test your installation:
echo   %INSTALL_PREFIX%\bin\curl.exe --version
echo.
pause