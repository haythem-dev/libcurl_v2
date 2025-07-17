# Complete Integration Flow: Narcotics → BASAR → libcurl

## System Architecture Flow Diagram

```
┌─────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                    NARCOTICS APPLICATION                                        │
├─────────────────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                                 │
│  ┌─────────────────────────────────────────────────────────────────────────────────────────────┐│
│  │                              Business Logic Layer                                          ││
│  │                                                                                             ││
│  │  ┌─────────────────────┐  ┌─────────────────────┐  ┌─────────────────────┐                ││
│  │  │ Contribution        │  │ Order Management    │  │ File Generation     │                ││
│  │  │ Voucher Processing  │  │ & Cancellation      │  │ & Transfer          │                ││
│  │  └─────────────────────┘  └─────────────────────┘  └─────────────────────┘                ││
│  │                                         │                                                   ││
│  │                                         │ Calls FTP Operations                              ││
│  │                                         ▼                                                   ││
│  │  ┌─────────────────────────────────────────────────────────────────────────────────────────┐││
│  │  │                    BasarFtpFileTransfer                                                │││
│  │  │                                                                                         │││
│  │  │  // Narcotics-specific FTP operations                                                  │││
│  │  │  storeFile(printData, bgano, voucherno, date)                                          │││
│  │  │  cancelOrder(bgano, voucherno, date)                                                   │││
│  │  │  deleteFile(filePath)                                                                  │││
│  │  │  listDirectory(dir, entries)                                                           │││
│  │  │  doesDirectoryExist(dir)                                                               │││
│  │  │                                                                                         │││
│  │  │  // Private member using BASAR interface                                               │││
│  │  │  std::unique_ptr<basar::cmnutil::IFtpClient> m_ftpClient;                              │││
│  │  │                                                                                         │││
│  │  │  // Constructor calls factory                                                           │││
│  │  │  BasarFtpFileTransfer() :                                                              │││
│  │  │      m_ftpClient(basar::cmnutil::createFtpClient()) {}                                 │││
│  │  └─────────────────────────────────────────────────────────────────────────────────────────┘││
│  │                                         │                                                   ││
│  │                                         │ Factory Call                                      ││
│  │                                         │ basar::cmnutil::createFtpClient()                ││
│  │                                         ▼                                                   ││
│  └─────────────────────────────────────────────────────────────────────────────────────────────┘│
│                                                                                                 │
└─────────────────────────────────────────────────────────────────────────────────────────────────┘
                                            │
                                            │ Interface Boundary
                                            ▼
┌─────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                    BASAR FRAMEWORK                                              │
├─────────────────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                                 │
│  ┌─────────────────────────────────────────────────────────────────────────────────────────────┐│
│  │                           Abstract Interface Layer                                         ││
│  │  📁 libbasarcmnutil_iftpclient.h                                                           ││
│  │                                                                                             ││
│  │  ┌─────────────────────────────────────────────────────────────────────────────────────────┐││
│  │  │                            IFtpClient Interface                                        │││
│  │  │                                                                                         │││
│  │  │  namespace basar::cmnutil {                                                             │││
│  │  │                                                                                         │││
│  │  │  enum RetCodeEnum {                                                                     │││
│  │  │      FTP_OK = 0,                                                                        │││
│  │  │      FTP_OFFLINE,                                                                       │││
│  │  │      FTP_AUTHERROR,                                                                     │││
│  │  │      FTP_FILENOTFOUND,                                                                  │││
│  │  │      FTP_PERMISSIONDENIED,                                                              │││
│  │  │      FTP_TIMEOUT,                                                                       │││
│  │  │      FTP_PROTOCOL_ERROR,                                                                │││
│  │  │      FTP_UNKNOWN_ERROR                                                                  │││
│  │  │  };                                                                                     │││
│  │  │                                                                                         │││
│  │  │  // Abstract interface methods                                                          │││
│  │  │  virtual RetCodeEnum connect(const VarString& hostname, Int16 port = 21) = 0;          │││
│  │  │  virtual RetCodeEnum login(const VarString& username, const VarString& password) = 0;  │││
│  │  │  virtual RetCodeEnum disconnect() = 0;                                                 │││
│  │  │  virtual RetCodeEnum changeDirectory(const VarString& path) = 0;                       │││
│  │  │  virtual RetCodeEnum createDirectory(const VarString& dirName) = 0;                    │││
│  │  │  virtual RetCodeEnum listDirectory(VarString& listing) = 0;                            │││
│  │  │  virtual RetCodeEnum uploadFile(const VarString& localPath,                            │││
│  │  │                                 const VarString& remotePath) = 0;                      │││
│  │  │  virtual RetCodeEnum downloadFile(const VarString& remotePath,                         │││
│  │  │                                   const VarString& localPath) = 0;                     │││
│  │  │  virtual RetCodeEnum deleteFile(const VarString& remotePath) = 0;                      │││
│  │  │  virtual void setProgressCallback(ProgressCallback callback, void* userData) = 0;     │││
│  │  │  virtual void setTimeout(Int32 timeoutSeconds) = 0;                                    │││
│  │  │  virtual void setPassiveMode(bool passive) = 0;                                        │││
│  │  │                                                                                         │││
│  │  │  } // namespace basar::cmnutil                                                          │││
│  │  └─────────────────────────────────────────────────────────────────────────────────────────┘││
│  │                                         │                                                   ││
│  │                                         │ Implemented by                                    ││
│  │                                         ▼                                                   ││
│  │  ┌─────────────────────────────────────────────────────────────────────────────────────────┐││
│  │  │                      Factory Function                                                  ││
│  │  │  📁 libbasarcmnutil_ftpclient.cpp                                                      ││
│  │  │                                                                                         │││
│  │  │  std::unique_ptr<IFtpClient> createFtpClient() {                                       │││
│  │  │      return std::make_unique<CurlFtpClient>();                                          │││
│  │  │  }                                                                                      │││
│  │  └─────────────────────────────────────────────────────────────────────────────────────────┘││
│  │                                         │                                                   ││
│  │                                         │ Creates                                           ││
│  │                                         ▼                                                   ││
│  └─────────────────────────────────────────────────────────────────────────────────────────────┘│
│                                                                                                 │
│  ┌─────────────────────────────────────────────────────────────────────────────────────────────┐│
│  │                      libcurl Implementation Layer                                          ││
│  │  📁 libbasarcmnutil_ftpclient.h & .cpp                                                     ││
│  │                                                                                             ││
│  │  ┌─────────────────────────────────────────────────────────────────────────────────────────┐││
│  │  │                       CurlFtpClient : public IFtpClient                                │││
│  │  │                                                                                         │││
│  │  │  struct Impl {                                                                          │││
│  │  │      CURL* curl;                    // libcurl handle                                   │││
│  │  │      VarString hostname;                                                                │││
│  │  │      Int16 port;                                                                        │││
│  │  │      bool connected;                                                                    │││
│  │  │      VarString lastError;                                                               │││
│  │  │      ProgressCallback progressCallback;                                                 │││
│  │  │      void* progressUserData;                                                            │││
│  │  │  };                                                                                     │││
│  │  │  std::unique_ptr<Impl> m_pImpl;                                                         │││
│  │  │                                                                                         │││
│  │  │  // Interface implementation using libcurl                                              │││
│  │  │  RetCodeEnum connect(const VarString& hostname, Int16 port) override {                 │││
│  │  │      m_pImpl->curl = curl_easy_init();                    // LIBCURL CALL              │││
│  │  │      curl_easy_setopt(m_pImpl->curl, CURLOPT_URL, url);   // LIBCURL CALL              │││
│  │  │      return mapCurlError(curl_easy_perform(m_pImpl->curl)); // LIBCURL CALL            │││
│  │  │  }                                                                                      │││
│  │  │                                                                                         │││
│  │  │  RetCodeEnum uploadFile(const VarString& localPath,                                     │││
│  │  │                         const VarString& remotePath) override {                        │││
│  │  │      curl_easy_setopt(m_pImpl->curl, CURLOPT_UPLOAD, 1L);        // LIBCURL CALL      │││
│  │  │      curl_easy_setopt(m_pImpl->curl, CURLOPT_READDATA, file);    // LIBCURL CALL      │││
│  │  │      return mapCurlError(curl_easy_perform(m_pImpl->curl));       // LIBCURL CALL      │││
│  │  │  }                                                                                      │││
│  │  │                                                                                         │││
│  │  │  // Error mapping helper                                                                │││
│  │  │  RetCodeEnum mapCurlError(CURLcode code) {                                             │││
│  │  │      switch (code) {                                                                    │││
│  │  │          case CURLE_OK: return FTP_OK;                                                  │││
│  │  │          case CURLE_COULDNT_CONNECT: return FTP_OFFLINE;                               │││
│  │  │          case CURLE_LOGIN_DENIED: return FTP_AUTHERROR;                                │││
│  │  │          case CURLE_REMOTE_FILE_NOT_FOUND: return FTP_FILENOTFOUND;                    │││
│  │  │          case CURLE_UPLOAD_FAILED: return FTP_PERMISSIONDENIED;                        │││
│  │  │          case CURLE_OPERATION_TIMEDOUT: return FTP_TIMEOUT;                            │││
│  │  │          default: return FTP_UNKNOWN_ERROR;                                             │││
│  │  │      }                                                                                  │││
│  │  │  }                                                                                      │││
│  │  └─────────────────────────────────────────────────────────────────────────────────────────┘││
│  │                                         │                                                   ││
│  │                                         │ Uses libcurl API                                  ││
│  │                                         ▼                                                   ││
│  └─────────────────────────────────────────────────────────────────────────────────────────────┘│
│                                                                                                 │
└─────────────────────────────────────────────────────────────────────────────────────────────────┘
                                            │
                                            │ Library Boundary
                                            ▼
┌─────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                    LIBCURL LIBRARY                                             │
├─────────────────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                                 │
│  ┌─────────────────────────────────────────────────────────────────────────────────────────────┐│
│  │                              libcurl Easy Interface                                        ││
│  │  📁 ext/libcurl/include/curl/curl.h                                                        ││
│  │                                                                                             ││
│  │  Core Functions Used:                                                                       ││
│  │  ├─ CURL* curl_easy_init()                     // Initialize session                       ││
│  │  ├─ CURLcode curl_easy_setopt(...)             // Configure options                        ││
│  │  ├─ CURLcode curl_easy_perform(CURL* curl)     // Execute FTP operation                    ││
│  │  ├─ void curl_easy_cleanup(CURL* curl)         // Cleanup session                          ││
│  │  ├─ const char* curl_easy_strerror(CURLcode)   // Error to string conversion               ││
│  │  └─ CURLcode curl_easy_getinfo(...)            // Get operation info                       ││
│  │                                                                                             ││
│  │  FTP-Specific Options Used:                                                                 ││
│  │  ├─ CURLOPT_URL                  // "ftp://server:port/path"                               ││
│  │  ├─ CURLOPT_USERNAME             // FTP username                                           ││
│  │  ├─ CURLOPT_PASSWORD             // FTP password                                           ││
│  │  ├─ CURLOPT_FTP_USE_EPSV         // Extended passive mode                                  ││
│  │  ├─ CURLOPT_UPLOAD               // Enable upload mode                                     ││
│  │  ├─ CURLOPT_READDATA             // Data source for uploads                                ││
│  │  ├─ CURLOPT_WRITEDATA            // Data destination for downloads                         ││
│  │  ├─ CURLOPT_INFILESIZE           // Upload file size                                       ││
│  │  ├─ CURLOPT_TIMEOUT              // Operation timeout                                      ││
│  │  ├─ CURLOPT_CONNECTTIMEOUT       // Connection timeout                                     ││
│  │  ├─ CURLOPT_FTP_CREATE_MISSING_DIRS // Auto-create directories                            ││
│  │  ├─ CURLOPT_POSTQUOTE            // Post-transfer FTP commands                             ││
│  │  ├─ CURLOPT_PROGRESSFUNCTION     // Progress callback                                      ││
│  │  └─ CURLOPT_PROGRESSDATA         // Progress callback data                                 ││
│  └─────────────────────────────────────────────────────────────────────────────────────────────┘│
│                                                                                                 │
│  ┌─────────────────────────────────────────────────────────────────────────────────────────────┐│
│  │                           Protocol Implementation                                           ││
│  │  📁 ext/libcurl/lib/                                                                       ││
│  │                                                                                             ││
│  │  Static Libraries Used:                                                                     ││
│  │  ├─ Win32/Debug/libcurl_a.lib      // Windows 32-bit Debug                                 ││
│  │  ├─ Win32/Release/libcurl_a.lib    // Windows 32-bit Release                               ││
│  │  ├─ x64/Debug/libcurl_a.lib        // Windows 64-bit Debug                                 ││
│  │  └─ x64/Release/libcurl_a.lib      // Windows 64-bit Release                               ││
│  │                                                                                             ││
│  │  FTP Protocol Support:                                                                      ││
│  │  ├─ Connection establishment via TCP                                                        ││
│  │  ├─ Authentication (USER/PASS commands)                                                     ││
│  │  ├─ Passive/Active mode negotiation                                                         ││
│  │  ├─ Directory navigation (CWD, CDUP)                                                        ││
│  │  ├─ File transfer (STOR, RETR)                                                              ││
│  │  ├─ Directory listing (LIST, NLST)                                                          ││
│  │  ├─ File/directory operations (DELE, MKD, RMD)                                             ││
│  │  └─ Progress reporting and error handling                                                   ││
│  └─────────────────────────────────────────────────────────────────────────────────────────────┘│
│                                                                                                 │
└─────────────────────────────────────────────────────────────────────────────────────────────────┘
                                            │
                                            │ System Boundary
                                            ▼
┌─────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                 OPERATING SYSTEM                                               │
├─────────────────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                                 │
│  ┌─────────────────────────────────────────────────────────────────────────────────────────────┐│
│  │                              Network Stack                                                 ││
│  │                                                                                             ││
│  │  Windows Dependencies:                                                                      ││
│  │  ├─ ws2_32.lib                  // Winsock2 for network operations                         ││
│  │  ├─ wldap32.lib                 // LDAP support                                            ││
│  │  ├─ crypt32.lib                 // Cryptographic functions                                 ││
│  │  └─ normaliz.lib                // Unicode normalization                                   ││
│  │                                                                                             ││
│  │  Unix/Linux Dependencies:                                                                   ││
│  │  ├─ libssl.so                   // SSL/TLS support                                         ││
│  │  ├─ libcrypto.so                // Cryptographic functions                                 ││
│  │  ├─ libz.so                     // Compression support                                     ││
│  │  └─ System socket libraries     // Network operations                                      ││
│  └─────────────────────────────────────────────────────────────────────────────────────────────┘│
│                                                                                                 │
└─────────────────────────────────────────────────────────────────────────────────────────────────┘
```

## Complete File Workflow Evidence

### 1. Interface Layer (Abstract)
```
📁 libbasarcmnutil_iftpclient.h (✅ CONFIRMED PRESENT)
├── namespace basar::cmnutil
├── class IFtpClient (abstract interface)
├── enum RetCodeEnum (error codes)
├── typedef ProgressCallback (progress function pointer)
└── virtual methods for all FTP operations
```

### 2. Implementation Layer (Concrete)
```
basar/libbasarcmnutil/
├── libbasarcmnutil_iftpclient.h     ✅ CONFIRMED PRESENT - Abstract FTP Interface
├── libbasarcmnutil_ftpclient.h      📝 Concrete Implementation Header  
├── libbasarcmnutil_ftpclient.cpp    📝 libcurl Integration Implementation
├── libbasarcmnutil_definitions.h    ✅ BASAR Type Definitions
└── libbasarcmnutil_functions.h      ✅ Utility Functions

ext/libcurl/                          ✅ BUILT AND INTEGRATED
├── include/curl/                     ✅ Headers Available
├── lib/                             ✅ Static Libraries Available
└── bin/                             ✅ Tools Available
```

### 3. Application Layer (Usage)
```
📁 basarftpfiletransfer.h/.cpp (Narcotics wrapper)
├── class BasarFtpFileTransfer
├── std::unique_ptr<IFtpClient> m_ftpClient
├── Business logic methods (storeFile, cancelOrder, etc.)
└── Constructor calls createFtpClient()
```

## Call Flow Examples with Code Evidence

### 1. File Upload Flow
```cpp
// Narcotics Application
bool BasarFtpFileTransfer::storeFile(const QString& data, const QString& bgano, 
                                     const QString& voucherno, const QString& date) {
    // Use BASAR interface
    auto result = m_ftpClient->connect(hostname, port);
    if (result == basar::cmnutil::IFtpClient::FTP_OK) {
        result = m_ftpClient->uploadFile(localPath.toStdString(), remotePath.toStdString());
    }
    m_ftpClient->disconnect();
    return result == basar::cmnutil::IFtpClient::FTP_OK;
}

// BASAR Implementation  
RetCodeEnum CurlFtpClient::uploadFile(const VarString& localPath, const VarString& remotePath) {
    // Direct libcurl calls
    curl_easy_setopt(m_pImpl->curl, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(m_pImpl->curl, CURLOPT_READDATA, file);
    curl_easy_setopt(m_pImpl->curl, CURLOPT_URL, ftpUrl.c_str());

    CURLcode res = curl_easy_perform(m_pImpl->curl);
    return mapCurlError(res);
}
```

### 2. Error Handling Chain
```cpp
// libcurl returns CURLcode
CURLcode libcurl_result = curl_easy_perform(curl);

// BASAR maps to enum
RetCodeEnum CurlFtpClient::mapCurlError(CURLcode code) {
    switch (code) {
        case CURLE_OK:                  return FTP_OK;
        case CURLE_COULDNT_CONNECT:     return FTP_OFFLINE;
        case CURLE_LOGIN_DENIED:        return FTP_AUTHERROR;
        case CURLE_REMOTE_FILE_NOT_FOUND: return FTP_FILENOTFOUND;
        default:                        return FTP_UNKNOWN_ERROR;
    }
}

// Narcotics handles BASAR enum
if (ftpResult == basar::cmnutil::IFtpClient::FTP_OK) {
    // Success
} else {
    // Handle specific error cases
}
```

### 3. Include Dependency Chain
```cpp
// Narcotics Application
#include <libbasarcmnutil_iftpclient.h>     // ✅ Interface only
   ↓
// BASAR Framework  
#include "libbasarcmnutil_definitions.h"    // BASAR types
#include "libbasarcmnutil_functions.h"      // BASAR utilities
   ↓
// Implementation (hidden from narcotics)
#include <curl/curl.h>                      // libcurl API
#include <curl/easy.h>                      // libcurl Easy interface
```

## Build Integration Evidence

### 1. Project Configuration
```xml
<!-- narcotic.vcxproj -->
<AdditionalIncludeDirectories>
    $(SolutionDir)..\..\pharmos.base.basar\dev\src;     <!-- BASAR Headers -->
    $(SolutionDir)ext\libcurl\include;                   <!-- libcurl Headers -->
</AdditionalIncludeDirectories>

<AdditionalDependencies>
    libcurl_a.lib;                                       <!-- Static libcurl -->
    libbasarcmnutil.lib;                                 <!-- BASAR Framework -->
    ws2_32.lib;wldap32.lib;crypt32.lib;normaliz.lib;     <!-- Windows System -->
</AdditionalDependencies>

<PreprocessorDefinitions>
    CURL_STATICLIB;                                      <!-- Static linking -->
</PreprocessorDefinitions>
```

### 2. Factory Pattern Implementation
```cpp
// Factory function in libbasarcmnutil_ftpclient.cpp
namespace basar {
namespace cmnutil {

std::unique_ptr<IFtpClient> createFtpClient() {
    return std::make_unique<CurlFtpClient>();
}

} // namespace cmnutil
} // namespace basar
```

## Summary

This architecture provides:

1. **Clean Separation**: Narcotics → Interface → Implementation → Library
2. **Type Safety**: Strong typing with BASAR framework types (VarString, Int16, etc.)
3. **Error Abstraction**: libcurl errors mapped to business-friendly enum values
4. **Maintainability**: Implementation can change without affecting narcotics code
5. **Testability**: Interface can be mocked for unit testing
6. **Dependency Isolation**: Narcotics never directly depends on libcurl

The key discovery is that `libbasarcmnutil_iftpclient.h` exists and serves as the **critical abstraction layer** that enables this clean architecture.