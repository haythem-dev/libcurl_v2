//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  implemtation of use case "booking catalog" BTM BUCH NA03
 *  \author	Bjoern Bischof
 *  \date   28.02.2006
 */
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "bookingcatalog.h"
#include "narcoticsappl.h"
#include "narcbookprintdata.h"
#include "dissolvereceiptprintdata.h"
#include "selloffprintdata.h"
#include "manualcontributionvoucherprintdata.h"
#include "loggerpool.h"
#include "contributionvoucherftptransfer.h"

//-------------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------------//
namespace narcotics
{
namespace useCase 
{

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
BookingCatalogUC::BookingCatalogUC() :
	m_DeepCopied( false ),
	m_ChangingState( "" )
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "BookingCatalogUC::BookingCatalogUC()");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
BookingCatalogUC::~BookingCatalogUC()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "BookingCatalogUC::~BookingCatalogUC()");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// macro for registrating and deregistrating EventHandlers!
// 1st para: name of UC
// 2nd para: name of handle-Method
/////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_HANDLERS_REGISTRATION(BookingCatalogUC)
	SYSTEM_EVENT_HANDLER_REGISTRATION( BookingCatalogUC, SearchBookingCatalog )
	SYSTEM_EVENT_HANDLER_REGISTRATION( BookingCatalogUC, SearchArticleNarcoticData )
	SYSTEM_EVENT_HANDLER_REGISTRATION( BookingCatalogUC, SearchCustomerSupplierByBgaNo )
	SYSTEM_EVENT_HANDLER_REGISTRATION( BookingCatalogUC, DisplayInternalBookings )
	SYSTEM_EVENT_HANDLER_REGISTRATION( BookingCatalogUC, DisplayIncompeteBookings )
	SYSTEM_EVENT_HANDLER_REGISTRATION( BookingCatalogUC, SearchDoubleBookingsRequested )
	SYSTEM_EVENT_HANDLER_REGISTRATION( BookingCatalogUC, DeleteBooking )
	SYSTEM_EVENT_HANDLER_REGISTRATION( BookingCatalogUC, CreateBooking )
	SYSTEM_EVENT_HANDLER_REGISTRATION( BookingCatalogUC, UsedBooking )
	SYSTEM_EVENT_HANDLER_REGISTRATION( BookingCatalogUC, RebuildBooking )
	SYSTEM_EVENT_HANDLER_REGISTRATION( BookingCatalogUC, PrintBookingCatalog )
	SYSTEM_EVENT_HANDLER_REGISTRATION( BookingCatalogUC, PrintDeliveryReceipt )
	SYSTEM_EVENT_HANDLER_REGISTRATION( BookingCatalogUC, PrintDissolveReceipt )
	SYSTEM_EVENT_HANDLER_REGISTRATION( BookingCatalogUC, SearchNarcoticsSupplier )
	SYSTEM_EVENT_HANDLER_REGISTRATION( BookingCatalogUC, SaveBooking )
	SYSTEM_EVENT_HANDLER_REGISTRATION( BookingCatalogUC, FindBranchData )
	SYSTEM_EVENT_HANDLER_REGISTRATION( BookingCatalogUC, FindCustomer )
	SYSTEM_EVENT_HANDLER_REGISTRATION( BookingCatalogUC, CancelBooking)
	SYSTEM_EVENT_HANDLER_REGISTRATION( BookingCatalogUC, RegisterOrderForPrinting)
END_HANDLERS_REGISTRATION()

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! \ret	void
//!	\throw	no-throw 
/////////////////////////////////////////////////////////////////////////////////////////////////////
basar::appl::EventReturnType BookingCatalogUC::run()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "BookingCatalogUC::run()");

	registerEventHandlers( true );

	getManager().m_NarcoticsDM->init(		basar::login::Manager::getInstance().getAreaID(), 
											getManager().m_Connection.getCurrentConnection(),
											basar::login::Manager::getInstance().getUserName() );

	getManager().m_NarcoticsGroupDM->init(	getManager().m_Connection.getCurrentConnection() );
	
	getManager().m_BranchDM->init(			basar::login::Manager::getInstance().getAreaID(), 
											getManager().m_Connection.getCurrentConnection() );

	getManager().m_CustomerDM->init(		basar::login::Manager::getInstance().getAreaID(), 
											getManager().m_Connection.getCurrentConnection() );

	getManager().m_ArticleDM.init(			basar::login::Manager::getInstance().getAreaID(), 
											getManager().m_Connection.getCurrentConnection() );

	getManager().m_ContributionVoucherDM->init(		
											basar::login::Manager::getInstance().getAreaID(), 
											getManager().m_Connection.getCurrentConnection());

	getManager().m_NarcoticBookVC->init(	getManager().m_NarcoticsGroupDM->getAccNarCatalogPropTab(),
											getManager().m_NarcoticsDM->getAccNarCatalogPropTab(), 
											getManager().m_NarcoticsDM->getAccNarInitialInventoryPropTab() );
	
	getManager().m_NarcoticBookVC->show();
	
	getManager().m_NarcoticBookVC->shutdown();
	getManager().m_NarcoticBookVC.reset();
	
	getManager().m_currentBooking.clear();
	getManager().m_ContributionVoucherDM->shutdown();
	getManager().m_CustomerDM->shutdown();
	getManager().m_ArticleDM.shutdown();
	getManager().m_BranchDM->shutdown();
	getManager().m_NarcoticsGroupDM->shutdown();
	getManager().m_NarcoticsDM->shutdown();

	registerEventHandlers( false );

	return basar::appl::EventReturnStruct(); //std with ret = OK, msg = ""
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! \retval	true
//! \retval	otherwise false
/*!	\throw	no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
SYSTEM_EVENT_HANDLER_DEFINITION( BookingCatalogUC, SearchArticleNarcoticData )
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "BookingCatalogUC::SearchArticleNarcoticData");

	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	basar::Int32 newArticleNo = 0;

	if( ( getManager().m_NarcoticsDM->getSearchIterator().getState( constants::ARTICLENO ) ).getState() == basar::SS_UNSET )
	{
		getManager().m_currentBooking = getManager().m_NarcoticBookVC->getCurrentAccessorIterator();	
		newArticleNo = getManager().m_currentBooking.getInt32( constants::ARTICLENO );
	}
	else{
		newArticleNo = atoi( getManager().m_NarcoticsDM->getSearchIterator().getString( constants::ARTICLENO ).c_str() );
	}

	basar::db::aspect::AccessorPropertyTable_YIterator yitExecute 
		= getManager().m_NarcoticsGroupDM->findStockLocationByArticleNo( newArticleNo, basar::login::Manager::getInstance().getAreaID() );

	if( yitExecute.isEnd() )
	{
		// message box that no hits are found
		std::ostringstream os;
		os	<< QApplication::translate("BookingCatalogUC", "No record for article number ").toLocal8Bit().data() ;
		
		if (getManager().m_NarcoticsDM->getSearchIterator().isContainedAndSet(constants::ARTICLECODE))
			os	<< atoi( getManager().m_NarcoticsDM->getSearchIterator().getString( constants::ARTICLECODE ).c_str() );
		else
			os << newArticleNo;
		
		os << QApplication::translate("BookingCatalogUC", " found!").toLocal8Bit().data();
		return basar::appl::EventReturnStruct(basar::appl::HANDLER_INFO, os.str().c_str());
	}

	getManager().m_NarcoticBookVC->matchToNarcoticArticleData();

	return basar::appl::EventReturnStruct();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! \ret	void
/*!	\throw	no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
SYSTEM_EVENT_HANDLER_DEFINITION(BookingCatalogUC, SearchBookingCatalog)
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "BookingCatalogUC::SearchBookingCatalog");

	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings
	
	//////////////////////////////////////////////////////
	// 1. select (join) narcotics and article specific data
	//////////////////////////////////////////////////////
	matchToSearchCriteria();
	
	basar::appl::EventReturnStruct ret = getManager().m_ResultsetSearch.findResultset();

	if( ret.ret == basar::appl::HANDLER_INFO ){
		getManager().m_NarcoticsDM->getAccNarCatalogPropTab().clear();
		return ret;
	}

	ret = basar::appl::SystemEventManager::getInstance().fire( "SearchArticleNarcoticData" );

	if( ret.ret == basar::appl::HANDLER_INFO )
	{
		getManager().m_NarcoticsDM->getAccNarCatalogPropTab().clear();
		return ret;
	}
	
	//////////////////////////////////////////////////////
	// 2. select the initial qty in stock
	//////////////////////////////////////////////////////
	LOG_YIT("searchIt", LoggerPool::loggerUseCases, getManager().m_NarcoticsDM->getSearchIterator());
	basar::Int32		articleNo;
	basar::VarString	articleCode;
	basar::Long32		fromDate;

	if( getManager().m_NarcoticsDM->getSearchIterator().getState(constants::ARTICLENO).getState() == basar::SS_UNSET )
	{
		articleNo	= getManager().m_currentBooking.getInt32( constants::ARTICLENO );
		articleCode	= getManager().m_currentBooking.getString( constants::ARTICLECODE );
	}
	else
	{
		articleNo	= getManager().m_NarcoticsDM->getSearchIterator().getString( constants::ARTICLENO ).stoi();
		articleCode	= getManager().m_NarcoticsDM->getSearchIterator().getString( constants::ARTICLECODE );
	}
	fromDate = static_cast<basar::Long32>(getManager().m_NarcoticsDM->getSearchIterator().getString( constants::FROMDATE ).stol());

	basar::db::aspect::AccessorPropertyTable_YIterator yitExecute 
		= getManager().m_NarcoticsDM->findInitialInventoryByArticleNo( articleNo, basar::login::Manager::getInstance().getAreaID(), fromDate );

	if( yitExecute.isEnd() )
	{
		// set default 0 if no initial inventory has been found
		yitExecute = getManager().m_NarcoticsDM->getAccNarInitialInventoryPropTab().insert( basar::FOR_CLEAN );
		yitExecute.setInt32( constants::QTYINSTOCK, 0 );
	}
	getManager().m_NarcoticBookVC->matchToNarcoticArticleData4InitialInventory();

	//////////////////////////////////////////////////////
	// 3. select narcotics bookings
	//////////////////////////////////////////////////////
	// set further parameter
	getManager().m_ResultsetSearch.setAdditionalSearchCriteriaProcessed();	
	if( ( getManager().m_NarcoticsDM->findCatalogByPattern( getManager().m_NarcoticsDM->getSearchIterator() ) ).empty() )
	{
		// message box that no hits are found
		std::ostringstream os;
		os	<< QApplication::translate("BookingCatalogUC", "No record for article number ").toLocal8Bit().data()
			<< articleCode 
			<< QApplication::translate("BookingCatalogUC", " found!").toLocal8Bit().data();
		
		basar::gui::tie::infoMsgBox( getManager().m_NarcoticBookVC.getWidgetPtr(),
			os.str().c_str(),
			QApplication::translate( "BookingCatalogUC", "Narcotics" ).toLocal8Bit().constData());
		// return basar::appl::EventReturnStruct(); --> table widget has to be cleared !!! Done in matchToNarcoticBookings()
	}
	getManager().m_NarcoticBookVC->matchToNarcoticBookings();
	matchFromSearchCriteria();

	return basar::appl::EventReturnStruct();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! \ret	void
/*!	\throw	no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
SYSTEM_EVENT_HANDLER_DEFINITION( BookingCatalogUC, DisplayInternalBookings )
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "BookingCatalogUC::DisplayInternalBookings");

	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings
	
	matchToSearchCriteria();

	basar::I18nString bgaNo;
	if( false == basar::login::Manager::getInstance().getParaValue( UADM_START_INTERNAL_BOOKINGS_NO, bgaNo ) )
	{
		std::ostringstream os;
		os	<< QApplication::translate("BookingCatalogUC", "No parameter data set entry for 'START_INTERNAL_BOOKINGS_NO' found!").toLocal8Bit().data();
		return basar::appl::EventReturnStruct( basar::appl::HANDLER_ERROR, os.str().c_str() );
	}

	getManager().m_NarcoticsDM->getSearchIterator().setString( constants::PROCESSEDFLAG,	 constants::ISPROCESSED  );
	getManager().m_NarcoticsDM->getSearchIterator().setString( constants::DELETEDFLAG,		 constants::ISNOTDELETED );
	getManager().m_NarcoticsDM->getSearchIterator().setString( constants::INTERNAL_ACCOUNTS, bgaNo					 );
	
	if( ( getManager().m_NarcoticsDM->findCatalogByPattern( getManager().m_NarcoticsDM->getSearchIterator() ) ).empty() )
	{
		// message box that no hits are found
		std::ostringstream os;
		os	<< QApplication::translate("BookingCatalogUC", "No internal bookings (accounts) found!").toLocal8Bit().data();
		return basar::appl::EventReturnStruct( basar::appl::HANDLER_INFO, os.str().c_str() );
	}
	getManager().m_NarcoticBookVC->matchToNarcoticBookings();
	
	return basar::appl::EventReturnStruct();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! \ret	void
/*!	\throw	no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
SYSTEM_EVENT_HANDLER_DEFINITION( BookingCatalogUC, SearchDoubleBookingsRequested )
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "BookingCatalogUC::SearchDoubleBookingsRequested");

	rSource = rSource;
	
	matchToSearchCriteria();
	getManager().m_NarcoticsDM->getSearchIterator().setString( constants::PROCESSEDFLAG, constants::ISPROCESSED );
	getManager().m_NarcoticsDM->getSearchIterator().setString( constants::DELETEDFLAG, constants::ISNOTDELETED );

	if( ( getManager().m_NarcoticsDM->findDoubleBookedBookings(getManager().m_NarcoticsDM->getSearchIterator())).empty() )
	{
		// message box that no hits are found
		std::ostringstream os;
		os	<< QApplication::translate("BookingCatalogUC", "No double booked bookings (accounts) found!").toLocal8Bit().data();
		return basar::appl::EventReturnStruct( basar::appl::HANDLER_INFO, os.str().c_str() );
	}
	getManager().m_NarcoticBookVC->matchToNarcoticBookings();

    return basar::appl::EventReturnStruct();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! \ret	void
/*!	\throw	no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
SYSTEM_EVENT_HANDLER_DEFINITION( BookingCatalogUC, DisplayIncompeteBookings )
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "BookingCatalogUC::DisplayIncompeteBookings");

	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings
	
	matchToSearchCriteria();
	getManager().m_NarcoticsDM->getSearchIterator().setString( constants::PROCESSEDFLAG, constants::ISPROCESSED );
	getManager().m_NarcoticsDM->getSearchIterator().setString( constants::DELETEDFLAG, constants::ISNOTDELETED );
	
	if( (getManager().m_NarcoticsDM->findUncompleteBookings(getManager().m_NarcoticsDM->getSearchIterator())).empty() )
	{
		// message box that no hits are found
		std::ostringstream os;
		os	<< QApplication::translate("BookingCatalogUC", "No incomplete bookings (accounts) found!").toLocal8Bit().data();
		return basar::appl::EventReturnStruct(basar::appl::HANDLER_INFO, os.str().c_str());
	}
	getManager().m_NarcoticBookVC->matchToNarcoticBookings();

	return basar::appl::EventReturnStruct();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//!	\retval	false if an error has been occured or no datasets has been affected 
//!	\retval	true otherwise
/*!	\throw	no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
SYSTEM_EVENT_HANDLER_DEFINITION( BookingCatalogUC, SaveBooking )
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "BookingCatalogUC::SaveBooking");

	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	basar::I18nString transactionTypeOriginalValue="";

	// switch off this code if narcotics digital voucher process is not running
	bool isNewProcess = narcotics::NarcoticsAppl::getInstance().isNewNarcoticsProcess();

	if( basar::FOR_INSERT != getManager().m_currentBooking.getInsertType() )
	{
		// back up the old values of the current Iterator in a ParameterList-object
		basar::db::aspect::AccessorPropertyTable_YIterator currentBooking = getManager().m_currentBooking;

		if( false == m_DeepCopied )
		{
			getManager().m_NarcoticsDM->deepCopy( currentBooking );
			m_DeepCopied = true;
			transactionTypeOriginalValue = getManager().m_currentBooking.getString( constants::TRANSACTIONTYPE );
		}

		getManager().m_NarcoticBookVC->matchFromCurrentBooking( getManager().m_currentBooking );
	}
	else
	{
		getManager().m_NarcoticBookVC->matchFromCurrentBooking( getManager().m_currentBooking );

		// bei Inserts: Suche PZN zu Code, da diese vorher nicht im yit stehen kann!
		// read article info in iterator
		if( getManager().m_currentBooking.isContainedAndSet(constants::ARTICLECODE) )
		{
			getManager().m_currentBooking.setString( constants::ARTICLECODE, removeLeadingZeros( getManager().m_currentBooking.getString( constants::ARTICLECODE ) ) );
		
			basar::db::aspect::AccessorPropertyTable_YIterator yitArticle = 
				getManager().m_ArticleDM.findArticleByPattern( getManager().m_currentBooking );

			if( yitArticle.isEnd() )
			{
				return basar::appl::EventReturnStruct( basar::appl::HANDLER_INFO, 
													QApplication::translate("GoodsTransactionUC", "Given articleno is not valid!").toLocal8Bit().data() );
			}
			else{
				getManager().m_currentBooking.setInt32( constants::ARTICLENO, yitArticle.getInt32( constants::ARTICLENO ) );
			}
		}
	}

	// add new fields narcunit, narcname, narcgroup to m_currentBooking
	getManager().m_NarcoticBookVC->matchToNarcoticData( getManager().m_currentBooking );
	checkCustomerSupplierData( getManager().m_currentBooking );
	basar::I18nString transactiontype = getManager().m_currentBooking.getString( constants::TRANSACTIONTYPE );

// IMPORTANTCHANGES
    basar::db::aspect::AccessorPropertyTable_YIterator currentBooking = getManager().m_currentBooking;
	
	basar::Int16 acknowledgementStatusOriginal = 0;

	if( !currentBooking.isNull() && currentBooking.isContainedAndSet( constants::ACKNOWLEDGEMENTSTATUS ) ){
		acknowledgementStatusOriginal = currentBooking.getInt16( constants::ACKNOWLEDGEMENTSTATUS );
	}

    //set acknowledgementstatus to INSERTED - dataset will not be printed - user is asked later, if she will print a contributionvoucher
    if( getManager().m_currentBooking.contains( constants::ACKNOWLEDGEMENTSTATUS ) && 
        ( getManager().m_currentBooking.getState( constants::ACKNOWLEDGEMENTSTATUS ) == basar::SS_UNSET || getManager().m_currentBooking.getInt16(constants::ACKNOWLEDGEMENTSTATUS) <= constants::FOR_PRINT)&&
		 isNewProcess && (transactiontype != transactionTypeOriginalValue) && 
		((transactiontype == TransactionType::CUSTOMER_ORDER) ||(transactiontype == TransactionType::SUPPLIER_RETURNS))
        )
    {
        //since narctransfer sets all datasets with status INSERTED to FOR_PRINT, we need a new status
        //set to invalid here, need definitive solution later
        getManager().m_currentBooking.setInt16( constants::ACKNOWLEDGEMENTSTATUS, constants::INVALID );
    }

    //mark booking as processed (should be anyway)
    if( getManager().m_currentBooking.contains(constants::PROCESSEDFLAG) && 
		getManager().m_currentBooking.getState(constants::PROCESSEDFLAG) == basar::SS_UNSET )
    {
        getManager().m_currentBooking.setString(constants::PROCESSEDFLAG, constants::ISPROCESSED);
    }

    basar::db::aspect::AccessorPropertyTable_YIterator testYit = getManager().m_currentBooking.cloneToUnboundedPropertyRow();

	ExecuteReturnType ret = getManager().m_NarcoticsDM->saveBooking( getManager().m_currentBooking, true, true );
	
	basar::db::aspect::ExecuteResultInfo::RowNumber affectedRows = 0; // according to basar implementation 
																	  // affectedRows will be initialized with 0
	if( !ret.hasError() ){
		affectedRows = ret.getAffectedRows();
	}

	if( ret.getError() == ExecuteReturnType::INVALID_VALUE ) 
	{
		basar::Int32 quantitySave = 
				basar::gui::tie::questionMsgBox( getManager().m_NarcoticBookVC.getWidgetPtr(),
				QApplication::translate( "BookingCatalogUC", "One parameter isn't valid.").toLocal8Bit().data(),
				basar::gui::tie::ButtonCancel | basar::gui::tie::ButtonSave );

		rollbackWorkaroundSaving();

		if( quantitySave == basar::gui::tie::ButtonSave )
		{
// IMPORTANTCHANGES			
			ret = getManager().m_NarcoticsDM->saveBooking( getManager().m_currentBooking, true, false );
			affectedRows = 0; // reset value for affectedRows
			if ( !ret.hasError() ){
				affectedRows = ret.getAffectedRows();
			}
		}
		else{
			return basar::appl::EventReturnStruct(basar::appl::HANDLER_INFO, 
												QApplication::translate( "BookingCatalogUC", "booking has not been saved").toLocal8Bit().data());
		}
	}
// IMPORTANTCHANGES
	if( ExecuteReturnType::NEGATIVE_QTY_IN_STOCK == ret.getError() )
	{
		basar::Int32 quantitySave = 
			basar::gui::tie::questionMsgBox( getManager().m_NarcoticBookVC.getWidgetPtr(),
			QApplication::translate( "BookingCatalogUC", "Your last action has caused a negative quantity in stock!").toLocal8Bit().data() );

		rollbackWorkaroundSaving();

        if( quantitySave == basar::gui::tie::ButtonYes )
		{
// IMPORTANTCHANGES	
			ret = getManager().m_NarcoticsDM->saveBooking( getManager().m_currentBooking, false, false );
			affectedRows = 0; // reset value for affectedRows
			if( !ret.hasError() ){
				affectedRows = ret.getAffectedRows();
			}
		}
		else {
			return basar::appl::EventReturnStruct( basar::appl::HANDLER_INFO, 
												QApplication::translate( "BookingCatalogUC", "booking has not been saved").toLocal8Bit().data() );
		}
	}
// IMPORTANTCHANGES

	if( ExecuteReturnType::OK_BY_SELECT_BUT_NOTHING_SAVED_BY_MANIPULATION == affectedRows )
	{
		std::ostringstream os;
		switch( ret.getError() )
		{
			case ExecuteReturnType::OK_BY_SELECT_BUT_NOTHING_SAVED_BY_MANIPULATION:
									os	<< QApplication::translate( "BookingCatalogUC", 
											"No saving for current narcotics booking!").toLocal8Bit().data();
									break;
		
			case ExecuteReturnType::BASAR_EXCEPTION_THROWN:	
									os	<< QApplication::translate( "BookingCatalogUC", 
										"An error has been occurred for narcotics booking!").toLocal8Bit().data();
									break;
			case ExecuteReturnType::LOCK_ERROR:
									os	<< QApplication::translate( "BookingCatalogUC", 
										"An error has been occurred with locking the article!").toLocal8Bit().data();
									break;
			case ExecuteReturnType::ATTRIBUTES_UNSET: 	
									os	<< QApplication::translate( "BookingCatalogUC", 
										"All necessary attributes of the dataset are not set!\n"
										"Please, check if all data required are filled in the textfields!").toLocal8Bit().data();
									break;
			case ExecuteReturnType::CURRENT_DATASET_UNEQUAL_DB_DATASET: 	
									os	<< QApplication::translate( "BookingCatalogUC", 
											"The dataset has been changed since it has been read out of the database!\n"
											"Please read in the dataset(s) once more!").toLocal8Bit().data();
						break;
			case ExecuteReturnType::ARTICLE_LOCKED: 	
									os	<< QApplication::translate( "BookingCatalogUC", 
											"The article is locked for processing.\n"
											"Please try it at a later date!").toLocal8Bit().data();
						break;
            case ExecuteReturnType::OUTDATED_DATE:
									os << QApplication::translate("BookingCatalogUC",
											"The date is invalid and won't be saved.").toLocal8Bit().data();
				break;
			default:	break;
		}
		return basar::appl::EventReturnStruct(basar::appl::HANDLER_ERROR, os.str().c_str());
	}

	if (isNewProcess && 
		// (transactiontype != transactionTypeOriginalValue) && --> Nachdruck Abgabebeleg auch bei Änderung Menge (nicht nur bei Änderung transactiontype!)
		((transactiontype == TransactionType::CUSTOMER_ORDER) ||(transactiontype == TransactionType::SUPPLIER_RETURNS))  )// only for selloffs (Abverkäufe) and returns to supplier (Lieferantenretoure) a contribution voucher shall be printed
	{
		basar::gui::tie::MessageBoxButtonEnum questionResult = basar::gui::tie::questionMsgBox(
			getManager().m_NarcoticBookVC->getRecordEditVC().getWidgetPtr(),
			QApplication::translate("BookingCatalogUC","Do you want a contributionvoucher being created when printprocess is running?").toLocal8Bit().data());

        basar::db::aspect::AccessorPropertyTable_YIterator yit = getManager().m_currentBooking;

		if( questionResult == basar::gui::tie::ButtonYes )
		{
			// set acknowledgementstatus to FOR_PRINT
			if( !yit.isNull() && !yit.isEnd() )
			{
				basar::Int32 catalogId=-1;
				basar::VarString changedAttribute="NN";
				basar::VarString oldValue="NN";
				basar::VarString newValue="NN";
				
				if( yit.isContainedAndSet( constants::CATALOGID ) ){
					catalogId = yit.getInt32( constants::CATALOGID );
				}
				
				if( yit.isContainedAndSet( constants::ACKNOWLEDGEMENTSTATUS ) ){
					oldValue.itos( yit.getInt16( constants::ACKNOWLEDGEMENTSTATUS ) );
				}

				yit.setInt16( constants::ACKNOWLEDGEMENTSTATUS, constants::FOR_PRINT );

				changedAttribute = constants::ACKNOWLEDGEMENTSTATUS;

				newValue.itos( constants::FOR_PRINT );
				
				getManager().getLogDM()->saveLogEntryUpdate( catalogId, changedAttribute, oldValue, newValue );
			}
        }
        else
        {
			if( !yit.isNull() && !yit.isEnd() && acknowledgementStatusOriginal == constants::FOR_PRINT )
			{				
				basar::Int32 catalogId=-1;
				basar::VarString changedAttribute="NN";
				basar::VarString oldValue="NN";
				basar::VarString newValue="NN";
				
				if( yit.isContainedAndSet( constants::CATALOGID ) ){
					catalogId = yit.getInt32( constants::CATALOGID );
				}
				
				if( yit.isContainedAndSet( constants::ACKNOWLEDGEMENTSTATUS ) ){
					oldValue.itos( yit.getInt16( constants::ACKNOWLEDGEMENTSTATUS ) );
				}

				yit.setInt16( constants::ACKNOWLEDGEMENTSTATUS, constants::INVALID );
				changedAttribute = constants::ACKNOWLEDGEMENTSTATUS;
				newValue.itos( constants::INVALID );
				getManager().getLogDM()->saveLogEntryUpdate( catalogId, changedAttribute, oldValue, newValue );
			}
			else if( !yit.isNull() && !yit.isEnd() && acknowledgementStatusOriginal != constants::FOR_PRINT ){
				yit.setInt16( constants::ACKNOWLEDGEMENTSTATUS, acknowledgementStatusOriginal );
			}
        }

		narcotics::ExecuteReturnType retn = getManager().m_ContributionVoucherDM->setAcknowledgementstatusOfOrder( yit );
			
		if( retn.hasError() )
		{
			basar::appl::EventReturnEnum info = basar::appl::HANDLER_INFO;
			basar::I18nString msg = QApplication::translate( "BookingCatalogUC", 
														"Acknowledgementstatus could not be updated!" ).toLocal8Bit().data();
			return basar::appl::EventReturnStruct( info, msg );
		}
	}

	getManager().m_NarcoticBookVC->matchToNarcoticBookings();	
	return basar::appl::EventReturnStruct();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//!	\return	void
//!	\throw	no-throw
/////////////////////////////////////////////////////////////////////////////////////////////////////
void BookingCatalogUC::checkCustomerSupplierData( basar::db::aspect::AccessorPropertyTable_YIterator currentBooking )
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "BookingCatalogUC::checkCustomerSupplierData()");

	if( (currentBooking.getString( "customersuppliername" ).length() <= 1)					|| 
		(currentBooking.getString( "customersupplierlocation" ).length() <= 1)				||
		(currentBooking.getString("customersuppliercip").length() <= 1)						||
		(currentBooking.getString("customersupplierstreet").length() <= 1)					||
		(getManager().m_NarcoticsDM->getBackUpCurrentIterator().getString("customersuppliername") != 
		 currentBooking.getString( "customersuppliername" ) )								||
		(getManager().m_NarcoticsDM->getBackUpCurrentIterator().getString( "customersupplierlocation" ) != 
		 currentBooking.getString( "customersupplierlocation" ) )							||
		(getManager().m_NarcoticsDM->getBackUpCurrentIterator().getInt32( "customersupplierno" ) != 
		 currentBooking.getInt32( "customersupplierno" ) )									||
		(getManager().m_NarcoticsDM->getBackUpCurrentIterator().getString("customersuppliercip") !=
		 currentBooking.getString("customersuppliercip") )									||
		(getManager().m_NarcoticsDM->getBackUpCurrentIterator().getString("customersupplierstreet") !=
		 currentBooking.getString("customersupplierstreet")))
	{

		// to read in once more the customer/supplier data if name or city is missed or a new customersupplierno is set
		const basar::VarString transactionType = currentBooking.getString( constants::TRANSACTIONTYPE );
		basar::db::aspect::AccessorPropertyTable_YIterator yit;
		const basar::Int32 bgaNo = currentBooking.getInt32( "customersupplierno" );
		if( transactionType == TransactionType::GOODSIN_DELIVERY || transactionType == TransactionType::SUPPLIER_RETURNS )
		{
			yit = getManager().m_NarcoticsDM->findNarcoticsSupplierByBgaNo( bgaNo );
			if( !yit.isEnd() )
			{
                currentBooking.setString( constants::CUSTOMER_SUPPLIER_NAME, yit.getString( constants::SUPPLIERNAME ) );
                currentBooking.setString( constants::CUSTOMER_SUPPLIER_STREET, yit.getString( constants::STREET ) );
                currentBooking.setString( constants::CUSTOMER_SUPPLIER_LOCATION, yit.getString( constants::SUPPLIERLOCATION ) );
				currentBooking.setString( constants::CUSTOMER_SUPPLIER_CIP, yit.getString(constants::POSTCODE)); 
			}
		}
		else if( transactionType == TransactionType::CUSTOMER_ORDER || transactionType == TransactionType::CUSTOMER_RETURNS )
		{
			basar::I18nString pl;
			pl.format(	"branchno=%d;customerno=%d;", basar::login::Manager::getInstance().getAreaID(), bgaNo );
			yit = getManager().m_CustomerDM->findCustomerByPattern( pl );
			if( !yit.isEnd() )
			{
                currentBooking.setString( constants::CUSTOMER_SUPPLIER_NAME, yit.getString(constants::CUSTOMERNAME) );
                currentBooking.setString( constants::CUSTOMER_SUPPLIER_STREET, yit.getString(constants::STREET) );
                currentBooking.setString( constants::CUSTOMER_SUPPLIER_LOCATION, yit.getString(constants::CUSTOMERLOCATION) );
				currentBooking.setString( constants::CUSTOMER_SUPPLIER_CIP, yit.getString(constants::POSTCODE));
			}
		}
        else if( transactionType == TransactionType::DESTRUCTION || transactionType == TransactionType::STOCK_DIFFERENCE_INCREASE || transactionType == TransactionType::STOCK_DIFFERENCE_DECREASE )
        {
            //these types (destruction, stockdifference plus and minus) are handled internally
            //use branches own address
            yit = getManager().m_BranchDM->findBranch();
			if( !yit.isEnd() )
			{
                currentBooking.setString( constants::CUSTOMER_SUPPLIER_NAME,		yit.getString(constants::COMPANYNAME));
                currentBooking.setString( constants::CUSTOMER_SUPPLIER_STREET,		yit.getString(constants::STREET)); 
				currentBooking.setString( constants::CUSTOMER_SUPPLIER_LOCATION,	yit.getString(constants::LOCATION_LONG));
				currentBooking.setString( constants::CUSTOMER_SUPPLIER_CIP,			yit.getString(constants::POSTCODE));
			}
        }
        else{
            //todo: throw exception
        }
        //todo: missing transactiontypes: destruction, stockdifferences
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//!	\return	void
//!	\throw	no-throw
/////////////////////////////////////////////////////////////////////////////////////////////////////
SYSTEM_EVENT_HANDLER_DEFINITION( BookingCatalogUC, CreateBooking )
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "BookingCatalogUC::CreateBooking");

	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	//	set changing state according to former user interaction
	m_ChangingState = constants::CREATE;
	//	Independent on an existing deep copy in the narcotics module, 
	//	here via a bool value is set that there is no deep copy, 
	//	so that is made a new one by invoking the method deepCopy() of the narcotics module.
	//	Actually the deep copy has to be reset if the user do any cancel action, 
	//	but there is no adequate method in the view connectors to do this. 
	//	So it is done implicit if a new narcotic booking is intended to be processed.
	m_DeepCopied = false;
	getManager().m_currentBooking = getManager().m_NarcoticsDM->addEmptyBooking();
	getManager().m_NarcoticBookVC->matchToNewBooking( getManager().m_currentBooking );

	return basar::appl::EventReturnStruct();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//!	\return	void
//!	\throw	no-throw
/////////////////////////////////////////////////////////////////////////////////////////////////////
SYSTEM_EVENT_HANDLER_DEFINITION( BookingCatalogUC, UsedBooking )
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "BookingCatalogUC::UsedBooking");

	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	//	set changing state according to former user interaction
	m_ChangingState = constants::EDIT;
	//	Independent on an existing deep copy in the narcotics module, 
	//	here via an bool value is set that there is no deep copy, 
	//	so that is made a new one by invoking the method deepCopy() of the narcotics module.
	//	Actually the deep copy has to be reset if the user do any cancel action, 
	//	but there is no adequate method in the view connectors to do this. 
	//	So it is done implicit if a new narcotic booking is intended to be processed.
	m_DeepCopied = false;
	getManager().m_currentBooking = getManager().m_NarcoticBookVC->getCurrentAccessorIterator();
	return basar::appl::EventReturnStruct();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
SYSTEM_EVENT_HANDLER_DEFINITION( BookingCatalogUC, CancelBooking )
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "BookingCatalogUC::CancelBooking");

	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings
	
	basar::appl::EventReturnStruct retVal;
	basar::I18nString hostname;
	basar::I18nString user;
	basar::I18nString pw;
	basar::I18nString localTmpDirectory;
	basar::I18nString directoryname;
	basar::I18nString startDatetxt;
	basar::I18nString branchnotxt;
	
	branchnotxt.itos( basar::login::Manager::getInstance().getAreaID() );
	basar::login::Manager::getInstance().getParaValue( branchnotxt, UADM_STARTDATE, startDatetxt );
	basar::login::Manager::getInstance().getParaValue( UADM_CONTRIBUTIONVOUCHERDIR, directoryname );				
	basar::login::Manager::getInstance().getParaValue( UADM_FTP_CITRIX_HOST, hostname );
	basar::login::Manager::getInstance().getParaValue( UADM_FTP_CITRIX_USER, user );	
	basar::login::Manager::getInstance().getParaValue( UADM_FTP_CITRIX_PW, pw );	
	basar::login::Manager::getInstance().getParaValue( UADM_FTP_LOCAL_TMP, localTmpDirectory );	
		
	if( hostname.empty() )
	{
		basar::cmnutil::ExceptionInfoStruct excInfo;
		excInfo.context = "PrintThread::init";
		excInfo .file =__FILE__;
		excInfo.line=__LINE__;
		excInfo.reason = "Parametervalue constants::FTP_CITRIX_HOST is empty!!";
		
		throw basar::cmnutil::BasarInvalidStateException( excInfo );	
	}
	
	if( user.empty() )
	{
		basar::cmnutil::ExceptionInfoStruct excInfo;
		excInfo.context = "PrintThread::init";
		excInfo .file =__FILE__;
		excInfo.line=__LINE__;
		excInfo.reason = "Parametervalue constants::FTP_CITRIX_USER is empty!!";
		
		throw basar::cmnutil::BasarInvalidStateException(excInfo);	
	}
	
	if( pw.empty() )
	{
		basar::cmnutil::ExceptionInfoStruct excInfo;
		excInfo.context = "PrintThread::init";
		excInfo .file =__FILE__;
		excInfo.line=__LINE__;
		excInfo.reason = "Parametervalue constants::FTP_CITRIX_PW is empty!!";
		
		throw basar::cmnutil::BasarInvalidStateException(excInfo);	
	}	
	
	if( localTmpDirectory.empty() )
	{
		basar::cmnutil::ExceptionInfoStruct excInfo;
		excInfo.context = "PrintThread::init";
		excInfo .file =__FILE__;
		excInfo.line=__LINE__;
		excInfo.reason = "Parametervalue constants::FTP_LOCAL_TMP is empty!!";
		
		throw basar::cmnutil::BasarInvalidStateException(excInfo);	
	}	

	basar::db::aspect::AccessorPropertyTable_YIterator currentBooking = getManager().m_NarcoticBookVC->getCurrentAccessorIterator();
	
	if( !currentBooking.isNull() && !currentBooking.isEnd() && currentBooking.isContainedAndSet("contributionvoucherno") )
	{	
		getManager().m_BranchDM->init(	basar::login::Manager::getInstance().getAreaID(), 
										getManager().m_Connection.getCurrentConnection() );
											
		// get branch data
		basar::db::aspect::AccessorPropertyTable_YIterator yitBranch = getManager().m_BranchDM->findBranch();
		
		LOG_YIT("",narcotics::LoggerPool::loggerNarcotics, yitBranch);
		
		if( !yitBranch.isNull() && !yitBranch.isEnd() && yitBranch.isContainedAndSet("bgano") )
		{	
			basar::Int32 bgano = yitBranch.getInt32("bgano");
			getManager().m_BranchDM->shutdown();
			basar::Int32 contrVoucherNo = currentBooking.getInt32("contributionvoucherno");
			ContributionVoucherFTPTransfer ftpTrans(hostname,user,pw, directoryname, localTmpDirectory );
			
			if( ftpTrans.cancelOrder( bgano, contrVoucherNo, startDatetxt ) ){
				retVal.ret = basar::appl::HANDLER_OK;
			}
			else
			{
				retVal.ret = basar::appl::HANDLER_ERROR;
				retVal.message = QApplication::translate("BookingCatalogUC", "No Contribution voucher found!! Maybe it has already been transmitted?").toLocal8Bit().data();
			}
		}
	}
	return retVal;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
SYSTEM_EVENT_HANDLER_DEFINITION( BookingCatalogUC, DeleteBooking )
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "BookingCatalogUC::DeleteBooking");

	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	getManager().m_currentBooking		=  getManager().m_NarcoticBookVC->getCurrentAccessorIterator();
	getManager().m_NarcoticsDM->deepCopy( getManager().m_currentBooking ); // deep copy could always be done

// IMPORTANTCHANGES	
	ExecuteReturnType ret = getManager().m_NarcoticsDM->strikeOut( getManager().m_currentBooking, true );
	basar::db::aspect::ExecuteResultInfo::RowNumber AFFECTED_ROWS = 0;	
	if ( !ret.hasError() ){
		AFFECTED_ROWS = ret.getAffectedRows();
	}

// IMPORTANTCHANGES	
	if( ExecuteReturnType::BASAR_EXCEPTION_THROWN == ret.getError() || 
		( (AFFECTED_ROWS == ExecuteReturnType::OK_BY_SELECT_BUT_NOTHING_SAVED_BY_MANIPULATION) && !ret.hasError() ) )
	{
		std::ostringstream os;
		os	<< QApplication::translate( "BookingCatalogUC", "No strike out for narcotics booking with catalogid: ").toLocal8Bit().data() 
			<< getManager().m_currentBooking.getInt32(constants::CATALOGID)
			<< QApplication::translate( "BookingCatalogUC", " found!").toLocal8Bit().data();
		return basar::appl::EventReturnStruct( basar::appl::HANDLER_INFO, os.str().c_str() );
	}

	if( ExecuteReturnType::NEGATIVE_QTY_IN_STOCK == ret.getError() )
	{
		basar::Int32 quantitySave = basar::gui::tie::questionMsgBox(
							getManager().m_NarcoticBookVC.getWidgetPtr(),
							QApplication::translate( "BookingCatalogUC", "Your last action has caused a negative quantity in stock! Do you want to store this action / quantity?").toLocal8Bit().data());

		rollbackWorkaroundDeleting();

		if( quantitySave == basar::gui::tie::ButtonYes ){
			getManager().m_NarcoticsDM->strikeOut( getManager().m_currentBooking );
		} 
		else{
			return basar::appl::EventReturnStruct();	
		}
	}

	return basar::appl::EventReturnStruct();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
SYSTEM_EVENT_HANDLER_DEFINITION( BookingCatalogUC, SearchNarcoticsSupplier )
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "BookingCatalogUC::SearchNarcoticsSupplier");

	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	basar::db::aspect::AccessorPropertyTableRef aptNarcSupp = getManager().m_NarcoticsDM->findNarcoticsSupplier();
	
	if( aptNarcSupp.empty() )
	{
		// message box that no hits are found
		std::ostringstream os;
		os	<< QApplication::translate("BookingCatalogUC", "No narcotic specific supplier found!").toLocal8Bit().data();
		return basar::appl::EventReturnStruct(basar::appl::HANDLER_INFO, os.str().c_str() );
	}
	
	getManager().m_NarcoticBookVC->matchToSuppliers( aptNarcSupp );
	return basar::appl::EventReturnStruct();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! \retval	true
//! \retval	otherwise false
/*!	\throw	no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
SYSTEM_EVENT_HANDLER_DEFINITION( BookingCatalogUC, FindBranchData )
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "BookingCatalogUC::FindBranchData");

	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	basar::db::aspect::AccessorPropertyTable_YIterator yit = getManager().m_BranchDM->findBranch();

	if( !yit.isEnd() ){
		getManager().m_NarcoticBookVC->matchToBranch(yit);
	}

	return basar::appl::EventReturnStruct();	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
SYSTEM_EVENT_HANDLER_DEFINITION( BookingCatalogUC, SearchCustomerSupplierByBgaNo )
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "BookingCatalogUC::SearchCustomerSupplierByBgaNo");

	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	if( basar::FOR_INSERT != getManager().m_currentBooking.getInsertType() )
	{
		// back up the old values of the current Iterator in a ParameterList-object
		if( false == m_DeepCopied )
		{
			getManager().m_NarcoticsDM->deepCopy( getManager().m_currentBooking );
			m_DeepCopied = true;
		}
	}

	getManager().m_NarcoticBookVC->matchFromCurrentBooking( getManager().m_currentBooking );

	using basar::db::aspect::AccessorPropertyTable_YIterator;
	AccessorPropertyTable_YIterator yitSupplier;
	bool hit = true;
	if( ( TransactionType::GOODSIN_DELIVERY == getManager().m_currentBooking.getString( constants::TRANSACTIONTYPE ) )	|| 	
		( TransactionType::SUPPLIER_RETURNS == getManager().m_currentBooking.getString( constants::TRANSACTIONTYPE ) )	)
	{
		yitSupplier = getManager().m_NarcoticsDM->findNarcoticsSupplierByBgaNo( 
			getManager().m_currentBooking.getInt32( constants::CUSTOMERSUPPLIERNO ) );
		if( yitSupplier.isEnd() ){
			hit = false;
		} 
		else
		{
			getManager().m_currentBooking.setString(	constants::CUSTOMER_SUPPLIER_NAME,  
														yitSupplier.getString("suppliername") );					
			getManager().m_currentBooking.setString(	constants::CUSTOMER_SUPPLIER_LOCATION,  
														yitSupplier.getString("supplierlocation") );
		}
	} 
	else if( ( TransactionType::CUSTOMER_ORDER == getManager().m_currentBooking.getString( constants::TRANSACTIONTYPE ) )	|| 
			 ( TransactionType::CUSTOMER_RETURNS == getManager().m_currentBooking.getString( constants::TRANSACTIONTYPE ) )	)
	{
		basar::I18nString pl;
		pl.format(	"branchno=%d;customerno=%d;", basar::login::Manager::getInstance().getAreaID(), 
			getManager().m_currentBooking.getInt32( constants::CUSTOMERSUPPLIERNO ) );
		AccessorPropertyTable_YIterator yitCustomer = getManager().m_CustomerDM->findCustomerByPattern( pl );
		if( yitCustomer.isEnd() ){
			hit = false;
		} 
		else 
		{
			getManager().m_currentBooking.setString(	constants::CUSTOMER_SUPPLIER_NAME, 
				yitCustomer.getString("customername") );
			getManager().m_currentBooking.setString(	constants::CUSTOMER_SUPPLIER_LOCATION,  
				yitCustomer.getString("customerlocation") );
		}
	} 
	else{
		return basar::appl::EventReturnStruct();
	}
	
	basar::I18nString msg = "";
	basar::appl::EventReturnEnum ret = basar::appl::HANDLER_OK;

	if( false == hit )
	{
		// message box that no hits are found
		std::ostringstream os;
		os	<< QApplication::translate( "BookingCatalogUC", "No record for Narcotics Supplier/Customer No found!").toLocal8Bit().data();
		
		msg = os.str().c_str();
		ret = basar::appl::HANDLER_INFO;
		
		getManager().m_currentBooking.setString( "customersuppliername", " " );	
	}

	getManager().m_NarcoticBookVC->matchToNarcoticCustomerSupplierName( getManager().m_currentBooking );

	return basar::appl::EventReturnStruct( ret, msg );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
SYSTEM_EVENT_HANDLER_DEFINITION( BookingCatalogUC, RebuildBooking )
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "BookingCatalogUC::RebuildBooking");

	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	getManager().m_DeletedBookingsVC.setParent(getManager().m_NarcoticBookVC.getWidgetPtr());

	getManager().m_DeletedBookingsUC.run();
	return basar::appl::EventReturnStruct();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
SYSTEM_EVENT_HANDLER_DEFINITION( BookingCatalogUC, PrintBookingCatalog )
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "BookingCatalogUC::PrintBookingCatalog");

	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	matchToSearchCriteria();

	basar::db::aspect::AccessorPropertyTable_YIterator yit = getManager().m_NarcoticsDM->getSearchIterator();

	//check maximum date range of one month if all articles should be printed
    if( yit.getState( constants::ARTICLENO ) == basar::SS_UNSET && yit.getState( constants::ARTICLECODE ) == basar::SS_UNSET )
	{
		basar::Long32 testDateFrom = atoi( yit.getString( constants::FROMDATE ).c_str() );
		basar::Long32 testDateTo   = atoi( yit.getString( constants::TODATE ).c_str() );

		if( ((testDateFrom % 10000) / 100 != (testDateTo % 10000) / 100) || 
			 ((testDateFrom / 10000) != (testDateTo / 10000)) )
		{
			return basar::appl::EventReturnStruct(basar::appl::HANDLER_INFO, 
					QApplication::translate( "BookingCatalogUC", "if bookings of all articles should be printed,\n"
											 "the date range filter must contain the same month in from- and to-date!" ).toLocal8Bit().data());
		}
	}
	//TODO:Is that rubbish!?!? Isn't it better to select the needed articles from narctransactioncatalog instead of seraching all narcarticles?? 
	basar::db::aspect::AccessorPropertyTableRef prTabNarcArticles = 
		getManager().m_NarcoticsDM->findNarcArticles( yit );
		
	//--- reading of stocks and bookings to selected narc articles:
	basar::Long32 fromDate = atoi( yit.getString(constants::FROMDATE).c_str() );
	basar::cmnutil::DateTime dateFrom( fromDate, 235959000 );
	dateFrom.addDays(-1); // to get balance from previous print

	basar::I18nString Where;
	Where.format("%s=%d; transactiondate=%ld; transactiontime=%ld; %s;", 
					constants::BRANCHNO,
					basar::login::Manager::getInstance().getAreaID(),
					dateFrom.getDate(), 
					dateFrom.getTime()/1000,
					constants::ARTICLENO);
	basar::db::aspect::AccessorPropertyTable_YIterator yit1 = 
			basar::db::aspect::Manager::getInstance().createAccessorYIterator(Where);

	// parameter list used for searching if there are any bookings in this half year if qtyinstock == 0 !
	basar::VarString temp;
	
	if( (fromDate % 10000) > 700 ){
		temp.format("%d%s", fromDate / 10000, "0701");
	}
	else{
		temp.format("%d%s", fromDate / 10000, "0101");
	}

	basar::I18nString searchHalfYear;
	searchHalfYear.format("%s=%d; %s=%s; %s=%s; %s;", 
					constants::BRANCHNO,
					basar::login::Manager::getInstance().getAreaID(),
					constants::FROMDATE,
					temp.c_str(), 
					constants::TODATE, 
					yit.getString(constants::TODATE).c_str(),
					constants::ARTICLENO);
	basar::db::aspect::AccessorPropertyTable_YIterator yitHalfYear = 
			basar::db::aspect::Manager::getInstance().createAccessorYIterator(searchHalfYear);

	//collection of propertytables (one for each narcotic article)
	CollOfNarcArtBookings collBookings;

	basar::db::aspect::AccessorPropertyTable_YIterator yitNarcArticle = prTabNarcArticles.begin();
	while( !yitNarcArticle.isEnd() )
	{
		Where.format( "%ld", yitNarcArticle.getInt32( constants::ARTICLENO ) );
		yit1.setString( constants::ARTICLENO , Where );
		basar::db::aspect::AccessorPropertyTable_YIterator yitStock = getManager().m_NarcoticsDM->findPreviousBooking( yit1 );

		yit.setString( constants::ARTICLENO, Where );
			
		if( yitStock.isEnd() ){
			yitNarcArticle.setInt32( "balance", 0 );
		}
		else{
			yitNarcArticle.setInt32( "balance", yitStock.getInt32("qtyinstock") );
		}

		if(  yitNarcArticle.getInt32("balance") <= 0 ) 
		{
			yitHalfYear.setString( constants::ARTICLENO, Where );
			if( getManager().m_NarcoticsDM->checkBookingsExistance( yitHalfYear ) == false )
			{
				basar::db::aspect::AccessorPropertyTable_YIterator yitTemp = yitNarcArticle;
				++yitNarcArticle;
				prTabNarcArticles.erase( yitTemp );
			}
			else
			{
				basar::db::aspect::AccessorPropertyTableRef prTabNarcArt = 
					getManager().m_NarcoticsDM->findBookings4PrintCatalogByPattern( yit );
				
				collBookings.insert( CollOfNarcArtBookings::value_type( yitNarcArticle.getInt32( constants::ARTICLENO ), prTabNarcArt ) );
				++yitNarcArticle;
			}
		}
		else
		{
			basar::db::aspect::AccessorPropertyTableRef propTabNarcArticles = 
				getManager().m_NarcoticsDM->findBookings4PrintCatalogByPattern( yit );

			collBookings.insert( CollOfNarcArtBookings::value_type( yitNarcArticle.getInt32( constants::ARTICLENO ), propTabNarcArticles ) );
								
			++yitNarcArticle;
		}
	}

	if( prTabNarcArticles.empty() || collBookings.empty() )
	{
		return basar::appl::EventReturnStruct( basar::appl::HANDLER_INFO, 
											  QApplication::translate( "BookingCatalogUC", "No Bookings for printing narcotics book found!" ).toLocal8Bit().data() );
	}

	NarcBookPrintData printData( "NarcoticsBook.ps", "NarcoticsBook", prTabNarcArticles, &collBookings );

	basar::gui::qt::PrintPreview preview( printData, 0, 
					QApplication::translate( "BookingCatalogUC", "Narcotics - Book" ).toLocal8Bit().data() );
	
	preview.setOutputFormats( static_cast<basar::gui::qt::OutputEnum>( basar::gui::qt::PDF | basar::gui::qt::WIN_PRINT ) );
	
	preview.show();

	collBookings.clear();
	getManager().m_NarcoticsDM->resetAccPrintBookingsList();

	return basar::appl::EventReturnStruct();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
SYSTEM_EVENT_HANDLER_DEFINITION( BookingCatalogUC, PrintDeliveryReceipt )
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "BookingCatalogUC::PrintDeliveryReceipt");

	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	basar::appl::SystemEventManager::getInstance().fire("UsedBooking");

	if( getManager().m_currentBooking.getString( constants::TRANSACTIONTYPE ) == TransactionType::CUSTOMER_ORDER ||
		getManager().m_currentBooking.getString( constants::TRANSACTIONTYPE ) == TransactionType::SUPPLIER_RETURNS )
	{
		basar::db::aspect::AccessorPropertyTable_YIterator yitTransaction =
			getManager().m_NarcoticsDM->findTempCatalogByPattern( getManager().m_currentBooking ).begin();

		if( yitTransaction.isEnd() )
		{
			return basar::appl::EventReturnStruct( basar::appl::HANDLER_INFO, 
													QApplication::translate( "BookingCatalogUC", "transaction data not found!" ).toLocal8Bit().data() );
		}

		basar::db::aspect::AccessorPropertyTable_YIterator yitBranch = getManager().m_BranchDM->findBranch();

		basar::db::aspect::AccessorPropertyTable_YIterator yitCustSuppl;

		if( yitTransaction.getString( constants::TRANSACTIONTYPE ) == TransactionType::CUSTOMER_ORDER )
		{
			//Customer
			basar::I18nString pl;
			pl.format("branchno=%d;customerno=%d;",
                basar::login::Manager::getInstance().getAreaID(), 
				yitTransaction.getInt32( constants::CUSTOMERSUPPLIERNO ));
			yitCustSuppl = getManager().m_CustomerDM->findCustomerByPattern( pl );	
		}
		else
		{
			//Supplier
			yitCustSuppl = getManager().m_NarcoticsDM->findNarcoticsSupplierByBgaNo( yitTransaction.getInt32( constants::CUSTOMERSUPPLIERNO ) );
		}

		if( yitCustSuppl.isEnd() || yitBranch.isEnd() )
		{
			return basar::appl::EventReturnStruct( basar::appl::HANDLER_INFO, 
				QApplication::translate( "BookingCatalogUC", "customer / supplier or branch not found!" ).toLocal8Bit().data() );
		}
		// important: the order of the parameters in printData might declare the layout of the document
		SellOffPrintData printData( "NarcSellOff", "NarcSellOff", yitTransaction, yitBranch, yitCustSuppl );
		basar::cmnutil::InterpretPrint print;
		print.print( printData );
	}
	else
	{
		return basar::appl::EventReturnStruct( basar::appl::HANDLER_INFO,
            QApplication::translate( "BookingCatalogUC", "Selected booking is no sell off!\nSo no contribution voucher can be printed!" ).toLocal8Bit().data() );
	}
	return basar::appl::EventReturnStruct();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
SYSTEM_EVENT_HANDLER_DEFINITION( BookingCatalogUC, PrintDissolveReceipt ) // REFACTORING remove code behind these buttons
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "BookingCatalogUC::PrintDissolveReceipt");

	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	basar::appl::SystemEventManager::getInstance().fire( "UsedBooking" ); 

	if( getManager().m_currentBooking.getString( constants::TRANSACTIONTYPE ) == TransactionType::DESTRUCTION )
	{
		basar::db::aspect::AccessorPropertyTableRef prTabDissolveReceiptList
			= getManager().m_NarcoticsDM->findTempCatalogByPattern( getManager().m_currentBooking );
	
		if( prTabDissolveReceiptList.empty() )
		{
			return basar::appl::EventReturnStruct( basar::appl::HANDLER_INFO, 
													QApplication::translate( "BookingCatalogUC", "No bookings with transactiontype 'z' found!\nNo data available for printing dissolve receipt!" ).toLocal8Bit().data() );
		}

		basar::db::aspect::AccessorPropertyTable_YIterator yitBranch = getManager().m_BranchDM->findBranch();
		// print only one specific catalogid!
		printDissolveReceipt( prTabDissolveReceiptList, yitBranch, getManager().m_currentBooking );
	}
	else
	{
		return basar::appl::EventReturnStruct( basar::appl::HANDLER_INFO, 
													QApplication::translate( "BookingCatalogUC", "Selected booking is no elimination or sell off!\nSo no contribution or destruction voucher can be printed!" ).toLocal8Bit().data() );
	}
	return basar::appl::EventReturnStruct();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
void BookingCatalogUC::printDissolveReceipt( basar::db::aspect::AccessorPropertyTableRef prTabDissolveReceiptList, basar::db::aspect::AccessorPropertyTable_YIterator yitBranch, basar::db::aspect::AccessorPropertyTable_YIterator yitDate )
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "BookingCatalogUC::printDissolveReceipt()");

	if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "Method printDissolveReceipt executed in BookingCatalogUC.";
		LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
	
	DissolveReceiptPrintData printData( "DissolveReceipt.ps", "DissolveReceipt", 
										prTabDissolveReceiptList, yitBranch, yitDate );
	
	basar::gui::qt::PrintPreview preview( printData, 0, 
					QApplication::translate( "BookingCatalogUC", "Narcotics - Dissolve Receipt" ).toLocal8Bit().data() );
	preview.setOutputFormats( static_cast<basar::gui::qt::OutputEnum>( basar::gui::qt::PDF | basar::gui::qt::WIN_PRINT ) );
	preview.show();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! \retval	true
//! \retval	otherwise false
/*!	\throw	no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
SYSTEM_EVENT_HANDLER_DEFINITION( BookingCatalogUC, FindCustomer )
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "BookingCatalogUC::FindCustomer");

	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

    if( basar::FOR_INSERT != getManager().m_currentBooking.getInsertType() )
	{
		// back up the old values of the current Iterator in a ParameterList-object
		if( false == m_DeepCopied )
		{
			getManager().m_NarcoticsDM->deepCopy( getManager().m_currentBooking );
			m_DeepCopied = true;
		}
	}
    getManager().m_NarcoticBookVC->matchFromCurrentBooking( getManager().m_currentBooking );

	basar::VarString pl;
	pl.format(	"%s=%ld; %s=%ld;", 
				constants::BRANCHNO,
				basar::login::Manager::getInstance().getAreaID(),
				constants::CUSTOMERNO,
				getManager().m_currentBooking.getInt32( constants::CUSTOMERSUPPLIERNO ) );

	basar::db::aspect::AccessorPropertyTable_YIterator yit = getManager().m_CustomerDM->findCustomerByPattern( basar::cmnutil::ParameterList( pl ) );
	if( !yit.isEnd() )
	{
		getManager().m_NarcoticBookVC->matchToCustomer( yit );
		return basar::appl::EventReturnStruct();
	}
	else{
		return basar::appl::EventReturnStruct( basar::appl::HANDLER_INFO, 
											   QApplication::translate( "BookingCatalogUC", "No Records for given customer number found!" ).toLocal8Bit().data() );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! \retval	no retval
/*!	\throw	no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
SYSTEM_EVENT_HANDLER_DEFINITION( BookingCatalogUC, RegisterOrderForPrinting )
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "BookingCatalogUC::RegisterOrderForPrinting");

	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	// get selected order
	basar::db::aspect::AccessorPropertyTable_YIterator currentOrder = getManager().m_NarcoticBookVC->getCurrentAccessorIterator();
	
	// get order positions
	basar::db::aspect::AccessorPropertyTableRef orderPositions = getManager().m_ContributionVoucherDM->findOrderPositionsRegister( currentOrder );
	
	// check if data is valid
	if( orderPositions.empty() )
	{
		basar::appl::EventReturnEnum ret = basar::appl::HANDLER_INFO;
		basar::I18nString msg = QApplication::translate( "BookingCatalogUC", 
														"No order positions for given order found! Thus new contribution voucher can't be created." ).toLocal8Bit().data();
		return basar::appl::EventReturnStruct( ret, msg );
	}
	// set acknowledgementstatus to FOR_PRINT
	basar::db::aspect::AccessorPropertyTable_YIterator yit = orderPositions.begin();

	while( !yit.isEnd() )
	{
		basar::Int32 catalogId=-1;
		basar::VarString changedAttribute="NN";
		basar::VarString oldValue="NN";
		basar::VarString newValue="NN";
		
		if( yit.isContainedAndSet( constants::CATALOGID ) ){
			catalogId = yit.getInt32( constants::CATALOGID );
		}
		
		if( yit.isContainedAndSet( constants::ACKNOWLEDGEMENTSTATUS ) ){
			oldValue.itos( yit.getInt16( constants::ACKNOWLEDGEMENTSTATUS ) );
		}

		yit.setInt16( constants::ACKNOWLEDGEMENTSTATUS, constants::FOR_PRINT );

		changedAttribute = constants::ACKNOWLEDGEMENTSTATUS;

		newValue.itos( constants::FOR_PRINT );

		getManager().getLogDM()->saveLogEntryUpdate( catalogId, changedAttribute, oldValue, newValue );
		yit++;
	}

	narcotics::ExecuteReturnType ret = getManager().m_ContributionVoucherDM->setAcknowledgementstatus( orderPositions );
		
	if( ret.hasError() )
	{
		basar::appl::EventReturnEnum info = basar::appl::HANDLER_INFO;
		basar::I18nString msg = QApplication::translate( "BookingCatalogUC", 
													"Acknowledgementstatus could not be updated!" ).toLocal8Bit().data();
		return basar::appl::EventReturnStruct( info, msg );
	}

	return basar::appl::EventReturnStruct();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! \no retval
/*!	\throw	no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
void BookingCatalogUC::matchToSearchCriteria()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "BookingCatalogUC::matchToSearchCriteria()");

	getManager().m_NarcoticsDM->resetSearchIterator();
	getManager().m_NarcoticBookVC->matchFromSelection( getManager().m_NarcoticsDM->getSearchIterator() );

	if( getManager().m_NarcoticsDM->getSearchIterator().isContainedAndSet( constants::ARTICLECODE ) )
	{
		getManager().m_NarcoticsDM->getSearchIterator().setString( constants::ARTICLECODE, 
			removeLeadingZeros( getManager().m_NarcoticsDM->getSearchIterator().getString( constants::ARTICLECODE ) ) );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! \no retval
/*!	\throw	no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
void BookingCatalogUC::matchFromSearchCriteria()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "BookingCatalogUC::matchFromSearchCriteria()");

	if( getManager().m_ResultsetSearch.matchingFromSearchCriteriaAllowed() )
	{
		basar::I18nString searchCriteria = getManager().m_ResultsetSearch.getSearchCriteria();
		if( searchCriteria == resultsetdefines::SEARCH_FOR_ARTICLENO ){
			getManager().m_NarcoticBookVC->matchToArticleNo( getManager().m_NarcoticsDM->getSearchIterator().getString( constants::ARTICLECODE ) );
		}
	}

	getManager().m_ResultsetSearch.resetSearchCriteria();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! \no retval
/*!	\throw	no-throw */
/*! Workaround: clear m_currentBooking and match again, as iterator might have been destroyed due to rollback problems */
/////////////////////////////////////////////////////////////////////////////////////////////////////
void BookingCatalogUC::rollbackWorkaroundSaving()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "BookingCatalogUC::rollbackWorkaroundSaving()");

	// clear iterator
	getManager().m_currentBooking = basar::db::aspect::AccessorPropertyTable_YIterator();
	// reinitialize m_currentBooking
	// it has to be differentiated if a new booking shall be created or an already existing one edited
	if( m_ChangingState == constants::CREATE )
	{
		getManager().m_currentBooking = getManager().m_NarcoticsDM->addEmptyBooking();
		getManager().m_NarcoticBookVC->matchFromCurrentBooking( getManager().m_currentBooking );
		
        if( getManager().m_currentBooking.isContainedAndSet(constants::ARTICLECODE) )
		{
			getManager().m_currentBooking.setString( constants::ARTICLECODE, removeLeadingZeros( getManager().m_currentBooking.getString( constants::ARTICLECODE ) ) );
		
			basar::db::aspect::AccessorPropertyTable_YIterator yitArticle = 
				getManager().m_ArticleDM.findArticleByPattern( getManager().m_currentBooking );

			if( !yitArticle.isEnd() ){
				getManager().m_currentBooking.setInt32( constants::ARTICLENO, yitArticle.getInt32( constants::ARTICLENO ) );
			}
		}
	}
	if( m_ChangingState == constants::EDIT ){
		getManager().m_currentBooking = getManager().m_NarcoticBookVC->getCurrentAccessorIterator();
	}

	// refill iterator with recommended values
	getManager().m_NarcoticBookVC->matchFromCurrentBooking( getManager().m_currentBooking );
	getManager().m_NarcoticBookVC->matchToNarcoticData( getManager().m_currentBooking );
	checkCustomerSupplierData( getManager().m_currentBooking );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! \no retval
/*!	\throw	no-throw */
/*!	\Workaround: clear m_currentBooking and match again, as iterator might have been destroyed due to rollback problems */
/////////////////////////////////////////////////////////////////////////////////////////////////////
void BookingCatalogUC::rollbackWorkaroundDeleting()
{	
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "BookingCatalogUC::rollbackWorkaroundDeleting()");

	// clear iterator and match 
	getManager().m_currentBooking = basar::db::aspect::AccessorPropertyTable_YIterator();
	getManager().m_currentBooking = getManager().m_NarcoticBookVC->getCurrentAccessorIterator();
}

} //namespace useCase
} //namespace narcotics