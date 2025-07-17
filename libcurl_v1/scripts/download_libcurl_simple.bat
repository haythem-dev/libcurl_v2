@echo off
REM Simple libcurl Source Download Script for Windows

REM Default configuration
set CURL_VERSION=8.5.0
set DOWNLOAD_DIR=libcurl-build
set CURL_URL=https://curl.se/download/curl-%CURL_VERSION%.tar.gz

echo [INFO] Starting libcurl source download process
echo [INFO] Version: %CURL_VERSION%
echo [INFO] Download directory: %DOWNLOAD_DIR%
echo.

REM Create download directory
if not exist "%DOWNLOAD_DIR%" mkdir "%DOWNLOAD_DIR%"
cd /d "%DOWNLOAD_DIR%"

REM Download source archive
set ARCHIVE_FILE=curl-%CURL_VERSION%.tar.gz
echo [INFO] Downloading from: %CURL_URL%

REM Try PowerShell first (most reliable on Windows)
echo [INFO] Using PowerShell to download...
powershell -Command "try { (New-Object Net.WebClient).DownloadFile('%CURL_URL%', '%ARCHIVE_FILE%'); Write-Host 'Download successful' } catch { Write-Host 'Download failed:' $_.Exception.Message; exit 1 }"

if not exist "%ARCHIVE_FILE%" (
    echo [ERROR] Download failed - file not found
    pause
    exit /b 1
)

echo [INFO] Download completed successfully
echo [INFO] Archive: %ARCHIVE_FILE%

REM Extract using built-in tar (Windows 10 1803+)
echo [INFO] Extracting archive...
tar -xzf "%ARCHIVE_FILE%"

if not exist "curl-%CURL_VERSION%" (
    echo [ERROR] Extraction failed - source directory not found
    pause
    exit /b 1
)

echo [SUCCESS] libcurl source downloaded and extracted successfully
echo [INFO] Source location: %CD%\curl-%CURL_VERSION%
echo.
echo [INFO] Next steps:
echo   1. cd curl-%CURL_VERSION%
echo   2. Run the build script: ..\build_libcurl.bat
echo.
echo [SUCCESS] Download process completed
pause