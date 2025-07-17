
#ifndef LIBBASARCMNUTIL_IFTPCLIENT_H
#define LIBBASARCMNUTIL_IFTPCLIENT_H

#include "libbasarcmnutil_definitions.h"
#include "libbasarcmnutil_functions.h"

namespace basar {
namespace cmnutil {

/**
 * @brief Abstract interface for FTP client operations
 */
class IFtpClient {
public:
    /**
     * @brief Return codes for FTP operations
     */
    enum RetCodeEnum {
        FTP_OK = 0,
        FTP_OFFLINE,
        FTP_AUTHERROR,
        FTP_FILENOTFOUND,
        FTP_PERMISSIONDENIED,
        FTP_TIMEOUT,
        FTP_PROTOCOL_ERROR,
        FTP_UNKNOWN_ERROR
    };

    /**
     * @brief Progress callback function type
     */
    typedef void (*ProgressCallback)(double bytesTransferred, double totalBytes, void* userData);

    virtual ~IFtpClient() = default;

    // Connection management
    virtual RetCodeEnum connect(const VarString& hostname, Int16 port = 21) = 0;
    virtual RetCodeEnum login(const VarString& username, const VarString& password) = 0;
    virtual RetCodeEnum disconnect() = 0;

    // Directory operations
    virtual RetCodeEnum changeDirectory(const VarString& path) = 0;
    virtual RetCodeEnum createDirectory(const VarString& dirName) = 0;
    virtual RetCodeEnum listDirectory(VarString& listing) = 0;

    // File operations
    virtual RetCodeEnum uploadFile(const VarString& localPath, const VarString& remotePath) = 0;
    virtual RetCodeEnum downloadFile(const VarString& remotePath, const VarString& localPath) = 0;
    virtual RetCodeEnum deleteFile(const VarString& remotePath) = 0;

    // Configuration
    virtual void setProgressCallback(ProgressCallback callback, void* userData) = 0;
    virtual void setTimeout(Int32 timeoutSeconds) = 0;
    virtual void setPassiveMode(bool passive) = 0;
};

} // namespace cmnutil
} // namespace basar

#endif // LIBBASARCMNUTIL_IFTPCLIENT_H
