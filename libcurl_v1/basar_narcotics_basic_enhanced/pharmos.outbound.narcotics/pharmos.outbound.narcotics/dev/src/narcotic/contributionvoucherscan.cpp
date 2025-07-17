//-----------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  scans (= marks as received) contributionvouchers
 *  \author Marco Köppendörfer
 *  \date   04.01.2013
 */
//-----------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "contributionvoucherscan.h"
#include "loggerpool.h"

//exceptions
#include "missingdependencyinjectionexception.h"

//-------------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------------//
namespace narcotics
{
namespace useCase
{
ContributionVoucherScan::ContributionVoucherScan( basar::Int16 branchNo ) 
: m_BranchNo( branchNo )
{
}

ContributionVoucherScan::~ContributionVoucherScan()
{
}

//-------------------------------------------------------------------------------------------------//
// macro for registrating and deregistrating EventHandlers!
// 1st para: name of UC
// 2nd para: name of handle-Method
BEGIN_HANDLERS_REGISTRATION( ContributionVoucherScan )
	SYSTEM_EVENT_HANDLER_REGISTRATION( ContributionVoucherScan, MarkScannedContributionVoucherAsReceived )
END_HANDLERS_REGISTRATION()

//-------------------------------------------------------------------------------------------------//
basar::appl::EventReturnType ContributionVoucherScan::run()
{
    checkInjections();
    
    registerEventHandlers(true);
    m_ContributionVoucherScanVC->init();
    m_ContributionVoucherScanVC->show();            
    registerEventHandlers(false);

    return basar::appl::EventReturnType();
}

//-------------------------------------------------------------------------------------------------//
void ContributionVoucherScan::injectParentWindow( QWidget * mw )
{
	m_ContributionVoucherScanVC.setParent(mw);	
}

//-------------------------------------------------------------------------------------------------//
void ContributionVoucherScan::injectContributionVoucherDM( boost::shared_ptr<domMod::ContributionVoucherDM> contributionVoucherDM )
{
    m_ContributionVoucherDM = contributionVoucherDM;
}

//-------------------------------------------------------------------------------------------------//
void ContributionVoucherScan::checkInjections()
{
    if( 0 == m_ContributionVoucherDM.get() ){
        throw infrastructure::MissingDependencyInjectionException( basar::ExceptInfo("ContributionVoucherScan::checkInjections", "ContributionVoucherDM missing! Inject ContributionVoucherDM first!", __FILE__, __LINE__) );
    }
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
SYSTEM_EVENT_HANDLER_DEFINITION( ContributionVoucherScan, MarkScannedContributionVoucherAsReceived )
{
    rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings
    
    basar::appl::EventReturnType ret;

    //get text message
    basar::VarString scannedContributionVoucherNo = m_ContributionVoucherScanVC->getScannedContributionVoucherNo();

    constants::ContributionVoucherScanResults scanResults;

    try
    {
        scanResults = m_ContributionVoucherDM->markContributionVoucherAsReceived( scannedContributionVoucherNo );
    }
    catch( const basar::Exception& e )
    {
        ret.ret = basar::appl::HANDLER_ERROR;
        ret.message = e.what();
        return ret;
    }

    if( constants::MARKED_AS_RECEIVED == scanResults )
    {
        //success - scanned number was found in database and set to state received
        ret.ret = basar::appl::HANDLER_OK;
		ret.message.format(QCoreApplication::translate("narcotics::useCase::ContributionVoucherScan","Scanned ContributionVoucher %s").toLocal8Bit().constData(),
			scannedContributionVoucherNo.substr(8, 8).c_str());
		BLOG_INFO(LoggerPool::loggerUseCases, ret.message);
    }
    else
    {
        if( constants::VOUCHER_ALREADY_MARKED == scanResults )
        {
            ret.ret = basar::appl::HANDLER_INFO;
            ret.message.format(QCoreApplication::translate("narcotics::useCase::ContributionVoucherScan","ContributionVoucher %s already scanned").toLocal8Bit().constData(),
				scannedContributionVoucherNo.substr(8, 8).c_str());
        }
        else if( constants::VOUCHER_NOT_FOUND_IN_DB == scanResults )
        {
            ret.ret = basar::appl::HANDLER_INFO;
            ret.message.format(QCoreApplication::translate("narcotics::useCase::ContributionVoucherScan","ContributionVoucher %s not found").toLocal8Bit().constData(),
				scannedContributionVoucherNo.substr(8, 8).c_str());
        }
        else
        {
            ret.ret = basar::appl::HANDLER_ERROR;
            ret.message = QCoreApplication::translate("narcotics::useCase::ContributionVoucherScan","Unknown state returned").toLocal8Bit().constData();
        }
    }         
    return ret;
}

} //namespace useCase
} //namespace narcotics