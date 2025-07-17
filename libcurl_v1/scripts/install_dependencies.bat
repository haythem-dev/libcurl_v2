@echo off
REM libcurl Dependencies Installation Script for Windows
REM Automatically detects system and installs required build dependencies

setlocal EnableDelayedExpansion

REM Configuration
if "%INSTALL_OPTIONAL%"=="" set INSTALL_OPTIONAL=yes
if "%INSTALL_DEVELOPMENT%"=="" set INSTALL_DEVELOPMENT=no
if "%DRY_RUN%"=="" set DRY_RUN=no
if "%USE_CHOCOLATEY%"=="" set USE_CHOCOLATEY=auto
if "%USE_WINGET%"=="" set USE_WINGET=auto

echo [INFO] Starting libcurl dependencies installation for Windows
echo Configuration:
echo   Install optional: %INSTALL_OPTIONAL%
echo   Install development tools: %INSTALL_DEVELOPMENT%
echo   Dry run: %DRY_RUN%
echo.

REM Detect package managers
echo [INFO] Detecting available package managers...

REM Check for Chocolatey
choco version >nul 2>&1
if %errorlevel%==0 (
    echo [INFO] Chocolatey detected
    set HAS_CHOCOLATEY=1
) else (
    echo [INFO] Chocolatey not found
    set HAS_CHOCOLATEY=0
)

REM Check for winget
winget --version >nul 2>&1
if %errorlevel%==0 (
    echo [INFO] Windows Package Manager (winget) detected
    set HAS_WINGET=1
) else (
    echo [INFO] Windows Package Manager (winget) not found
    set HAS_WINGET=0
)

REM Check for scoop
where scoop >nul 2>&1
if %errorlevel%==0 (
    echo [INFO] Scoop detected
    set HAS_SCOOP=1
) else (
    echo [INFO] Scoop not found
    set HAS_SCOOP=0
)

if %HAS_CHOCOLATEY%==0 if %HAS_WINGET%==0 if %HAS_SCOOP%==0 (
    echo [WARNING] No package manager found!
    echo [INFO] Please install one of the following:
    echo   - Chocolatey: https://chocolatey.org/install
    echo   - Windows Package Manager: https://github.com/microsoft/winget-cli
    echo   - Scoop: https://scoop.sh/
    echo.
    echo [INFO] Or install dependencies manually:
    echo   1. Visual Studio Build Tools or Visual Studio Community
    echo   2. CMake
    echo   3. Git
    echo   4. 7-Zip or WinRAR
    pause
    exit /b 1
)

REM Choose package manager
set PACKAGE_MANAGER=none
if "%USE_CHOCOLATEY%"=="yes" if %HAS_CHOCOLATEY%==1 set PACKAGE_MANAGER=chocolatey
if "%USE_WINGET%"=="yes" if %HAS_WINGET%==1 set PACKAGE_MANAGER=winget
if "%PACKAGE_MANAGER%"=="none" (
    if %HAS_CHOCOLATEY%==1 set PACKAGE_MANAGER=chocolatey
    if %HAS_WINGET%==1 set PACKAGE_MANAGER=winget
    if %HAS_SCOOP%==1 set PACKAGE_MANAGER=scoop
)

echo [INFO] Using package manager: %PACKAGE_MANAGER%
echo.

REM Function to install package (simulated with goto)
goto :start_installation

:install_package
REM %1 = package name, %2 = description
set PKG_NAME=%~1
set PKG_DESC=%~2

echo [INFO] Installing %PKG_DESC%: %PKG_NAME%

if "%DRY_RUN%"=="yes" (
    echo [DRY RUN] Would install: %PKG_NAME%
    goto :eof
)

if "%PACKAGE_MANAGER%"=="chocolatey" (
    choco install %PKG_NAME% -y
) else if "%PACKAGE_MANAGER%"=="winget" (
    winget install %PKG_NAME% --silent
) else if "%PACKAGE_MANAGER%"=="scoop" (
    scoop install %PKG_NAME%
)

if %errorlevel%==0 (
    echo [SUCCESS] Installed %PKG_DESC%
) else (
    echo [WARNING] Failed to install %PKG_DESC%
)
goto :eof

:start_installation
echo ================================================
echo [INFO] Installing build tools...
echo ================================================

REM Check for Visual Studio or Build Tools
where cl >nul 2>&1
if %errorlevel% neq 0 (
    echo [INFO] Visual Studio compiler not found, installing Build Tools...
    if "%PACKAGE_MANAGER%"=="chocolatey" (
        call :install_package "visualstudio2022buildtools" "Visual Studio Build Tools 2022"
    ) else if "%PACKAGE_MANAGER%"=="winget" (
        call :install_package "Microsoft.VisualStudio.2022.BuildTools" "Visual Studio Build Tools 2022"
    ) else if "%PACKAGE_MANAGER%"=="scoop" (
        echo [WARNING] Scoop cannot install Visual Studio Build Tools
        echo [INFO] Please download and install manually from:
        echo https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2022
    )
) else (
    echo [SUCCESS] Visual Studio compiler already available
)

REM Install CMake
cmake --version >nul 2>&1
if %errorlevel% neq 0 (
    if "%PACKAGE_MANAGER%"=="chocolatey" (
        call :install_package "cmake" "CMake"
    ) else if "%PACKAGE_MANAGER%"=="winget" (
        call :install_package "Kitware.CMake" "CMake"
    ) else if "%PACKAGE_MANAGER%"=="scoop" (
        call :install_package "cmake" "CMake"
    )
) else (
    echo [SUCCESS] CMake already available
)

REM Install Git
git --version >nul 2>&1
if %errorlevel% neq 0 (
    if "%PACKAGE_MANAGER%"=="chocolatey" (
        call :install_package "git" "Git"
    ) else if "%PACKAGE_MANAGER%"=="winget" (
        call :install_package "Git.Git" "Git"
    ) else if "%PACKAGE_MANAGER%"=="scoop" (
        call :install_package "git" "Git"
    )
) else (
    echo [SUCCESS] Git already available
)

REM Install 7-Zip for archive extraction
7z >nul 2>&1
if %errorlevel% neq 0 (
    if "%PACKAGE_MANAGER%"=="chocolatey" (
        call :install_package "7zip" "7-Zip"
    ) else if "%PACKAGE_MANAGER%"=="winget" (
        call :install_package "7zip.7zip" "7-Zip"
    ) else if "%PACKAGE_MANAGER%"=="scoop" (
        call :install_package "7zip" "7-Zip"
    )
) else (
    echo [SUCCESS] 7-Zip already available
)

if "%INSTALL_OPTIONAL%"=="yes" (
    echo.
    echo ================================================
    echo [INFO] Installing optional dependencies...
    echo ================================================
    
    REM Install OpenSSL development libraries
    if "%PACKAGE_MANAGER%"=="chocolatey" (
        call :install_package "openssl" "OpenSSL"
    ) else if "%PACKAGE_MANAGER%"=="winget" (
        call :install_package "ShiningLight.OpenSSL" "OpenSSL"
    ) else if "%PACKAGE_MANAGER%"=="scoop" (
        call :install_package "openssl" "OpenSSL"
    )
    
    REM Install zlib
    if "%PACKAGE_MANAGER%"=="chocolatey" (
        call :install_package "zlib" "zlib"
    ) else if "%PACKAGE_MANAGER%"=="scoop" (
        call :install_package "zlib" "zlib"
    ) else (
        echo [INFO] zlib will be built as part of libcurl
    )
)

if "%INSTALL_DEVELOPMENT%"=="yes" (
    echo.
    echo ================================================
    echo [INFO] Installing development tools...
    echo ================================================
    
    REM Install Python (useful for additional tools)
    python --version >nul 2>&1
    if %errorlevel% neq 0 (
        if "%PACKAGE_MANAGER%"=="chocolatey" (
            call :install_package "python" "Python"
        ) else if "%PACKAGE_MANAGER%"=="winget" (
            call :install_package "Python.Python.3" "Python"
        ) else if "%PACKAGE_MANAGER%"=="scoop" (
            call :install_package "python" "Python"
        )
    )
    
    REM Install text editor
    where code >nul 2>&1
    if %errorlevel% neq 0 (
        if "%PACKAGE_MANAGER%"=="chocolatey" (
            call :install_package "vscode" "Visual Studio Code"
        ) else if "%PACKAGE_MANAGER%"=="winget" (
            call :install_package "Microsoft.VisualStudioCode" "Visual Studio Code"
        ) else if "%PACKAGE_MANAGER%"=="scoop" (
            call :install_package "vscode" "Visual Studio Code"
        )
    )
)

echo.
echo ================================================
echo [INFO] Verifying installation...
echo ================================================

set MISSING_TOOLS=0

REM Check essential tools
echo [INFO] Checking essential tools...

where cl >nul 2>&1
if %errorlevel%==0 (
    echo ✓ Visual Studio compiler (cl.exe)
) else (
    echo ✗ Visual Studio compiler (cl.exe)
    set /a MISSING_TOOLS+=1
)

cmake --version >nul 2>&1
if %errorlevel%==0 (
    echo ✓ CMake
) else (
    echo ✗ CMake
    set /a MISSING_TOOLS+=1
)

git --version >nul 2>&1
if %errorlevel%==0 (
    echo ✓ Git
) else (
    echo ✗ Git
    set /a MISSING_TOOLS+=1
)

7z >nul 2>&1
if %errorlevel%==0 (
    echo ✓ 7-Zip
) else (
    tar --version >nul 2>&1
    if %errorlevel%==0 (
        echo ✓ tar (Windows 10 built-in)
    ) else (
        echo ✗ Archive extraction tool
        set /a MISSING_TOOLS+=1
    )
)

if "%INSTALL_OPTIONAL%"=="yes" (
    echo.
    echo [INFO] Checking optional tools...
    
    where openssl >nul 2>&1
    if %errorlevel%==0 (
        echo ✓ OpenSSL
    ) else (
        echo ✗ OpenSSL
    )
)

echo.
echo ================================================
echo [INFO] Installation Summary
echo ================================================
echo Package Manager: %PACKAGE_MANAGER%
echo Install Optional: %INSTALL_OPTIONAL%
echo Install Development: %INSTALL_DEVELOPMENT%

if %MISSING_TOOLS%==0 (
    echo.
    echo [SUCCESS] All required tools are available!
    echo You can now proceed with building libcurl.
) else (
    echo.
    echo [WARNING] %MISSING_TOOLS% essential tools are missing.
    echo Please install them manually or try a different package manager.
)

echo.
echo [INFO] Tool versions:
where cl >nul 2>&1 && (
    echo Visual Studio: 
    cl 2>&1 | findstr /C:"Version"
) || echo Visual Studio: Not found

cmake --version 2>nul | findstr /C:"cmake version" || echo CMake: Not found
git --version 2>nul || echo Git: Not found

echo.
echo [INFO] Next Steps:
echo   1. Download libcurl source: download_libcurl.bat
echo   2. Build libcurl: build_libcurl.bat
echo   3. Verify installation: verify_build.bat
echo   4. Clean up: cleanup.bat

echo.
echo [SUCCESS] Dependencies installation completed!
pause