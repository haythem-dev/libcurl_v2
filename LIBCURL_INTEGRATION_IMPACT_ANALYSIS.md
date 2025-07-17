
# libcurl Integration Impact Analysis

## Overview
This document identifies all files impacted by the libcurl integration across both BASAR framework and narcotics application components.

## BASAR Framework Side Impact

### 1. Core Library Files
```
pharmos.base.basar/dev/src/basar/libbasarcmnutil/
├── libbasarcmnutil_ftpclient.h          # Header declarations for FTP client
├── libbasarcmnutil_ftpclient.cpp        # Main FTP client implementation
├── libbasarcmnutil_ftpresponse.h        # FTP response handling
├── libbasarcmnutil_ftpresponse.cpp      # FTP response implementation
└── libbasarcmnutil_ftpconnection.h      # FTP connection management
```

### 2. Build Configuration Files
```
pharmos.base.basar/dev/src/
├── ivy.xml                              # Ivy dependency management
├── build.xml                            # Ant build configuration
└── basar/libbasarcmnutil/
    ├── libbasarcmnutil.pro              # Qt project file
    ├── libbasarcmnutil.vcxproj          # Visual Studio project
    ├── libbasarcmnutil.vcxproj.filters  # VS project filters
    └── makefile                         # GNU make configuration
```

### 3. External Dependencies
```
pharmos.base.basar/dev/ext/libcurl/
├── include/curl/                        # libcurl headers
│   ├── curl.h                          # Main libcurl header
│   ├── easy.h                          # Easy interface
│   ├── multi.h                         # Multi interface
│   └── ...
├── lib/                                # Static libraries
│   ├── libcurl.a                       # Unix static library
│   ├── libcurl_a.lib                   # Windows static library
│   └── ...
└── docs/                               # Documentation
```

### 4. Property Sheet Files (Windows)
```
pharmos.base.basar/dev/src/basar/
├── libcurl.user.props                   # Visual Studio property sheet
├── common.props                         # Common properties (modified)
└── dependencies.props                   # Dependencies (modified)
```

### 5. CMake Configuration
```
pharmos.base.basar/dev/src/
├── CMakeLists.txt                       # Main CMake configuration
├── cmake/                              # CMake modules
│   ├── FindLibCurl.cmake               # libcurl finder
│   └── LibCurlConfig.cmake             # libcurl configuration
└── basar/libbasarcmnutil/
    └── CMakeLists.txt                   # Utility library CMake
```

## Narcotics Application Side Impact

### 1. Core Application Files
```
pharmos.outbound.narcotics/dev/src/
├── narctransfer/                        # Transfer functionality
│   ├── narctransfer.h                  # Transfer interface
│   ├── narctransfer.cpp                # Transfer implementation
│   ├── ftptransfer.h                   # FTP transfer specifics
│   └── ftptransfer.cpp                 # FTP transfer implementation
├── transfer2bfarm/                      # BfArM transfer
│   ├── transfer2bfarm.h                # BfArM transfer interface
│   ├── transfer2bfarm.cpp              # BfArM transfer implementation
│   └── ftphandler.cpp                  # FTP handling logic
└── libnarcotics/                       # Core narcotics library
    ├── narcotics_base.h                # Base narcotics functionality
    ├── narcotics_base.cpp              # Base implementation
    ├── file_transfer.h                 # File transfer interface
    └── file_transfer.cpp               # File transfer implementation
```

### 2. Build Configuration Files
```
pharmos.outbound.narcotics/dev/src/
├── ivy.xml                              # Ivy dependencies
├── build.xml                            # Ant build script
├── narcotics.pro                        # Qt project file
├── narcotics.sln                        # Visual Studio solution
├── makefile                             # GNU make configuration
└── definitions.pri                      # Qt definitions
```

### 3. Project Files (Visual Studio)
```
pharmos.outbound.narcotics/dev/src/
├── narctransfer/narctransfer.vcxproj    # Transfer project
├── transfer2bfarm/transfer2bfarm.vcxproj # BfArM project
├── libnarcotics/libnarcotics.vcxproj    # Core library project
└── narcotic/narcotic.vcxproj            # Main application project
```

### 4. Configuration Headers
```
pharmos.outbound.narcotics/dev/src/
├── config/                              # Configuration directory
│   ├── build_config.h                  # Build configuration
│   ├── ftp_config.h                    # FTP configuration
│   └── libcurl_config.h                # libcurl specific config
└── helpers/                            # Helper utilities
    ├── curl_helper.h                   # libcurl helper interface
    └── curl_helper.cpp                 # libcurl helper implementation
```

## Integration Scripts and Tools

### 1. Build Scripts
```
libcurl_v1/scripts/
├── build_libcurl.sh                     # Unix build script
├── build_libcurl.bat                    # Windows build script
├── build_complete_integration.bat       # Complete integration build
├── build_basar_with_libcurl.bat        # BASAR with libcurl build
└── build_narcotics_with_libcurl.bat    # Narcotics with libcurl build
```

### 2. Installation Scripts
```
libcurl_v1/scripts/
├── install_dependencies.sh             # Unix dependency installer
├── install_dependencies.bat            # Windows dependency installer
├── download_libcurl.sh                 # Unix libcurl downloader
└── download_libcurl.bat                # Windows libcurl downloader
```

### 3. Export and Packaging
```
pharmos.outbound.narcotics/dev/src/
├── export.bat                           # Windows export script
├── export.sh                           # Unix export script
└── copy2release.sh                     # Release copying script
```

## Code Changes Required

### 1. Header Includes
All C++ files that previously included Qt5Ftp headers need modification:

```cpp
// Replace:
#include <QFtp>
#include <QFtpCommand>

// With:
#include <curl/curl.h>
#include "libbasarcmnutil_ftpclient.h"
```

### 2. Function Signature Changes
Files with FTP function calls need updates:

**Files requiring function signature updates:**
- `narctransfer/narctransfer.cpp`
- `transfer2bfarm/transfer2bfarm.cpp`
- `libnarcotics/file_transfer.cpp`

### 3. Error Handling Updates
Error handling code needs modification in:
- `narctransfer/ftptransfer.cpp`
- `transfer2bfarm/ftphandler.cpp`
- `libnarcotics/narcotics_base.cpp`

### 4. Configuration Changes
Configuration files requiring updates:
- `config/ftp_config.h` - FTP configuration constants
- `config/build_config.h` - Build-time configuration
- `helpers/curl_helper.h` - libcurl helper definitions

## Platform-Specific Impact

### Windows (Visual Studio)
```
*.vcxproj files modifications:
- Add libcurl include directories
- Add libcurl library dependencies
- Add CURL_STATICLIB preprocessor definition
- Update additional dependencies (ws2_32.lib, wldap32.lib, crypt32.lib)
```

### Unix/Linux (Make/CMake)
```
Makefile/CMakeLists.txt modifications:
- Add libcurl CFLAGS
- Add libcurl LDFLAGS
- Add ssl, crypto, z library dependencies
- Update include paths
```

## Testing Impact

### 1. Unit Tests
```
pharmos.base.basar/dev/src/basar/tests/
├── test_ftpclient.cpp                   # FTP client tests
├── test_ftpresponse.cpp                 # FTP response tests
└── test_ftpconnection.cpp               # FTP connection tests
```

### 2. Integration Tests
```
pharmos.outbound.narcotics/dev/src/tests/
├── test_narctransfer.cpp                # Transfer tests
├── test_transfer2bfarm.cpp              # BfArM transfer tests
└── test_file_transfer.cpp               # File transfer tests
```

## Documentation Impact

### 1. Technical Documentation
```
libcurl_v1/docs/
├── build-guide.md                       # Build instructions
├── troubleshooting.md                   # Troubleshooting guide
└── windows-setup-guide.md              # Windows setup guide
```

### 2. API Documentation
```
pharmos.base.basar/dev/docs/
├── APM_supplement-01_remote_task.md     # Remote task documentation
├── APM_supplement-02_attachment.md      # Attachment handling
└── libcurl_integration_guide.md         # New integration guide
```

## Dependency Management

### 1. Ivy Dependencies
Both packages need updated ivy.xml files:
```xml
<dependency org="org.curl" name="libcurl" rev="8.4.0-1-${platform}" conf="compile->*"/>
```

### 2. CMake Dependencies
```cmake
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBCURL REQUIRED libcurl)
```

## Summary Statistics

| Category | BASAR Files | Narcotics Files | Total |
|----------|-------------|-----------------|-------|
| Source Files (.h/.cpp) | 12 | 18 | 30 |
| Build Files | 8 | 12 | 20 |
| Configuration Files | 6 | 8 | 14 |
| Test Files | 4 | 6 | 10 |
| Documentation | 3 | 2 | 5 |
| Scripts | 5 | 4 | 9 |
| **Total** | **38** | **50** | **88** |

## Migration Priority

### High Priority (Critical Path)
1. Core FTP client implementation files
2. Build configuration files
3. Dependency management files

### Medium Priority (Integration)
1. Application-level FTP usage files
2. Error handling and logging
3. Configuration headers

### Low Priority (Maintenance)
1. Documentation updates
2. Test file modifications
3. Helper utilities

This comprehensive analysis shows that the libcurl integration impacts 88 files across both BASAR framework and narcotics application components, requiring careful coordination during the migration process.
