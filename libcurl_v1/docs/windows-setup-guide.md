# Windows Setup Guide for libcurl Build

This guide provides step-by-step instructions for building libcurl from source on Windows 10 and later.

## Prerequisites

### Required Software

1. **Visual Studio Build Tools 2019/2022** or **Visual Studio Community**
   - Download from: https://visualstudio.microsoft.com/downloads/
   - Install the "C++ build tools" workload
   - Make sure to include Windows 10/11 SDK

2. **CMake 3.15 or later**
   - Download from: https://cmake.org/download/
   - Add to system PATH during installation

3. **Git for Windows** (optional but recommended)
   - Download from: https://git-scm.com/download/win

### Optional Software (for advanced users)

- **7-Zip** for archive extraction (Windows 10 1803+ has built-in tar)
- **Chocolatey** or **Windows Package Manager** for automated dependency installation

## Quick Start

### Method 1: Using Simple Scripts (Recommended for beginners)

1. **Download the simplified scripts**:
   - `download_libcurl_simple.bat`
   - `build_libcurl_simple.bat`

2. **Open Command Prompt** (preferably as Administrator)

3. **Navigate to your build directory**:
   ```cmd
   cd C:\dev\build_libcurl
   ```

4. **Download libcurl source**:
   ```cmd
   download_libcurl_simple.bat
   ```

5. **Build libcurl**:
   ```cmd
   build_libcurl_simple.bat
   ```

### Method 2: Using the Web Interface

1. **Install Python** (if not already installed):
   - Download from: https://python.org/downloads/
   - Make sure to add Python to PATH

2. **Install Flask**:
   ```cmd
   pip install flask
   ```

3. **Start the web application**:
   ```cmd
   python app.py
   ```

4. **Open your browser** to `http://localhost:5000`

5. **Use the web interface** to configure and build libcurl

## Manual Build Process

If you prefer to build manually or need custom configuration:

### Step 1: Download Source Code

```cmd
# Create build directory
mkdir C:\dev\libcurl-build
cd C:\dev\libcurl-build

# Download using PowerShell
powershell -Command "(New-Object Net.WebClient).DownloadFile('https://curl.se/download/curl-8.5.0.tar.gz', 'curl-8.5.0.tar.gz')"

# Extract using built-in tar (Windows 10 1803+)
tar -xzf curl-8.5.0.tar.gz
cd curl-8.5.0
```

### Step 2: Configure Build

```cmd
# Create build directory
mkdir build
cd build

# Configure with CMake
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="C:\libcurl" -DCURL_USE_SCHANNEL=ON -DCURL_USE_WINSSL=ON -DBUILD_SHARED_LIBS=ON
```

### Step 3: Build

```cmd
# Build with all available CPU cores
cmake --build . --config Release --parallel

# Install (optional)
cmake --install . --config Release
```

### Step 4: Test Installation

```cmd
# Test curl executable
C:\libcurl\bin\curl.exe --version

# Test simple HTTP request
C:\libcurl\bin\curl.exe -I https://httpbin.org/get
```

## Common Build Options

### SSL/TLS Support
- `-DCURL_USE_SCHANNEL=ON` - Use Windows native SSL (recommended)
- `-DCURL_USE_OPENSSL=ON` - Use OpenSSL (requires OpenSSL installation)

### Library Types
- `-DBUILD_SHARED_LIBS=ON` - Build DLL (recommended)
- `-DBUILD_STATIC_LIBS=ON` - Build static library

### Additional Features
- `-DCURL_USE_LIBSSH2=ON` - SSH support (requires libssh2)
- `-DCURL_ZLIB=ON` - Compression support (requires zlib)

## Troubleshooting

### Common Issues

1. **"CMake not found"**
   - Install CMake and add to PATH
   - Restart Command Prompt after installation

2. **"Visual Studio compiler not found"**
   - Install Visual Studio Build Tools
   - Run `vcvars64.bat` or use "Developer Command Prompt"

3. **"tar command not found"**
   - Use Windows 10 version 1803 or later
   - Or install 7-Zip and use: `7z x curl-8.5.0.tar.gz`

4. **Build fails with linking errors**
   - Ensure Windows SDK is installed
   - Try using Developer Command Prompt

### Getting Help

- Check the Windows-specific documentation in libcurl source
- Visit: https://curl.se/docs/install.html
- Ask on the curl mailing list: https://curl.se/mail/

## Using libcurl in Your Projects

### CMake Integration

```cmake
find_package(CURL REQUIRED)
target_link_libraries(your_target CURL::libcurl)
```

### Manual Linking

```cmake
target_include_directories(your_target PRIVATE "C:/libcurl/include")
target_link_libraries(your_target "C:/libcurl/lib/libcurl.lib")
```

### Environment Setup

Add to your system PATH:
```
C:\libcurl\bin
```

Add to your development environment:
```
CURL_INCLUDE_DIR=C:\libcurl\include
CURL_LIBRARY=C:\libcurl\lib\libcurl.lib
```

## Advanced Configuration

### Custom Build Options

You can customize the build by setting environment variables before running the build script:

```cmd
set CURL_VERSION=8.5.0
set INSTALL_PREFIX=C:\MyLibcurl
set BUILD_TYPE=Debug
build_libcurl_simple.bat
```

### Building with Different Compilers

- **MinGW**: Use `-G "MinGW Makefiles"` with CMake
- **Clang**: Use `-G "Ninja"` with clang-cl

### Building for Different Architectures

```cmd
# For 32-bit
cmake .. -A Win32

# For 64-bit (default)
cmake .. -A x64

# For ARM64
cmake .. -A ARM64
```

This guide should help you successfully build libcurl on Windows. If you encounter any issues, please refer to the troubleshooting section or seek help from the libcurl community.