# Narcotics Application - libcurl Integration through BASAR Framework

## Integration Overview

The narcotics application uses the BASAR framework to access FTP functionality. With the migration from Qt5Ftp to libcurl, the application code remains unchanged while the underlying implementation is updated.

## UML Class Diagram: FTP Client Architecture

```
┌─────────────────────────────────────────────────────────────────┐
│                    Narcotics Application                        │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  ┌─────────────────────────────────────────────────────────────┐│
│  │              BasarFtpFileTransfer                           ││
│  │─────────────────────────────────────────────────────────────││
│  │ - m_ftpClient: std::unique_ptr<IFtpClient>                  ││
│  │ - m_userName: VarString                                     ││
│  │ - m_password: VarString                                     ││
│  │ - m_hostname: VarString                                     ││
│  │ - m_port: Int32                                             ││
│  │─────────────────────────────────────────────────────────────││
│  │ + BasarFtpFileTransfer(host, user, pass, dir, tmp, port)   ││
│  │ + storeFile(printData, bgano, voucherno, date): bool       ││
│  │ + cancelOrder(bgano, voucherno, date): bool                ││
│  │ + deleteFile(filePath): void                               ││
│  │ + listDirectory(dir, entries): bool                        ││
│  │ + doesDirectoryExist(dir): bool                            ││
│  │ - establishFTPConnection(): bool                           ││
│  │ - releaseFTPConnection(): bool                             ││
│  │ - handleFtpError(errorCode, context): void                 ││
│  └─────────────────────────────────────────────────────────────┘│
│                                │                                │
│                                │ uses                           │
│                                ▼                                │
└─────────────────────────────────────────────────────────────────┘
                                 │
                                 │ Factory Call
                                 │ createFtpClient()
                                 ▼
┌─────────────────────────────────────────────────────────────────┐
│                    BASAR Framework                              │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  ┌─────────────────────────────────────────────────────────────┐│
│  │                    IFtpClient                               ││
│  │                   <<interface>>                             ││
│  │─────────────────────────────────────────────────────────────││
│  │ + connect(host, port, user, pass): RetCodeEnum             ││
│  │ + disconnect(): RetCodeEnum                                 ││
│  │ + uploadFile(localPath, remotePath): RetCodeEnum           ││
│  │ + downloadFile(remotePath, localPath): RetCodeEnum         ││
│  │ + deleteFile(remotePath): RetCodeEnum                      ││
│  │ + listDirectory(path, entries): RetCodeEnum                ││
│  │ + createDirectory(path): RetCodeEnum                       ││
│  │ + setTransferMode(binary): void                            ││
│  │ + setPassiveMode(passive): void                            ││
│  │ + setTimeout(seconds): void                                ││
│  │ + isConnected(): bool                                      ││
│  │ + getLastError(): VarString                                ││
│  └─────────────────────────────────────────────────────────────┘│
│                                │                                │
│                                │ implements                     │
│                                ▼                                │
│  ┌─────────────────────────────────────────────────────────────┐│
│  │                    FtpClient                                ││
│  │                (libcurl implementation)                     ││
│  │─────────────────────────────────────────────────────────────││
│  │ - m_curl: CURL*                                             ││
│  │ - m_curlList: curl_slist*                                   ││
│  │ - m_isConnected: bool                                       ││
│  │ - m_transferMode: bool                                      ││
│  │ - m_passiveMode: bool                                       ││
│  │ - m_timeout: int                                            ││
│  │ - m_lastError: VarString                                    ││
│  │─────────────────────────────────────────────────────────────││
│  │ + connect(host, port, user, pass): RetCodeEnum             ││
│  │ + disconnect(): RetCodeEnum                                 ││
│  │ + uploadFile(localPath, remotePath): RetCodeEnum           ││
│  │ + downloadFile(remotePath, localPath): RetCodeEnum         ││
│  │ + deleteFile(remotePath): RetCodeEnum                      ││
│  │ + listDirectory(path, entries): RetCodeEnum                ││
│  │ + createDirectory(path): RetCodeEnum                       ││
│  │ + setTransferMode(binary): void                            ││
│  │ + setPassiveMode(passive): void                            ││
│  │ + setTimeout(seconds): void                                ││
│  │ + isConnected(): bool                                      ││
│  │ + getLastError(): VarString                                ││
│  │─────────────────────────────────────────────────────────────││
│  │ - initializeCurl(): bool                                   ││
│  │ - cleanupCurl(): void                                      ││
│  │ - setCurlOptions(): void                                   ││
│  │ - handleCurlError(code): RetCodeEnum                       ││
│  │ - buildUrl(path): VarString                                ││
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
│  │                    libcurl API                              ││
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

## UML Sequence Diagram: FTP File Upload Process

```
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   Narcotics     │    │ BasarFtpFile    │    │   BASAR FTP     │    │    libcurl     │
│   Application   │    │   Transfer      │    │   Client        │    │   Library      │
└─────────────────┘    └─────────────────┘    └─────────────────┘    └─────────────────┘
         │                       │                       │                       │
         │  storeFile()          │                       │                       │
         │──────────────────────▶│                       │                       │
         │                       │                       │                       │
         │                       │ establishFTPConnection() │                   │
         │                       │──────────────────────▶│                       │
         │                       │                       │                       │
         │                       │                       │ connect()             │
         │                       │                       │──────────────────────▶│
         │                       │                       │                       │
         │                       │                       │                       │ curl_easy_init()
         │                       │                       │                       │◄──────────────────
         │                       │                       │                       │
         │                       │                       │                       │ curl_easy_setopt()
         │                       │                       │                       │◄──────────────────
         │                       │                       │                       │ (CURLOPT_URL)
         │                       │                       │                       │ (CURLOPT_USERNAME)
         │                       │                       │                       │ (CURLOPT_PASSWORD)
         │                       │                       │                       │ (CURLOPT_FTP_USE_EPSV)
         │                       │                       │                       │
         │                       │                       │                       │ curl_easy_perform()
         │                       │                       │                       │◄──────────────────
         │                       │                       │                       │
         │                       │                       │ RetCodeEnum::Success  │
         │                       │                       │◄──────────────────────│
         │                       │                       │                       │
         │                       │ true                  │                       │
         │                       │◄──────────────────────│                       │
         │                       │                       │                       │
         │                       │ generateFilePath()    │                       │
         │                       │◄──────────────────────│                       │
         │                       │                       │                       │
         │                       │ createDirectory()     │                       │
         │                       │──────────────────────▶│                       │
         │                       │                       │                       │
         │                       │                       │ createDirectory()     │
         │                       │                       │──────────────────────▶│
         │                       │                       │                       │
         │                       │                       │                       │ curl_easy_setopt()
         │                       │                       │                       │◄──────────────────
         │                       │                       │                       │ (CURLOPT_FTP_CREATE_MISSING_DIRS)
         │                       │                       │                       │
         │                       │                       │                       │ curl_easy_perform()
         │                       │                       │                       │◄──────────────────
         │                       │                       │                       │
         │                       │                       │ RetCodeEnum::Success  │
         │                       │                       │◄──────────────────────│
         │                       │                       │                       │
         │                       │ uploadFile()          │                       │
         │                       │──────────────────────▶│                       │
         │                       │                       │                       │
         │                       │                       │ uploadFile()          │
         │                       │                       │──────────────────────▶│
         │                       │                       │                       │
         │                       │                       │                       │ curl_easy_setopt()
         │                       │                       │                       │◄──────────────────
         │                       │                       │                       │ (CURLOPT_UPLOAD)
         │                       │                       │                       │ (CURLOPT_READDATA)
         │                       │                       │                       │ (CURLOPT_INFILESIZE)
         │                       │                       │                       │
         │                       │                       │                       │ curl_easy_perform()
         │                       │                       │                       │◄──────────────────
         │                       │                       │                       │
         │                       │                       │ RetCodeEnum::Success  │
         │                       │                       │◄──────────────────────│
         │                       │                       │                       │
         │                       │ setPermissions()      │                       │
         │                       │──────────────────────▶│                       │
         │                       │                       │                       │
         │                       │                       │ // FTP SITE command   │
         │                       │                       │──────────────────────▶│
         │                       │                       │                       │
         │                       │                       │                       │ curl_easy_setopt()
         │                       │                       │                       │◄──────────────────
         │                       │                       │                       │ (CURLOPT_POSTQUOTE)
         │                       │                       │                       │
         │                       │                       │                       │ curl_easy_perform()
         │                       │                       │                       │◄──────────────────
         │                       │                       │                       │
         │                       │ releaseFTPConnection()│                       │
         │                       │──────────────────────▶│                       │
         │                       │                       │                       │
         │                       │                       │ disconnect()          │
         │                       │                       │──────────────────────▶│
         │                       │                       │                       │
         │                       │                       │                       │ curl_easy_cleanup()
         │                       │                       │                       │◄──────────────────
         │                       │                       │                       │
         │                       │ true                  │                       │
         │                       │◄──────────────────────│                       │
         │                       │                       │                       │
         │ true                  │                       │                       │
         │◄──────────────────────│                       │                       │
         │                       │                       │                       │
```

## Key Integration Points

### 1. Factory Pattern Usage
The narcotics application uses the factory pattern to create FTP clients:

```cpp
// In BasarFtpFileTransfer constructor
m_ftpClient = basar::cmnutil::createFtpClient();
```

### 2. Interface Abstraction
The application only depends on the `IFtpClient` interface:

```cpp
std::unique_ptr<basar::cmnutil::IFtpClient> m_ftpClient;
```

### 3. Error Code Mapping
libcurl error codes are mapped to BASAR framework error codes:

```cpp
enum class RetCodeEnum {
    Success = 0,
    ConnectionFailed = 1,
    AuthenticationFailed = 2,
    TransferFailed = 3,
    NetworkError = 4,
    InvalidParameter = 5
};
```

### 4. Configuration Management
FTP client configuration is handled through the interface:

```cpp
m_ftpClient->setTransferMode(true);  // Binary mode
m_ftpClient->setPassiveMode(true);   // Passive FTP
m_ftpClient->setTimeout(30);         // 30-second timeout
```

## Benefits of This Architecture

1. **Zero Code Changes**: Narcotics application remains unchanged
2. **Transparent Migration**: libcurl integration is invisible to the application
3. **Consistent Error Handling**: All errors handled through BASAR framework
4. **Maintainable**: Easy to update or replace FTP implementation
5. **Testable**: Interface allows for easy unit testing
6. **Regulatory Compliant**: Centralized validation and logging
7. **Future-Proof**: Easy to add new FTP features without application changes

## Migration Impact

| Component | Before (Qt5Ftp) | After (libcurl) | Change Required |
|-----------|-----------------|-----------------|-----------------|
| Narcotics App | Uses BasarFtpFileTransfer | Uses BasarFtpFileTransfer | None |
| BASAR Interface | IFtpClient interface | IFtpClient interface | None |
| BASAR Implementation | Qt5Ftp-based FtpClient | libcurl-based FtpClient | Complete replacement |
| Build System | Qt5Ftp.lib dependency | libcurl_a.lib dependency | Updated |
| Error Handling | Qt error codes mapped | libcurl error codes mapped | Internal mapping |

## Performance Considerations

- **Connection Pooling**: libcurl handles connection reuse efficiently
- **Memory Management**: RAII pattern ensures proper cleanup
- **Error Recovery**: Robust error handling with retry mechanisms
- **Logging**: Comprehensive logging for debugging and compliance
- **Threading**: Thread-safe implementation for concurrent operations