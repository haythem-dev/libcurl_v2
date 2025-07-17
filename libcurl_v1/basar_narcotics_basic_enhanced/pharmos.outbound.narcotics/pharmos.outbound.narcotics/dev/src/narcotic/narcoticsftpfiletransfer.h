#ifndef NARCOTICSFTPFILETRANSFER_H_GUARD
#define NARCOTICSFTPFILETRANSFER_H_GUARD

//-------------------------------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "basarftpfiletransfer.h"
#include "contributionvoucherprintdata.h"

//-------------------------------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------------------------------//
namespace narcotics
{

//----------------------------------------------------------------------------
class NarcoticsFtpFileTransfer : public BasarFtpFileTransfer
{
public:
    NarcoticsFtpFileTransfer(const basar::VarString& hostname,
                            const basar::VarString& username, 
                            const basar::VarString& password,
                            const basar::VarString& saveDirectory,
                            const basar::VarString& localTmp,
                            const basar::Int32 port = 21);
                                                                
    virtual ~NarcoticsFtpFileTransfer();
    
    virtual bool storeFile(basar::cmnutil::IPrintDataProvider &printData, 
                          const basar::Int32 senderbgano, 
                          const basar::Int32 contributionvoucherno,
                          const basar::VarString startDate,
                          const FileCreationBehaviour& fileCreationBehaviour = Preserve) override;  
    
    virtual bool cancelOrder(const basar::Int32 bgano, 
                            const basar::Int32 contributionvoucherno,
                            const basar::VarString startDate) override;  
                                                    
protected:
    
    virtual basar::VarString generateFileName(const basar::VarString& directoryName, 
                                             const basar::Int32& contributionVoucherNo,
                                             const FileCreationBehaviour& fileCreationBehaviour = Preserve) override;
                                                                            
    virtual basar::VarString generateDirectory(const basar::Int32 bgano, 
                                              const basar::VarString startDate) override;
    
    virtual basar::VarString generateFilePath(const basar::Int32 bgano, 
                                             const basar::Int32 contributionvoucherno,
                                             const basar::VarString startDate,
                                             const FileCreationBehaviour& fileCreationBehaviour = Preserve) override;
    
    virtual basar::VarString generateLocalTmpFile(const basar::VarString& remotePath);
    
    virtual bool uploadFileToServer(const basar::VarString& localFile, const basar::VarString& remoteFile);
    
    virtual bool downloadFileFromServer(const basar::VarString& remoteFile, const basar::VarString& localFile);
    
    virtual bool createRemoteDirectoryStructure(const basar::VarString& remotePath);
    
    virtual void cleanupLocalTmpFile(const basar::VarString& localFile);
    
    virtual bool validateFileUpload(const basar::VarString& remoteFile);
    
    virtual basar::VarString formatDate(const basar::VarString& inputDate);
    
    virtual basar::VarString extractDirectoryFromPath(const basar::VarString& fullPath);
    
    virtual basar::VarString extractFilenameFromPath(const basar::VarString& fullPath);
    
    virtual bool isValidContributionVoucherNo(const basar::Int32 contributionVoucherNo);
    
    virtual bool isValidBgaNo(const basar::Int32 bgano);
    
private:
    // Progress callback for file transfers
    int progressCallback(double totalBytes, double nowBytes, double totalUpload, double nowUpload);
    
    // Internal state
    bool m_transferInProgress;
    double m_lastProgress;
    basar::VarString m_currentOperation;
};

} //namespace narcotics

#endif //NARCOTICSFTPFILETRANSFER_H_GUARD