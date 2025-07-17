# Windows Quick Start Guide

Having issues with the batch scripts? Here's a simple step-by-step guide to build libcurl on Windows 10+.

## Problem with the Original Scripts

The original batch scripts had syntax issues. Here's a working solution:

## Prerequisites

1. **Visual Studio Build Tools** or **Visual Studio Community**
2. **CMake** (download from cmake.org)
3. **Windows 10 version 1803+** (for built-in tar command)

## Quick Solution

### Step 1: Download Source

```cmd
# Create and navigate to build directory
mkdir C:\dev\libcurl-build
cd C:\dev\libcurl-build

# Download using PowerShell
powershell -Command "(New-Object Net.WebClient).DownloadFile('https://curl.se/download/curl-8.5.0.tar.gz', 'curl-8.5.0.tar.gz')"

# Extract
tar -xzf curl-8.5.0.tar.gz
cd curl-8.5.0
```

### Step 2: Build

```cmd
# Create build directory
mkdir build
cd build

# Configure
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="C:\libcurl" -DCURL_USE_SCHANNEL=ON

# Build
cmake --build . --config Release

# Install (optional)
cmake --install . --config Release
```

### Step 3: Test

```cmd
# Test the installation
C:\libcurl\bin\curl.exe --version
```

## If You Get Errors

1. **"cmake not found"** - Install CMake and restart Command Prompt
2. **"compiler not found"** - Install Visual Studio Build Tools
3. **"tar not found"** - Use 7-Zip: `7z x curl-8.5.0.tar.gz`

## Working Scripts

Use these simplified scripts instead:

- [download_libcurl_simple.bat](scripts/download_libcurl_simple.bat)
- [build_libcurl_simple.bat](scripts/build_libcurl_simple.bat)

These scripts have been tested and work correctly on Windows 10/11.