@echo off
REM libcurl Build Verification Script for Windows
REM Verifies that libcurl was built and installed correctly

setlocal EnableDelayedExpansion

REM Configuration
if "%INSTALL_PREFIX%"=="" set INSTALL_PREFIX=C:\libcurl
if "%VERBOSE%"=="" set VERBOSE=no

echo [INFO] Starting libcurl verification process for Windows
echo [INFO] Install prefix: %INSTALL_PREFIX%
echo.

REM Test counters
set TESTS_TOTAL=0
set TESTS_PASSED=0
set TESTS_FAILED=0

REM Function to run a test (simulated with goto)
goto :start_tests

:run_test
REM %1 = test name, %2 = command to test
set /a TESTS_TOTAL+=1
echo [INFO] Running test: %~1

if "%VERBOSE%"=="yes" echo [VERBOSE] Command: %~2

%~2 >nul 2>&1
if %errorlevel%==0 (
    echo [SUCCESS] PASS: %~1
    set /a TESTS_PASSED+=1
) else (
    echo [ERROR] FAIL: %~1
    set /a TESTS_FAILED+=1
    if "%VERBOSE%"=="yes" (
        echo [VERBOSE] Command failed with error level: %errorlevel%
    )
)
goto :eof

:start_tests
echo ================================================
echo [INFO] Verifying curl binary...
echo ================================================

REM Check if curl.exe exists in install directory
if exist "%INSTALL_PREFIX%\bin\curl.exe" (
    call :run_test "curl.exe exists" "dir \"%INSTALL_PREFIX%\bin\curl.exe\""
    
    REM Test curl version
    call :run_test "curl version check" "\"%INSTALL_PREFIX%\bin\curl.exe\" --version"
    
    REM Test curl help
    call :run_test "curl help output" "\"%INSTALL_PREFIX%\bin\curl.exe\" --help"
    
    if "%VERBOSE%"=="yes" (
        echo [VERBOSE] curl version information:
        "%INSTALL_PREFIX%\bin\curl.exe" --version 2>nul
    )
) else (
    echo [WARNING] curl.exe not found in %INSTALL_PREFIX%\bin\
    REM Check if curl is in system PATH
    where curl >nul 2>&1
    if %errorlevel%==0 (
        echo [INFO] curl found in system PATH
        call :run_test "system curl version" "curl --version"
    ) else (
        echo [ERROR] curl not found anywhere
        set /a TESTS_TOTAL+=1
        set /a TESTS_FAILED+=1
    )
)

echo.
echo ================================================
echo [INFO] Verifying libcurl library...
echo ================================================

REM Check library files
if exist "%INSTALL_PREFIX%\lib\libcurl.lib" (
    call :run_test "static library (libcurl.lib)" "dir \"%INSTALL_PREFIX%\lib\libcurl.lib\""
) else (
    echo [WARNING] Static library libcurl.lib not found
)

if exist "%INSTALL_PREFIX%\lib\libcurl.dll.a" (
    call :run_test "import library (libcurl.dll.a)" "dir \"%INSTALL_PREFIX%\lib\libcurl.dll.a\""
) else if exist "%INSTALL_PREFIX%\lib\curl.lib" (
    call :run_test "import library (curl.lib)" "dir \"%INSTALL_PREFIX%\lib\curl.lib\""
) else (
    echo [WARNING] Import library not found
)

if exist "%INSTALL_PREFIX%\bin\libcurl.dll" (
    call :run_test "shared library (libcurl.dll)" "dir \"%INSTALL_PREFIX%\bin\libcurl.dll\""
) else (
    echo [WARNING] Shared library libcurl.dll not found
)

REM Check CMake config files
if exist "%INSTALL_PREFIX%\lib\cmake\CURL\CURLConfig.cmake" (
    call :run_test "CMake config file" "dir \"%INSTALL_PREFIX%\lib\cmake\CURL\CURLConfig.cmake\""
) else (
    echo [WARNING] CMake config files not found
)

echo.
echo ================================================
echo [INFO] Verifying header files...
echo ================================================

REM Check main header directory
if exist "%INSTALL_PREFIX%\include\curl" (
    call :run_test "curl include directory" "dir \"%INSTALL_PREFIX%\include\curl\""
    
    REM Check essential headers
    if exist "%INSTALL_PREFIX%\include\curl\curl.h" (
        call :run_test "main curl header" "dir \"%INSTALL_PREFIX%\include\curl\curl.h\""
    ) else (
        echo [ERROR] Main curl.h header not found
        set /a TESTS_TOTAL+=1
        set /a TESTS_FAILED+=1
    )
    
    if exist "%INSTALL_PREFIX%\include\curl\easy.h" (
        call :run_test "curl easy interface header" "dir \"%INSTALL_PREFIX%\include\curl\easy.h\""
    ) else (
        echo [WARNING] easy.h header not found
    )
    
    if "%VERBOSE%"=="yes" (
        echo [VERBOSE] Available header files:
        dir "%INSTALL_PREFIX%\include\curl\" /b 2>nul
    )
) else (
    echo [ERROR] curl include directory not found
    set /a TESTS_TOTAL+=1
    set /a TESTS_FAILED+=1
)

echo.
echo ================================================
echo [INFO] Testing basic functionality...
echo ================================================

REM Test simple HTTP request
if exist "%INSTALL_PREFIX%\bin\curl.exe" (
    call :run_test "HTTP request test" "\"%INSTALL_PREFIX%\bin\curl.exe\" -s --max-time 10 --fail http://httpbin.org/get"
    call :run_test "HTTPS request test" "\"%INSTALL_PREFIX%\bin\curl.exe\" -s --max-time 10 --fail https://httpbin.org/get"
    call :run_test "User agent test" "\"%INSTALL_PREFIX%\bin\curl.exe\" -s --max-time 10 --fail -A \"libcurl-test\" http://httpbin.org/user-agent"
) else (
    REM Try system curl if available
    where curl >nul 2>&1
    if %errorlevel%==0 (
        echo [INFO] Testing with system curl...
        call :run_test "HTTP request test (system)" "curl -s --max-time 10 --fail http://httpbin.org/get"
        call :run_test "HTTPS request test (system)" "curl -s --max-time 10 --fail https://httpbin.org/get"
    ) else (
        echo [WARNING] No curl available for functionality tests
    )
)

echo.
echo ================================================
echo [INFO] Testing compilation...
echo ================================================

REM Check for compiler
where cl >nul 2>&1
if %errorlevel%==0 (
    set COMPILER=cl
    set COMPILER_FOUND=1
) else (
    where gcc >nul 2>&1
    if %errorlevel%==0 (
        set COMPILER=gcc
        set COMPILER_FOUND=1
    ) else (
        echo [WARNING] No C compiler available, skipping compilation test
        set COMPILER_FOUND=0
    )
)

if %COMPILER_FOUND%==1 (
    echo [INFO] Found compiler: %COMPILER%
    
    REM Create temporary test program
    set TEST_DIR=%TEMP%\libcurl_test_%RANDOM%
    mkdir "%TEST_DIR%" 2>nul
    
    echo #include ^<stdio.h^> > "%TEST_DIR%\test_libcurl.c"
    echo #include ^<curl/curl.h^> >> "%TEST_DIR%\test_libcurl.c"
    echo. >> "%TEST_DIR%\test_libcurl.c"
    echo int main() { >> "%TEST_DIR%\test_libcurl.c"
    echo     CURL *curl; >> "%TEST_DIR%\test_libcurl.c"
    echo     curl = curl_easy_init(); >> "%TEST_DIR%\test_libcurl.c"
    echo     if(curl) { >> "%TEST_DIR%\test_libcurl.c"
    echo         curl_easy_setopt(curl, CURLOPT_URL, "http://httpbin.org/get"); >> "%TEST_DIR%\test_libcurl.c"
    echo         curl_easy_setopt(curl, CURLOPT_NOBODY, 1L); >> "%TEST_DIR%\test_libcurl.c"
    echo         curl_easy_cleanup(curl); >> "%TEST_DIR%\test_libcurl.c"
    echo         printf("libcurl test successful\n"); >> "%TEST_DIR%\test_libcurl.c"
    echo         return 0; >> "%TEST_DIR%\test_libcurl.c"
    echo     } >> "%TEST_DIR%\test_libcurl.c"
    echo     printf("Failed to initialize libcurl\n"); >> "%TEST_DIR%\test_libcurl.c"
    echo     return 1; >> "%TEST_DIR%\test_libcurl.c"
    echo } >> "%TEST_DIR%\test_libcurl.c"
    
    cd /d "%TEST_DIR%"
    
    if "%COMPILER%"=="cl" (
        REM Visual Studio compiler
        call :run_test "Compile test program (MSVC)" "cl /I\"%INSTALL_PREFIX%\include\" test_libcurl.c /link /LIBPATH:\"%INSTALL_PREFIX%\lib\" libcurl.lib ws2_32.lib wldap32.lib crypt32.lib normaliz.lib"
        
        if exist "test_libcurl.exe" (
            call :run_test "Run test program" "test_libcurl.exe"
        )
    ) else if "%COMPILER%"=="gcc" (
        REM MinGW/GCC compiler
        call :run_test "Compile test program (GCC)" "gcc -I\"%INSTALL_PREFIX%\include\" -L\"%INSTALL_PREFIX%\lib\" test_libcurl.c -lcurl -lws2_32 -lwldap32 -lcrypt32 -lnormaliz -o test_libcurl.exe"
        
        if exist "test_libcurl.exe" (
            call :run_test "Run test program" "test_libcurl.exe"
        )
    )
    
    REM Cleanup
    cd /d %~dp0
    rmdir /s /q "%TEST_DIR%" 2>nul
)

echo.
echo ================================================
echo [INFO] Verification Summary
echo ================================================
echo Total tests: %TESTS_TOTAL%
echo Passed: %TESTS_PASSED%
echo Failed: %TESTS_FAILED%
echo.

if %TESTS_FAILED%==0 (
    echo [SUCCESS] All verification tests passed!
    echo libcurl appears to be correctly built and installed.
) else (
    echo [WARNING] %TESTS_FAILED% out of %TESTS_TOTAL% tests failed
    echo There may be issues with the libcurl installation.
    echo Run with VERBOSE=yes for more detailed information.
)

echo.
echo [INFO] Installation details:
echo   Install prefix: %INSTALL_PREFIX%

if exist "%INSTALL_PREFIX%\bin\curl.exe" (
    echo   curl version:
    "%INSTALL_PREFIX%\bin\curl.exe" --version 2>nul | findstr /C:"curl"
)

echo.
echo [INFO] Quick usage examples:
echo   "%INSTALL_PREFIX%\bin\curl.exe" --version
echo   "%INSTALL_PREFIX%\bin\curl.exe" -I https://httpbin.org/get
echo   Add %INSTALL_PREFIX%\bin to your PATH environment variable

pause