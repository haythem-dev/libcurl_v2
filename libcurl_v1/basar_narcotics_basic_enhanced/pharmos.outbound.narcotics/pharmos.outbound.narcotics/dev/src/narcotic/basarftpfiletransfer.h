#ifndef BASARFTPFILETRANSFER_H_GUARD
#define BASARFTPFILETRANSFER_H_GUARD

//-------------------------------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "contributionvoucherprintdata.h"
#include "ftpexception.h"
#include <libbasarcmnutil_ftpclient.h>
#include <memory>

//-------------------------------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------------------------------//
namespace narcotics
{

//----------------------------------------------------------------------------
class BasarFtpFileTransfer
{
public:
    enum FileCreationBehaviour {Overwrite, Preserve};

    BasarFtpFileTransfer(const basar::VarString& hostname,
                         const basar::VarString& username, 
                         const basar::VarString& password,
                         const basar::VarString& saveDirectory,
                         const basar::VarString& localTmp,
                         const basar::Int32 port = 21);

    virtual ~BasarFtpFileTransfer();

    virtual bool storeFile(basar::cmnutil::IPrintDataProvider &printData, 
                          const basar::Int32 senderbgano, 
                          const basar::Int32 contributionvoucherno,
                          const basar::VarString startDate,
                          const FileCreationBehaviour& fileCreationBehaviour = Preserve) = 0;  

    virtual bool cancelOrder(const basar::Int32 bgano, 
                            const basar::Int32 contributionvoucherno,
                            const basar::VarString startDate) = 0;  

    virtual void deleteFile(const basar::VarString& filePath);

    virtual bool listDirectory(const basar::VarString& dir, std::vector<basar::cmnutil::FtpFileInfo>& entries); 

    virtual bool doesDirectoryExist(const basar::VarString& searchedDir); 

protected:

    virtual basar::VarString adaptDirectoryPath(const basar::VarString& dir);

    virtual basar::VarString generateFileName(const basar::VarString& directoryName, 
                                             const basar::Int32& contributionVoucherNo,
                                             const FileCreationBehaviour& fileCreationBehaviour = Preserve) = 0;

    virtual basar::VarString generateDirectory(const basar::Int32 bgano, 
                                              const basar::VarString startDate) = 0;

    virtual basar::VarString generateFilePath(const basar::Int32 bgano, 
                                             const basar::Int32 contributionvoucherno,
                                             const basar::VarString startDate,
                                             const FileCreationBehaviour& fileCreationBehaviour = Preserve) = 0;

    virtual void setPermissions(const basar::VarString& permission, const basar::VarString& path);

    virtual void checkPrerequisites();

    virtual bool isValidPath(const basar::VarString& path);               

    virtual bool createDirectory(const basar::VarString& dir);

    virtual bool establishFTPConnection();

    virtual bool releaseFTPConnection();

    virtual bool splitDir(const basar::VarString& dir, std::vector<basar::VarString>& parts);

    virtual bool doesDirectoryExist(const basar::VarString& currentDir, const basar::VarString& searchedDir);     

    // Error handling
    virtual void handleFtpError(basar::cmnutil::IFtpClient::RetCodeEnum errorCode, const basar::VarString& context);

protected:
    std::unique_ptr<basar::cmnutil::IFtpClient> m_ftpClient;

    basar::VarString m_userName;
    basar::VarString m_password;
    basar::VarString m_hostname;
    basar::VarString m_localTmp;
    basar::VarString m_saveDirectory;
    basar::Int32 m_port;

    bool m_ftpErrorOccurred;
    basar::VarString m_lastErrorMessage;
};

} //namespace narcotics

#endif //BASARFTPFILETRANSFER_H_GUARD