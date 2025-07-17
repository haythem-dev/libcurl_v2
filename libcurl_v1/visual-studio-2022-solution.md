# Visual Studio 2022 Solution for libcurl Build

## The Problem
The CMake configuration failed because it's trying to find optional dependencies (Libpsl, ZLIB, Brotli, etc.) that aren't installed on your system.

## Quick Solution

### Option 1: Use the Updated Simple Script (Recommended)
Run the updated `build_libcurl_simple.bat` which now disables all optional dependencies:

```cmd
build_libcurl_simple.bat
```

### Option 2: Use the VS2022 Optimized Script
Use the new `build_libcurl_vs2022.bat` script:

```cmd
build_libcurl_vs2022.bat
```

### Option 3: Use vcpkg (Easiest for development)
If you plan to use libcurl in Visual Studio projects:

```cmd
build_libcurl_vcpkg.bat
```

## Manual Solution

If you prefer to run the commands manually:

```cmd
cd libcurl-build\curl-8.14.1
mkdir build
cd build

cmake .. -G "Visual Studio 17 2022" -A x64 ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DCMAKE_INSTALL_PREFIX="C:\libcurl" ^
    -DCURL_USE_SCHANNEL=ON ^
    -DCURL_USE_WINSSL=ON ^
    -DCURL_USE_OPENSSL=OFF ^
    -DCURL_USE_LIBSSH2=OFF ^
    -DCURL_USE_NGHTTP2=OFF ^
    -DCURL_USE_LIBIDN2=OFF ^
    -DCURL_USE_LIBPSL=OFF ^
    -DCURL_ZLIB=OFF ^
    -DCURL_BROTLI=OFF ^
    -DCURL_ZSTD=OFF ^
    -DBUILD_SHARED_LIBS=ON ^
    -DENABLE_MANUAL=OFF

cmake --build . --config Release
cmake --install . --config Release
```

## What Changed

The key changes disable optional dependencies that were causing the build failure:

- `-DCURL_USE_LIBPSL=OFF` - Disables Public Suffix List support
- `-DCURL_ZLIB=OFF` - Disables compression support
- `-DCURL_BROTLI=OFF` - Disables Brotli compression
- `-DCURL_ZSTD=OFF` - Disables Zstandard compression
- `-DCURL_USE_NGHTTP2=OFF` - Disables HTTP/2 support
- `-DCURL_USE_LIBIDN2=OFF` - Disables international domain name support

## Features You Still Get

- Full HTTP/HTTPS support with Windows native SSL (Schannel)
- Both shared (.dll) and static (.lib) libraries
- curl.exe command-line tool
- All basic libcurl functionality
- No external dependencies required

## For Development Projects

If you're using this for development, the vcpkg option is recommended as it:
- Automatically integrates with Visual Studio
- Handles all dependencies
- Provides easy #include support
- No manual configuration needed

## Testing Your Build

After successful build:

```cmd
C:\libcurl\bin\curl.exe --version
C:\libcurl\bin\curl.exe -I https://httpbin.org/get
```

This solution gives you a fully functional libcurl build optimized for Visual Studio 2022 without dependency headaches.