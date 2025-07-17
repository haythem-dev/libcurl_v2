//----------------------------------------------------------------------------
/*! \file
 *  \brief  libcurl-based FTP client implementation
 *  \author Generated for QtFTP replacement
 *  \date   January 2025
 */
//----------------------------------------------------------------------------

#include "libbasarcmnutil_ftpclient.h"
#include "libbasarcmnutil_logging.h"
#include <curl/curl.h>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstring>

//----------------------------------------------------------------------------
namespace basar         {
namespace cmnutil       {

//############################################################################
//   Internal implementation structure
//############################################################################

struct CurlFtpClient::Impl
{
    CURL* curl;
    VarString hostname;
    Int16 port;
    VarString username;
    VarString password;
    VarString currentDir;
    VarString lastError;
    bool connected;
    bool binaryMode;
    bool passiveMode;
    Int32 timeoutSeconds;
    ProgressCallback progressCallback;
    
    // Response data storage
    VarString responseData;
    std::vector<char> downloadBuffer;
    std::ifstream uploadStream;
    
    Impl() : curl(nullptr), port(21), connected(false), binaryMode(true), 
             passiveMode(true), timeoutSeconds(30) {}
    
    ~Impl() {
        if (curl) {
            curl_easy_cleanup(curl);
        }
    }
};

//############################################################################
//   Static callbacks
//############################################################################

//############################################################################
//   CurlFtpClient Implementation
//############################################################################

namespace basar {
namespace cmnutil {

struct CurlFtpClient::CurlFtpClientImpl {
    CURL* curl;
    VarString hostname;
    Int16 port;
    VarString username;
    VarString password;
    VarString currentDir;
    VarString responseData;
    ProgressCallback progressCb;
    void* progressUserData;
    Int32 timeoutSeconds;
    bool passiveMode;
    
    CurlFtpClientImpl() 
        : curl(nullptr), port(21), progressCb(nullptr), 
          progressUserData(nullptr), timeoutSeconds(30), passiveMode(true) {
        curl = curl_easy_init();
    }
    
    ~CurlFtpClientImpl() {
        if (curl) {
            curl_easy_cleanup(curl);
        }
    }
};

//----------------------------------------------------------------------------
CurlFtpClient::CurlFtpClient() : m_pImpl(std::make_unique<CurlFtpClientImpl>()) {
    if (m_pImpl->curl) {
        curl_easy_setopt(m_pImpl->curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(m_pImpl->curl, CURLOPT_WRITEDATA, &m_pImpl->responseData);
        curl_easy_setopt(m_pImpl->curl, CURLOPT_NOPROGRESS, 0L);
        curl_easy_setopt(m_pImpl->curl, CURLOPT_XFERINFOFUNCTION, progressCallback);
    }
}

//----------------------------------------------------------------------------
CurlFtpClient::~CurlFtpClient() = default;

//----------------------------------------------------------------------------
CurlFtpClient::RetCodeEnum CurlFtpClient::connect(const VarString& hostname, Int16 port) {
    if (!m_pImpl->curl) {
        return FTP_UNKNOWN_ERROR;
    }
    
    m_pImpl->hostname = hostname;
    m_pImpl->port = port;
    
    VarString url;
    url.format("ftp://%s:%d/", hostname.c_str(), port);
    
    curl_easy_setopt(m_pImpl->curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(m_pImpl->curl, CURLOPT_TIMEOUT, m_pImpl->timeoutSeconds);
    curl_easy_setopt(m_pImpl->curl, CURLOPT_FTP_RESPONSE_TIMEOUT, m_pImpl->timeoutSeconds);
    
    if (m_pImpl->passiveMode) {
        curl_easy_setopt(m_pImpl->curl, CURLOPT_FTPPORT, nullptr);
    }
    
    return FTP_OK;
}

//----------------------------------------------------------------------------
CurlFtpClient::RetCodeEnum CurlFtpClient::login(const VarString& username, const VarString& password) {
    if (!m_pImpl->curl) {
        return FTP_UNKNOWN_ERROR;
    }
    
    m_pImpl->username = username;
    m_pImpl->password = password;
    
    VarString userpass;
    userpass.format("%s:%s", username.c_str(), password.c_str());
    
    curl_easy_setopt(m_pImpl->curl, CURLOPT_USERPWD, userpass.c_str());
    curl_easy_setopt(m_pImpl->curl, CURLOPT_NOBODY, 1L);
    
    m_pImpl->responseData.clear();
    CURLcode res = curl_easy_perform(m_pImpl->curl);
    
    curl_easy_setopt(m_pImpl->curl, CURLOPT_NOBODY, 0L);
    
    return mapCurlError(res);
}

//----------------------------------------------------------------------------
CurlFtpClient::RetCodeEnum CurlFtpClient::disconnect() {
    // libcurl doesn't maintain persistent connections by default
    return FTP_OK;
}

//----------------------------------------------------------------------------
CurlFtpClient::RetCodeEnum CurlFtpClient::uploadFile(const VarString& localPath, 
                                                    const VarString& remotePath) {
    if (!m_pImpl->curl) {
        return FTP_UNKNOWN_ERROR;
    }
    
    std::ifstream file(localPath.c_str(), std::ios::binary);
    if (!file.is_open()) {
        return FTP_FILENOTFOUND;
    }
    
    VarString url;
    url.format("ftp://%s:%d%s", m_pImpl->hostname.c_str(), m_pImpl->port, remotePath.c_str());
    
    curl_easy_setopt(m_pImpl->curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(m_pImpl->curl, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(m_pImpl->curl, CURLOPT_READFUNCTION, readCallback);
    curl_easy_setopt(m_pImpl->curl, CURLOPT_READDATA, &file);
    
    if (m_pImpl->progressCb) {
        curl_easy_setopt(m_pImpl->curl, CURLOPT_XFERINFODATA, m_pImpl->progressUserData);
    }
    
    CURLcode res = curl_easy_perform(m_pImpl->curl);
    
    curl_easy_setopt(m_pImpl->curl, CURLOPT_UPLOAD, 0L);
    file.close();
    
    return mapCurlError(res);
}

//----------------------------------------------------------------------------
CurlFtpClient::RetCodeEnum CurlFtpClient::mapCurlError(CURLcode result) {
    switch (result) {
        case CURLE_OK:
            return FTP_OK;
        case CURLE_COULDNT_CONNECT:
        case CURLE_FTP_CANT_GET_HOST:
            return FTP_OFFLINE;
        case CURLE_LOGIN_DENIED:
        case CURLE_FTP_ACCESS_DENIED:
            return FTP_AUTHERROR;
        case CURLE_REMOTE_FILE_NOT_FOUND:
            return FTP_FILENOTFOUND;
        case CURLE_UPLOAD_FAILED:
            return FTP_PERMISSIONDENIED;
        case CURLE_OPERATION_TIMEDOUT:
            return FTP_TIMEOUT;
        case CURLE_FTP_WEIRD_SERVER_REPLY:
            return FTP_PROTOCOL_ERROR;
        default:
            return FTP_UNKNOWN_ERROR;
    }
}

//----------------------------------------------------------------------------
void CurlFtpClient::setProgressCallback(ProgressCallback callback, void* userData) {
    m_pImpl->progressCb = callback;
    m_pImpl->progressUserData = userData;
}

//----------------------------------------------------------------------------
void CurlFtpClient::setTimeout(Int32 timeoutSeconds) {
    m_pImpl->timeoutSeconds = timeoutSeconds;
}

//----------------------------------------------------------------------------
void CurlFtpClient::setPassiveMode(bool passive) {
    m_pImpl->passiveMode = passive;
}

} // namespace cmnutil
} // namespace basar

//############################################################################
//   Static callbacks
//############################################################################

//----------------------------------------------------------------------------
size_t CurlFtpClient::writeCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    size_t totalSize = size * nmemb;
    VarString* response = static_cast<VarString*>(userp);
    response->append(static_cast<const char*>(contents), totalSize);
    return totalSize;
}

//----------------------------------------------------------------------------
size_t CurlFtpClient::readCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    std::ifstream* stream = static_cast<std::ifstream*>(userp);
    size_t totalSize = size * nmemb;
    
    if (!stream->good()) {
        return 0;
    }
    
    stream->read(static_cast<char*>(contents), totalSize);
    return static_cast<size_t>(stream->gcount());
}

//----------------------------------------------------------------------------
int CurlFtpClient::progressCallback(void* userData, curl_off_t dltotal, curl_off_t dlnow, 
                                   curl_off_t ultotal, curl_off_t ulnow) {
    if (userData) {
        CurlFtpClient::CurlFtpClientImpl* impl = 
            static_cast<CurlFtpClient::CurlFtpClientImpl*>(userData);
        
        if (impl->progressCb) {
            double total = static_cast<double>(dltotal + ultotal);
            double current = static_cast<double>(dlnow + ulnow);
            impl->progressCb(current, total, impl->progressUserData);
        }
    }
    
    return 0; // Return 0 to continue
}

//----------------------------------------------------------------------------
int CurlFtpClient::progressCallback(void* clientp, double dltotal, double dlnow, double ultotal, double ulnow)
{
    CurlFtpClient* client = static_cast<CurlFtpClient*>(clientp);
    if (client->m_pImpl->progressCallback) {
        return client->m_pImpl->progressCallback(dltotal, dlnow, ultotal, ulnow);
    }
    return 0;
}

//############################################################################
//   Constructor and destructor
//############################################################################

//----------------------------------------------------------------------------
CurlFtpClient::CurlFtpClient() : m_pImpl(std::make_unique<Impl>())
{
    // Initialize libcurl
    m_pImpl->curl = curl_easy_init();
    if (!m_pImpl->curl) {
        ExceptInfo info;
        info.context = "CurlFtpClient::CurlFtpClient()";
        info.file = __FILE__;
        info.line = __LINE__;
        info.reason = "Failed to initialize libcurl";
        throw FtpClientException(info);
    }
    
    // Set default options
    curl_easy_setopt(m_pImpl->curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(m_pImpl->curl, CURLOPT_READFUNCTION, readCallback);
    curl_easy_setopt(m_pImpl->curl, CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(m_pImpl->curl, CURLOPT_PROGRESSFUNCTION, progressCallback);
    curl_easy_setopt(m_pImpl->curl, CURLOPT_PROGRESSDATA, this);
    curl_easy_setopt(m_pImpl->curl, CURLOPT_TIMEOUT, 30L);
    curl_easy_setopt(m_pImpl->curl, CURLOPT_CONNECTTIMEOUT, 10L);
    curl_easy_setopt(m_pImpl->curl, CURLOPT_FTP_RESPONSE_TIMEOUT, 30L);
}

//----------------------------------------------------------------------------
CurlFtpClient::~CurlFtpClient()
{
    disconnect();
}

//############################################################################
//   Connection management
//############################################################################

//----------------------------------------------------------------------------
CurlFtpClient::RetCodeEnum CurlFtpClient::connect(const VarString& hostname, Int16 port)
{
    if (!m_pImpl->curl) {
        updateLastError("libcurl not initialized");
        return FTP_GENERAL_ERROR;
    }
    
    m_pImpl->hostname = hostname;
    m_pImpl->port = port;
    
    // Build URL
    VarString url;
    url.format("ftp://%s:%d/", hostname.c_str(), port);
    
    curl_easy_setopt(m_pImpl->curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(m_pImpl->curl, CURLOPT_NOBODY, 1L);
    curl_easy_setopt(m_pImpl->curl, CURLOPT_WRITEDATA, &m_pImpl->responseData);
    
    // Perform connection test
    CURLcode res = curl_easy_perform(m_pImpl->curl);
    
    if (res == CURLE_OK) {
        m_pImpl->connected = true;
        return FTP_SUCCESS;
    }
    
    return mapCurlError(res);
}

//----------------------------------------------------------------------------
CurlFtpClient::RetCodeEnum CurlFtpClient::login(const VarString& username, const VarString& password)
{
    if (!m_pImpl->curl) {
        updateLastError("libcurl not initialized");
        return FTP_GENERAL_ERROR;
    }
    
    m_pImpl->username = username;
    m_pImpl->password = password;
    
    // Set authentication
    curl_easy_setopt(m_pImpl->curl, CURLOPT_USERNAME, username.c_str());
    curl_easy_setopt(m_pImpl->curl, CURLOPT_PASSWORD, password.c_str());
    
    // Test login with PWD command
    VarString url;
    url.format("ftp://%s:%d/", m_pImpl->hostname.c_str(), m_pImpl->port);
    
    curl_easy_setopt(m_pImpl->curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(m_pImpl->curl, CURLOPT_NOBODY, 1L);
    curl_easy_setopt(m_pImpl->curl, CURLOPT_WRITEDATA, &m_pImpl->responseData);
    
    CURLcode res = curl_easy_perform(m_pImpl->curl);
    
    if (res == CURLE_OK) {
        long response_code;
        curl_easy_getinfo(m_pImpl->curl, CURLINFO_RESPONSE_CODE, &response_code);
        
        if (response_code >= 200 && response_code < 300) {
            m_pImpl->connected = true;
            return FTP_SUCCESS;
        } else {
            updateLastError("Authentication failed");
            return FTP_AUTHERROR;
        }
    }
    
    return mapCurlError(res);
}

//----------------------------------------------------------------------------
void CurlFtpClient::disconnect()
{
    if (m_pImpl->curl) {
        // Clean up any ongoing transfers
        curl_easy_reset(m_pImpl->curl);
    }
    
    m_pImpl->connected = false;
    m_pImpl->currentDir.clear();
    m_pImpl->responseData.clear();
}

//----------------------------------------------------------------------------
bool CurlFtpClient::isConnected() const
{
    return m_pImpl->connected;
}

//############################################################################
//   Directory operations
//############################################################################

//----------------------------------------------------------------------------
CurlFtpClient::RetCodeEnum CurlFtpClient::changeDirectory(const VarString& path)
{
    if (!m_pImpl->connected) {
        updateLastError("Not connected to server");
        return FTP_GENERAL_ERROR;
    }
    
    VarString url = buildFtpUrl(path);
    
    curl_easy_setopt(m_pImpl->curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(m_pImpl->curl, CURLOPT_NOBODY, 1L);
    curl_easy_setopt(m_pImpl->curl, CURLOPT_WRITEDATA, &m_pImpl->responseData);
    
    CURLcode res = curl_easy_perform(m_pImpl->curl);
    
    if (res == CURLE_OK) {
        m_pImpl->currentDir = path;
        return FTP_SUCCESS;
    }
    
    return mapCurlError(res);
}

//----------------------------------------------------------------------------
CurlFtpClient::RetCodeEnum CurlFtpClient::getCurrentDirectory(VarString& path)
{
    path = m_pImpl->currentDir;
    return FTP_SUCCESS;
}

//----------------------------------------------------------------------------
CurlFtpClient::RetCodeEnum CurlFtpClient::createDirectory(const VarString& path)
{
    if (!m_pImpl->connected) {
        updateLastError("Not connected to server");
        return FTP_GENERAL_ERROR;
    }
    
    VarString url = buildFtpUrl(path);
    
    curl_easy_setopt(m_pImpl->curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(m_pImpl->curl, CURLOPT_FTP_CREATE_MISSING_DIRS, 1L);
    curl_easy_setopt(m_pImpl->curl, CURLOPT_WRITEDATA, &m_pImpl->responseData);
    
    CURLcode res = curl_easy_perform(m_pImpl->curl);
    
    if (res == CURLE_OK) {
        return FTP_SUCCESS;
    }
    
    return mapCurlError(res);
}

//----------------------------------------------------------------------------
CurlFtpClient::RetCodeEnum CurlFtpClient::removeDirectory(const VarString& path)
{
    if (!m_pImpl->connected) {
        updateLastError("Not connected to server");
        return FTP_GENERAL_ERROR;
    }
    
    VarString command;
    command.format("RMD %s", path.c_str());
    
    VarString response;
    return executeCommand(command, response);
}

//----------------------------------------------------------------------------
CurlFtpClient::RetCodeEnum CurlFtpClient::listDirectory(const VarString& path, std::vector<FtpFileInfo>& entries)
{
    if (!m_pImpl->connected) {
        updateLastError("Not connected to server");
        return FTP_GENERAL_ERROR;
    }
    
    entries.clear();
    m_pImpl->responseData.clear();
    
    VarString url = buildFtpUrl(path);
    
    curl_easy_setopt(m_pImpl->curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(m_pImpl->curl, CURLOPT_DIRLISTONLY, 1L);
    curl_easy_setopt(m_pImpl->curl, CURLOPT_WRITEDATA, &m_pImpl->responseData);
    
    CURLcode res = curl_easy_perform(m_pImpl->curl);
    
    if (res == CURLE_OK) {
        // Parse directory listing
        std::istringstream iss(m_pImpl->responseData.c_str());
        VarString line;
        char lineBuffer[1024];
        
        while (iss.getline(lineBuffer, sizeof(lineBuffer))) {
            line = lineBuffer;
            line.trim();
            
            if (!line.empty()) {
                FtpFileInfo info;
                info.name = line;
                info.fullPath = path + "/" + line;
                info.isDirectory = false; // Would need detailed parsing for this
                entries.push_back(info);
            }
        }
        
        return FTP_SUCCESS;
    }
    
    return mapCurlError(res);
}

//############################################################################
//   File operations
//############################################################################

//----------------------------------------------------------------------------
CurlFtpClient::RetCodeEnum CurlFtpClient::uploadFile(const VarString& localPath, const VarString& remotePath)
{
    if (!m_pImpl->connected) {
        updateLastError("Not connected to server");
        return FTP_GENERAL_ERROR;
    }
    
    std::ifstream file(localPath.c_str(), std::ios::binary);
    if (!file.is_open()) {
        updateLastError("Cannot open local file for reading");
        return FTP_FILENOTFOUND;
    }
    
    // Get file size
    file.seekg(0, std::ios::end);
    long fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    
    VarString url = buildFtpUrl(remotePath);
    
    curl_easy_setopt(m_pImpl->curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(m_pImpl->curl, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(m_pImpl->curl, CURLOPT_READDATA, &file);
    curl_easy_setopt(m_pImpl->curl, CURLOPT_INFILESIZE, fileSize);
    
    if (m_pImpl->binaryMode) {
        curl_easy_setopt(m_pImpl->curl, CURLOPT_TRANSFERTEXT, 0L);
    } else {
        curl_easy_setopt(m_pImpl->curl, CURLOPT_TRANSFERTEXT, 1L);
    }
    
    CURLcode res = curl_easy_perform(m_pImpl->curl);
    
    file.close();
    
    if (res == CURLE_OK) {
        return FTP_SUCCESS;
    }
    
    return mapCurlError(res);
}

//----------------------------------------------------------------------------
CurlFtpClient::RetCodeEnum CurlFtpClient::downloadFile(const VarString& remotePath, const VarString& localPath)
{
    if (!m_pImpl->connected) {
        updateLastError("Not connected to server");
        return FTP_GENERAL_ERROR;
    }
    
    std::ofstream file(localPath.c_str(), std::ios::binary);
    if (!file.is_open()) {
        updateLastError("Cannot open local file for writing");
        return FTP_PERMISSIONDENIED;
    }
    
    VarString url = buildFtpUrl(remotePath);
    
    curl_easy_setopt(m_pImpl->curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(m_pImpl->curl, CURLOPT_WRITEDATA, &file);
    curl_easy_setopt(m_pImpl->curl, CURLOPT_UPLOAD, 0L);
    
    CURLcode res = curl_easy_perform(m_pImpl->curl);
    
    file.close();
    
    if (res == CURLE_OK) {
        return FTP_SUCCESS;
    }
    
    return mapCurlError(res);
}

//----------------------------------------------------------------------------
CurlFtpClient::RetCodeEnum CurlFtpClient::deleteFile(const VarString& remotePath)
{
    if (!m_pImpl->connected) {
        updateLastError("Not connected to server");
        return FTP_GENERAL_ERROR;
    }
    
    VarString command;
    command.format("DELE %s", remotePath.c_str());
    
    VarString response;
    return executeCommand(command, response);
}

//----------------------------------------------------------------------------
CurlFtpClient::RetCodeEnum CurlFtpClient::renameFile(const VarString& oldPath, const VarString& newPath)
{
    if (!m_pImpl->connected) {
        updateLastError("Not connected to server");
        return FTP_GENERAL_ERROR;
    }
    
    VarString command;
    command.format("RNFR %s", oldPath.c_str());
    
    VarString response;
    RetCodeEnum result = executeCommand(command, response);
    
    if (result == FTP_SUCCESS) {
        command.format("RNTO %s", newPath.c_str());
        result = executeCommand(command, response);
    }
    
    return result;
}

//----------------------------------------------------------------------------
CurlFtpClient::RetCodeEnum CurlFtpClient::getFileSize(const VarString& remotePath, BULong& size)
{
    if (!m_pImpl->connected) {
        updateLastError("Not connected to server");
        return FTP_GENERAL_ERROR;
    }
    
    VarString url = buildFtpUrl(remotePath);
    
    curl_easy_setopt(m_pImpl->curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(m_pImpl->curl, CURLOPT_NOBODY, 1L);
    curl_easy_setopt(m_pImpl->curl, CURLOPT_HEADER, 1L);
    
    CURLcode res = curl_easy_perform(m_pImpl->curl);
    
    if (res == CURLE_OK) {
        double contentLength;
        curl_easy_getinfo(m_pImpl->curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &contentLength);
        size = static_cast<BULong>(contentLength);
        return FTP_SUCCESS;
    }
    
    return mapCurlError(res);
}

//############################################################################
//   Advanced operations
//############################################################################

//----------------------------------------------------------------------------
CurlFtpClient::RetCodeEnum CurlFtpClient::setPermissions(const VarString& path, const VarString& permissions)
{
    if (!m_pImpl->connected) {
        updateLastError("Not connected to server");
        return FTP_GENERAL_ERROR;
    }
    
    VarString command;
    command.format("SITE CHMOD %s %s", permissions.c_str(), path.c_str());
    
    VarString response;
    return executeCommand(command, response);
}

//----------------------------------------------------------------------------
CurlFtpClient::RetCodeEnum CurlFtpClient::executeCommand(const VarString& command, VarString& response)
{
    if (!m_pImpl->connected) {
        updateLastError("Not connected to server");
        return FTP_GENERAL_ERROR;
    }
    
    m_pImpl->responseData.clear();
    
    struct curl_slist* commands = nullptr;
    commands = curl_slist_append(commands, command.c_str());
    
    curl_easy_setopt(m_pImpl->curl, CURLOPT_QUOTE, commands);
    curl_easy_setopt(m_pImpl->curl, CURLOPT_WRITEDATA, &m_pImpl->responseData);
    
    CURLcode res = curl_easy_perform(m_pImpl->curl);
    
    curl_slist_free_all(commands);
    
    response = m_pImpl->responseData;
    
    if (res == CURLE_OK) {
        return FTP_SUCCESS;
    }
    
    return mapCurlError(res);
}

//----------------------------------------------------------------------------
CurlFtpClient::RetCodeEnum CurlFtpClient::setTransferMode(bool binaryMode)
{
    m_pImpl->binaryMode = binaryMode;
    return FTP_SUCCESS;
}

//----------------------------------------------------------------------------
CurlFtpClient::RetCodeEnum CurlFtpClient::setPassiveMode(bool passive)
{
    m_pImpl->passiveMode = passive;
    
    if (m_pImpl->curl) {
        curl_easy_setopt(m_pImpl->curl, CURLOPT_FTPPORT, passive ? nullptr : "-");
    }
    
    return FTP_SUCCESS;
}

//----------------------------------------------------------------------------
CurlFtpClient::RetCodeEnum CurlFtpClient::setTimeout(Int32 timeoutSeconds)
{
    m_pImpl->timeoutSeconds = timeoutSeconds;
    
    if (m_pImpl->curl) {
        curl_easy_setopt(m_pImpl->curl, CURLOPT_TIMEOUT, timeoutSeconds);
    }
    
    return FTP_SUCCESS;
}

//----------------------------------------------------------------------------
void CurlFtpClient::setProgressCallback(ProgressCallback callback)
{
    m_pImpl->progressCallback = callback;
}

//----------------------------------------------------------------------------
const VarString& CurlFtpClient::getLastError() const
{
    return m_pImpl->lastError;
}

//############################################################################
//   Private helper methods
//############################################################################

//----------------------------------------------------------------------------
CurlFtpClient::RetCodeEnum CurlFtpClient::mapCurlError(long curlCode)
{
    switch (curlCode) {
        case CURLE_OK:
            return FTP_SUCCESS;
        case CURLE_COULDNT_CONNECT:
        case CURLE_COULDNT_RESOLVE_HOST:
            updateLastError("Cannot connect to server");
            return FTP_OFFLINE;
        case CURLE_LOGIN_DENIED:
            updateLastError("Authentication failed");
            return FTP_AUTHERROR;
        case CURLE_REMOTE_FILE_NOT_FOUND:
            updateLastError("File not found");
            return FTP_FILENOTFOUND;
        case CURLE_UPLOAD_FAILED:
        case CURLE_REMOTE_ACCESS_DENIED:
            updateLastError("Permission denied");
            return FTP_PERMISSIONDENIED;
        case CURLE_OPERATION_TIMEDOUT:
            updateLastError("Operation timed out");
            return FTP_TIMEOUT;
        case CURLE_FTP_WEIRD_SERVER_REPLY:
        case CURLE_FTP_ACCEPT_FAILED:
            updateLastError("Protocol error");
            return FTP_PROTOCOL_ERROR;
        default:
            updateLastError("Unknown error");
            return FTP_GENERAL_ERROR;
    }
}

//----------------------------------------------------------------------------
void CurlFtpClient::updateLastError(const VarString& error)
{
    m_pImpl->lastError = error;
}

//----------------------------------------------------------------------------
VarString CurlFtpClient::buildFtpUrl(const VarString& path) const
{
    VarString url;
    url.format("ftp://%s:%d%s", m_pImpl->hostname.c_str(), m_pImpl->port, path.c_str());
    return url;
}

//############################################################################
//   Factory function
//############################################################################

//----------------------------------------------------------------------------
std::unique_ptr<IFtpClient> createFtpClient()
{
    return std::make_unique<CurlFtpClient>();
}

//-----------------------------------------------------------------------------
}       // namespace cmnutil
}       // namespace basar