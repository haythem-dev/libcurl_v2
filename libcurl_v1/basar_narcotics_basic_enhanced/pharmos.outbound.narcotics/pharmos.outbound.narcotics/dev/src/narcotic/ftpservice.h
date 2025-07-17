#ifndef INFRASTRUCTURE_FTPSERVICE_H
#define INFRASTRUCTURE_FTPSERVICE_H

//--------------------------------------------------------------------------------------------//
// includes
//--------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "iftpservice.h"

//--------------------------------------------------------------------------------------------//
// namespace
//--------------------------------------------------------------------------------------------//
namespace infrastructure
{
	//-------------------------------------------------------------------
    struct FtpParameters
    {
        basar::VarString hostname;
	    basar::VarString username; 
	    basar::VarString password;
	    basar::VarString saveDirectory;
	    basar::VarString localTmp;
	    basar::Int32 port;
    };

	//-------------------------------------------------------------------
    class FtpService : public IFtpService
    {
        public:
            
            FtpService( basar::Int32 branchBgaNo, basar::VarString branchStartDate, const FtpParameters& params );
            ~FtpService();

            virtual bool doesFileExist( basar::Int32 contributionVoucherNo );
            virtual bool storeFile( basar::cmnutil::IPrintDataProvider& printData, basar::Int32 contributionVoucherNo );

            void shutdown();

        private:

            basar::Int32        m_BranchBgaNo;
            basar::VarString    m_BranchStartDate;
            FtpParameters       m_FtpParameters;

    };

} //namespace infrastructure

#endif //INFRASTRUCTURE_FTPSERVICE_H