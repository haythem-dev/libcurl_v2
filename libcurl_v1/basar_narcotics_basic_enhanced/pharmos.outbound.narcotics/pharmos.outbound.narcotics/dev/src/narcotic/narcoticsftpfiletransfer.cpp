//-------------------------------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "narcoticsftpfiletransfer.h"
#include "loggerpool.h"
#include "ftpexception.h"
#include <fstream>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <functional>

//-------------------------------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------------------------------//
namespace narcotics
{

//----------------------------------------------------------------------------
NarcoticsFtpFileTransfer::NarcoticsFtpFileTransfer(const basar::VarString& hostname,
                                                   const basar::VarString& username, 
                                                   const basar::VarString& password,
                                                   const basar::VarString& saveDirectory,
                                                   const basar::VarString& localTmp,
                                                   const basar::Int32 port)
    : BasarFtpFileTransfer(hostname, username, password, saveDirectory, localTmp, port)
    , m_transferInProgress(false)
    , m_lastProgress(0.0)
    , m_currentOperation("idle")
{
    BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerNarcotics, "NarcoticsFtpFileTransfer::NarcoticsFtpFileTransfer()");
    
    // Set up progress callback
    auto progressFunc = [this](double totalBytes, double nowBytes, double totalUpload, double nowUpload) -> int {
        return this->progressCallback(totalBytes, nowBytes, totalUpload, nowUpload);
    };
    
    m_ftpClient->setProgressCallback(progressFunc);
    
    BLOG_TRACE(narcotics::LoggerPool::loggerNarcotics, "NarcoticsFtpFileTransfer initialized successfully");
}

//----------------------------------------------------------------------------
NarcoticsFtpFileTransfer::~NarcoticsFtpFileTransfer()
{
    BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerNarcotics, "NarcoticsFtpFileTransfer::~NarcoticsFtpFileTransfer()");
}

//----------------------------------------------------------------------------
bool NarcoticsFtpFileTransfer::storeFile(basar::cmnutil::IPrintDataProvider &printData, 
                                         const basar::Int32 senderbgano, 
                                         const basar::Int32 contributionvoucherno,
                                         const basar::VarString startDate,
                                         const FileCreationBehaviour& fileCreationBehaviour)
{
    BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerNarcotics, "NarcoticsFtpFileTransfer::storeFile()");
    
    try {
        // Validate input parameters
        if (!isValidBgaNo(senderbgano)) {
            BLOG_ERROR(LoggerPool::loggerNarcotics, "Invalid BGA number: " + basar::VarString::valueOf(senderbgano));
            return false;
        }
        
        if (!isValidContributionVoucherNo(contributionvoucherno)) {
            BLOG_ERROR(LoggerPool::loggerNarcotics, "Invalid contribution voucher number: " + basar::VarString::valueOf(contributionvoucherno));
            return false;
        }
        
        if (startDate.empty()) {
            BLOG_ERROR(LoggerPool::loggerNarcotics, "Start date is empty");
            return false;
        }
        
        // Check prerequisites
        checkPrerequisites();
        
        // Establish FTP connection
        if (!establishFTPConnection()) {
            BLOG_ERROR(LoggerPool::loggerNarcotics, "Failed to establish FTP connection");
            return false;
        }
        
        // Generate remote file path
        basar::VarString remoteFilePath = generateFilePath(senderbgano, contributionvoucherno, startDate, fileCreationBehaviour);
        
        BLOG_TRACE(LoggerPool::loggerNarcotics, "Generated remote file path: " + remoteFilePath);
        
        // Create directory structure on server
        basar::VarString remoteDir = extractDirectoryFromPath(remoteFilePath);
        if (!createRemoteDirectoryStructure(remoteDir)) {
            BLOG_ERROR(LoggerPool::loggerNarcotics, "Failed to create remote directory structure: " + remoteDir);
            return false;
        }
        
        // Generate local temporary file
        basar::VarString localTmpFile = generateLocalTmpFile(remoteFilePath);
        
        BLOG_TRACE(LoggerPool::loggerNarcotics, "Generated local temp file: " + localTmpFile);
        
        // Generate print data and save to local file
        try {
            std::ofstream outFile(localTmpFile.c_str(), std::ios::binary);
            if (!outFile.is_open()) {
                BLOG_ERROR(LoggerPool::loggerNarcotics, "Cannot create local temporary file: " + localTmpFile);
                return false;
            }
            
            // Get print data from provider
            basar::VarString printDataContent = printData.getPrintData();
            outFile.write(printDataContent.c_str(), printDataContent.length());
            outFile.close();
            
            BLOG_TRACE(LoggerPool::loggerNarcotics, "Print data written to local file");
            
        } catch (const std::exception& e) {
            BLOG_ERROR(LoggerPool::loggerNarcotics, "Error writing print data to local file: " + basar::VarString(e.what()));
            cleanupLocalTmpFile(localTmpFile);
            return false;
        }
        
        // Upload file to server
        m_currentOperation = "uploading";
        m_transferInProgress = true;
        
        bool uploadSuccess = uploadFileToServer(localTmpFile, remoteFilePath);
        
        m_transferInProgress = false;
        m_currentOperation = "idle";
        
        if (!uploadSuccess) {
            BLOG_ERROR(LoggerPool::loggerNarcotics, "Failed to upload file to server");
            cleanupLocalTmpFile(localTmpFile);
            return false;
        }
        
        // Validate upload
        if (!validateFileUpload(remoteFilePath)) {
            BLOG_WARNING(LoggerPool::loggerNarcotics, "File upload validation failed");
        }
        
        // Set permissions if needed
        setPermissions("644", remoteFilePath);
        
        // Cleanup local temporary file
        cleanupLocalTmpFile(localTmpFile);
        
        BLOG_TRACE(LoggerPool::loggerNarcotics, "File stored successfully: " + remoteFilePath);
        
        return true;
        
    } catch (const exception::FTPException& e) {
        BLOG_ERROR(LoggerPool::loggerNarcotics, "FTP Exception in storeFile: " + basar::VarString(e.what()));
        m_transferInProgress = false;
        return false;
    } catch (const basar::Exception& e) {
        BLOG_ERROR(LoggerPool::loggerNarcotics, "BASAR Exception in storeFile: " + basar::VarString(e.what()));
        m_transferInProgress = false;
        return false;
    } catch (const std::exception& e) {
        BLOG_ERROR(LoggerPool::loggerNarcotics, "Standard Exception in storeFile: " + basar::VarString(e.what()));
        m_transferInProgress = false;
        return false;
    }
}

//----------------------------------------------------------------------------
bool NarcoticsFtpFileTransfer::cancelOrder(const basar::Int32 bgano, 
                                           const basar::Int32 contributionvoucherno,
                                           const basar::VarString startDate)
{
    BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerNarcotics, "NarcoticsFtpFileTransfer::cancelOrder()");
    
    try {
        // Validate input parameters
        if (!isValidBgaNo(bgano)) {
            BLOG_ERROR(LoggerPool::loggerNarcotics, "Invalid BGA number: " + basar::VarString::valueOf(bgano));
            return false;
        }
        
        if (!isValidContributionVoucherNo(contributionvoucherno)) {
            BLOG_ERROR(LoggerPool::loggerNarcotics, "Invalid contribution voucher number: " + basar::VarString::valueOf(contributionvoucherno));
            return false;
        }
        
        // Check prerequisites
        checkPrerequisites();
        
        // Establish FTP connection
        if (!establishFTPConnection()) {
            BLOG_ERROR(LoggerPool::loggerNarcotics, "Failed to establish FTP connection");
            return false;
        }
        
        // Generate file path for both creation behaviours
        basar::VarString filePathPreserve = generateFilePath(bgano, contributionvoucherno, startDate, Preserve);
        basar::VarString filePathOverwrite = generateFilePath(bgano, contributionvoucherno, startDate, Overwrite);
        
        bool fileDeleted = false;
        
        // Try to delete the file with Preserve behaviour first
        try {
            deleteFile(filePathPreserve);
            fileDeleted = true;
            BLOG_TRACE(LoggerPool::loggerNarcotics, "Deleted file (Preserve): " + filePathPreserve);
        } catch (const exception::FTPException& e) {
            BLOG_TRACE(LoggerPool::loggerNarcotics, "File not found (Preserve): " + filePathPreserve);
        }
        
        // Try to delete the file with Overwrite behaviour if not found
        if (!fileDeleted) {
            try {
                deleteFile(filePathOverwrite);
                fileDeleted = true;
                BLOG_TRACE(LoggerPool::loggerNarcotics, "Deleted file (Overwrite): " + filePathOverwrite);
            } catch (const exception::FTPException& e) {
                BLOG_TRACE(LoggerPool::loggerNarcotics, "File not found (Overwrite): " + filePathOverwrite);
            }
        }
        
        if (fileDeleted) {
            BLOG_TRACE(LoggerPool::loggerNarcotics, "Order cancelled successfully");
            return true;
        } else {
            BLOG_WARNING(LoggerPool::loggerNarcotics, "No files found to cancel for order");
            return false;
        }
        
    } catch (const exception::FTPException& e) {
        BLOG_ERROR(LoggerPool::loggerNarcotics, "FTP Exception in cancelOrder: " + basar::VarString(e.what()));
        return false;
    } catch (const basar::Exception& e) {
        BLOG_ERROR(LoggerPool::loggerNarcotics, "BASAR Exception in cancelOrder: " + basar::VarString(e.what()));
        return false;
    } catch (const std::exception& e) {
        BLOG_ERROR(LoggerPool::loggerNarcotics, "Standard Exception in cancelOrder: " + basar::VarString(e.what()));
        return false;
    }
}

//----------------------------------------------------------------------------
basar::VarString NarcoticsFtpFileTransfer::generateFileName(const basar::VarString& directoryName, 
                                                           const basar::Int32& contributionVoucherNo,
                                                           const FileCreationBehaviour& fileCreationBehaviour)
{
    BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerNarcotics, "NarcoticsFtpFileTransfer::generateFileName()");
    
    basar::VarString filename;
    
    // Base filename format: contributionvoucherno.xml
    filename.format("%d.xml", contributionVoucherNo);
    
    // Handle file creation behavior
    if (fileCreationBehaviour == Preserve) {
        // Check if file already exists and add suffix if necessary
        basar::VarString fullPath = directoryName + "/" + filename;
        
        if (m_ftpClient && m_ftpClient->isConnected()) {
            basar::BULong fileSize;
            basar::cmnutil::IFtpClient::RetCodeEnum result = m_ftpClient->getFileSize(fullPath, fileSize);
            
            if (result == basar::cmnutil::IFtpClient::FTP_SUCCESS) {
                // File exists, add timestamp suffix
                std::time_t now = std::time(nullptr);
                std::stringstream ss;
                ss << contributionVoucherNo << "_" << now << ".xml";
                filename = ss.str();
            }
        }
    }
    
    BLOG_TRACE(LoggerPool::loggerNarcotics, "Generated filename: " + filename);
    return filename;
}

//----------------------------------------------------------------------------
basar::VarString NarcoticsFtpFileTransfer::generateDirectory(const basar::Int32 bgano, 
                                                            const basar::VarString startDate)
{
    BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerNarcotics, "NarcoticsFtpFileTransfer::generateDirectory()");
    
    basar::VarString directory;
    basar::VarString formattedDate = formatDate(startDate);
    
    // Format: /bgano/YYYY/MM/DD/
    directory.format("/%d/%s", bgano, formattedDate.c_str());
    
    BLOG_TRACE(LoggerPool::loggerNarcotics, "Generated directory: " + directory);
    return directory;
}

//----------------------------------------------------------------------------
basar::VarString NarcoticsFtpFileTransfer::generateFilePath(const basar::Int32 bgano, 
                                                           const basar::Int32 contributionvoucherno,
                                                           const basar::VarString startDate,
                                                           const FileCreationBehaviour& fileCreationBehaviour)
{
    BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerNarcotics, "NarcoticsFtpFileTransfer::generateFilePath()");
    
    basar::VarString directory = generateDirectory(bgano, startDate);
    basar::VarString filename = generateFileName(directory, contributionvoucherno, fileCreationBehaviour);
    
    basar::VarString filePath = directory + "/" + filename;
    
    BLOG_TRACE(LoggerPool::loggerNarcotics, "Generated file path: " + filePath);
    return filePath;
}

//----------------------------------------------------------------------------
basar::VarString NarcoticsFtpFileTransfer::generateLocalTmpFile(const basar::VarString& remotePath)
{
    BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerNarcotics, "NarcoticsFtpFileTransfer::generateLocalTmpFile()");
    
    basar::VarString filename = extractFilenameFromPath(remotePath);
    basar::VarString localTmpFile = m_localTmp + "/" + filename;
    
    BLOG_TRACE(LoggerPool::loggerNarcotics, "Generated local tmp file: " + localTmpFile);
    return localTmpFile;
}

//----------------------------------------------------------------------------
bool NarcoticsFtpFileTransfer::uploadFileToServer(const basar::VarString& localFile, const basar::VarString& remoteFile)
{
    BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerNarcotics, "NarcoticsFtpFileTransfer::uploadFileToServer()");
    
    if (!m_ftpClient || !m_ftpClient->isConnected()) {
        BLOG_ERROR(LoggerPool::loggerNarcotics, "FTP client not connected");
        return false;
    }
    
    basar::cmnutil::IFtpClient::RetCodeEnum result = m_ftpClient->uploadFile(localFile, remoteFile);
    
    if (result != basar::cmnutil::IFtpClient::FTP_SUCCESS) {
        handleFtpError(result, "uploadFile");
        return false;
    }
    
    BLOG_TRACE(LoggerPool::loggerNarcotics, "File uploaded successfully: " + remoteFile);
    return true;
}

//----------------------------------------------------------------------------
bool NarcoticsFtpFileTransfer::downloadFileFromServer(const basar::VarString& remoteFile, const basar::VarString& localFile)
{
    BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerNarcotics, "NarcoticsFtpFileTransfer::downloadFileFromServer()");
    
    if (!m_ftpClient || !m_ftpClient->isConnected()) {
        BLOG_ERROR(LoggerPool::loggerNarcotics, "FTP client not connected");
        return false;
    }
    
    basar::cmnutil::IFtpClient::RetCodeEnum result = m_ftpClient->downloadFile(remoteFile, localFile);
    
    if (result != basar::cmnutil::IFtpClient::FTP_SUCCESS) {
        handleFtpError(result, "downloadFile");
        return false;
    }
    
    BLOG_TRACE(LoggerPool::loggerNarcotics, "File downloaded successfully: " + localFile);
    return true;
}

//----------------------------------------------------------------------------
bool NarcoticsFtpFileTransfer::createRemoteDirectoryStructure(const basar::VarString& remotePath)
{
    BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerNarcotics, "NarcoticsFtpFileTransfer::createRemoteDirectoryStructure()");
    
    return createDirectory(remotePath);
}

//----------------------------------------------------------------------------
void NarcoticsFtpFileTransfer::cleanupLocalTmpFile(const basar::VarString& localFile)
{
    BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerNarcotics, "NarcoticsFtpFileTransfer::cleanupLocalTmpFile()");
    
    try {
        if (std::remove(localFile.c_str()) == 0) {
            BLOG_TRACE(LoggerPool::loggerNarcotics, "Local tmp file cleaned up: " + localFile);
        } else {
            BLOG_WARNING(LoggerPool::loggerNarcotics, "Failed to cleanup local tmp file: " + localFile);
        }
    } catch (const std::exception& e) {
        BLOG_WARNING(LoggerPool::loggerNarcotics, "Exception during cleanup: " + basar::VarString(e.what()));
    }
}

//----------------------------------------------------------------------------
bool NarcoticsFtpFileTransfer::validateFileUpload(const basar::VarString& remoteFile)
{
    BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerNarcotics, "NarcoticsFtpFileTransfer::validateFileUpload()");
    
    if (!m_ftpClient || !m_ftpClient->isConnected()) {
        BLOG_ERROR(LoggerPool::loggerNarcotics, "FTP client not connected");
        return false;
    }
    
    basar::BULong fileSize;
    basar::cmnutil::IFtpClient::RetCodeEnum result = m_ftpClient->getFileSize(remoteFile, fileSize);
    
    if (result == basar::cmnutil::IFtpClient::FTP_SUCCESS && fileSize > 0) {
        BLOG_TRACE(LoggerPool::loggerNarcotics, "File upload validated successfully. Size: " + basar::VarString::valueOf(fileSize));
        return true;
    }
    
    BLOG_WARNING(LoggerPool::loggerNarcotics, "File upload validation failed");
    return false;
}

//----------------------------------------------------------------------------
basar::VarString NarcoticsFtpFileTransfer::formatDate(const basar::VarString& inputDate)
{
    BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerNarcotics, "NarcoticsFtpFileTransfer::formatDate()");
    
    // Assuming input format is YYYY-MM-DD and we want YYYY/MM/DD
    basar::VarString formattedDate = inputDate;
    
    // Replace dashes with slashes
    size_t pos = 0;
    while ((pos = formattedDate.find('-', pos)) != basar::VarString::npos) {
        formattedDate.replace(pos, 1, "/");
        pos++;
    }
    
    BLOG_TRACE(LoggerPool::loggerNarcotics, "Formatted date: " + formattedDate);
    return formattedDate;
}

//----------------------------------------------------------------------------
basar::VarString NarcoticsFtpFileTransfer::extractDirectoryFromPath(const basar::VarString& fullPath)
{
    BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerNarcotics, "NarcoticsFtpFileTransfer::extractDirectoryFromPath()");
    
    size_t lastSlash = fullPath.find_last_of('/');
    if (lastSlash != basar::VarString::npos) {
        return fullPath.substr(0, lastSlash);
    }
    
    return "";
}

//----------------------------------------------------------------------------
basar::VarString NarcoticsFtpFileTransfer::extractFilenameFromPath(const basar::VarString& fullPath)
{
    BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerNarcotics, "NarcoticsFtpFileTransfer::extractFilenameFromPath()");
    
    size_t lastSlash = fullPath.find_last_of('/');
    if (lastSlash != basar::VarString::npos) {
        return fullPath.substr(lastSlash + 1);
    }
    
    return fullPath;
}

//----------------------------------------------------------------------------
bool NarcoticsFtpFileTransfer::isValidContributionVoucherNo(const basar::Int32 contributionVoucherNo)
{
    // Valid contribution voucher numbers are positive integers
    return contributionVoucherNo > 0;
}

//----------------------------------------------------------------------------
bool NarcoticsFtpFileTransfer::isValidBgaNo(const basar::Int32 bgano)
{
    // Valid BGA numbers are positive integers
    return bgano > 0;
}

//----------------------------------------------------------------------------
int NarcoticsFtpFileTransfer::progressCallback(double totalBytes, double nowBytes, double totalUpload, double nowUpload)
{
    if (m_transferInProgress) {
        double progress = 0.0;
        
        if (m_currentOperation == "uploading" && totalUpload > 0) {
            progress = (nowUpload / totalUpload) * 100.0;
        } else if (m_currentOperation == "downloading" && totalBytes > 0) {
            progress = (nowBytes / totalBytes) * 100.0;
        }
        
        // Only log progress at 10% intervals to avoid spam
        if (progress - m_lastProgress >= 10.0) {
            BLOG_TRACE(LoggerPool::loggerNarcotics, 
                      "Transfer progress (" + m_currentOperation + "): " + basar::VarString::valueOf(progress) + "%");
            m_lastProgress = progress;
        }
    }
    
    return 0; // Continue transfer
}

} //namespace narcotics