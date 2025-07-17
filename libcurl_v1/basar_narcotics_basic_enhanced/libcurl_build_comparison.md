
# libcurl Build Comparison: Static vs Shared for BASAR Framework

## Executive Summary

This document provides a detailed comparison between static and shared libcurl builds for integration into the BASAR framework. The analysis covers technical aspects, deployment implications, performance characteristics, and maintenance considerations to help make an informed decision for the narcotics project migration from QtFTP.

## Table of Contents

1. [Build Configuration Comparison](#build-configuration-comparison)
2. [Technical Architecture Differences](#technical-architecture-differences)
3. [Deployment and Distribution](#deployment-and-distribution)
4. [Performance Analysis](#performance-analysis)
5. [Security and Compliance](#security-and-compliance)
6. [Maintenance and Updates](#maintenance-and-updates)
7. [Integration Approaches](#integration-approaches)
8. [Platform-Specific Considerations](#platform-specific-considerations)
9. [Recommendations](#recommendations)

---

## Build Configuration Comparison

### Static Build Configuration

```bash
# Static libcurl configuration
./configure --prefix=/usr/local/libcurl \
            --enable-static \
            --disable-shared \
            --with-ssl \
            --enable-ftp \
            --enable-ftps \
            --with-openssl \
            --disable-ldap \
            --disable-ldaps \
            --disable-rtsp \
            --disable-dict \
            --disable-telnet \
            --disable-tftp \
            --disable-pop3 \
            --disable-imap \
            --disable-smb \
            --disable-smtp \
            --disable-gopher \
            --disable-manual \
            --disable-debug \
            --enable-optimize
```

### Shared Build Configuration

```bash
# Shared libcurl configuration
./configure --prefix=/usr/local/libcurl \
            --enable-shared \
            --disable-static \
            --with-ssl \
            --enable-ftp \
            --enable-ftps \
            --with-openssl \
            --enable-threaded-resolver \
            --with-ca-bundle=/etc/ssl/certs/ca-certificates.crt \
            --with-ca-path=/etc/ssl/certs \
            --enable-ipv6 \
            --enable-versioned-symbols
```

### Build Outputs Comparison

| Aspect | Static Build | Shared Build |
|--------|--------------|--------------|
| **Library File** | `libcurl.a` | `libcurl.so` (Linux), `libcurl.dll` (Windows) |
| **Size** | ~2-4 MB | ~500KB - 1MB |
| **Dependencies** | None (self-contained) | System libraries (OpenSSL, zlib, etc.) |
| **Linking** | Compile-time | Runtime |
| **Executable Size** | Larger (+2-4 MB) | Smaller |
| **Memory Usage** | One copy per process | Shared across processes |

---

## Technical Architecture Differences

### Static Linking Architecture

```cpp
// Static linking - all code embedded in executable
┌─────────────────────────────────────────────────────────────────┐
│                    BASAR Application                            │
│  ┌─────────────────────────────────────────────────────────────┐ │
│  │                 Application Code                            │ │
│  │  ┌─────────────────────────────────────────────────────────┐ │ │
│  │  │            CurlFtpClient                                │ │ │
│  │  │  ┌─────────────────────────────────────────────────────┐ │ │ │
│  │  │  │          libcurl Code                               │ │ │ │
│  │  │  │  ┌─────────────────────────────────────────────────┐ │ │ │ │
│  │  │  │  │        OpenSSL Code                             │ │ │ │ │
│  │  │  │  │  ┌─────────────────────────────────────────────┐ │ │ │ │ │
│  │  │  │  │  │          zlib Code                          │ │ │ │ │ │
│  │  │  │  │  └─────────────────────────────────────────────┘ │ │ │ │ │
│  │  │  │  └─────────────────────────────────────────────────┘ │ │ │ │
│  │  │  └─────────────────────────────────────────────────────┘ │ │ │
│  │  └─────────────────────────────────────────────────────────┘ │ │
│  └─────────────────────────────────────────────────────────────┘ │
└─────────────────────────────────────────────────────────────────┘
```

### Shared Linking Architecture

```cpp
// Shared linking - libraries loaded separately
┌─────────────────────────────────────────────────────────────────┐
│                    BASAR Application                            │
│  ┌─────────────────────────────────────────────────────────────┐ │
│  │                 Application Code                            │ │
│  │  ┌─────────────────────────────────────────────────────────┐ │ │
│  │  │            CurlFtpClient                                │ │ │
│  │  │          (references libcurl)                           │ │ │
│  │  └─────────────────────────────────────────────────────────┘ │ │
│  └─────────────────────────────────────────────────────────────┘ │
└─────────────────────────────────────────────────────────────────┘
                                  │
                                  ▼
┌─────────────────────────────────────────────────────────────────┐
│                    System Libraries                             │
│  ┌─────────────────┐ ┌─────────────────┐ ┌─────────────────┐   │
│  │   libcurl.so    │ │   libssl.so     │ │    libz.so      │   │
│  │   (shared)      │ │   (shared)      │ │   (shared)      │   │
│  └─────────────────┘ └─────────────────┘ └─────────────────┘   │
└─────────────────────────────────────────────────────────────────┘
```

---

## Deployment and Distribution

### Static Build Deployment

#### Advantages
- **Self-Contained**: No external dependencies to manage
- **Simplified Deployment**: Single executable file
- **Version Control**: Guaranteed consistent library versions
- **Portable**: Works on systems without libcurl installed
- **Isolation**: No conflicts with system libraries

#### Disadvantages
- **Larger Executables**: Increased disk space and memory usage
- **Duplicate Code**: Multiple processes duplicate the same library code
- **Update Complexity**: Must recompile and redeploy entire application for library updates

#### Deployment Process
```bash
# Static build deployment
┌─────────────────────────────────────────────────────────────────┐
│                    Build Environment                            │
│  1. Download libcurl source                                     │
│  2. Configure with --enable-static --disable-shared            │
│  3. Compile libcurl.a                                          │
│  4. Link with BASAR application                                │
│  5. Generate self-contained executable                         │
└─────────────────────────────────────────────────────────────────┘
                                  │
                                  ▼
┌─────────────────────────────────────────────────────────────────┐
│                   Target Environment                            │
│  1. Copy executable to target system                           │
│  2. No additional library installation required                │
│  3. Application runs immediately                               │
└─────────────────────────────────────────────────────────────────┘
```

### Shared Build Deployment

#### Advantages
- **Smaller Executables**: Reduced disk space and memory usage
- **Shared Resources**: Multiple processes share the same library code
- **Easy Updates**: Update library without recompiling applications
- **System Integration**: Uses system-maintained libraries

#### Disadvantages
- **Dependency Management**: Must ensure correct library versions are available
- **Runtime Failures**: Missing or incompatible libraries cause startup failures
- **Version Conflicts**: Different applications may require different library versions
- **Security Exposure**: Vulnerable to system library compromises

#### Deployment Process
```bash
# Shared build deployment
┌─────────────────────────────────────────────────────────────────┐
│                    Build Environment                            │
│  1. Install libcurl development packages                       │
│  2. Configure with --enable-shared --disable-static           │
│  3. Compile libcurl.so                                         │
│  4. Link with BASAR application (references only)             │
│  5. Generate executable with library dependencies             │
└─────────────────────────────────────────────────────────────────┘
                                  │
                                  ▼
┌─────────────────────────────────────────────────────────────────┐
│                   Target Environment                            │
│  1. Install libcurl runtime packages                           │
│  2. Ensure compatible library versions                         │
│  3. Configure library paths (LD_LIBRARY_PATH)                 │
│  4. Copy executable to target system                           │
│  5. Verify dependencies before running                        │
└─────────────────────────────────────────────────────────────────┘
```

---

## Performance Analysis

### Memory Usage Comparison

| Scenario | Static Build | Shared Build | Notes |
|----------|--------------|--------------|-------|
| **Single Process** | ~8 MB | ~6 MB | Static includes all dependencies |
| **Multiple Processes** | ~8 MB × N | ~6 MB + 2 MB shared | Shared library loaded once |
| **System Impact** | Higher per-process | Lower overall | Depends on number of processes |

### Load Time Comparison

```cpp
// Static build load time
Application Startup:
├── Load executable (8-10 MB)          : ~50ms
├── Initialize embedded libcurl        : ~5ms
├── Initialize embedded OpenSSL        : ~10ms
└── Ready for FTP operations          : ~65ms total

// Shared build load time  
Application Startup:
├── Load executable (2-3 MB)          : ~15ms
├── Load libcurl.so                    : ~20ms
├── Load libssl.so                     : ~25ms
├── Load libz.so                       : ~10ms
├── Resolve symbols                    : ~15ms
└── Ready for FTP operations          : ~85ms total
```

### Runtime Performance

| Operation | Static Build | Shared Build | Difference |
|-----------|--------------|--------------|------------|
| **FTP Connect** | ~200ms | ~200ms | No difference |
| **File Upload** | ~50 MB/s | ~50 MB/s | No difference |
| **Memory Allocation** | Direct | Indirect | Negligible |
| **Function Calls** | Direct | PLT lookup | ~1-2% overhead |

---

## Security and Compliance

### Static Build Security

#### Advantages
- **Controlled Environment**: Known library versions and configurations
- **Reduced Attack Surface**: No external library dependencies
- **Audit Trail**: Exact code versions embedded and traceable
- **Regulatory Compliance**: Easier to validate for medical software

#### Disadvantages
- **Update Lag**: Security patches require full application rebuild
- **Vulnerability Window**: Longer time to deploy security fixes
- **Code Duplication**: Multiple copies of potentially vulnerable code

### Shared Build Security

#### Advantages
- **Rapid Updates**: Security patches applied system-wide
- **Centralized Management**: Single point for library updates
- **Reduced Duplication**: Single copy of library code

#### Disadvantages
- **Dependency Risk**: Vulnerable to system library compromises
- **Version Conflicts**: Security updates may break compatibility
- **External Control**: Security depends on system administration

### BASAR Framework Security Requirements

For pharmaceutical applications like BASAR:

```cpp
// Security validation requirements
┌─────────────────────────────────────────────────────────────────┐
│                   Regulatory Compliance                         │
│  ┌─────────────────────────────────────────────────────────────┐ │
│  │  Static Build Advantages:                                   │ │
│  │  • Exact version control and validation                    │ │
│  │  • Reproducible builds for auditing                        │ │
│  │  • No external dependency vulnerabilities                  │ │
│  │  • Controlled security update timeline                     │ │
│  └─────────────────────────────────────────────────────────────┘ │
│  ┌─────────────────────────────────────────────────────────────┐ │
│  │  Shared Build Challenges:                                  │ │
│  │  • Dependency on system security practices                 │ │
│  │  • Potential for uncontrolled updates                      │ │
│  │  • Complex validation of library combinations              │ │
│  │  • External attack vectors through system libraries        │ │
│  └─────────────────────────────────────────────────────────────┘ │
└─────────────────────────────────────────────────────────────────┘
```

---

## Maintenance and Updates

### Static Build Maintenance

#### Update Process
```bash
# Static build update workflow
1. Security Advisory Released
   ├── Evaluate impact on BASAR
   ├── Download new libcurl version
   ├── Test compatibility
   ├── Rebuild application
   ├── Full regression testing
   ├── Package for deployment
   └── Deploy to production

Timeline: 2-4 weeks
Control: High
Risk: Low (controlled testing)
```

#### Maintenance Considerations
- **Planned Updates**: Regular schedule for library updates
- **Testing Requirements**: Full application testing for each update
- **Rollback Strategy**: Easy to revert to previous application version
- **Documentation**: Exact library versions documented

### Shared Build Maintenance

#### Update Process
```bash
# Shared build update workflow
1. Security Advisory Released
   ├── System administrator updates libcurl
   ├── Application may stop working
   ├── Emergency compatibility testing
   ├── Possible application hotfix
   └── Production verification

Timeline: Hours to days
Control: Low (depends on system admin)
Risk: Medium (potential breakage)
```

#### Maintenance Considerations
- **Reactive Updates**: Respond to system changes
- **Compatibility Testing**: Verify each system update
- **Dependency Tracking**: Monitor system library versions
- **Emergency Procedures**: Handle unexpected breakages

---

## Integration Approaches

### Static Integration in BASAR

#### CMake Configuration
```cmake
# Static libcurl integration
find_package(PkgConfig REQUIRED)

# Find static libcurl
set(LIBCURL_USE_STATIC_LIBS ON)
find_package(CURL REQUIRED)

# Or manually specify paths
if(LIBCURL_STATIC_BUILD)
    set(LIBCURL_ROOT "/usr/local/libcurl")
    set(LIBCURL_INCLUDE_DIRS "${LIBCURL_ROOT}/include")
    set(LIBCURL_LIBRARIES "${LIBCURL_ROOT}/lib/libcurl.a")
    
    # Add static library dependencies
    if(WIN32)
        list(APPEND LIBCURL_LIBRARIES 
            ws2_32 wldap32 crypt32 normaliz)
    else()
        list(APPEND LIBCURL_LIBRARIES 
            ssl crypto z)
    endif()
endif()

# Configure compiler flags
add_definitions(-DCURL_STATICLIB)
target_include_directories(libbasarcmnutil PUBLIC ${LIBCURL_INCLUDE_DIRS})
target_link_libraries(libbasarcmnutil ${LIBCURL_LIBRARIES})
```

#### Visual Studio Configuration
```xml
<!-- Static libcurl configuration -->
<PropertyGroup>
  <LibCurlRoot>C:\dev\3rdparty\libcurl\win64</LibCurlRoot>
  <LibCurlStaticLib>$(LibCurlRoot)\lib\libcurl_a.lib</LibCurlStaticLib>
</PropertyGroup>

<ItemDefinitionGroup>
  <ClCompile>
    <AdditionalIncludeDirectories>$(LibCurlRoot)\include</AdditionalIncludeDirectories>
    <PreprocessorDefinitions>CURL_STATICLIB;%(PreprocessorDefinitions)</PreprocessorDefinitions>
  </ClCompile>
  <Link>
    <AdditionalDependencies>$(LibCurlStaticLib);ws2_32.lib;wldap32.lib;crypt32.lib;normaliz.lib</AdditionalDependencies>
  </Link>
</ItemDefinitionGroup>
```

### Shared Integration in BASAR

#### CMake Configuration
```cmake
# Shared libcurl integration
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBCURL REQUIRED libcurl)

# Or use find_package
find_package(CURL REQUIRED)

# Configure compiler flags
target_include_directories(libbasarcmnutil PUBLIC ${LIBCURL_INCLUDE_DIRS})
target_link_libraries(libbasarcmnutil ${LIBCURL_LIBRARIES})
target_compile_definitions(libbasarcmnutil PRIVATE ${LIBCURL_DEFINITIONS})
```

#### Runtime Library Path
```cpp
// Shared library runtime configuration
// Linux: LD_LIBRARY_PATH
export LD_LIBRARY_PATH=/usr/local/libcurl/lib:$LD_LIBRARY_PATH

// Windows: PATH
set PATH=C:\dev\3rdparty\libcurl\win64\bin;%PATH%

// Or use rpath in CMake
set_target_properties(libbasarcmnutil PROPERTIES
    INSTALL_RPATH "/usr/local/libcurl/lib"
    BUILD_WITH_INSTALL_RPATH TRUE)
```

---

## Platform-Specific Considerations

### Windows Platform

#### Static Build
```cpp
// Windows static build considerations
Advantages:
+ No DLL hell issues
+ Portable across Windows versions
+ No runtime redistributables needed
+ Consistent behavior across deployments

Disadvantages:
- Larger executable size
- Increased memory usage per process
- Update complexity for security patches

Build Requirements:
- Visual Studio 2019/2022
- Windows SDK
- Static runtime libraries (/MT)
```

#### Shared Build
```cpp
// Windows shared build considerations
Advantages:
+ Smaller executable size
+ Easier security updates
+ Shared across multiple applications

Disadvantages:
- DLL dependency management
- Runtime redistributables required
- Potential version conflicts
- System PATH configuration needed

Build Requirements:
- Visual Studio 2019/2022
- Windows SDK
- Dynamic runtime libraries (/MD)
- Proper DLL deployment
```

### Linux Platform

#### Static Build
```cpp
// Linux static build considerations
Advantages:
+ No shared library dependencies
+ Portable across Linux distributions
+ Consistent behavior regardless of system libraries
+ Simplified deployment

Disadvantages:
- Larger executable size
- Increased memory usage
- Must rebuild for security updates

Build Requirements:
- GCC/Clang compiler
- Static development libraries
- OpenSSL static libraries
```

#### Shared Build
```cpp
// Linux shared build considerations
Advantages:
+ Smaller executable size
+ System-wide security updates
+ Standard package management

Disadvantages:
- Distribution-specific dependencies
- Runtime library version management
- LD_LIBRARY_PATH configuration

Build Requirements:
- GCC/Clang compiler
- libcurl development packages
- Proper library versioning
```

---

## Recommendations

### For BASAR Framework Integration

Based on the analysis, here are the recommendations:

#### Primary Recommendation: Static Build

```cpp
┌─────────────────────────────────────────────────────────────────┐
│                    RECOMMENDED: Static Build                    │
│                                                                 │
│  Reasons for BASAR Framework:                                   │
│  ✓ Pharmaceutical/medical software regulatory requirements     │
│  ✓ Predictable, controlled deployment environment             │
│  ✓ Simplified dependency management                           │
│  ✓ Consistent behavior across different systems               │
│  ✓ Reduced risk of runtime failures                           │
│  ✓ Better suited for enterprise environments                  │
│  ✓ Easier validation and audit processes                      │
│                                                                 │
│  Trade-offs Accepted:                                          │
│  • Larger executable size (~2-4 MB increase)                  │
│  • Slightly longer build times                                │
│  • More complex update process                                │
└─────────────────────────────────────────────────────────────────┘
```

#### Implementation Strategy

1. **Phase 1: Static Build Setup**
   ```bash
   # Configure static build
   ./configure --enable-static --disable-shared --with-ssl --enable-ftp
   make install
   ```

2. **Phase 2: BASAR Integration**
   ```cpp
   // Update CMakeLists.txt
   set(LIBCURL_USE_STATIC_LIBS ON)
   add_definitions(-DCURL_STATICLIB)
   ```

3. **Phase 3: Testing and Validation**
   ```cpp
   // Comprehensive testing
   - Unit tests for FTP operations
   - Integration tests with narcotics workflow
   - Performance benchmarks
   - Security validation
   ```

#### Alternative: Hybrid Approach

For environments where both approaches are needed:

```cpp
// Conditional compilation
#ifdef BASAR_STATIC_LIBCURL
    // Static build configuration
    #define CURL_STATICLIB
    #include <curl/curl.h>
#else
    // Shared build configuration
    #include <curl/curl.h>
#endif

// Factory pattern for build type
std::unique_ptr<IFtpClient> createFtpClient() {
#ifdef BASAR_STATIC_LIBCURL
    return std::make_unique<StaticCurlFtpClient>();
#else
    return std::make_unique<SharedCurlFtpClient>();
#endif
}
```

### Decision Matrix

| Criterion | Weight | Static Score | Shared Score | Winner |
|-----------|---------|--------------|--------------|--------|
| **Deployment Simplicity** | 20% | 9/10 | 6/10 | Static |
| **Security/Compliance** | 25% | 9/10 | 6/10 | Static |
| **Performance** | 15% | 8/10 | 8/10 | Tie |
| **Maintenance** | 15% | 7/10 | 8/10 | Shared |
| **Resource Usage** | 10% | 6/10 | 9/10 | Shared |
| **Update Flexibility** | 10% | 6/10 | 9/10 | Shared |
| **Risk Management** | 5% | 9/10 | 7/10 | Static |

**Weighted Score: Static (8.1/10) vs Shared (7.2/10)**

---

## Conclusion

For the BASAR framework and narcotics project, **static linking with libcurl is the recommended approach**. This decision prioritizes:

1. **Regulatory Compliance**: Critical for pharmaceutical software
2. **Deployment Reliability**: Reduced risk of runtime failures
3. **Security Control**: Managed update process with full testing
4. **Enterprise Suitability**: Simplified deployment and maintenance

The trade-offs in executable size and update complexity are acceptable given the requirements and constraints of the medical software domain.

The implementation should follow the static build configuration outlined in the [howTo.md](howTo.md) document, with the specific configure flags optimized for the BASAR framework's needs.
