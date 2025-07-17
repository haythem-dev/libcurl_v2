@echo off
REM libcurl Build Script for Windows
REM Compiles libcurl from source with customizable options using CMake or Visual Studio

setlocal EnableDelayedExpansion

REM Default configuration
if "%BUILD_TYPE%"=="" set BUILD_TYPE=Release
if "%INSTALL_PREFIX%"=="" set INSTALL_PREFIX=C:\libcurl
if "%BUILD_SSL%"=="" set BUILD_SSL=ON
if "%BUILD_LIBSSH2%"=="" set BUILD_LIBSSH2=ON
if "%BUILD_ZLIB%"=="" set BUILD_ZLIB=ON
if "%BUILD_SHARED%"=="" set BUILD_SHARED=ON
if "%BUILD_STATIC%"=="" set BUILD_STATIC=ON
if "%PARALLEL_JOBS%"=="" set PARALLEL_JOBS=%NUMBER_OF_PROCESSORS%
if "%SOURCE_DIR%"=="" set SOURCE_DIR=

echo [INFO] Starting libcurl build process for Windows
echo [INFO] Build type: %BUILD_TYPE%
echo [INFO] Install prefix: %INSTALL_PREFIX%
echo [INFO] SSL support: %BUILD_SSL%
echo [INFO] SSH2 support: %BUILD_LIBSSH2%
echo [INFO] ZLIB support: %BUILD_ZLIB%
echo [INFO] Parallel jobs: %PARALLEL_JOBS%

REM Find source directory
if not "%SOURCE_DIR%"=="" (
    if exist "%SOURCE_DIR%" (
        set FOUND_SOURCE=%SOURCE_DIR%
        goto :source_found
    )
)

REM Look for curl source directory
set FOUND_SOURCE=
for /d %%i in (curl-*) do (
    if exist "%%i\CMakeLists.txt" (
        set FOUND_SOURCE=%%i
        goto :source_found
    )
)

REM Check in download directory
if exist "libcurl-build" (
    for /d %%i in (libcurl-build\curl-*) do (
        if exist "%%i\CMakeLists.txt" (
            set FOUND_SOURCE=%%i
            goto :source_found
        )
    )
)

if "%FOUND_SOURCE%"=="" (
    echo [ERROR] No libcurl source directory found
    echo [INFO] Please ensure you have downloaded the source code first
    exit /b 1
)

:source_found
echo [INFO] Using source directory: %FOUND_SOURCE%

REM Check for build tools
echo [INFO] Checking for build tools...

REM Check for CMake
cmake --version >nul 2>&1
if %errorlevel%==0 (
    echo [INFO] CMake found
    set USE_CMAKE=1
) else (
    echo [WARNING] CMake not found
    set USE_CMAKE=0
)

REM Check for Visual Studio Build Tools
where cl >nul 2>&1
if %errorlevel%==0 (
    echo [INFO] Visual Studio compiler found
    set HAS_MSVC=1
) else (
    echo [WARNING] Visual Studio compiler not found
    set HAS_MSVC=0
)

REM Check for MinGW
where gcc >nul 2>&1
if %errorlevel%==0 (
    echo [INFO] MinGW GCC found
    set HAS_MINGW=1
) else (
    echo [WARNING] MinGW GCC not found
    set HAS_MINGW=0
)

if %USE_CMAKE%==0 (
    echo [ERROR] CMake is required for building on Windows
    echo [INFO] Please install CMake from https://cmake.org/download/
    exit /b 1
)

if %HAS_MSVC%==0 if %HAS_MINGW%==0 (
    echo [ERROR] No suitable compiler found
    echo [INFO] Please install Visual Studio Build Tools or MinGW
    exit /b 1
)

REM Create build directory
cd /d "%FOUND_SOURCE%"
if exist "build" rmdir /s /q build
mkdir build
cd build

echo [INFO] Configuring build with CMake...

REM Build CMake command
set CMAKE_CMD=cmake ..
set CMAKE_CMD=%CMAKE_CMD% -DCMAKE_BUILD_TYPE=%BUILD_TYPE%
set CMAKE_CMD=%CMAKE_CMD% -DCMAKE_INSTALL_PREFIX="%INSTALL_PREFIX%"

REM Library type options
if "%BUILD_SHARED%"=="ON" (
    set CMAKE_CMD=%CMAKE_CMD% -DBUILD_SHARED_LIBS=ON
) else (
    set CMAKE_CMD=%CMAKE_CMD% -DBUILD_SHARED_LIBS=OFF
)

if "%BUILD_STATIC%"=="ON" (
    set CMAKE_CMD=%CMAKE_CMD% -DBUILD_STATIC_LIBS=ON
) else (
    set CMAKE_CMD=%CMAKE_CMD% -DBUILD_STATIC_LIBS=OFF
)

REM Feature options
if "%BUILD_SSL%"=="ON" (
    set CMAKE_CMD=%CMAKE_CMD% -DCURL_USE_OPENSSL=ON
) else (
    set CMAKE_CMD=%CMAKE_CMD% -DCURL_USE_OPENSSL=OFF
)

if "%BUILD_ZLIB%"=="ON" (
    set CMAKE_CMD=%CMAKE_CMD% -DCURL_ZLIB=ON
) else (
    set CMAKE_CMD=%CMAKE_CMD% -DCURL_ZLIB=OFF
)

if "%BUILD_LIBSSH2%"=="ON" (
    set CMAKE_CMD=%CMAKE_CMD% -DCURL_USE_LIBSSH2=ON
) else (
    set CMAKE_CMD=%CMAKE_CMD% -DCURL_USE_LIBSSH2=OFF
)

REM Additional Windows-specific options
set CMAKE_CMD=%CMAKE_CMD% -DCURL_USE_SCHANNEL=ON
set CMAKE_CMD=%CMAKE_CMD% -DCURL_USE_WINSSL=ON
set CMAKE_CMD=%CMAKE_CMD% -DENABLE_MANUAL=OFF

REM Detect and set generator
if %HAS_MSVC%==1 (
    echo [INFO] Using Visual Studio generator
    set CMAKE_CMD=%CMAKE_CMD% -G "Visual Studio 16 2019" -A x64
) else if %HAS_MINGW%==1 (
    echo [INFO] Using MinGW Makefiles generator
    set CMAKE_CMD=%CMAKE_CMD% -G "MinGW Makefiles"
)

echo [INFO] CMake command: %CMAKE_CMD%
%CMAKE_CMD%
if %errorlevel% neq 0 (
    echo [ERROR] CMake configuration failed
    exit /b 1
)

echo [SUCCESS] Configuration completed successfully

echo [INFO] Building libcurl with %PARALLEL_JOBS% parallel jobs...
cmake --build . --config %BUILD_TYPE% --parallel %PARALLEL_JOBS%
if %errorlevel% neq 0 (
    echo [ERROR] Build failed
    exit /b 1
)

echo [SUCCESS] Build completed successfully

REM Ask for installation
set /p "INSTALL_NOW=Install libcurl to %INSTALL_PREFIX%? [Y/n]: "
if /i not "%INSTALL_NOW%"=="n" (
    echo [INFO] Installing libcurl...
    cmake --install . --config %BUILD_TYPE%
    if %errorlevel% neq 0 (
        echo [ERROR] Installation failed
        exit /b 1
    )
    echo [SUCCESS] Installation completed successfully
)

echo.
echo [SUCCESS] Build Summary
echo ===================
echo Source directory: %FOUND_SOURCE%
echo Build directory: %FOUND_SOURCE%\build
echo Install prefix: %INSTALL_PREFIX%
echo Build type: %BUILD_TYPE%
echo SSL support: %BUILD_SSL%
echo SSH2 support: %BUILD_LIBSSH2%
echo ZLIB support: %BUILD_ZLIB%
echo Shared library: %BUILD_SHARED%
echo Static library: %BUILD_STATIC%

echo.
echo [INFO] Installation locations:
echo   Binaries: %INSTALL_PREFIX%\bin\
echo   Libraries: %INSTALL_PREFIX%\lib\
echo   Headers: %INSTALL_PREFIX%\include\curl\
echo   CMake config: %INSTALL_PREFIX%\lib\cmake\CURL\

if exist "%INSTALL_PREFIX%\bin\curl.exe" (
    echo.
    echo [INFO] Testing curl installation:
    "%INSTALL_PREFIX%\bin\curl.exe" --version
)

echo.
echo [SUCCESS] libcurl build process completed successfully!
pause