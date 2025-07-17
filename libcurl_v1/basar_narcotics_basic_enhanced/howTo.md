
# How to Download and Integrate libcurl into BASAR Packages

## Overview

This guide provides step-by-step instructions for downloading libcurl from the internet and integrating it into the BASAR framework packages (`pharmos.base.basar` and `pharmos.outbound.narcotics`).

## Table of Contents

1. [Prerequisites](#prerequisites)
2. [Download libcurl](#download-libcurl)
3. [Platform-Specific Setup](#platform-specific-setup)
4. [Integration into BASAR Packages](#integration-into-basar-packages)
5. [Build Configuration](#build-configuration)
6. [Verification](#verification)
7. [Troubleshooting](#troubleshooting)

---

## Prerequisites

Before starting, ensure you have:

- **Build Environment**: Visual Studio (Windows) or GCC/Make (Linux/AIX)
- **CMake**: Version 3.10 or higher
- **Git**: For version control
- **Internet Connection**: To download libcurl
- **Administrative Rights**: For system-wide installation (optional)

---

## Download libcurl

### Option 1: Download Pre-compiled Binaries (Recommended)

#### For Windows (Win64):
```bash
# Navigate to your development directory
cd /path/to/your/dev/directory

# Download libcurl for Windows
curl -L -o libcurl-win64.zip "https://curl.se/windows/dl-8.4.0/curl-8.4.0-win64-mingw.zip"

# Extract the archive
unzip libcurl-win64.zip
mv curl-8.4.0-win64-mingw libcurl-8.4.0-win64
```

#### For Linux/AIX:
```bash
# Using package manager (Ubuntu/Debian)
sudo apt-get update
sudo apt-get install libcurl4-openssl-dev

# Or download source for custom build
wget https://curl.se/download/curl-8.4.0.tar.gz
tar -xzf curl-8.4.0.tar.gz
cd curl-8.4.0
```

### Option 2: Build from Source

```bash
# Download source code
wget https://curl.se/download/curl-8.4.0.tar.gz
tar -xzf curl-8.4.0.tar.gz
cd curl-8.4.0

# Configure build
./configure --prefix=/usr/local/libcurl --enable-static --disable-shared \
            --with-ssl --enable-ftp --enable-ftps

# Build and install
make -j4
sudo make install
```

---

## Platform-Specific Setup

### Windows Setup

1. **Create libcurl directory structure:**
```
C:\dev\3rdparty\libcurl\
├── win64\
│   ├── include\
│   │   └── curl\
│   │       ├── curl.h
│   │       ├── easy.h
│   │       └── ...
│   ├── lib\
│   │   ├── libcurl.lib
│   │   └── libcurl_a.lib
│   └── bin\
│       └── libcurl.dll
```

2. **Copy downloaded files:**
```cmd
copy curl-8.4.0-win64-mingw\include\* C:\dev\3rdparty\libcurl\win64\include\
copy curl-8.4.0-win64-mingw\lib\* C:\dev\3rdparty\libcurl\win64\lib\
copy curl-8.4.0-win64-mingw\bin\* C:\dev\3rdparty\libcurl\win64\bin\
```

### Linux/AIX Setup

1. **Verify installation:**
```bash
pkg-config --cflags libcurl
pkg-config --libs libcurl
```

2. **For custom installation, set environment variables:**
```bash
export LIBCURL_ROOT=/usr/local/libcurl
export LD_LIBRARY_PATH=$LIBCURL_ROOT/lib:$LD_LIBRARY_PATH
export PKG_CONFIG_PATH=$LIBCURL_ROOT/lib/pkgconfig:$PKG_CONFIG_PATH
```

---

## Integration into BASAR Packages

### 1. Update Ivy Dependencies

The libcurl dependency is already configured in both packages:

**pharmos.base.basar/dev/src/ivy.xml:**
```xml
<dependency org="org.curl" name="libcurl" rev="7.85.0-1-${platform}" conf="compile->*"/>
```

**pharmos.outbound.narcotics/dev/src/ivy.xml:**
```xml
<dependency org="org.curl" name="libcurl" rev="7.85.0-1-${platform}" conf="compile->*"/>
```

### 2. Update Build Configuration

#### For Visual Studio Projects (.vcxproj):

Add to your project files:

```xml
<PropertyGroup>
  <LibCurlRoot Condition="'$(Platform)'=='x64'">C:\dev\3rdparty\libcurl\win64</LibCurlRoot>
</PropertyGroup>

<ItemDefinitionGroup>
  <ClCompile>
    <AdditionalIncludeDirectories>$(LibCurlRoot)\include;%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>
    <PreprocessorDefinitions>CURL_STATICLIB;%(PreprocessorDefinitions)</PreprocessorDefinitions>
  </ClCompile>
  <Link>
    <AdditionalLibraryDirectories>$(LibCurlRoot)\lib;%(AdditionalLibraryDirectories)</AdditionalLibraryDirectories>
    <AdditionalDependencies>libcurl_a.lib;ws2_32.lib;wldap32.lib;crypt32.lib;%(AdditionalDependencies)</AdditionalDependencies>
  </Link>
</ItemDefinitionGroup>
```

#### For Makefiles:

Add to your Makefile:

```makefile
# libcurl configuration
LIBCURL_ROOT ?= /usr/local/libcurl
LIBCURL_CFLAGS = -I$(LIBCURL_ROOT)/include -DCURL_STATICLIB
LIBCURL_LIBS = -L$(LIBCURL_ROOT)/lib -lcurl -lssl -lcrypto -lz

# Add to existing flags
CPPFLAGS += $(LIBCURL_CFLAGS)
LDFLAGS += $(LIBCURL_LIBS)
```

### 3. Verify Header Integration

Ensure the libcurl headers are properly integrated in your source files:

**pharmos.base.basar/dev/src/basar/libbasarcmnutil/libbasarcmnutil_ftpclient.h:**
```cpp
#include <curl/curl.h>
```

**pharmos.base.basar/dev/src/basar/libbasarcmnutil/libbasarcmnutil_ftpclient.cpp:**
```cpp
#include <curl/curl.h>
#include "libbasarcmnutil_ftpclient.h"
```

---

## Build Configuration

### 1. CMake Configuration (if using CMake)

Create or update `CMakeLists.txt`:

```cmake
# Find libcurl
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBCURL REQUIRED libcurl)

# Or manually specify
if(WIN32)
    set(LIBCURL_ROOT "C:/dev/3rdparty/libcurl/win64")
    set(LIBCURL_INCLUDE_DIRS "${LIBCURL_ROOT}/include")
    set(LIBCURL_LIBRARIES "${LIBCURL_ROOT}/lib/libcurl_a.lib")
    set(LIBCURL_DEFINITIONS "-DCURL_STATICLIB")
endif()

# Add definitions and include directories
add_definitions(${LIBCURL_DEFINITIONS})
include_directories(${LIBCURL_INCLUDE_DIRS})

# Link libraries
target_link_libraries(your_target ${LIBCURL_LIBRARIES})

# Windows additional libraries
if(WIN32)
    target_link_libraries(your_target ws2_32 wldap32 crypt32)
endif()
```

### 2. Environment Setup Script

Create a setup script for development environment:

**setup_libcurl.bat (Windows):**
```batch
@echo off
set LIBCURL_ROOT=C:\dev\3rdparty\libcurl\win64
set PATH=%LIBCURL_ROOT%\bin;%PATH%
echo libcurl environment configured
```

**setup_libcurl.sh (Linux/AIX):**
```bash
#!/bin/bash
export LIBCURL_ROOT=/usr/local/libcurl
export LD_LIBRARY_PATH=$LIBCURL_ROOT/lib:$LD_LIBRARY_PATH
export PKG_CONFIG_PATH=$LIBCURL_ROOT/lib/pkgconfig:$PKG_CONFIG_PATH
echo "libcurl environment configured"
```

---

## Verification

### 1. Compilation Test

Create a simple test file to verify libcurl integration:

**test_libcurl.cpp:**
```cpp
#include <curl/curl.h>
#include <iostream>

int main() {
    curl_version_info_data* version_info = curl_version_info(CURLVERSION_NOW);
    std::cout << "libcurl version: " << version_info->version << std::endl;
    std::cout << "Supported protocols: " << std::endl;
    
    for (int i = 0; version_info->protocols[i]; i++) {
        std::cout << "  " << version_info->protocols[i] << std::endl;
    }
    
    return 0;
}
```

### 2. Build Test

```bash
# Linux/AIX
g++ -I/usr/local/libcurl/include test_libcurl.cpp -L/usr/local/libcurl/lib -lcurl -o test_libcurl

# Windows (with Visual Studio)
cl /I"C:\dev\3rdparty\libcurl\win64\include" test_libcurl.cpp /link "C:\dev\3rdparty\libcurl\win64\lib\libcurl_a.lib" ws2_32.lib wldap32.lib crypt32.lib
```

### 3. Runtime Test

```bash
./test_libcurl
```

Expected output:
```
libcurl version: 8.4.0
Supported protocols:
  dict
  file
  ftp
  ftps
  gopher
  ...
```

---

## Troubleshooting

### Common Issues and Solutions

#### 1. **Header Not Found Error**
```
error: curl/curl.h: No such file or directory
```

**Solution:**
- Verify include path is correctly set
- Check that libcurl headers are installed in the specified directory
- Ensure case sensitivity (Linux/AIX)

#### 2. **Linking Errors on Windows**
```
unresolved external symbol __imp_curl_easy_init
```

**Solution:**
- Add `CURL_STATICLIB` preprocessor definition
- Link against `libcurl_a.lib` (static version)
- Include Windows system libraries: `ws2_32.lib`, `wldap32.lib`, `crypt32.lib`

#### 3. **Runtime Library Conflicts**
```
The procedure entry point could not be located
```

**Solution:**
- Ensure consistent runtime library linking (MT/MD)
- Use static linking to avoid DLL conflicts
- Check PATH environment variable for conflicting DLLs

#### 4. **SSL/TLS Support Missing**
```
Protocol "ftps" not supported or disabled in libcurl
```

**Solution:**
- Download libcurl with SSL support enabled
- Or rebuild from source with `--with-ssl` option
- Verify SSL backend: `curl-config --ssl-backends`

#### 5. **Version Mismatch**
**Solution:**
- Update ivy.xml to match your libcurl version:
```xml
<dependency org="org.curl" name="libcurl" rev="8.4.0-1-${platform}" conf="compile->*"/>
```

### Debug Commands

```bash
# Check libcurl configuration
curl-config --version
curl-config --protocols
curl-config --features

# Verify library linking (Linux)
ldd your_executable | grep curl

# Windows: Use Dependency Walker or similar tool
```

---

## Best Practices

1. **Version Control**: Always specify exact libcurl versions in dependencies
2. **Static Linking**: Prefer static linking to avoid runtime dependencies
3. **Security**: Keep libcurl updated for security patches
4. **Testing**: Test on all target platforms before deployment
5. **Documentation**: Document any custom build flags or configurations

---

## Additional Resources

- [Official libcurl Website](https://curl.se/libcurl/)
- [libcurl Documentation](https://curl.se/libcurl/c/)
- [Building libcurl](https://curl.se/docs/install.html)
- [libcurl Examples](https://curl.se/libcurl/c/example.html)

---

## Contact and Support

For BASAR-specific integration issues, refer to:
- BASAR Framework Documentation
- Internal development team
- Architecture documentation: `basar_libcurl_architecture.md`
