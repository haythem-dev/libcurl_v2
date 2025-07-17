//-------------------------------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "basarftpfiletransfer.h"
#include "loggerpool.h"
#include <algorithm>
#include <sstream>

//-------------------------------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------------------------------//
namespace narcotics
{

//----------------------------------------------------------------------------
BasarFtpFileTransfer::BasarFtpFileTransfer(const basar::VarString& hostname,
                                           const basar::VarString& username, 
                                           const basar::VarString& password,
                                           const basar::VarString& saveDirectory,
                                           const basar::VarString& localTmp,
                                           const basar::Int32 port)
    : m_ftpClient(basar::cmnutil::createFtpClient())
    , m_userName(username)
    , m_password(password)
    , m_hostname(hostname)
    , m_localTmp(localTmp)
    , m_saveDirectory(saveDirectory)
    , m_port(port)
    , m_ftpErrorOccurred(false)
{
    BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerNarcotics, "BasarFtpFileTransfer::BasarFtpFileTransfer()");
    
    if (!m_ftpClient) {
        BLOG_ERROR(LoggerPool::loggerNarcotics, "Failed to create FTP client");
        
        basar::cmnutil::ExceptionInfoStruct excInfo;
        excInfo.context = "BasarFtpFileTransfer::BasarFtpFileTransfer()";
        excInfo.file = __FILE__;
        excInfo.line = __LINE__;
        excInfo.reason = "Failed to create FTP client";
        
        throw basar::cmnutil::BasarRuntimeException(excInfo);
    }
    
    // Configure FTP client
    m_ftpClient->setTransferMode(true); // Binary mode
    m_ftpClient->setPassiveMode(true);
    m_ftpClient->setTimeout(30);
}

//----------------------------------------------------------------------------
BasarFtpFileTransfer::~BasarFtpFileTransfer()
{
    BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerNarcotics, "BasarFtpFileTransfer::~BasarFtpFileTransfer()");
    
    if (m_ftpClient && m_ftpClient->isConnected()) {
        m_ftpClient->disconnect();
    }
}

//----------------------------------------------------------------------------
void BasarFtpFileTransfer::deleteFile(const basar::VarString& filePath)
{
    BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerNarcotics, "BasarFtpFileTransfer::deleteFile()");
    
    if (!m_ftpClient || !m_ftpClient->isConnected()) {
        BLOG_ERROR(LoggerPool::loggerNarcotics, "FTP client not connected");
        
        basar::cmnutil::ExceptionInfoStruct excInfo;
        excInfo.context = "BasarFtpFileTransfer::deleteFile()";
        excInfo.file = __FILE__;
        excInfo.line = __LINE__;
        excInfo.reason = "FTP client not connected";
        
        throw exception::FTPException(excInfo);
    }
    
    basar::cmnutil::IFtpClient::RetCodeEnum result = m_ftpClient->deleteFile(filePath);
    
    if (result != basar::cmnutil::IFtpClient::FTP_SUCCESS) {
        handleFtpError(result, "deleteFile");
        
        basar::cmnutil::ExceptionInfoStruct excInfo;
        excInfo.context = "BasarFtpFileTransfer::deleteFile()";
        excInfo.file = __FILE__;
        excInfo.line = __LINE__;
        excInfo.reason = m_ftpClient->getLastError();
        
        throw exception::FTPException(excInfo);
    }
    
    BLOG_TRACE(LoggerPool::loggerUseCases, "basarftpfiletransfer: deleted file: " + filePath);
}

//----------------------------------------------------------------------------
bool BasarFtpFileTransfer::listDirectory(const basar::VarString& dir, std::vector<basar::cmnutil::FtpFileInfo>& entries)
{
    BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerNarcotics, "BasarFtpFileTransfer::listDirectory()");
    
    if (!m_ftpClient || !m_ftpClient->isConnected()) {
        BLOG_ERROR(LoggerPool::loggerNarcotics, "FTP client not connected");
        return false;
    }
    
    if (dir.empty()) {
        BLOG_ERROR(LoggerPool::loggerNarcotics, "Empty directory path");
        
        basar::cmnutil::ExceptionInfoStruct excInfo;
        excInfo.context = "BasarFtpFileTransfer::listDirectory()";
        excInfo.file = __FILE__;
        excInfo.line = __LINE__;
        excInfo.reason = "Empty directory path";
        
        throw basar::cmnutil::BasarInvalidParameterException(excInfo);
    }
    
    basar::VarString adaptedDir = adaptDirectoryPath(dir);
    
    basar::cmnutil::IFtpClient::RetCodeEnum result = m_ftpClient->listDirectory(adaptedDir, entries);
    
    if (result != basar::cmnutil::IFtpClient::FTP_SUCCESS) {
        handleFtpError(result, "listDirectory");
        return false;
    }
    
    return true;
}

//----------------------------------------------------------------------------
bool BasarFtpFileTransfer::doesDirectoryExist(const basar::VarString& searchedDir)
{
    BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerNarcotics, "BasarFtpFileTransfer::doesDirectoryExist()");
    
    if (!m_ftpClient || !m_ftpClient->isConnected()) {
        BLOG_ERROR(LoggerPool::loggerNarcotics, "FTP client not connected");
        return false;
    }
    
    basar::VarString adaptedDir = adaptDirectoryPath(searchedDir);
    
    // Try to change to the directory
    basar::cmnutil::IFtpClient::RetCodeEnum result = m_ftpClient->changeDirectory(adaptedDir);
    
    if (result == basar::cmnutil::IFtpClient::FTP_SUCCESS) {
        return true;
    }
    
    return false;
}

//----------------------------------------------------------------------------
basar::VarString BasarFtpFileTransfer::adaptDirectoryPath(const basar::VarString& dir)
{
    basar::VarString adaptedDir = dir;
    
    if (adaptedDir.empty()) {
        basar::cmnutil::ExceptionInfoStruct excInfo;
        excInfo.context = "BasarFtpFileTransfer::adaptDirectoryPath()";
        excInfo.file = __FILE__;
        excInfo.line = __LINE__;
        excInfo.reason = "Empty directory path";
        
        throw basar::cmnutil::BasarInvalidParameterException(excInfo);
    }
    
    // Replace backslashes with forward slashes
    std::replace(adaptedDir.begin(), adaptedDir.end(), '\\', '/');
    
    // Remove trailing slash if present
    if (adaptedDir.back() == '/') {
        adaptedDir.pop_back();
    }
    
    return adaptedDir;
}

//----------------------------------------------------------------------------
void BasarFtpFileTransfer::setPermissions(const basar::VarString& permission, const basar::VarString& path)
{
    BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerNarcotics, "BasarFtpFileTransfer::setPermissions()");
    
    if (!m_ftpClient || !m_ftpClient->isConnected()) {
        BLOG_ERROR(LoggerPool::loggerNarcotics, "FTP client not connected");
        return;
    }
    
    basar::cmnutil::IFtpClient::RetCodeEnum result = m_ftpClient->setPermissions(path, permission);
    
    if (result != basar::cmnutil::IFtpClient::FTP_SUCCESS) {
        handleFtpError(result, "setPermissions");
        BLOG_WARNING(LoggerPool::loggerNarcotics, "Failed to set permissions: " + m_ftpClient->getLastError());
    }
}

//----------------------------------------------------------------------------
void BasarFtpFileTransfer::checkPrerequisites()
{
    BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerNarcotics, "BasarFtpFileTransfer::checkPrerequisites()");
    
    if (m_hostname.empty()) {
        basar::cmnutil::ExceptionInfoStruct excInfo;
        excInfo.context = "BasarFtpFileTransfer::checkPrerequisites()";
        excInfo.file = __FILE__;
        excInfo.line = __LINE__;
        excInfo.reason = "Hostname is empty";
        
        throw basar::cmnutil::BasarInvalidParameterException(excInfo);
    }
    
    if (m_userName.empty()) {
        basar::cmnutil::ExceptionInfoStruct excInfo;
        excInfo.context = "BasarFtpFileTransfer::checkPrerequisites()";
        excInfo.file = __FILE__;
        excInfo.line = __LINE__;
        excInfo.reason = "Username is empty";
        
        throw basar::cmnutil::BasarInvalidParameterException(excInfo);
    }
}

//----------------------------------------------------------------------------
bool BasarFtpFileTransfer::isValidPath(const basar::VarString& path)
{
    // Simple regex-like validation
    if (path.empty()) {
        return false;
    }
    
    // Check for invalid characters
    const char* invalidChars = "<>:\"|?*";
    for (const char* c = invalidChars; *c; ++c) {
        if (path.find(*c) != basar::VarString::npos) {
            return false;
        }
    }
    
    return true;
}

//----------------------------------------------------------------------------
bool BasarFtpFileTransfer::createDirectory(const basar::VarString& dir)
{
    BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerNarcotics, "BasarFtpFileTransfer::createDirectory()");
    
    if (!m_ftpClient || !m_ftpClient->isConnected()) {
        BLOG_ERROR(LoggerPool::loggerNarcotics, "FTP client not connected");
        return false;
    }
    
    basar::VarString adaptedDir = adaptDirectoryPath(dir);
    
    // Split path into components
    std::vector<basar::VarString> pathParts;
    if (!splitDir(adaptedDir, pathParts)) {
        return false;
    }
    
    basar::VarString currentPath = "";
    
    for (size_t i = 0; i < pathParts.size(); ++i) {
        if (i > 0) {
            currentPath += "/";
        }
        currentPath += pathParts[i];
        
        if (!doesDirectoryExist(currentPath)) {
            basar::cmnutil::IFtpClient::RetCodeEnum result = m_ftpClient->createDirectory(currentPath);
            
            if (result != basar::cmnutil::IFtpClient::FTP_SUCCESS) {
                handleFtpError(result, "createDirectory");
                return false;
            }
        }
    }
    
    return true;
}

//----------------------------------------------------------------------------
bool BasarFtpFileTransfer::establishFTPConnection()
{
    BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerNarcotics, "BasarFtpFileTransfer::establishFTPConnection()");
    BLOG_TRACE(narcotics::LoggerPool::loggerNarcotics, "Establishing FTP connection");
    
    if (!m_ftpClient) {
        BLOG_ERROR(LoggerPool::loggerNarcotics, "FTP client not initialized");
        return false;
    }
    
    if (m_ftpClient->isConnected()) {
        BLOG_TRACE(narcotics::LoggerPool::loggerNarcotics, "Already connected");
        return true;
    }
    
    // Connect to server
    basar::cmnutil::IFtpClient::RetCodeEnum result = m_ftpClient->connect(m_hostname, m_port);
    
    if (result != basar::cmnutil::IFtpClient::FTP_SUCCESS) {
        handleFtpError(result, "connect");
        
        basar::cmnutil::ExceptionInfoStruct excInfo;
        excInfo.context = "BasarFtpFileTransfer::establishFTPConnection()";
        excInfo.file = __FILE__;
        excInfo.line = __LINE__;
        excInfo.reason = "Failed to connect to server: " + m_ftpClient->getLastError();
        
        throw basar::cmnutil::BasarRuntimeException(excInfo);
    }
    
    // Login
    result = m_ftpClient->login(m_userName, m_password);
    
    if (result != basar::cmnutil::IFtpClient::FTP_SUCCESS) {
        handleFtpError(result, "login");
        
        basar::cmnutil::ExceptionInfoStruct excInfo;
        excInfo.context = "BasarFtpFileTransfer::establishFTPConnection()";
        excInfo.file = __FILE__;
        excInfo.line = __LINE__;
        excInfo.reason = "Failed to login: " + m_ftpClient->getLastError();
        
        throw basar::cmnutil::BasarRuntimeException(excInfo);
    }
    
    // Change to save directory if specified
    if (!m_saveDirectory.empty()) {
        result = m_ftpClient->changeDirectory(m_saveDirectory);
        
        if (result != basar::cmnutil::IFtpClient::FTP_SUCCESS) {
            handleFtpError(result, "changeDirectory");
            BLOG_WARNING(LoggerPool::loggerNarcotics, "Failed to change to save directory: " + m_saveDirectory);
        }
    }
    
    BLOG_TRACE(narcotics::LoggerPool::loggerNarcotics, "FTP connection established successfully");
    return true;
}

//----------------------------------------------------------------------------
bool BasarFtpFileTransfer::releaseFTPConnection()
{
    BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerNarcotics, "BasarFtpFileTransfer::releaseFTPConnection()");
    BLOG_TRACE(narcotics::LoggerPool::loggerNarcotics, "Closing FTP connection");
    
    if (!m_ftpClient) {
        return false;
    }
    
    if (m_ftpClient->isConnected()) {
        m_ftpClient->disconnect();
        return true;
    }
    
    return false;
}

//----------------------------------------------------------------------------
bool BasarFtpFileTransfer::splitDir(const basar::VarString& dir, std::vector<basar::VarString>& parts)
{
    parts.clear();
    
    if (dir.empty()) {
        return false;
    }
    
    std::stringstream ss(dir.c_str());
    basar::VarString item;
    
    while (std::getline(ss, item, '/')) {
        if (!item.empty()) {
            parts.push_back(item);
        }
    }
    
    return !parts.empty();
}

//----------------------------------------------------------------------------
bool BasarFtpFileTransfer::doesDirectoryExist(const basar::VarString& currentDir, const basar::VarString& searchedDir)
{
    BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerNarcotics, "BasarFtpFileTransfer::doesDirectoryExist(currentDir, searchedDir)");
    
    if (!m_ftpClient || !m_ftpClient->isConnected()) {
        return false;
    }
    
    std::vector<basar::cmnutil::FtpFileInfo> entries;
    
    basar::cmnutil::IFtpClient::RetCodeEnum result = m_ftpClient->listDirectory(currentDir, entries);
    
    if (result != basar::cmnutil::IFtpClient::FTP_SUCCESS) {
        return false;
    }
    
    // Search for the directory in the listing
    for (const auto& entry : entries) {
        if (entry.isDirectory && entry.name == searchedDir) {
            return true;
        }
    }
    
    return false;
}

//----------------------------------------------------------------------------
void BasarFtpFileTransfer::handleFtpError(basar::cmnutil::IFtpClient::RetCodeEnum errorCode, const basar::VarString& context)
{
    m_ftpErrorOccurred = true;
    
    basar::VarString errorMsg;
    switch (errorCode) {
        case basar::cmnutil::IFtpClient::FTP_OFFLINE:
            errorMsg = "Server offline or unreachable";
            break;
        case basar::cmnutil::IFtpClient::FTP_CONNREFUSED:
            errorMsg = "Connection refused by server";
            break;
        case basar::cmnutil::IFtpClient::FTP_AUTHERROR:
            errorMsg = "Authentication failed";
            break;
        case basar::cmnutil::IFtpClient::FTP_FILENOTFOUND:
            errorMsg = "File or directory not found";
            break;
        case basar::cmnutil::IFtpClient::FTP_PERMISSIONDENIED:
            errorMsg = "Permission denied";
            break;
        case basar::cmnutil::IFtpClient::FTP_TIMEOUT:
            errorMsg = "Operation timed out";
            break;
        case basar::cmnutil::IFtpClient::FTP_PROTOCOL_ERROR:
            errorMsg = "Protocol error";
            break;
        default:
            errorMsg = "Unknown FTP error";
            break;
    }
    
    m_lastErrorMessage = errorMsg;
    
    BLOG_ERROR(LoggerPool::loggerNarcotics, "FTP Error in " + context + ": " + errorMsg);
    
    // Get detailed error from client
    if (m_ftpClient) {
        basar::VarString detailedError = m_ftpClient->getLastError();
        if (!detailedError.empty()) {
            BLOG_ERROR(LoggerPool::loggerNarcotics, "Detailed error: " + detailedError);
        }
    }
}

} //namespace narcotics