# Complete libcurl Integration Summary

## Overview

This document provides a comprehensive overview of how the BASAR framework works in general and specifically how the narcotics application calls libcurl functionalities through the BASAR framework.

## BASAR Framework Architecture

### General Design Principles

The BASAR framework is designed as a **layered architecture** with clear separation of concerns:

1. **Application Layer** - Contains business logic (Narcotics, other apps)
2. **Interface Layer** - Provides stable contracts and abstractions
3. **Implementation Layer** - Contains concrete implementations of interfaces
4. **External Library Layer** - Third-party libraries (libcurl, libssh2, etc.)

### Key Design Patterns

#### 1. **Abstract Factory Pattern**
```cpp
namespace basar::cmnutil {
    std::unique_ptr<IFtpClient> createFtpClient();
    std::unique_ptr<IEmailSender> createEmailSender();
    std::unique_ptr<IRemoteTaskSubmitter> createRemoteTaskSubmitter();
}
```

#### 2. **Strategy Pattern**
Different implementations can be swapped without affecting client code:
- Qt5Ftp → libcurl migration
- Different database backends
- Various authentication mechanisms

#### 3. **RAII (Resource Acquisition Is Initialization)**
```cpp
class FtpClient : public IFtpClient {
    CURL* m_curl;
public:
    FtpClient() : m_curl(curl_easy_init()) {}
    ~FtpClient() { if (m_curl) curl_easy_cleanup(m_curl); }
};
```

### Framework Components

#### libbasarcmnutil (Common Utilities)
- **Responsibilities**: FTP, Email, Remote Tasks, Credentials, Logging
- **Key Classes**: `IFtpClient`, `FtpClient`, `IEmailSender`, `IRemoteTaskSubmitter`
- **Design**: Interface-based with factory creation

#### libbasarappl (Application Layer)
- **Responsibilities**: Application lifecycle, configuration, resources
- **Key Classes**: Application managers, configuration handlers
- **Design**: Singleton and factory patterns

#### libbasardb* (Database Layer)
- **Responsibilities**: Database abstraction, connection pooling, transactions
- **Key Classes**: Database interfaces, connection managers
- **Design**: Connection pool pattern, transaction managers

#### libbasargui* (GUI Layer)
- **Responsibilities**: Qt-based GUI abstractions
- **Key Classes**: Dialog managers, widget factories
- **Design**: Observer pattern for UI updates

## Narcotics Application Integration

### Class Structure

```
┌─────────────────────────────────────────────────────────────────┐
│                    Narcotics Application                        │
│                                                                 │
│  ┌─────────────────────────────────────────────────────────────┐│
│  │              BasarFtpFileTransfer                           ││
│  │                                                             ││
│  │  - Manages FTP operations for narcotics business logic     ││
│  │  - Handles file uploads, downloads, directory operations   ││
│  │  - Provides error handling and logging                     ││
│  │  - Abstracts FTP complexity from business logic            ││
│  │                                                             ││
│  │  Key Methods:                                               ││
│  │  • storeFile() - Upload contribution voucher files         ││
│  │  • cancelOrder() - Delete order-related files              ││
│  │  • deleteFile() - Remove specific files                    ││
│  │  • listDirectory() - Get directory contents                ││
│  │  • doesDirectoryExist() - Check directory existence        ││
│  │                                                             ││
│  │  Dependencies:                                              ││
│  │  • std::unique_ptr<IFtpClient> m_ftpClient                 ││
│  │  • Configuration parameters (host, user, pass, etc.)      ││
│  │  • Error handling and logging utilities                    ││
│  └─────────────────────────────────────────────────────────────┘│
│                                │                                │
│                                │ Factory Pattern                │
│                                │ createFtpClient()               │
│                                ▼                                │
└─────────────────────────────────────────────────────────────────┘
```

### Interface Definition

```cpp
class IFtpClient {
public:
    enum class RetCodeEnum {
        Success = 0,
        ConnectionFailed = 1,
        AuthenticationFailed = 2,
        TransferFailed = 3,
        NetworkError = 4,
        InvalidParameter = 5,
        FileNotFound = 6,
        DirectoryNotFound = 7,
        PermissionDenied = 8,
        DiskFull = 9,
        Timeout = 10
    };

    virtual ~IFtpClient() = default;
    
    // Connection management
    virtual RetCodeEnum connect(const VarString& host, Int32 port, 
                               const VarString& user, const VarString& pass) = 0;
    virtual RetCodeEnum disconnect() = 0;
    virtual bool isConnected() const = 0;
    
    // File operations
    virtual RetCodeEnum uploadFile(const VarString& localPath, 
                                  const VarString& remotePath) = 0;
    virtual RetCodeEnum downloadFile(const VarString& remotePath, 
                                    const VarString& localPath) = 0;
    virtual RetCodeEnum deleteFile(const VarString& remotePath) = 0;
    
    // Directory operations
    virtual RetCodeEnum listDirectory(const VarString& path, 
                                     std::vector<FtpFileInfo>& entries) = 0;
    virtual RetCodeEnum createDirectory(const VarString& path) = 0;
    virtual RetCodeEnum removeDirectory(const VarString& path) = 0;
    
    // Configuration
    virtual void setTransferMode(bool binary) = 0;
    virtual void setPassiveMode(bool passive) = 0;
    virtual void setTimeout(Int32 seconds) = 0;
    
    // Error handling
    virtual VarString getLastError() const = 0;
};
```

### libcurl Implementation

```cpp
class FtpClient : public IFtpClient {
private:
    CURL* m_curl;
    curl_slist* m_curlList;
    bool m_isConnected;
    bool m_transferMode;    // true = binary, false = ASCII
    bool m_passiveMode;     // true = passive, false = active
    Int32 m_timeout;
    VarString m_lastError;
    VarString m_hostname;
    VarString m_username;
    VarString m_password;
    Int32 m_port;

public:
    FtpClient();
    ~FtpClient();
    
    // Interface implementation
    RetCodeEnum connect(const VarString& host, Int32 port, 
                       const VarString& user, const VarString& pass) override;
    RetCodeEnum disconnect() override;
    bool isConnected() const override;
    
    RetCodeEnum uploadFile(const VarString& localPath, 
                          const VarString& remotePath) override;
    RetCodeEnum downloadFile(const VarString& remotePath, 
                            const VarString& localPath) override;
    RetCodeEnum deleteFile(const VarString& remotePath) override;
    
    RetCodeEnum listDirectory(const VarString& path, 
                             std::vector<FtpFileInfo>& entries) override;
    RetCodeEnum createDirectory(const VarString& path) override;
    RetCodeEnum removeDirectory(const VarString& path) override;
    
    void setTransferMode(bool binary) override;
    void setPassiveMode(bool passive) override;
    void setTimeout(Int32 seconds) override;
    
    VarString getLastError() const override;

private:
    // Helper methods
    bool initializeCurl();
    void cleanupCurl();
    void setCurlOptions();
    RetCodeEnum handleCurlError(CURLcode code);
    VarString buildUrl(const VarString& path);
    
    // Callback functions
    static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp);
    static size_t readCallback(void* ptr, size_t size, size_t nmemb, void* userp);
};
```

## Call Flow Analysis

### Typical FTP Operation Flow

#### 1. **Initialization** (Constructor)
```cpp
// In narcotics application
BasarFtpFileTransfer::BasarFtpFileTransfer(...)
    : m_ftpClient(basar::cmnutil::createFtpClient())  // Factory creates libcurl-based client
{
    // Configure client
    m_ftpClient->setTransferMode(true);   // Binary mode
    m_ftpClient->setPassiveMode(true);    // Passive FTP
    m_ftpClient->setTimeout(30);          // 30-second timeout
}
```

#### 2. **File Upload Operation**
```cpp
// Business logic call
bool BasarFtpFileTransfer::storeFile(IPrintDataProvider& printData, 
                                    Int32 senderbgano, 
                                    Int32 contributionvoucherno,
                                    const VarString& startDate,
                                    const FileCreationBehaviour& fileCreationBehaviour)
{
    // Step 1: Establish connection
    if (!establishFTPConnection()) {
        return false;
    }
    
    // Step 2: Generate paths
    VarString directoryPath = generateDirectory(senderbgano, startDate);
    VarString filePath = generateFilePath(senderbgano, contributionvoucherno, startDate, fileCreationBehaviour);
    
    // Step 3: Create directory if needed
    if (!createDirectory(directoryPath)) {
        return false;
    }
    
    // Step 4: Upload file
    auto result = m_ftpClient->uploadFile(localFilePath, filePath);
    if (result != IFtpClient::RetCodeEnum::Success) {
        handleFtpError(result, "File upload failed");
        return false;
    }
    
    // Step 5: Set permissions
    setPermissions("644", filePath);
    
    // Step 6: Cleanup
    releaseFTPConnection();
    
    return true;
}
```

#### 3. **libcurl Level Implementation**
```cpp
// In BASAR FtpClient implementation
IFtpClient::RetCodeEnum FtpClient::uploadFile(const VarString& localPath, 
                                             const VarString& remotePath)
{
    if (!m_curl || !m_isConnected) {
        return RetCodeEnum::ConnectionFailed;
    }
    
    // Open local file
    FILE* localFile = fopen(localPath.c_str(), "rb");
    if (!localFile) {
        m_lastError = "Cannot open local file: " + localPath;
        return RetCodeEnum::FileNotFound;
    }
    
    // Get file size
    fseek(localFile, 0, SEEK_END);
    long fileSize = ftell(localFile);
    fseek(localFile, 0, SEEK_SET);
    
    // Build FTP URL
    VarString ftpUrl = buildUrl(remotePath);
    
    // Configure curl for upload
    curl_easy_setopt(m_curl, CURLOPT_URL, ftpUrl.c_str());
    curl_easy_setopt(m_curl, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(m_curl, CURLOPT_READDATA, localFile);
    curl_easy_setopt(m_curl, CURLOPT_INFILESIZE, fileSize);
    
    // Set transfer mode
    if (m_transferMode) {
        // Binary mode - no special handling needed
    } else {
        // ASCII mode - could add CRLF conversion if needed
    }
    
    // Perform upload
    CURLcode result = curl_easy_perform(m_curl);
    
    // Cleanup
    fclose(localFile);
    
    // Handle result
    return handleCurlError(result);
}
```

## Error Handling Chain

### Error Code Translation
```cpp
IFtpClient::RetCodeEnum FtpClient::handleCurlError(CURLcode code)
{
    switch (code) {
        case CURLE_OK:
            return RetCodeEnum::Success;
        case CURLE_COULDNT_CONNECT:
        case CURLE_COULDNT_RESOLVE_HOST:
            m_lastError = "Connection failed: " + VarString(curl_easy_strerror(code));
            return RetCodeEnum::ConnectionFailed;
        case CURLE_LOGIN_DENIED:
            m_lastError = "Authentication failed: " + VarString(curl_easy_strerror(code));
            return RetCodeEnum::AuthenticationFailed;
        case CURLE_OPERATION_TIMEDOUT:
            m_lastError = "Operation timeout: " + VarString(curl_easy_strerror(code));
            return RetCodeEnum::Timeout;
        case CURLE_UPLOAD_FAILED:
        case CURLE_SEND_ERROR:
        case CURLE_RECV_ERROR:
            m_lastError = "Transfer failed: " + VarString(curl_easy_strerror(code));
            return RetCodeEnum::TransferFailed;
        default:
            m_lastError = "Network error: " + VarString(curl_easy_strerror(code));
            return RetCodeEnum::NetworkError;
    }
}
```

### Application-Level Error Handling
```cpp
void BasarFtpFileTransfer::handleFtpError(IFtpClient::RetCodeEnum errorCode, 
                                         const VarString& context)
{
    m_ftpErrorOccurred = true;
    m_lastErrorMessage = context + ": " + m_ftpClient->getLastError();
    
    // Log error for debugging and compliance
    BLOG_ERROR(LoggerPool::loggerNarcotics, m_lastErrorMessage);
    
    // Throw exception for application handling
    basar::cmnutil::ExceptionInfoStruct excInfo;
    excInfo.context = context;
    excInfo.file = __FILE__;
    excInfo.line = __LINE__;
    excInfo.reason = m_lastErrorMessage;
    
    throw basar::cmnutil::BasarRuntimeException(excInfo);
}
```

## Benefits of This Architecture

### 1. **Separation of Concerns**
- **Narcotics App**: Focuses on business logic (voucher processing, order management)
- **BASAR Framework**: Provides stable interfaces and error handling
- **libcurl Implementation**: Handles low-level FTP protocol details

### 2. **Maintainability**
- Interface changes don't affect application code
- Implementation can be swapped (Qt5Ftp → libcurl)
- Centralized error handling and logging

### 3. **Testability**
- Mock implementations possible for unit testing
- Interface contracts clearly defined
- Dependency injection through factory pattern

### 4. **Regulatory Compliance**
- Centralized logging for audit trails
- Standardized error handling
- Consistent validation patterns

### 5. **Performance**
- Connection pooling possible
- Efficient resource management (RAII)
- Asynchronous operations support

### 6. **Platform Independence**
- Same interface works on Windows, Linux, AIX
- Platform-specific implementations hidden
- Consistent behavior across platforms

## Migration Benefits (Qt5Ftp → libcurl)

| Aspect | Qt5Ftp (Before) | libcurl (After) | Benefit |
|--------|-----------------|-----------------|---------|
| **Maintenance** | Deprecated in Qt 5.15 | Actively maintained | Future-proof |
| **Performance** | Qt-specific overhead | Optimized for protocols | Better performance |
| **Features** | Limited FTP features | Rich protocol support | More capabilities |
| **Security** | Qt security model | Modern TLS/SSL support | Enhanced security |
| **Compliance** | Qt dependencies | Static linking possible | Pharmaceutical ready |
| **Documentation** | Qt-specific docs | Extensive libcurl docs | Better support |

## Conclusion

The BASAR framework provides a robust, maintainable architecture that successfully abstracts the narcotics application from FTP implementation details. The migration from Qt5Ftp to libcurl demonstrates the framework's flexibility and the benefits of interface-based design.

**Key Achievement**: Zero code changes required in the narcotics application while gaining modern, actively maintained FTP functionality through libcurl integration.