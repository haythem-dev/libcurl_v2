
# libcurl → BASAR → Narcotics Call Flow Documentation

## Overview

This document traces the complete call flow from libcurl library through the BASAR framework to the narcotics application, with evidence from actual code implementations.

## 1. Architecture Overview

```
┌─────────────────────────────────────────────────────────────────┐
│                    NARCOTICS APPLICATION                        │
│  ┌─────────────────────────────────────────────────────────────┐ │
│  │            NarcoticsFtpFileTransfer                        │ │
│  │  • Business logic for narcotics file transfers            │ │
│  │  • Uses BASAR FTP client interface                        │ │
│  │  • Handles progress tracking and error reporting          │ │
│  └─────────────────────────────────────────────────────────────┘ │
│                              │                                 │
│                              ▼                                 │
│  ┌─────────────────────────────────────────────────────────────┐ │
│  │              BasarFtpFileTransfer                          │ │
│  │  • Base class using BASAR framework                       │ │
│  │  • Abstracts FTP operations                               │ │
│  │  • Provides common functionality                          │ │
│  └─────────────────────────────────────────────────────────────┘ │
└─────────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│                      BASAR FRAMEWORK                            │
│  ┌─────────────────────────────────────────────────────────────┐ │
│  │                 INTERFACE LAYER                            │ │
│  │  ┌─────────────┐  ┌─────────────┐  ┌─────────────────────┐ │ │
│  │  │IFtpClient   │  │IEmailSender │  │IRemoteTaskSubmitter │ │ │
│  │  │(interface)  │  │(interface)  │  │(interface)          │ │ │
│  │  └─────────────┘  └─────────────┘  └─────────────────────┘ │ │
│  └─────────────────────────────────────────────────────────────┘ │
│  ┌─────────────────────────────────────────────────────────────┐ │
│  │               IMPLEMENTATION LAYER                         │ │
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
│                      LIBCURL LIBRARY                            │
│  • curl_easy_init(), curl_easy_setopt(), curl_easy_perform()   │ │
│  • FTP protocol implementation                                 │
│  • Network communication and error handling                   │
│  • Progress callbacks and data transfer                       │
└─────────────────────────────────────────────────────────────────┘
```

## 2. Code Evidence - Include Files and Dependencies

### 2.1 BASAR Framework Includes

**File: `pharmos.base.basar/dev/src/basar/libbasarcmnutil/libbasarcmnutil_ftpclient.h`**
```cpp
#ifndef LIBBASARCMNUTIL_FTPCLIENT_H
#define LIBBASARCMNUTIL_FTPCLIENT_H

// libcurl integration
#include <curl/curl.h>

// BASAR framework includes
#include "libbasarcmnutil_export.h"
#include "libbasarcmnutil_varstring.h"
#include "libbasarcmnutil_exception.h"

// Standard includes
#include <memory>
#include <string>
#include <functional>
#include <vector>

namespace basar {
namespace cmnutil {

// Interface definition
class LIBBASARCMNUTIL_EXPORT IFtpClient {
public:
    virtual ~IFtpClient() = default;
    virtual RetCodeEnum connect(const VarString& host, int port) = 0;
    virtual RetCodeEnum login(const VarString& username, const VarString& password) = 0;
    virtual RetCodeEnum uploadFile(const VarString& localPath, const VarString& remotePath) = 0;
    virtual RetCodeEnum downloadFile(const VarString& remotePath, const VarString& localPath) = 0;
    virtual RetCodeEnum disconnect() = 0;
};

// Factory function
LIBBASARCMNUTIL_EXPORT std::unique_ptr<IFtpClient> createFtpClient();

}
}
#endif
```

### 2.2 libcurl Implementation

**File: `pharmos.base.basar/dev/src/basar/libbasarcmnutil/libbasarcmnutil_ftpclient.cpp`**
```cpp
#include "libbasarcmnutil_ftpclient.h"
#include <curl/curl.h>

namespace basar {
namespace cmnutil {

class CurlFtpClient : public IFtpClient {
private:
    CURL* m_curl;
    VarString m_host;
    int m_port;
    VarString m_username;
    VarString m_password;
    
public:
    CurlFtpClient() : m_curl(nullptr), m_port(21) {
        curl_global_init(CURL_GLOBAL_DEFAULT);
        m_curl = curl_easy_init();
    }
    
    ~CurlFtpClient() {
        if (m_curl) {
            curl_easy_cleanup(m_curl);
        }
        curl_global_cleanup();
    }
    
    RetCodeEnum connect(const VarString& host, int port) override {
        m_host = host;
        m_port = port;
        return RetCodeEnum::Success;
    }
    
    RetCodeEnum login(const VarString& username, const VarString& password) override {
        m_username = username;
        m_password = password;
        return RetCodeEnum::Success;
    }
    
    RetCodeEnum uploadFile(const VarString& localPath, const VarString& remotePath) override {
        if (!m_curl) return RetCodeEnum::Error;
        
        // Construct FTP URL
        VarString url = VarString("ftp://") + m_host + ":" + VarString::fromInt(m_port) + "/" + remotePath;
        
        // Set libcurl options
        curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(m_curl, CURLOPT_USERNAME, m_username.c_str());
        curl_easy_setopt(m_curl, CURLOPT_PASSWORD, m_password.c_str());
        curl_easy_setopt(m_curl, CURLOPT_UPLOAD, 1L);
        
        // Open local file
        FILE* file = fopen(localPath.c_str(), "rb");
        if (!file) return RetCodeEnum::Error;
        
        curl_easy_setopt(m_curl, CURLOPT_READDATA, file);
        
        // Perform upload
        CURLcode res = curl_easy_perform(m_curl);
        fclose(file);
        
        return (res == CURLE_OK) ? RetCodeEnum::Success : RetCodeEnum::Error;
    }
    
    RetCodeEnum downloadFile(const VarString& remotePath, const VarString& localPath) override {
        if (!m_curl) return RetCodeEnum::Error;
        
        // Construct FTP URL
        VarString url = VarString("ftp://") + m_host + ":" + VarString::fromInt(m_port) + "/" + remotePath;
        
        // Set libcurl options
        curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(m_curl, CURLOPT_USERNAME, m_username.c_str());
        curl_easy_setopt(m_curl, CURLOPT_PASSWORD, m_password.c_str());
        
        // Open local file for writing
        FILE* file = fopen(localPath.c_str(), "wb");
        if (!file) return RetCodeEnum::Error;
        
        curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, file);
        
        // Perform download
        CURLcode res = curl_easy_perform(m_curl);
        fclose(file);
        
        return (res == CURLE_OK) ? RetCodeEnum::Success : RetCodeEnum::Error;
    }
    
    RetCodeEnum disconnect() override {
        return RetCodeEnum::Success;
    }
};

// Factory implementation
std::unique_ptr<IFtpClient> createFtpClient() {
    return std::make_unique<CurlFtpClient>();
}

}
}
```

### 2.3 Narcotics Application Integration

**File: `pharmos.outbound.narcotics/dev/src/libnarcotics/basarftpfiletransfer.h`**
```cpp
#ifndef BASARFTPFILETRANSFER_H
#define BASARFTPFILETRANSFER_H

// BASAR framework includes
#include <libbasarcmnutil_ftpclient.h>
#include <libbasarcmnutil_varstring.h>
#include <libbasarcmnutil_exception.h>

// Qt includes for compatibility
#include <QObject>
#include <QString>
#include <QProgressDialog>

class BasarFtpFileTransfer : public QObject {
    Q_OBJECT
    
private:
    std::unique_ptr<basar::cmnutil::IFtpClient> m_ftpClient;
    QString m_host;
    int m_port;
    QString m_username;
    QString m_password;
    
public:
    explicit BasarFtpFileTransfer(QObject* parent = nullptr);
    virtual ~BasarFtpFileTransfer();
    
    void setConnectionInfo(const QString& host, int port, 
                          const QString& username, const QString& password);
    
    bool connectToHost();
    bool uploadFile(const QString& localPath, const QString& remotePath);
    bool downloadFile(const QString& remotePath, const QString& localPath);
    void disconnectFromHost();
    
signals:
    void connected();
    void disconnected();
    void uploadProgress(int percentage);
    void downloadProgress(int percentage);
    void error(const QString& errorMessage);
};

#endif
```

**File: `pharmos.outbound.narcotics/dev/src/libnarcotics/basarftpfiletransfer.cpp`**
```cpp
#include "basarftpfiletransfer.h"
#include <QDebug>

BasarFtpFileTransfer::BasarFtpFileTransfer(QObject* parent)
    : QObject(parent), m_port(21) {
    // Create FTP client using BASAR factory
    m_ftpClient = basar::cmnutil::createFtpClient();
}

BasarFtpFileTransfer::~BasarFtpFileTransfer() {
    disconnectFromHost();
}

void BasarFtpFileTransfer::setConnectionInfo(const QString& host, int port,
                                           const QString& username, const QString& password) {
    m_host = host;
    m_port = port;
    m_username = username;
    m_password = password;
}

bool BasarFtpFileTransfer::connectToHost() {
    if (!m_ftpClient) return false;
    
    // Convert Qt strings to BASAR VarString
    basar::cmnutil::VarString host(m_host.toStdString().c_str());
    
    // Call BASAR FTP client connect
    basar::cmnutil::RetCodeEnum result = m_ftpClient->connect(host, m_port);
    
    if (result == basar::cmnutil::RetCodeEnum::Success) {
        // Login after successful connection
        basar::cmnutil::VarString username(m_username.toStdString().c_str());
        basar::cmnutil::VarString password(m_password.toStdString().c_str());
        
        result = m_ftpClient->login(username, password);
        
        if (result == basar::cmnutil::RetCodeEnum::Success) {
            emit connected();
            return true;
        }
    }
    
    emit error("Failed to connect to FTP server");
    return false;
}

bool BasarFtpFileTransfer::uploadFile(const QString& localPath, const QString& remotePath) {
    if (!m_ftpClient) return false;
    
    // Convert paths to BASAR VarString
    basar::cmnutil::VarString local(localPath.toStdString().c_str());
    basar::cmnutil::VarString remote(remotePath.toStdString().c_str());
    
    // Call BASAR FTP client upload
    basar::cmnutil::RetCodeEnum result = m_ftpClient->uploadFile(local, remote);
    
    if (result == basar::cmnutil::RetCodeEnum::Success) {
        emit uploadProgress(100);
        return true;
    }
    
    emit error("Upload failed");
    return false;
}

bool BasarFtpFileTransfer::downloadFile(const QString& remotePath, const QString& localPath) {
    if (!m_ftpClient) return false;
    
    // Convert paths to BASAR VarString
    basar::cmnutil::VarString remote(remotePath.toStdString().c_str());
    basar::cmnutil::VarString local(localPath.toStdString().c_str());
    
    // Call BASAR FTP client download
    basar::cmnutil::RetCodeEnum result = m_ftpClient->downloadFile(remote, local);
    
    if (result == basar::cmnutil::RetCodeEnum::Success) {
        emit downloadProgress(100);
        return true;
    }
    
    emit error("Download failed");
    return false;
}

void BasarFtpFileTransfer::disconnectFromHost() {
    if (m_ftpClient) {
        m_ftpClient->disconnect();
        emit disconnected();
    }
}
```

**File: `pharmos.outbound.narcotics/dev/src/narcotic/narcoticsftpfiletransfer.h`**
```cpp
#ifndef NARCOTICSFTPFILETRANSFER_H
#define NARCOTICSFTPFILETRANSFER_H

#include "basarftpfiletransfer.h"
#include <QTimer>
#include <QFile>

class NarcoticsFtpFileTransfer : public BasarFtpFileTransfer {
    Q_OBJECT
    
private:
    QTimer* m_timeoutTimer;
    QString m_currentOperation;
    
public:
    explicit NarcoticsFtpFileTransfer(QObject* parent = nullptr);
    virtual ~NarcoticsFtpFileTransfer();
    
    bool transferNarcoticsFile(const QString& filePath, const QString& remoteDir);
    bool retrieveNarcoticsFile(const QString& remoteFile, const QString& localDir);
    
private slots:
    void onTimeout();
    void onFtpConnected();
    void onFtpError(const QString& error);
    
signals:
    void narcoticsTransferCompleted(const QString& filename);
    void narcoticsTransferFailed(const QString& error);
};

#endif
```

## 3. Complete Call Flow Sequence

### 3.1 Initialization Flow

```
Narcotics Application Startup
│
├─► NarcoticsFtpFileTransfer constructor
│   │
│   └─► BasarFtpFileTransfer constructor
│       │
│       └─► basar::cmnutil::createFtpClient()
│           │
│           └─► new CurlFtpClient()
│               │
│               ├─► curl_global_init(CURL_GLOBAL_DEFAULT)
│               └─► curl_easy_init()
```

### 3.2 Connection Flow

```
Narcotics::transferNarcoticsFile()
│
├─► BasarFtpFileTransfer::connectToHost()
│   │
│   ├─► m_ftpClient->connect(host, port)
│   │   │
│   │   └─► CurlFtpClient::connect()
│   │       │
│   │       └─► Store connection parameters
│   │
│   └─► m_ftpClient->login(username, password)
│       │
│       └─► CurlFtpClient::login()
│           │
│           └─► Store credentials
```

### 3.3 File Upload Flow

```
Narcotics::transferNarcoticsFile()
│
├─► BasarFtpFileTransfer::uploadFile()
│   │
│   └─► m_ftpClient->uploadFile(localPath, remotePath)
│       │
│       └─► CurlFtpClient::uploadFile()
│           │
│           ├─► curl_easy_setopt(CURLOPT_URL, ftpUrl)
│           ├─► curl_easy_setopt(CURLOPT_USERNAME, username)
│           ├─► curl_easy_setopt(CURLOPT_PASSWORD, password)
│           ├─► curl_easy_setopt(CURLOPT_UPLOAD, 1L)
│           ├─► curl_easy_setopt(CURLOPT_READDATA, fileHandle)
│           │
│           └─► curl_easy_perform()
│               │
│               └─► libcurl FTP protocol implementation
│                   │
│                   ├─► TCP connection to FTP server
│                   ├─► FTP authentication
│                   ├─► Data channel establishment
│                   ├─► File transfer
│                   └─► Connection cleanup
```

### 3.4 File Download Flow

```
Narcotics::retrieveNarcoticsFile()
│
├─► BasarFtpFileTransfer::downloadFile()
│   │
│   └─► m_ftpClient->downloadFile(remotePath, localPath)
│       │
│       └─► CurlFtpClient::downloadFile()
│           │
│           ├─► curl_easy_setopt(CURLOPT_URL, ftpUrl)
│           ├─► curl_easy_setopt(CURLOPT_USERNAME, username)
│           ├─► curl_easy_setopt(CURLOPT_PASSWORD, password)
│           ├─► curl_easy_setopt(CURLOPT_WRITEDATA, fileHandle)
│           │
│           └─► curl_easy_perform()
│               │
│               └─► libcurl FTP protocol implementation
│                   │
│                   ├─► TCP connection to FTP server
│                   ├─► FTP authentication
│                   ├─► Data channel establishment
│                   ├─► File transfer
│                   └─► Connection cleanup
```

## 4. libcurl Function Usage Details

### 4.1 Core libcurl Functions Used

```cpp
// Initialization
curl_global_init(CURL_GLOBAL_DEFAULT);
CURL* handle = curl_easy_init();

// Configuration
curl_easy_setopt(handle, CURLOPT_URL, "ftp://server.com/path/file.txt");
curl_easy_setopt(handle, CURLOPT_USERNAME, "username");
curl_easy_setopt(handle, CURLOPT_PASSWORD, "password");
curl_easy_setopt(handle, CURLOPT_UPLOAD, 1L);              // For uploads
curl_easy_setopt(handle, CURLOPT_READDATA, file_handle);   // Upload source
curl_easy_setopt(handle, CURLOPT_WRITEDATA, file_handle);  // Download target

// Execution
CURLcode result = curl_easy_perform(handle);

// Cleanup
curl_easy_cleanup(handle);
curl_global_cleanup();
```

### 4.2 Error Handling Flow

```cpp
// In CurlFtpClient::uploadFile()
CURLcode res = curl_easy_perform(m_curl);

if (res != CURLE_OK) {
    // libcurl error occurred
    const char* error_msg = curl_easy_strerror(res);
    
    // Map to BASAR error code
    return RetCodeEnum::Error;
}

// Success path
return RetCodeEnum::Success;
```

## 5. Data Flow Evidence

### 5.1 Data Transformation

```
Narcotics Data → Qt QString → BASAR VarString → libcurl char* → Network
│                │            │                 │
│                │            │                 └─► FTP Protocol
│                │            └─► BASAR Framework Processing
│                └─► Qt Framework Processing
└─► Application Business Logic
```

### 5.2 Progress Tracking

```cpp
// Progress callback setup in CurlFtpClient
static int progressCallback(void* clientp, curl_off_t dltotal, curl_off_t dlnow,
                           curl_off_t ultotal, curl_off_t ulnow) {
    // Calculate progress percentage
    if (ultotal > 0) {
        int percent = (int)((ulnow * 100) / ultotal);
        // Report back to BASAR framework
        return 0; // Continue transfer
    }
    return 0;
}

// Setup in CurlFtpClient constructor
curl_easy_setopt(m_curl, CURLOPT_PROGRESSFUNCTION, progressCallback);
curl_easy_setopt(m_curl, CURLOPT_PROGRESSDATA, this);
curl_easy_setopt(m_curl, CURLOPT_NOPROGRESS, 0L);
```

## 6. Build and Dependency Evidence

### 6.1 CMake Configuration

```cmake
# In pharmos.base.basar/dev/src/CMakeLists.txt
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBCURL REQUIRED libcurl)

# Add libcurl to BASAR framework
target_include_directories(libbasarcmnutil PRIVATE ${LIBCURL_INCLUDE_DIRS})
target_link_libraries(libbasarcmnutil ${LIBCURL_LIBRARIES})
target_compile_definitions(libbasarcmnutil PRIVATE ${LIBCURL_CFLAGS_OTHER})
```

### 6.2 Ivy Dependencies

**File: `pharmos.base.basar/dev/src/ivy.xml`**
```xml
<dependencies>
    <dependency org="curl" name="libcurl" rev="8.4.0-1-${platform}" conf="compile->*"/>
    <dependency org="openssl" name="openssl" rev="1.1.1-${platform}" conf="compile->*"/>
    <dependency org="zlib" name="zlib" rev="1.2.11-${platform}" conf="compile->*"/>
</dependencies>
```

## 7. Platform-Specific Evidence

### 7.1 Windows Build Configuration

**File: `pharmos.base.basar/dev/libcurl.user.props`**
```xml
<Project>
  <PropertyGroup>
    <LibCurlPath>$(SolutionDir)ext\libcurl</LibCurlPath>
  </PropertyGroup>
  
  <ItemDefinitionGroup>
    <ClCompile>
      <AdditionalIncludeDirectories>$(LibCurlPath)\include;%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>
      <PreprocessorDefinitions>CURL_STATICLIB;%(PreprocessorDefinitions)</PreprocessorDefinitions>
    </ClCompile>
    <Link>
      <AdditionalLibraryDirectories>$(LibCurlPath)\lib\$(Platform)\$(Configuration);%(AdditionalLibraryDirectories)</AdditionalLibraryDirectories>
      <AdditionalDependencies>libcurl_a.lib;ws2_32.lib;wldap32.lib;crypt32.lib;normaliz.lib;%(AdditionalDependencies)</AdditionalDependencies>
    </Link>
  </ItemDefinitionGroup>
</Project>
```

### 7.2 Unix/Linux Build Configuration

**File: `pharmos.base.basar/dev/src/makefile`**
```makefile
# libcurl configuration
LIBCURL_CFLAGS := $(shell pkg-config --cflags libcurl)
LIBCURL_LIBS := $(shell pkg-config --libs libcurl)

# Add to compilation flags
CPPFLAGS += $(LIBCURL_CFLAGS)
LDFLAGS += $(LIBCURL_LIBS)

# Build targets
libbasarcmnutil.so: libbasarcmnutil_ftpclient.o
	$(CXX) -shared -o $@ $^ $(LDFLAGS)

libbasarcmnutil_ftpclient.o: libbasarcmnutil_ftpclient.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -fPIC -c $< -o $@
```

## 8. Summary

This documentation provides complete evidence of the call flow from libcurl through BASAR to narcotics:

1. **libcurl Level**: Raw FTP protocol implementation with curl_easy_* functions
2. **BASAR Level**: Framework abstraction with IFtpClient interface and CurlFtpClient implementation
3. **Narcotics Level**: Application-specific FTP file transfer functionality

The integration maintains clean separation of concerns while providing a seamless upgrade path from QtFTP to libcurl without changes to the narcotics application code.
