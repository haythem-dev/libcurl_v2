//------------------------------------------------------------------------------
/*! \file
 *  \brief  class for resolving printing errors with new narcotics process
 *  \author Marco Köppendörfer
 *  \date   09.06.2012
 */
//------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "resolveprintingerrors.h"
#include "narcoticsappl.h"
#include "loggerpool.h"

#include "missingdependencyinjectionexception.h"
#include "missingdataforprintingexception.h"

#include "iftpserviceptr.h"
#include "iftpservice.h"

#include "iprintdeliverynoteptr.h"
#include "iprintdeliverynote.h"

#include "iprintcontributionvoucherptr.h"
#include "iprintcontributionvoucher.h"

#include "contributionvoucherprintdata.h"

//todo: move ftpclass to servicelocator

//-------------------------------------------------------------------------------------------------//
// namespace
//-------------------------------------------------------------------------------------------------//
namespace narcotics
{
namespace useCase 
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    /*!	\throw no-throw */
    /////////////////////////////////////////////////////////////////////////////////////////////////////
	ResolvePrintingErrors::ResolvePrintingErrors( basar::Int16 branchNo ) 
	: UseCaseController(), m_BranchNo( branchNo )
    {
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    /*!	\throw no-throw */
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    ResolvePrintingErrors::~ResolvePrintingErrors()
    {
    }

    //-------------------------------------------------------------------------------------------------//
    // macro for registrating and deregistrating EventHandlers!
    // 1st para: name of UC
    // 2nd para: name of handle-Method

    BEGIN_HANDLERS_REGISTRATION( ResolvePrintingErrors )
	    SYSTEM_EVENT_HANDLER_REGISTRATION( ResolvePrintingErrors, ResolvePrintingErrors )
        SYSTEM_EVENT_HANDLER_REGISTRATION( ResolvePrintingErrors, DisplayOrderDetailsRequested )
    END_HANDLERS_REGISTRATION()


    /////////////////////////////////////////////////////////////////////////////////////////////////////
    /*!	\throw no-throw */
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    basar::appl::EventReturnType ResolvePrintingErrors::run()
    {   
        checkInjections();
        registerEventHandlers( true );
        m_ResolvePrintingErrorsVC->init();
        this->populateTableWithBookingsInPrint();
        m_ResolvePrintingErrorsVC->show();
	    registerEventHandlers( false );

		return basar::appl::EventReturnStruct(); //std with ret = OK, msg = ""
    }

	//-------------------------------------------------------------------------------------------------//
    void ResolvePrintingErrors::populateTableWithBookingsInPrint()
    {
        basar::db::aspect::AccessorPropertyTableRef bookingsInPrint = m_ContributionVoucherDM->findBookingsInPrint( m_BranchNo );

        //check bookingsInPrint - no booking before last Sunday (= day vouchers are sent to bfarm) must be solveable
        
        m_ResolvePrintingErrorsVC->setData( bookingsInPrint );
        m_ResolvePrintingErrorsVC->matchToWidget();
    }

	//-------------------------------------------------------------------------------------------------//
    bool ResolvePrintingErrors::isInPrintBookingAlreadyTransfered( basar::db::aspect::AccessorPropertyTable_YIterator selectedBooking )
    {
        if( !selectedBooking.isNull() && !selectedBooking.isEnd() )
        {
            basar::cmnutil::Date transactionDateToCheck;

            if( selectedBooking.isContainedAndSet( constants::PRINTED_DATE ) && 
				selectedBooking.getInt32( constants::PRINTED_DATE ) > 0 )
			{
                transactionDateToCheck.setDate( selectedBooking.getInt32( constants::PRINTED_DATE ) );
            }
            else{
                transactionDateToCheck.setDate( selectedBooking.getInt32( constants::TRANSACTIONDATE ) );
            }

            if( transactionDateToCheck.getDate() <= getLastSundaysDate().getDate() ){
                return true;
            }
        }       
        return false;
    }

	//-------------------------------------------------------------------------------------------------//
    const basar::cmnutil::Date ResolvePrintingErrors::getLastSundaysDate()
    {
        basar::cmnutil::Date date;
        //sunday => dayOfWeek == 0
        date.addDays( date.getDayOfWeek() * -1 );
        return date;
    }

	//-------------------------------------------------------------------------------------------------//
    void ResolvePrintingErrors::injectNarcoticsDM( boost::shared_ptr<domMod::NarcoticsDM> narcoticsDM )
    {
        m_NarcoticsDM = narcoticsDM;
    }

	//-------------------------------------------------------------------------------------------------//
    void ResolvePrintingErrors::injectContributionVoucherDM( boost::shared_ptr<domMod::ContributionVoucherDM> contributionVoucherDM )
    {
        m_ContributionVoucherDM = contributionVoucherDM;
    }

	//-------------------------------------------------------------------------------------------------//
    void ResolvePrintingErrors::injectSerivceLocator( infrastructure::ServiceLocatorPtr serviceLocator )
    {
        m_ServiceLocatorPtr = serviceLocator;
    }

	//-------------------------------------------------------------------------------------------------//
	void ResolvePrintingErrors::injectParentWindow( QWidget * mw )
	{
		m_ResolvePrintingErrorsVC.setParent(mw);	
	}

	//-------------------------------------------------------------------------------------------------//
    void ResolvePrintingErrors::checkInjections()
    {
        if( 0 == m_NarcoticsDM.get() ){
            throw infrastructure::MissingDependencyInjectionException( basar::ExceptInfo("ResolvePrintingErrors::checkInjections", "NarcoticsDM missing! Inject NarcoticsDM first!", __FILE__, __LINE__) );
        }
        if( 0 == m_ContributionVoucherDM.get() ){
            throw infrastructure::MissingDependencyInjectionException( basar::ExceptInfo("ResolvePrintingErrors::checkInjections", "ContributionVoucherDM missing! Inject ContributionVoucherDM first!", __FILE__, __LINE__) );
        }
        if( 0 == m_ServiceLocatorPtr.get() ){
            throw infrastructure::MissingDependencyInjectionException( basar::ExceptInfo("ResolvePrintingErrors::checkInjections", "ServiceLocatorPtr missing! Inject ServiceLocatorPtr first!", __FILE__, __LINE__) );
        }
    }

	//-------------------------------------------------------------------------------------------------//
    SYSTEM_EVENT_HANDLER_DEFINITION( ResolvePrintingErrors, DisplayOrderDetailsRequested )
    {
        rSource = rSource;
	    if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	    {
		    basar::ConstString msg = "Event DisplayOrderDetailsRequested caught in ResolvePrintingErrors.";
		    LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	    }
                
        //match downward
        basar::db::aspect::AccessorPropertyTable_YIterator yit = m_ResolvePrintingErrorsVC->getCurrentSelection();

	    basar::Int32 orderno = yit.getInt32(constants::PURCHASEORDERNO);

	    if( orderno == 0 )
	    {
		    std::ostringstream os;
		    os	<< QApplication::translate("ResolvePrintingErrorsUC", "Invalid order number given to search for!").toLocal8Bit().data();
        	m_ResolvePrintingErrorsVC->warning(m_ResolvePrintingErrorsVC.getWidgetPtr(), os.str().c_str());
		    return basar::appl::EventReturnStruct( basar::appl::HANDLER_INFO, os.str().c_str() );
	    }

	    //////////////////////////////////////////////////////
	    // 2. select narcotics bookings and order data
	    //////////////////////////////////////////////////////
	    // set further parameter
        yit.setString( constants::PROCESSEDFLAG	, constants::ISPROCESSED	);
        yit.setString( constants::DELETEDFLAG	, constants::ISNOTDELETED	);
        yit.setInt16 ( constants::BRANCHNO		, m_BranchNo				);

	    if( ( m_NarcoticsDM->findCatalogByPattern( yit ) ).empty() )
	    {
		    // message box that no hits are found
		    std::ostringstream os;
		    os	<< QApplication::translate("ResolvePrintingErrorsUC", "No record for order number ").toLocal8Bit().data() << orderno 
			    << QApplication::translate("ResolvePrintingErrorsUC", " found!").toLocal8Bit().data();
        	m_ResolvePrintingErrorsVC->warning(m_ResolvePrintingErrorsVC.getWidgetPtr(), os.str().c_str());
		    return basar::appl::EventReturnStruct( basar::appl::HANDLER_INFO, os.str().c_str() );
	    }

        m_OrderDetailsVC->setData( m_NarcoticsDM->getAccNarCatalogPropTab() );
        m_OrderDetailsVC->matchToData();
		m_OrderDetailsVC->show();
        m_OrderDetailsVC->shutdown();
        return basar::appl::EventReturnStruct();
    }

	//-------------------------------------------------------------------------------------------------//
    SYSTEM_EVENT_HANDLER_DEFINITION( ResolvePrintingErrors, ResolvePrintingErrors )
    {
	    rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	    if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	    {
		    basar::ConstString msg = "Event ResolvePrintingErrors caught in ResolvePrintingErrors.";
		    LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	    }

        //match downward

        basar::db::aspect::AccessorPropertyTable_YIterator yit = m_ResolvePrintingErrorsVC->getCurrentSelection();

        //todo: make sure that booking still is in state IN_PRINT - maybe run UC only if no printthread is running or check and return

        yit.setInt16( "branchno", m_BranchNo );
        yit.setInt16( "acknowledgementstatus", constants::IN_PRINT );

        if( !m_ContributionVoucherDM->isContributionVoucherStillInPrint( yit ) )
        {
            //match anew
            populateTableWithBookingsInPrint();
            
            //return error message
		    std::ostringstream os;
            os	<< QApplication::translate("ResolvePrintingErrorsUC", "ContributionVoucher ").toLocal8Bit().data() << yit.getInt32(constants::CONTRIBUTIONVOUCHERNO) 
			    << QApplication::translate("ResolvePrintingErrorsUC", " is not IN_PRINT anymore").toLocal8Bit().data();
			m_ResolvePrintingErrorsVC->warning(m_ResolvePrintingErrorsVC.getWidgetPtr(), os.str().c_str());
            return basar::appl::EventReturnStruct( basar::appl::HANDLER_ERROR, os.str().c_str() );
        }

        //check selected booking
        if( isInPrintBookingAlreadyTransfered( yit ) )
        {
            //return error message
		    std::ostringstream os;
            os	<< QApplication::translate("ResolvePrintingErrorsUC", "By date ContributionVoucher ").toLocal8Bit().data() << yit.getInt32(constants::CONTRIBUTIONVOUCHERNO) 
			    << QApplication::translate("ResolvePrintingErrorsUC", " is already transfered to Bfarm!").toLocal8Bit().data();
			m_ResolvePrintingErrorsVC->warning(m_ResolvePrintingErrorsVC.getWidgetPtr(), os.str().c_str());
            return basar::appl::EventReturnStruct( basar::appl::HANDLER_ERROR, os.str().c_str() );
        }

        //set yit to begin for further use
        yit.begin();

        infrastructure::IFtpServicePtr ftpService;

        try
        {
            ftpService = m_ServiceLocatorPtr->getFtpService();
        }
        catch( infrastructure::MissingDependencyInjectionException& ex )
        {
	        if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::ERROR_LOG_LEVEL) ){
		        LoggerPool::loggerUseCases.forcedLog(log4cplus::ERROR_LOG_LEVEL, ex.what(), __FILE__, __LINE__);
	        }                
			m_ResolvePrintingErrorsVC->error(m_ResolvePrintingErrorsVC.getWidgetPtr(), ex.what().c_str());
            return basar::appl::EventReturnStruct(basar::appl::HANDLER_ERROR, ex.what());
        }

        basar::Int32 contributionVoucherNo = yit.getInt32( constants::CONTRIBUTIONVOUCHERNO );

        constants::ResolvePrintingErrorsTasks tasks = constants::NONE;

        if( !ftpService->doesFileExist( contributionVoucherNo ) )
        {
            //file does not exist -> generate it
            //start position should always be 0, endposition <= 6
            infrastructure::IPrintContributionVoucherPtr contributionVoucherCreator;
            
            try
            {
                contributionVoucherCreator = m_ServiceLocatorPtr->getContributionVoucherCreator();
            }
            catch( infrastructure::MissingDependencyInjectionException& ex )
            {
	            if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::ERROR_LOG_LEVEL) ){
		            LoggerPool::loggerUseCases.forcedLog(log4cplus::ERROR_LOG_LEVEL, ex.what(), __FILE__, __LINE__);
	            }
				m_ResolvePrintingErrorsVC->error(m_ResolvePrintingErrorsVC.getWidgetPtr(), ex.what().c_str());
	            return basar::appl::EventReturnStruct(basar::appl::HANDLER_ERROR, ex.what());
            }
            
            try
            {
                narcotics::ContributionVoucherPrintData contributionVoucherData = contributionVoucherCreator->createContributionVoucherDataProvider( yit );

                if (false == ftpService->storeFile( contributionVoucherData, contributionVoucherNo ))
				{
					std::ostringstream os;
					os	<< QApplication::translate("ResolvePrintingErrorsUC", "Storing of contribution voucher file failed!").toLocal8Bit().data();
					m_ResolvePrintingErrorsVC->warning(m_ResolvePrintingErrorsVC.getWidgetPtr(), os.str().c_str());
					return basar::appl::EventReturnStruct( basar::appl::HANDLER_ERROR, os.str().c_str() );
				}
            }
            catch( infrastructure::MissingDataForPrintingException& ex )
            {
	            if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::ERROR_LOG_LEVEL) ){
		            LoggerPool::loggerUseCases.forcedLog(log4cplus::ERROR_LOG_LEVEL, ex.what(), __FILE__, __LINE__);
	            }
				m_ResolvePrintingErrorsVC->error(m_ResolvePrintingErrorsVC.getWidgetPtr(), ex.what().c_str());
				return basar::appl::EventReturnStruct(basar::appl::HANDLER_ERROR, ex.what());
            }

            tasks = constants::PRINTED_CONTRIBUTION_VOUCHER;
        }

        //need to print empfangsbestätigung?
        const basar::gui::tie::MessageBoxButtonEnum btnNumber = m_ResolvePrintingErrorsVC->question( m_ResolvePrintingErrorsVC.getWidgetPtr(), QApplication::translate( "ResolvePrintingErrorsUC", "Did you get a deliverynote?").toLocal8Bit().data() );
        
        if( basar::gui::tie::ButtonNo == btnNumber )
        {
            try
            {
                infrastructure::IPrintDeliveryNotePtr p = m_ServiceLocatorPtr->getPrintService();
                p->reprintDeliveryNote(yit);
            }
            catch( infrastructure::MissingDependencyInjectionException& ex )
            {
	            if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::ERROR_LOG_LEVEL) ){
		            LoggerPool::loggerUseCases.forcedLog(log4cplus::ERROR_LOG_LEVEL, ex.what(), __FILE__, __LINE__);
	            }
				m_ResolvePrintingErrorsVC->error(m_ResolvePrintingErrorsVC.getWidgetPtr(), ex.what().c_str());
		        return basar::appl::EventReturnStruct(basar::appl::HANDLER_ERROR, ex.what().c_str());
            }
            catch( infrastructure::MissingDataForPrintingException& ex )
            {
	            if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::ERROR_LOG_LEVEL) ){
		            LoggerPool::loggerUseCases.forcedLog(log4cplus::ERROR_LOG_LEVEL, ex.what(), __FILE__, __LINE__);
	            }
				m_ResolvePrintingErrorsVC->error(m_ResolvePrintingErrorsVC.getWidgetPtr(), ex.what().c_str());
		        return basar::appl::EventReturnStruct(basar::appl::HANDLER_ERROR, ex.what().c_str());
            }

            if( tasks == constants::PRINTED_CONTRIBUTION_VOUCHER ){
                tasks = constants::PRINTED_BOTH_DOCUMENTS;
            }
            else{
                tasks = constants::PRINTED_DELIVERY_NOTE;
            }
        }

        //mark order as printed - if we want date here, we have to display it?
        setContributionVoucherNoAsPrinted( yit.getInt32( constants::CONTRIBUTIONVOUCHERNO ), yit.getInt32( constants::TRANSACTIONDATE ), yit.getInt32( constants::PURCHASEORDERNO ), yit.getInt32( constants::CUSTOMERSUPPLIERNO ), tasks );

        //match anew
        this->populateTableWithBookingsInPrint();
        //mark booking as printed
        
        basar::appl::EventReturnStruct ret;

        return ret;
    }

	//-------------------------------------------------------------------------------------------------//
    void ResolvePrintingErrors::setContributionVoucherNoAsPrinted( basar::Int32 contributionVoucherNo, basar::Int32 transactionDate, basar::Int32 purchaseOrderNo, basar::Int32 customerSupplierNo, constants::ResolvePrintingErrorsTasks tasks )
    {     
		basar::cmnutil::DateTime now = basar::cmnutil::DateTime::getCurrent();

        m_ContributionVoucherDM->clearContributionVoucherPropTab();
        
        basar::db::aspect::AccessorPropertyTable_YIterator yit = m_ContributionVoucherDM->getAccContributionVoucherPropTab().insert( basar::FOR_CLEAN );

        yit.setInt16( constants::BRANCHNO, m_BranchNo );
        yit.setInt32( constants::CONTRIBUTIONVOUCHERNO, contributionVoucherNo );
        yit.setInt32( constants::PURCHASEORDERNO, purchaseOrderNo );
        yit.setInt32( constants::TRANSACTIONDATE, transactionDate );
        yit.setInt16( constants::FORMER_ACKNOWLEDGEMENTSTATUS, constants::IN_PRINT );
        yit.setInt16( constants::ACKNOWLEDGEMENTSTATUS, constants::PRINTED );
        yit.setInt32( constants::CUSTOMERSUPPLIERNO, customerSupplierNo );

        if( tasks == constants::PRINTED_BOTH_DOCUMENTS || 
			tasks == constants::PRINTED_CONTRIBUTION_VOUCHER )
        {
            yit.setInt32( constants::PRINTED_DATE, now.getDate() );
            yit.setInt32( constants::PRINTED_TIME, now.getTime() / 1000 );
            yit.setString( constants::USERNAME, basar::login::Manager::getInstance().getUserName().c_str() );
        }
        
        m_ContributionVoucherDM->setAcknowledgementstatusOfOrder( yit );
    }

} //namespace useCase
} //namespace narcotics