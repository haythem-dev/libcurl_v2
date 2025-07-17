
@echo off
setlocal EnableDelayedExpansion

echo [INFO] libcurl Export Script
echo ========================

REM Find the source directory with pkg-install (now one level up from build)
set SOURCE_DIR=
for /d %%i in (curl-*) do (
    if exist "%%i\pkg-install" (
        set SOURCE_DIR=%%i
        goto :source_found
    )
)

if exist "libcurl-build" (
    for /d %%i in (libcurl-build\curl-*) do (
        if exist "%%i\pkg-install" (
            set SOURCE_DIR=%%i
            goto :source_found
        )
    )
)

echo [ERROR] No pkg-install directory found
echo [ERROR] Please run build_libcurl_vs2022.bat first
pause
exit /b 1

:source_found
echo [INFO] Using pkg-install from: %SOURCE_DIR%

REM Set directories
set PKG_INSTALL_DIR=%SOURCE_DIR%\pkg-install
set EXPORT_TARGET_DIR=%CD%\libcurl

echo [INFO] Source: %PKG_INSTALL_DIR%
echo [INFO] Target: %EXPORT_TARGET_DIR%
echo.

REM Check if pkg-install exists and has content
if not exist "%PKG_INSTALL_DIR%" (
    echo [ERROR] pkg-install directory not found: %PKG_INSTALL_DIR%
    pause
    exit /b 1
)

if not exist "%PKG_INSTALL_DIR%\include\curl\curl.h" (
    echo [ERROR] libcurl headers not found in pkg-install
    echo [ERROR] Build may have failed
    pause
    exit /b 1
)

REM Create target directory structure
echo [INFO] Creating libcurl directory structure...
mkdir "%EXPORT_TARGET_DIR%" 2>NUL
mkdir "%EXPORT_TARGET_DIR%\include" 2>NUL
mkdir "%EXPORT_TARGET_DIR%\lib" 2>NUL
mkdir "%EXPORT_TARGET_DIR%\bin" 2>NUL
mkdir "%EXPORT_TARGET_DIR%\docs" 2>NUL

REM Copy headers
echo [INFO] Copying headers...
xcopy /E /I /Q /Y "%PKG_INSTALL_DIR%\include\*" "%EXPORT_TARGET_DIR%\include\"
if %ERRORLEVEL% neq 0 (
    echo [ERROR] Failed to copy headers
    goto :error
)

REM Copy libraries
echo [INFO] Copying libraries...
copy /Y "%PKG_INSTALL_DIR%\lib\*.lib" "%EXPORT_TARGET_DIR%\lib\" >NUL
copy /Y "%PKG_INSTALL_DIR%\lib\*.a" "%EXPORT_TARGET_DIR%\lib\" 2>NUL
copy /Y "%PKG_INSTALL_DIR%\lib\*.dll.a" "%EXPORT_TARGET_DIR%\lib\" 2>NUL
if %ERRORLEVEL% neq 0 (
    echo [WARNING] Some library files may not have been copied
)

REM Copy binaries and DLLs
echo [INFO] Copying binaries...
copy /Y "%PKG_INSTALL_DIR%\bin\*.dll" "%EXPORT_TARGET_DIR%\bin\" >NUL
copy /Y "%PKG_INSTALL_DIR%\bin\*.exe" "%EXPORT_TARGET_DIR%\bin\" >NUL
if %ERRORLEVEL% neq 0 (
    echo [WARNING] Some binary files may not have been copied
)

REM Copy documentation if available
if exist "%PKG_INSTALL_DIR%\share\man" (
    echo [INFO] Copying documentation...
    xcopy /E /I /Q /Y "%PKG_INSTALL_DIR%\share\man\*" "%EXPORT_TARGET_DIR%\docs\man\" 2>NUL
)

REM Create version info
echo [INFO] Creating version info...
(
echo libcurl Build Export
echo ===================
echo.
echo Build Date: %DATE% %TIME%
echo Platform: Windows x64
echo Compiler: Visual Studio 2022
echo SSL: Windows native ^(Schannel^)
echo.
echo Directory Structure:
echo   include\curl\    - Header files
echo   lib\            - Static and import libraries
echo   bin\            - DLLs and executables
echo   docs\           - Documentation
echo.
echo Usage:
echo   Include: #include ^<curl/curl.h^>
echo   Library: libcurl.lib ^(static^) or libcurl_imp.lib ^(import^)
echo   Runtime: libcurl.dll ^(if using shared library^)
) > "%EXPORT_TARGET_DIR%\docs\README.txt"

REM Create CMake helper file
echo [INFO] Creating CMake configuration helper...
(
echo # libcurl CMake configuration helper
echo # Add this to your CMakeLists.txt:
echo #
echo # set^(CURL_ROOT "path/to/libcurl"^)
echo # find_package^(CURL REQUIRED^)
echo # target_link_libraries^(your_target ${CURL_LIBRARIES}^)
echo # target_include_directories^(your_target PRIVATE ${CURL_INCLUDE_DIRS}^)
echo.
echo set^(CURL_ROOT "${CMAKE_CURRENT_LIST_DIR}"^)
echo set^(CURL_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/include"^)
echo set^(CURL_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/lib/libcurl.lib"^)
echo set^(CURL_FOUND TRUE^)
) > "%EXPORT_TARGET_DIR%\libcurl-config.cmake"

REM Display summary
echo.
echo [SUCCESS] libcurl export completed successfully!
echo.
echo Export Summary:
echo   Target Directory: %EXPORT_TARGET_DIR%
echo   Headers: %EXPORT_TARGET_DIR%\include\curl\
echo   Libraries: %EXPORT_TARGET_DIR%\lib\
echo   Binaries: %EXPORT_TARGET_DIR%\bin\
echo   Documentation: %EXPORT_TARGET_DIR%\docs\
echo.

REM Test the exported files
if exist "%EXPORT_TARGET_DIR%\bin\curl.exe" (
    echo [INFO] Testing exported curl.exe:
    "%EXPORT_TARGET_DIR%\bin\curl.exe" --version
    echo.
)

echo [INFO] Exported files are ready for integration into your project
echo [INFO] See %EXPORT_TARGET_DIR%\docs\README.txt for usage instructions
goto :end

:error
echo.
echo [ERROR] Export failed!
pause
exit /b 1

:end
pause
