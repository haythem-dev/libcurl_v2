
# BASAR Framework Architecture and Narcotics libcurl Integration

## 1. BASAR Framework General Architecture

### 1.1 Three-Layer Architecture Overview

The BASAR framework follows a **layered architecture pattern** with clear separation of concerns:

```
┌─────────────────────────────────────────────────────────────────┐
│                   APPLICATION LAYER                             │
│  ┌─────────────────┐  ┌─────────────────┐  ┌─────────────────┐ │
│  │   Narcotics     │  │   Inventory     │  │   Other Apps    │ │
│  │   Application   │  │   Management    │  │                 │ │
│  └─────────────────┘  └─────────────────┘  └─────────────────┘ │
└─────────────────────────────────────────────────────────────────┘
                                │
                                ▼
┌─────────────────────────────────────────────────────────────────┐
│                    BASAR FRAMEWORK LAYER                        │
│  ┌─────────────────────────────────────────────────────────────┐ │
│  │              INTERFACE ABSTRACTION LAYER                   │ │
│  │  ┌─────────────┐  ┌─────────────┐  ┌─────────────────────┐ │ │
│  │  │ IFtpClient  │  │IEmailSender │  │IRemoteTaskSubmitter │ │ │
│  │  │             │  │             │  │                     │ │ │
│  │  └─────────────┘  └─────────────┘  └─────────────────────┘ │ │
│  └─────────────────────────────────────────────────────────────┘ │
│  ┌─────────────────────────────────────────────────────────────┐ │
│  │            CONCRETE IMPLEMENTATION LAYER                   │ │
│  │  ┌─────────────┐  ┌─────────────┐  ┌─────────────────────┐ │ │
│  │  │CurlFtpClient│  │EmailSender  │  │RemoteTaskSubmitter  │ │ │
│  │  │(libcurl)    │  │(SMTP)       │  │(SSH)                │ │ │
│  │  └─────────────┘  └─────────────┘  └─────────────────────┘ │ │
│  └─────────────────────────────────────────────────────────────┘ │
│  ┌─────────────────────────────────────────────────────────────┐ │
│  │                FACTORY PATTERN LAYER                       │ │
│  │                createFtpClient()                           │ │
│  │                createEmailSender()                         │ │
│  │                createRemoteTaskSubmitter()                 │ │
│  └─────────────────────────────────────────────────────────────┘ │
└─────────────────────────────────────────────────────────────────┘
                                │
                                ▼
┌─────────────────────────────────────────────────────────────────┐
│                  EXTERNAL LIBRARIES LAYER                       │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐             │
│  │   libcurl   │  │   libssh2   │  │ PostgreSQL  │             │
│  │ (FTP/HTTP)  │  │   (SSH)     │  │ (Database)  │             │
│  └─────────────┘  └─────────────┘  └─────────────┘             │
└─────────────────────────────────────────────────────────────────┘
```

### 1.2 Key Design Patterns in BASAR

#### Abstract Factory Pattern
```cpp
namespace basar::cmnutil {
    // Factory functions create concrete implementations
    std::unique_ptr<IFtpClient> createFtpClient() {
        return std::make_unique<CurlFtpClient>();
    }
    
    std::unique_ptr<IEmailSender> createEmailSender() {
        return std::make_unique<SmtpEmailSender>();
    }
}
```

#### Strategy Pattern
```cpp
// Applications depend only on interfaces, not implementations
class BasarFtpFileTransfer {
private:
    std::unique_ptr<IFtpClient> m_ftpClient;  // Can be any implementation
    
public:
    BasarFtpFileTransfer() : 
        m_ftpClient(basar::cmnutil::createFtpClient()) {}  // Factory creates implementation
};
```

### 1.3 BASAR Framework Components

```
BASAR Framework Structure:
├── libbasarcmnutil (Common Utilities)
│   ├── IFtpClient interface
│   ├── CurlFtpClient implementation
│   ├── IEmailSender interface
│   ├── Exception handling system
│   └── Logging utilities
├── libbasarappl (Application Layer)
│   ├── Application lifecycle management
│   ├── Configuration management
│   └── Resource management
├── libbasardb* (Database Layer)
│   ├── Database connection management
│   ├── Query execution
│   └── Transaction management
└── libbasargui* (GUI Layer)
    ├── Qt-based GUI abstractions
    ├── Dialog managers
    └── Widget factories
```

## 2. Narcotics Application libcurl Integration

### 2.1 UML Class Diagram: Complete Architecture

```
┌─────────────────────────────────────────────────────────────────┐
│                    Narcotics Application                        │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  ┌─────────────────────────────────────────────────────────────┐│
│  │            NarcoticsFtpFileTransfer                         ││
│  │─────────────────────────────────────────────────────────────││
│  │ + storeFile(printData, bgano, voucherno, date): bool       ││
│  │ + cancelOrder(bgano, voucherno, date): bool                ││
│  │ + generateFileName(dir, voucherno, behavior): VarString    ││
│  │ + generateDirectory(bgano, date): VarString                ││
│  │ + generateFilePath(bgano, voucherno, date): VarString      ││
│  └─────────────────────────────────────────────────────────────┘│
│                                │                                │
│                                │ inherits                       │
│                                ▼                                │
│  ┌─────────────────────────────────────────────────────────────┐│
│  │              BasarFtpFileTransfer                           ││
│  │─────────────────────────────────────────────────────────────││
│  │ - m_ftpClient: std::unique_ptr<IFtpClient>                  ││
│  │ - m_userName: VarString                                     ││
│  │ - m_password: VarString                                     ││
│  │ - m_hostname: VarString                                     ││
│  │ - m_port: Int32                                             ││
│  │ - m_ftpErrorOccurred: bool                                  ││
│  │─────────────────────────────────────────────────────────────││
│  │ + establishFTPConnection(): bool                           ││
│  │ + releaseFTPConnection(): bool                             ││
│  │ + deleteFile(filePath): void                               ││
│  │ + listDirectory(dir, entries): bool                        ││
│  │ + doesDirectoryExist(dir): bool                            ││
│  │ + createDirectory(dir): bool                               ││
│  │ - handleFtpError(errorCode, context): void                 ││
│  └─────────────────────────────────────────────────────────────┘│
│                                │                                │
│                                │ uses (Factory Pattern)         │
│                                ▼                                │
└─────────────────────────────────────────────────────────────────┘
                                 │
                                 │ basar::cmnutil::createFtpClient()
                                 ▼
┌─────────────────────────────────────────────────────────────────┐
│                    BASAR Framework                              │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  ┌─────────────────────────────────────────────────────────────┐│
│  │                    IFtpClient                               ││
│  │                   <<interface>>                             ││
│  │─────────────────────────────────────────────────────────────││
│  │ + connect(host, port): RetCodeEnum                         ││
│  │ + login(user, pass): RetCodeEnum                           ││
│  │ + disconnect(): void                                       ││
│  │ + uploadFile(localPath, remotePath): RetCodeEnum           ││
│  │ + downloadFile(remotePath, localPath): RetCodeEnum         ││
│  │ + deleteFile(remotePath): RetCodeEnum                      ││
│  │ + listDirectory(path, entries): RetCodeEnum                ││
│  │ + createDirectory(path): RetCodeEnum                       ││
│  │ + setPermissions(path, permissions): RetCodeEnum           ││
│  │ + executeCommand(command, response): RetCodeEnum           ││
│  │ + isConnected(): bool                                      ││
│  │ + getLastError(): VarString                                ││
│  └─────────────────────────────────────────────────────────────┘│
│                                │                                │
│                                │ implements                     │
│                                ▼                                │
│  ┌─────────────────────────────────────────────────────────────┐│
│  │                    CurlFtpClient                            ││
│  │               (libcurl implementation)                      ││
│  │─────────────────────────────────────────────────────────────││
│  │ - m_curl: CURL*                                             ││
│  │ - m_hostname: VarString                                     ││
│  │ - m_port: Int16                                             ││
│  │ - m_username: VarString                                     ││
│  │ - m_password: VarString                                     ││
│  │ - m_connected: bool                                         ││
│  │ - m_lastError: VarString                                    ││
│  │─────────────────────────────────────────────────────────────││
│  │ + connect(host, port): RetCodeEnum                         ││
│  │ + login(user, pass): RetCodeEnum                           ││
│  │ + disconnect(): void                                       ││
│  │ + uploadFile(localPath, remotePath): RetCodeEnum           ││
│  │ + downloadFile(remotePath, localPath): RetCodeEnum         ││
│  │ + deleteFile(remotePath): RetCodeEnum                      ││
│  │ + listDirectory(path, entries): RetCodeEnum                ││
│  │ + createDirectory(path): RetCodeEnum                       ││
│  │ + setPermissions(path, permissions): RetCodeEnum           ││
│  │ + executeCommand(command, response): RetCodeEnum           ││
│  │ + isConnected(): bool                                      ││
│  │ + getLastError(): VarString                                ││
│  │─────────────────────────────────────────────────────────────││
│  │ - initializeCurl(): bool                                   ││
│  │ - cleanupCurl(): void                                      ││
│  │ - setCurlOptions(): void                                   ││
│  │ - mapCurlError(code): RetCodeEnum                          ││
│  │ - buildFtpUrl(path): VarString                             ││
│  └─────────────────────────────────────────────────────────────┘│
│                                │                                │
│                                │ uses                           │
│                                ▼                                │
└─────────────────────────────────────────────────────────────────┘
                                 │
                                 ▼
┌─────────────────────────────────────────────────────────────────┐
│                    libcurl Library                              │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  ┌─────────────────────────────────────────────────────────────┐│
│  │                    libcurl C API                            ││
│  │─────────────────────────────────────────────────────────────││
│  │ + curl_easy_init(): CURL*                                   ││
│  │ + curl_easy_setopt(curl, option, value): CURLcode          ││
│  │ + curl_easy_perform(curl): CURLcode                         ││
│  │ + curl_easy_cleanup(curl): void                             ││
│  │ + curl_easy_strerror(code): char*                           ││
│  │ + curl_slist_append(list, string): curl_slist*             ││
│  │ + curl_slist_free_all(list): void                           ││
│  └─────────────────────────────────────────────────────────────┘│
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

### 2.2 UML Sequence Diagram: File Upload Process

```
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   Narcotics     │    │ BasarFtpFile    │    │   BASAR FTP     │    │    libcurl     │
│   Application   │    │   Transfer      │    │   Client        │    │   Library      │
└─────────────────┘    └─────────────────┘    └─────────────────┘    └─────────────────┘
         │                       │                       │                       │
         │ storeFile(printData,  │                       │                       │
         │   bgano, voucherno)   │                       │                       │
         │──────────────────────▶│                       │                       │
         │                       │                       │                       │
         │                       │ Factory Pattern:      │                       │
         │                       │ createFtpClient()     │                       │
         │                       │──────────────────────▶│                       │
         │                       │                       │                       │
         │                       │                       │ curl_easy_init()      │
         │                       │                       │──────────────────────▶│
         │                       │                       │                       │
         │                       │                       │ CURL* handle          │
         │                       │                       │◀──────────────────────│
         │                       │                       │                       │
         │                       │ IFtpClient instance   │                       │
         │                       │◀──────────────────────│                       │
         │                       │                       │                       │
         │                       │ establishFTPConnection() │                   │
         │                       │──────────────────────▶│                       │
         │                       │                       │                       │
         │                       │                       │ connect(host, port)   │
         │                       │                       │──────────────────────▶│
         │                       │                       │                       │
         │                       │                       │                       │ curl_easy_setopt()
         │                       │                       │                       │◀──────────────────
         │                       │                       │                       │ (CURLOPT_URL)
         │                       │                       │                       │ (CURLOPT_USERNAME)
         │                       │                       │                       │ (CURLOPT_PASSWORD)
         │                       │                       │                       │
         │                       │                       │                       │ curl_easy_perform()
         │                       │                       │                       │◀──────────────────
         │                       │                       │                       │
         │                       │                       │ RetCodeEnum::Success  │
         │                       │                       │◀──────────────────────│
         │                       │                       │                       │
         │                       │ true                  │                       │
         │                       │◀──────────────────────│                       │
         │                       │                       │                       │
         │                       │ generateFilePath()    │                       │
         │                       │ (business logic)      │                       │
         │                       │◀──────────────────────│                       │
         │                       │                       │                       │
         │                       │ createDirectory()     │                       │
         │                       │──────────────────────▶│                       │
         │                       │                       │                       │
         │                       │                       │ createDirectory()     │
         │                       │                       │──────────────────────▶│
         │                       │                       │                       │
         │                       │                       │                       │ curl_easy_setopt()
         │                       │                       │                       │◀──────────────────
         │                       │                       │                       │ (CURLOPT_FTP_CREATE_MISSING_DIRS)
         │                       │                       │                       │
         │                       │                       │                       │ curl_easy_perform()
         │                       │                       │                       │◀──────────────────
         │                       │                       │                       │
         │                       │                       │ RetCodeEnum::Success  │
         │                       │                       │◀──────────────────────│
         │                       │                       │                       │
         │                       │ uploadFile()          │                       │
         │                       │──────────────────────▶│                       │
         │                       │                       │                       │
         │                       │                       │ uploadFile()          │
         │                       │                       │──────────────────────▶│
         │                       │                       │                       │
         │                       │                       │                       │ curl_easy_setopt()
         │                       │                       │                       │◀──────────────────
         │                       │                       │                       │ (CURLOPT_UPLOAD)
         │                       │                       │                       │ (CURLOPT_READDATA)
         │                       │                       │                       │ (CURLOPT_INFILESIZE)
         │                       │                       │                       │
         │                       │                       │                       │ curl_easy_perform()
         │                       │                       │                       │◀──────────────────
         │                       │                       │                       │
         │                       │                       │ RetCodeEnum::Success  │
         │                       │                       │◀──────────────────────│
         │                       │                       │                       │
         │                       │ setPermissions()      │                       │
         │                       │──────────────────────▶│                       │
         │                       │                       │                       │
         │                       │                       │ executeCommand()      │
         │                       │                       │ "SITE CHMOD 644"      │
         │                       │                       │──────────────────────▶│
         │                       │                       │                       │
         │                       │                       │                       │ curl_easy_setopt()
         │                       │                       │                       │◀──────────────────
         │                       │                       │                       │ (CURLOPT_POSTQUOTE)
         │                       │                       │                       │
         │                       │                       │                       │ curl_easy_perform()
         │                       │                       │                       │◀──────────────────
         │                       │                       │                       │
         │                       │ releaseFTPConnection()│                       │
         │                       │──────────────────────▶│                       │
         │                       │                       │                       │
         │                       │                       │ disconnect()          │
         │                       │                       │──────────────────────▶│
         │                       │                       │                       │
         │                       │                       │                       │ curl_easy_cleanup()
         │                       │                       │                       │◀──────────────────
         │                       │                       │                       │
         │                       │ true (success)        │                       │
         │                       │◀──────────────────────│                       │
         │                       │                       │                       │
         │ true (success)        │                       │                       │
         │◄──────────────────────│                       │                       │
         │                       │                       │                       │
```

## 3. Error Handling Flow

### 3.1 Error Mapping Strategy

```cpp
// In CurlFtpClient::mapCurlError()
RetCodeEnum CurlFtpClient::mapCurlError(CURLcode curlCode) {
    switch (curlCode) {
        case CURLE_OK:
            return FTP_SUCCESS;
        case CURLE_COULDNT_CONNECT:
        case CURLE_COULDNT_RESOLVE_HOST:
            return FTP_OFFLINE;
        case CURLE_LOGIN_DENIED:
            return FTP_AUTHERROR;
        case CURLE_REMOTE_FILE_NOT_FOUND:
            return FTP_FILENOTFOUND;
        case CURLE_UPLOAD_FAILED:
        case CURLE_REMOTE_ACCESS_DENIED:
            return FTP_PERMISSIONDENIED;
        case CURLE_OPERATION_TIMEDOUT:
            return FTP_TIMEOUT;
        default:
            return FTP_GENERAL_ERROR;
    }
}
```

### 3.2 Error Handling Sequence

```
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   Narcotics     │    │     BASAR       │    │    libcurl     │
│   Application   │    │   Framework     │    │    Library     │
└─────────────────┘    └─────────────────┘    └─────────────────┘
         │                       │                       │
         │ uploadFile()          │                       │
         │──────────────────────▶│                       │
         │                       │ curl_easy_perform()   │
         │                       │──────────────────────▶│
         │                       │                       │
         │                       │                       │ CURLE_UPLOAD_FAILED
         │                       │◀──────────────────────│
         │                       │                       │
         │                       │ mapCurlError()        │
         │                       │ (CURLE_UPLOAD_FAILED  │
         │                       │  → FTP_PERMISSIONDENIED)
         │                       │                       │
         │                       │ handleFtpError()      │
         │                       │ (log error, update    │
         │                       │  internal state)      │
         │                       │                       │
         │ FTPException          │                       │
         │◀──────────────────────│                       │
         │                       │                       │
         │ catch & log           │                       │
         │ return false          │                       │
```

## 4. Key Integration Benefits

### 4.1 Zero Code Changes for Narcotics
- **Application Layer**: No changes to narcotics business logic
- **Interface Consistency**: Same `BasarFtpFileTransfer` API maintained
- **Error Handling**: Existing exception patterns preserved
- **Configuration**: Same connection parameters used

### 4.2 Transparent Migration
- **Factory Pattern**: `createFtpClient()` returns libcurl implementation
- **Interface Abstraction**: `IFtpClient` hides implementation details
- **Dependency Injection**: Framework provides concrete implementation
- **Backward Compatibility**: All existing functionality preserved

### 4.3 Enhanced Capabilities
- **Modern Library**: libcurl actively maintained vs deprecated Qt5Ftp
- **Better Protocol Support**: FTPS, SFTP, HTTP/HTTPS capabilities
- **Improved Performance**: Optimized networking and memory management
- **Cross-Platform**: Better portability across different systems

## 5. Build Integration

### 5.1 Dependency Structure
```
Narcotics Package Dependencies:
├── pharmos.base.basar (contains libcurl integration)
│   ├── libbasarcmnutil_ftpclient.h/cpp
│   ├── IFtpClient interface
│   └── CurlFtpClient implementation
├── libcurl.lib (static library)
├── System libraries (ws2_32.lib, wldap32.lib, etc.)
└── OpenSSL (for SSL/TLS support)
```

### 5.2 Build Process
1. **BASAR Framework Build**: `ant full` includes libcurl integration
2. **Narcotics Application Build**: Uses BASAR framework with libcurl
3. **Dependency Management**: ivy.xml handles library dependencies
4. **Static Linking**: All dependencies embedded in final executable

This architecture demonstrates how the BASAR framework provides a clean abstraction layer that allows the narcotics application to use libcurl functionality without any code changes, while maintaining full backward compatibility and adding enhanced networking capabilities.
