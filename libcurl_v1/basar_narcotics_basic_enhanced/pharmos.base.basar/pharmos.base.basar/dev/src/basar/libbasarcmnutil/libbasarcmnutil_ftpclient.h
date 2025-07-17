//----------------------------------------------------------------------------
/*! \file
 *  \brief  public library interface: FTP client using libcurl
 *  \author Generated for QtFTP replacement
 *  \date   January 2025
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARCMNUTIL_FTPCLIENT_H
#define GUARD_LIBBASARCMNUTIL_FTPCLIENT_H

#include "libbasarcmnutil_iftpclient.h"
#include <curl/curl.h>
#include <memory>

//------------------------------------------------------------------------------
#ifndef LIBBASARCMNUTIL_API
        #ifdef _WIN32
                #ifdef LIBBASARCMNUTIL_EXPORTS
                        #define LIBBASARCMNUTIL_API __declspec(dllexport)
                #else
                        #define LIBBASARCMNUTIL_API __declspec(dllimport)
                #endif 
        #else 
                #define LIBBASARCMNUTIL_API
        #endif
#endif

//----------------------------------------------------------------------------
#include "libbasar_definitions.h"
#include "libbasarcmnutil_bstring.h"
#include "libbasarcmnutil_exceptions.h"
#include <vector>
#include <memory>
#include <functional>

// Forward declaration for libcurl
typedef void CURL;

//----------------------------------------------------------------------------
namespace basar         {
namespace cmnutil       {

//############################################################################
//   FTP file info structure
//############################################################################

//----------------------------------------------------------------------------
//! \brief FTP file/directory information structure
struct FtpFileInfo
{
    VarString name;         //!< file or directory name
    VarString fullPath;     //!< full path on server
    BULong size;           //!< file size in bytes (0 for directories)
    bool isDirectory;      //!< true if this is a directory
    VarString permissions; //!< file permissions string
    VarString owner;       //!< file owner
    VarString group;       //!< file group
    VarString modified;    //!< last modification time

    FtpFileInfo() : size(0), isDirectory(false) {}
};

//############################################################################
//   exception handler
//############################################################################

//----------------------------------------------------------------------------
//! \brief FTP client exception class; thrown if errors with FTP operations occur
//! \n final class
class FtpClientException : public basar::Exception
{
public:
        //! constructs exception object and initialises exception type
        LIBBASARCMNUTIL_API FtpClientException(const ExceptInfo & sInfo)
                : basar::Exception(sInfo, "FtpClientException") {};
private:
        FtpClientException();  //!< forbidden
};

//############################################################################
//   callback types
//############################################################################

//! Progress callback function type
typedef std::function<int(double totalBytes, double nowBytes, double totalUpload, double nowUpload)> ProgressCallback;

//! Data callback function type  
typedef std::function<void(const char* data, size_t size)> DataCallback;

//############################################################################
//   FTP client interface
//############################################################################

//----------------------------------------------------------------------------
//! \brief Abstract FTP client interface
//! \n interface class
class IFtpClient
{
public:
    enum RetCodeEnum
    {
        FTP_SUCCESS = 0,        //!< operation completed successfully
        FTP_OFFLINE = 1,        //!< server offline/unreachable
        FTP_CONNREFUSED = 2,    //!< connection refused by server
        FTP_AUTHERROR = 3,      //!< authentication failed
        FTP_FILENOTFOUND = 4,   //!< file or directory not found
        FTP_PERMISSIONDENIED = 5, //!< permission denied
        FTP_TIMEOUT = 6,        //!< operation timed out
        FTP_PROTOCOL_ERROR = 7, //!< protocol error
        FTP_GENERAL_ERROR = 8,  //!< general/unknown error
        FTP_RETCODE_MAX         //!< maximum return codes
    };

    //! virtual destructor
    virtual ~IFtpClient() = default;

    // Connection management

    //! connect to FTP server
    virtual RetCodeEnum connect(const VarString& hostname, Int16 port = 21) = 0;

    //! login with username and password
    virtual RetCodeEnum login(const VarString& username, const VarString& password) = 0;

    //! disconnect from server
    virtual void disconnect() = 0;

    //! check if connected
    virtual bool isConnected() const = 0;

    // Directory operations

    //! change current directory
    virtual RetCodeEnum changeDirectory(const VarString& path) = 0;

    //! get current directory
    virtual RetCodeEnum getCurrentDirectory(VarString& path) = 0;

    //! create directory
    virtual RetCodeEnum createDirectory(const VarString& path) = 0;

    //! remove directory
    virtual RetCodeEnum removeDirectory(const VarString& path) = 0;

    //! list directory contents
    virtual RetCodeEnum listDirectory(const VarString& path, std::vector<FtpFileInfo>& entries) = 0;

    // File operations

    //! upload file from local path to remote path
    virtual RetCodeEnum uploadFile(const VarString& localPath, const VarString& remotePath) = 0;

    //! download file from remote path to local path
    virtual RetCodeEnum downloadFile(const VarString& remotePath, const VarString& localPath) = 0;

    //! delete file on server
    virtual RetCodeEnum deleteFile(const VarString& remotePath) = 0;

    //! rename file on server
    virtual RetCodeEnum renameFile(const VarString& oldPath, const VarString& newPath) = 0;

    //! get file size
    virtual RetCodeEnum getFileSize(const VarString& remotePath, BULong& size) = 0;

    // Advanced operations

    //! set file permissions (Unix-style)
    virtual RetCodeEnum setPermissions(const VarString& path, const VarString& permissions) = 0;

    //! execute raw FTP command
    virtual RetCodeEnum executeCommand(const VarString& command, VarString& response) = 0;

    //! set transfer mode (ASCII/Binary)
    virtual RetCodeEnum setTransferMode(bool binaryMode) = 0;

    //! set passive mode
    virtual RetCodeEnum setPassiveMode(bool passive) = 0;

    //! set timeout for operations
    virtual RetCodeEnum setTimeout(Int32 timeoutSeconds) = 0;

    //! set progress callback
    virtual void setProgressCallback(ProgressCallback callback) = 0;

    //! get last error message
    virtual const VarString& getLastError() const = 0;
};

//############################################################################
//   libcurl-based FTP client implementation
//############################################################################

//----------------------------------------------------------------------------
//! \brief libcurl-based FTP client implementation
//! \n final class
class CurlFtpClient : public IFtpClient
{
public:
    //! constructor
    LIBBASARCMNUTIL_API CurlFtpClient();

    //! destructor
    LIBBASARCMNUTIL_API ~CurlFtpClient();

    // Connection management
    LIBBASARCMNUTIL_API RetCodeEnum connect(const VarString& hostname, Int16 port = 21) override;
    LIBBASARCMNUTIL_API RetCodeEnum login(const VarString& username, const VarString& password) override;
    LIBBASARCMNUTIL_API void disconnect() override;
    LIBBASARCMNUTIL_API bool isConnected() const override;

    // Directory operations
    LIBBASARCMNUTIL_API RetCodeEnum changeDirectory(const VarString& path) override;
    LIBBASARCMNUTIL_API RetCodeEnum getCurrentDirectory(VarString& path) override;
    LIBBASARCMNUTIL_API RetCodeEnum createDirectory(const VarString& path) override;
    LIBBASARCMNUTIL_API RetCodeEnum removeDirectory(const VarString& path) override;
    LIBBASARCMNUTIL_API RetCodeEnum listDirectory(const VarString& path, std::vector<FtpFileInfo>& entries) override;

    // File operations
    LIBBASARCMNUTIL_API RetCodeEnum uploadFile(const VarString& localPath, const VarString& remotePath) override;
    LIBBASARCMNUTIL_API RetCodeEnum downloadFile(const VarString& remotePath, const VarString& localPath) override;
    LIBBASARCMNUTIL_API RetCodeEnum deleteFile(const VarString& remotePath) override;
    LIBBASARCMNUTIL_API RetCodeEnum renameFile(const VarString& oldPath, const VarString& newPath) override;
    LIBBASARCMNUTIL_API RetCodeEnum getFileSize(const VarString& remotePath, BULong& size) override;

    // Advanced operations
    LIBBASARCMNUTIL_API RetCodeEnum setPermissions(const VarString& path, const VarString& permissions) override;
    LIBBASARCMNUTIL_API RetCodeEnum executeCommand(const VarString& command, VarString& response) override;
    LIBBASARCMNUTIL_API RetCodeEnum setTransferMode(bool binaryMode) override;
    LIBBASARCMNUTIL_API RetCodeEnum setPassiveMode(bool passive) override;
    LIBBASARCMNUTIL_API RetCodeEnum setTimeout(Int32 timeoutSeconds) override;
    LIBBASARCMNUTIL_API void setProgressCallback(ProgressCallback callback) override;
    LIBBASARCMNUTIL_API const VarString& getLastError() const override;

private:
    // Private implementation details
    struct Impl;
    std::unique_ptr<Impl> m_pImpl;

    // Forbidden operations
    CurlFtpClient(const CurlFtpClient& rCopy);           //!< forbidden
    CurlFtpClient& operator=(const CurlFtpClient& rCopy); //!< forbidden

    // Internal helper methods
    RetCodeEnum mapCurlError(long curlCode);
    void updateLastError(const VarString& error);
    VarString buildFtpUrl(const VarString& path) const;
    static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp);
    static size_t readCallback(void* contents, size_t size, size_t nmemb, void* userp);
    static int progressCallback(void* clientp, double dltotal, double dlnow, double ultotal, double ulnow);
};

//############################################################################
//   factory function
//############################################################################

//! Create FTP client instance
LIBBASARCMNUTIL_API std::unique_ptr<IFtpClient> createFtpClient();

//-----------------------------------------------------------------------------
}       // namespace cmnutil
}       // namespace basar

//-----------------------------------------------------------------------------
#endif  // GUARD_LIBBASARCMNUTIL_FTPCLIENT_H