@echo off
REM libcurl Source Download Script for Windows
REM Downloads the latest stable version of libcurl source code

setlocal EnableDelayedExpansion

REM Configuration
if "%CURL_VERSION%"=="" set CURL_VERSION=8.5.0
if "%DOWNLOAD_DIR%"=="" set DOWNLOAD_DIR=.\libcurl-build
set CURL_URL=https://curl.se/download/curl-%CURL_VERSION%.tar.gz
set BACKUP_URL=https://github.com/curl/curl/releases/download/curl-%CURL_VERSION:.=_%/curl-%CURL_VERSION%.tar.gz

REM Colors are not easily supported in Windows batch, using plain text
echo [INFO] Starting libcurl source download process
echo [INFO] Version: %CURL_VERSION%
echo [INFO] Download directory: %DOWNLOAD_DIR%

REM Create download directory
if not exist "%DOWNLOAD_DIR%" mkdir "%DOWNLOAD_DIR%"
cd /d "%DOWNLOAD_DIR%"

REM Check if source already exists
if exist "curl-%CURL_VERSION%" (
    echo [WARNING] Source directory curl-%CURL_VERSION% already exists
    set /p "REPLY=Do you want to remove it and re-download? [y/N]: "
    if /i "!REPLY!"=="y" (
        rmdir /s /q "curl-%CURL_VERSION%"
        echo [INFO] Removed existing source directory
    ) else (
        echo [INFO] Using existing source directory
        goto :end
    )
)

REM Download source archive
set ARCHIVE_FILE=curl-%CURL_VERSION%.tar.gz

if exist "%ARCHIVE_FILE%" (
    echo [INFO] Archive already exists, checking size...
    for %%A in ("%ARCHIVE_FILE%") do set FILE_SIZE=%%~zA
    if !FILE_SIZE! LSS 1000000 (
        echo [WARNING] Existing archive seems too small ^(!FILE_SIZE! bytes^), re-downloading...
        del "%ARCHIVE_FILE%"
    ) else (
        echo [INFO] Using existing archive
        goto :extract
    )
)

echo [INFO] Downloading from primary URL: %CURL_URL%

REM Try to download using curl (if available)
curl --version >nul 2>&1
if %errorlevel%==0 (
    echo [INFO] Using curl to download...
    curl -L --progress-bar --fail "%CURL_URL%" -o "%ARCHIVE_FILE%"
    if %errorlevel%==0 goto :verify_download
)

REM Try to download using PowerShell
echo [INFO] Using PowerShell to download...
powershell -Command "(New-Object Net.WebClient).DownloadFile('%CURL_URL%', '%ARCHIVE_FILE%')"
if %errorlevel%==0 goto :verify_download

REM Try backup URL with PowerShell
echo [WARNING] Primary download failed, trying backup URL...
powershell -Command "(New-Object Net.WebClient).DownloadFile('%BACKUP_URL%', '%ARCHIVE_FILE%')"
if %errorlevel%==0 goto :verify_download

echo [ERROR] All download attempts failed
exit /b 1

:verify_download
if not exist "%ARCHIVE_FILE%" (
    echo [ERROR] Downloaded file not found: %ARCHIVE_FILE%
    exit /b 1
)

for %%A in ("%ARCHIVE_FILE%") do set FILE_SIZE=%%~zA
if %FILE_SIZE% LSS 1000000 (
    echo [WARNING] Downloaded file seems too small ^(%FILE_SIZE% bytes^)
    exit /b 1
)

echo [SUCCESS] Download verification passed ^(%FILE_SIZE% bytes^)

:extract
echo [INFO] Extracting archive...

REM Check if tar is available (Windows 10 version 1803+ has tar.exe)
tar --version >nul 2>&1
if %errorlevel%==0 (
    echo [INFO] Using tar to extract...
    tar -xzf "%ARCHIVE_FILE%"
    if %errorlevel%==0 goto :verify_extraction
)

REM Try using 7-Zip if available
7z >nul 2>&1
if %errorlevel%==0 (
    echo [INFO] Using 7-Zip to extract...
    7z x "%ARCHIVE_FILE%" -so | 7z x -aoa -si -ttar
    if %errorlevel%==0 goto :verify_extraction
)

REM Try using PowerShell with .NET compression
echo [INFO] Using PowerShell to extract...
powershell -Command "Add-Type -AssemblyName System.IO.Compression.FileSystem; [System.IO.Compression.ZipFile]::ExtractToDirectory('%ARCHIVE_FILE%', '.')" 2>nul
if %errorlevel%==0 goto :verify_extraction

echo [ERROR] Could not extract archive. Please install 7-Zip or use Windows 10 version 1803+ for built-in tar support
exit /b 1

:verify_extraction
if not exist "curl-%CURL_VERSION%" (
    echo [ERROR] Source directory not found after extraction
    exit /b 1
)

echo [SUCCESS] libcurl source downloaded and extracted successfully
echo [INFO] Source location: %CD%\curl-%CURL_VERSION%

if exist "curl-%CURL_VERSION%\README" echo [INFO] README file found
if exist "curl-%CURL_VERSION%\configure" echo [INFO] Configure script found
if exist "curl-%CURL_VERSION%\CMakeLists.txt" echo [INFO] CMake configuration found

echo.
echo [INFO] Next steps:
echo   1. cd curl-%CURL_VERSION%
echo   2. Run the build script: ..\build_libcurl.bat
echo   3. Or use CMake: cmake -B build ^&^& cmake --build build

:end
echo [SUCCESS] Download process completed successfully
pause