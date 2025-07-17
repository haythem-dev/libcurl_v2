//--------------------------------------------------------------------------------------------//
// includes
//--------------------------------------------------------------------------------------------//
#include "commonheader.h"

#include "ftpservice.h"
#include "contributionvoucherftptransfer.h"

//--------------------------------------------------------------------------------------------//
// namespaces
//--------------------------------------------------------------------------------------------//
namespace infrastructure
{
	//-------------------------------------------------------------------
    FtpService::FtpService( basar::Int32 branchBgaNo, basar::VarString branchStartDate, const FtpParameters& params ) 
		: m_BranchBgaNo( branchBgaNo ), m_BranchStartDate( branchStartDate ), m_FtpParameters( params )
    {
    }

	//-------------------------------------------------------------------
    FtpService::~FtpService()
    {
    }

	//-------------------------------------------------------------------
    bool FtpService::doesFileExist( basar::Int32 contributionVoucherNo )
    {      
        narcotics::ContributionVoucherFTPTransfer ftp( m_FtpParameters.hostname, m_FtpParameters.username, m_FtpParameters.password, m_FtpParameters.saveDirectory, m_FtpParameters.localTmp );

        return ftp.checkFileExists(m_BranchBgaNo, contributionVoucherNo, m_BranchStartDate);
    }
  
	//-------------------------------------------------------------------  
    bool FtpService::storeFile( basar::cmnutil::IPrintDataProvider& printData, basar::Int32 contributionVoucherNo )
    {
        narcotics::ContributionVoucherFTPTransfer ftp( m_FtpParameters.hostname, m_FtpParameters.username, m_FtpParameters.password, m_FtpParameters.saveDirectory, m_FtpParameters.localTmp );

        return ftp.storeFile( printData, m_BranchBgaNo, contributionVoucherNo, m_BranchStartDate );
    }

	//-------------------------------------------------------------------
    void FtpService::shutdown()
    {
        //nothing to be done
    }

} //namespace infrastructure