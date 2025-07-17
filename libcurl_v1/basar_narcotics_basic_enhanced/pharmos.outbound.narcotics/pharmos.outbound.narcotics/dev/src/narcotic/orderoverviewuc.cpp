	//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  implemtation of use case "order data"
 *  \author	Roland Kiefert
 *  \date   26.06.2009
 */
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "constants.h"
#include "orderoverviewuc.h"
#include "narcoticsappl.h"
#include "loggerpool.h"
#include "contributionvoucherftptransfer.h"
#include "dissolvereceiptprintdata.h"

//-------------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------------//
namespace narcotics 
{
namespace useCase 
{

using basar::cmnutil::ParameterList;
using basar::VarString;
using basar::db::aspect::AccessorPropertyTableRef;

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
OrderOverviewUC::OrderOverviewUC()
{
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
OrderOverviewUC::~OrderOverviewUC()
{
}

//-------------------------------------------------------------------------------------------------//
// macro for registrating and deregistrating EventHandlers!
// 1st para: name of UC
// 2nd para: name of handle-Method
BEGIN_HANDLERS_REGISTRATION(OrderOverviewUC)
	SYSTEM_EVENT_HANDLER_REGISTRATION( OrderOverviewUC, SearchOrderBooking )
	SYSTEM_EVENT_HANDLER_REGISTRATION( OrderOverviewUC, SearchArticleNarcoticDataForTransaction )
	SYSTEM_EVENT_HANDLER_REGISTRATION( OrderOverviewUC, SearchNameAndLocation )
	SYSTEM_EVENT_HANDLER_REGISTRATION( OrderOverviewUC, SearchNarcoticsSupplier )
	SYSTEM_EVENT_HANDLER_REGISTRATION( OrderOverviewUC, SaveBooking )
	SYSTEM_EVENT_HANDLER_REGISTRATION( OrderOverviewUC, SaveOrderChanges )
	SYSTEM_EVENT_HANDLER_REGISTRATION( OrderOverviewUC, UsedBooking )
	SYSTEM_EVENT_HANDLER_REGISTRATION( OrderOverviewUC, CancelBookings )
	SYSTEM_EVENT_HANDLER_REGISTRATION( OrderOverviewUC, DeleteBooking )
	SYSTEM_EVENT_HANDLER_REGISTRATION( OrderOverviewUC, CheckTimeChanges )
	SYSTEM_EVENT_HANDLER_REGISTRATION( OrderOverviewUC, FindCustomer )
	SYSTEM_EVENT_HANDLER_REGISTRATION( OrderOverviewUC, FindBranchData )
	SYSTEM_EVENT_HANDLER_REGISTRATION( OrderOverviewUC, CancelFullOrder )
	SYSTEM_EVENT_HANDLER_REGISTRATION( OrderOverviewUC, LineCodeChanged )
	SYSTEM_EVENT_HANDLER_REGISTRATION( OrderOverviewUC, SearchNarcoticsSuppliersForOrderOverView )
	SYSTEM_EVENT_HANDLER_REGISTRATION( OrderOverviewUC, PrintDissolveReceipt )
END_HANDLERS_REGISTRATION()

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
basar::appl::EventReturnType OrderOverviewUC::run()
{
	registerEventHandlers(true);

	getManager().m_NarcoticsDM->init(		basar::login::Manager::getInstance().getAreaID(), 
											getManager().m_Connection.getCurrentConnection(),
											basar::login::Manager::getInstance().getUserName() );

	getManager().m_NarcoticsGroupDM->init(	getManager().m_Connection.getCurrentConnection() );

	getManager().m_BranchDM->init(			basar::login::Manager::getInstance().getAreaID(), 
											getManager().m_Connection.getCurrentConnection() );

	getManager().m_CustomerDM->init(		basar::login::Manager::getInstance().getAreaID(), 
											getManager().m_Connection.getCurrentConnection() );

	getManager().m_OrderOverviewVC->init(	getManager().m_NarcoticsDM->getAccNarCatalogPropTab(), 
											getManager().m_NarcoticsGroupDM->getAccNarCatalogPropTab() );
	
	getManager().m_OrderOverviewVC->show();
	
	getManager().m_OrderOverviewVC->shutdown();
	getManager().m_OrderOverviewVC.reset();
	
	getManager().m_currentBooking.clear();
	
	getManager().m_CustomerDM->shutdown();
	getManager().m_BranchDM->shutdown();
	getManager().m_NarcoticsGroupDM->shutdown();
	getManager().m_NarcoticsDM->shutdown();

	registerEventHandlers( false );

	return basar::appl::EventReturnStruct(); //std with ret = OK, msg = ""
}

//-------------------------------------------------------------------------------------------------//
SYSTEM_EVENT_HANDLER_DEFINITION( OrderOverviewUC, LineCodeChanged )
{
	rSource;

	basar::VarString currentLineCode = getManager().m_OrderOverviewVC->getCurrentLineCode();

	return basar::appl::EventReturnStruct();

}
//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
SYSTEM_EVENT_HANDLER_DEFINITION( OrderOverviewUC, SearchOrderBooking )
{
// new code according to bookingcatalog and narcoticsmodule

	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings
	
	if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "Event SEARCH caught in OrderOverviewUC.";
		LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
	getManager().m_NarcoticsDM->resetSearchIterator();
	
	//////////////////////////////////////////////////////
	// 1. select (join) narcotics and article specific data
	//////////////////////////////////////////////////////
	getManager().m_OrderOverviewVC->matchFromSelection( getManager().m_NarcoticsDM->getSearchIterator() );
	
	// test fields of searchIterator
	basar::db::aspect::AccessorPropertyTable_YIterator	yit = getManager().m_NarcoticsDM->getSearchIterator();

	if( ( getManager().m_NarcoticsDM->getSearchIterator().getState(constants::PURCHASEORDERNO) ).getState() == basar::SS_UNSET )
	{
		std::ostringstream os;
		os	<< QApplication::translate("OrderOverviewUC", "No order number given to search for!").toLocal8Bit().data();
		
		basar::gui::tie::infoMsgBox(
							getManager().m_OrderOverviewVC.getWidgetPtr(),
							os.str().c_str(),
							QApplication::translate("OrderOverviewUC", "Narcotics").toLocal8Bit().data() );
		
		return basar::appl::EventReturnStruct();
	}
	basar::Int32 orderno = atoi( getManager().m_NarcoticsDM->getSearchIterator().getString( constants::PURCHASEORDERNO ).c_str() );

	if( orderno == 0 )
	{
		std::ostringstream os;
		os	<< QApplication::translate("OrderOverviewUC", "Invalid order number given to search for!").toLocal8Bit().data();
		
		basar::gui::tie::infoMsgBox(
							getManager().m_OrderOverviewVC.getWidgetPtr(),
							os.str().c_str(),
							QApplication::translate("OrderOverviewUC", "Narcotics").toLocal8Bit().data() );
		
		return basar::appl::EventReturnStruct();
	}

	//////////////////////////////////////////////////////
	// 2. select narcotics bookings and order data
	//////////////////////////////////////////////////////
	// set further parameter
	getManager().m_NarcoticsDM->getSearchIterator().setString( constants::PROCESSEDFLAG, constants::ISPROCESSED );
	getManager().m_NarcoticsDM->getSearchIterator().setString( constants::DELETEDFLAG, constants::ISNOTDELETED );
	
	if( ( getManager().m_NarcoticsDM->findCatalogByPattern( getManager().m_NarcoticsDM->getSearchIterator() ) ).empty() )
	{
		// clear table widget entries
		getManager().m_OrderOverviewVC->clearTableWidget();
		// message box that no hits are found
		std::ostringstream os;
		os	<< QApplication::translate("OrderOverviewUC", "No record for order number ").toLocal8Bit().data() << orderno 
			<< QApplication::translate("OrderOverviewUC", " found!").toLocal8Bit().data();

		return basar::appl::EventReturnStruct( basar::appl::HANDLER_INFO, os.str().c_str() );
	}

	AccessorPropertyTable_YIterator orderPosition = getManager().m_NarcoticsDM->getAccNarCatalogPropTab().begin();
	getManager().m_OrderOverviewVC->matchToOrderData( orderPosition ); // fill order data line edit fields

	m_FirstPosYit = orderPosition.cloneToUnboundedPropertyRow();
	
	if( orderPosition.getString( constants::TRANSACTIONTYPE ) == TransactionType::GOODSIN_DELIVERY ||
		orderPosition.getString( constants::TRANSACTIONTYPE ) == TransactionType::SUPPLIER_RETURNS )
	{
		 basar::appl::SystemEventManager::getInstance().fire( "SearchNarcoticsSuppliersForOrderOverView" );
	}

	getManager().m_OrderOverviewVC->matchToNarcoticBookings( static_cast<basar::Int32>( getManager().m_NarcoticsDM->getAccNarCatalogPropTab().size() ) ); // fill table widget with narcotic bookings

	return basar::appl::EventReturnStruct();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
SYSTEM_EVENT_HANDLER_DEFINITION( OrderOverviewUC, SearchArticleNarcoticDataForTransaction )
{
	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings
	
	basar::Int32 articleNo = 0;
	basar::db::aspect::AccessorPropertyTable_YIterator currentBookingSearchIt = getManager().m_OrderOverviewVC->getCurrentAccessorIterator();

	if( ( currentBookingSearchIt.getState( constants::ARTICLENO ) ) == basar::SS_UNSET )
	{
		// message box that no articleNo for searching has been found
		std::ostringstream os;
		os	<< QApplication::translate("OrderOverviewUC", "No article number ").toLocal8Bit().data() 
			<< articleNo
			<< QApplication::translate("OrderOverviewUC", " found!").toLocal8Bit().data();
	} 
	else{
		articleNo = currentBookingSearchIt.getInt32( constants::ARTICLENO );
	}

	basar::db::aspect::AccessorPropertyTable_YIterator yitExecute 
		= getManager().m_NarcoticsGroupDM->findStockLocationByArticleNo( articleNo, basar::login::Manager::getInstance().getAreaID() );
	
	if( yitExecute.isEnd() )
	{
		// message box that no hits are found
		std::ostringstream os;
		os	<< QApplication::translate("OrderOverviewUC", "No article and narcotic specific data for article number ").toLocal8Bit().data() 
			<< articleNo
			<< QApplication::translate("OrderOverviewUC", " found!").toLocal8Bit().data();
		return basar::appl::EventReturnStruct(basar::appl::HANDLER_INFO, os.str().c_str());
	}
	getManager().m_OrderOverviewVC->matchToArticleNarcoticData( yitExecute );

	return basar::appl::EventReturnStruct();
}

//-------------------------------------------------------------------------------------------------//
//
//-------------------------------------------------------------------------------------------------//
SYSTEM_EVENT_HANDLER_DEFINITION( OrderOverviewUC, SearchNarcoticsSuppliersForOrderOverView )
{
    rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "Event search narcotics supplier caught in OrderOverviewUC.";
		LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	using basar::db::aspect::AccessorPropertyTableRef;
	AccessorPropertyTableRef aptNarcSupp = getManager().m_NarcoticsDM->findNarcoticsSupplier();
	
	if( aptNarcSupp.empty() )
	{
		// message box that no hits are found
		std::ostringstream os;
		os	<< QApplication::translate( "OrderOverviewUC", "No narcotic specific supplier found!" ).toLocal8Bit().data();
		return basar::appl::EventReturnStruct( basar::appl::HANDLER_INFO, os.str().c_str() );
	}
	
	getManager().m_OrderOverviewVC->matchToCustomerSuppliers( aptNarcSupp );
	return basar::appl::EventReturnStruct();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
SYSTEM_EVENT_HANDLER_DEFINITION( OrderOverviewUC, SearchNarcoticsSupplier )
{
    rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "Event search narcotics supplier caught in OrderOverviewUC.";
		LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	using basar::db::aspect::AccessorPropertyTableRef;
	AccessorPropertyTableRef aptNarcSupp = getManager().m_NarcoticsDM->findNarcoticsSupplier();
	
	if( aptNarcSupp.empty() )
	{
		// message box that no hits are found
		std::ostringstream os;
		os	<< QApplication::translate("OrderOverviewUC", "No narcotic specific supplier found!").toLocal8Bit().data();
		return basar::appl::EventReturnStruct(basar::appl::HANDLER_INFO, os.str().c_str() );
	}
	
	getManager().m_OrderOverviewVC->matchToSuppliers( aptNarcSupp );
	return basar::appl::EventReturnStruct();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
SYSTEM_EVENT_HANDLER_DEFINITION( OrderOverviewUC, SearchNameAndLocation )
{
	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "Event SearchNameAndLocation caught in OrderOverviewUC.";
		LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	std::ostringstream os;
	
	if( checkChangesApplied( constants::CUSTOMERSUPPLIERNO ) )
	{
		basar::db::aspect::AccessorPropertyTableRef orderDataPropTab = getManager().m_NarcoticsDM->getAccNarCatalogPropTab();
		basar::Int32 customersupplierno = getManager().m_OrderOverviewVC->matchFromCustomerSupplierNo();

// check if it has to be looked for a customer or a supplier by checking value of transactiontype
// if transactiontype == "a", a corresponding customerNo has to be searched as one is dealing with a sale 
// if transactiontype == "b", a corresponding supplierNo has to be searched as one is dealing with incoming goods
		if ( orderDataPropTab.begin().contains( constants::TRANSACTIONTYPE ) && 
			!( orderDataPropTab.begin().getState( constants::TRANSACTIONTYPE ) == basar::SS_UNSET ) )
		{
			basar::I18nString transactiontype = orderDataPropTab.begin().getString( constants::TRANSACTIONTYPE );
			
			if( transactiontype == TransactionType::CUSTOMER_ORDER )
			{
				basar::VarString pl;
				pl.format(	"%s=%ld; %s=%ld;", 
					constants::BRANCHNO,
					basar::login::Manager::getInstance().getAreaID(),
					constants::CUSTOMERNO,
					customersupplierno );

				// find corresponding customer to given parameter list
				basar::db::aspect::AccessorPropertyTable_YIterator customerData = getManager().m_CustomerDM->findCustomerByPattern( basar::cmnutil::ParameterList(pl) );
				
				if( !customerData.isEnd() )
				{
					getManager().m_OrderOverviewVC->matchToCustomerData( customerData );
					return basar::appl::EventReturnStruct();
				}
				else 
				{
					// message box that no hits are found
					os	<< QApplication::translate("OrderOverviewUC", "No record for customerNo ").toLocal8Bit().data() 
						<< customersupplierno
						<< QApplication::translate("OrderOverviewUC", " found!").toLocal8Bit().data();
					return basar::appl::EventReturnStruct(basar::appl::HANDLER_INFO, os.str().c_str());
				}
			}
			if( transactiontype == TransactionType::GOODSIN_DELIVERY )
			{	
				// find corresponding supplier to given supplierNo
				basar::db::aspect::AccessorPropertyTable_YIterator supplierData = getManager().m_NarcoticsDM->findNarcoticsSupplierByBgaNo( customersupplierno );
			
				if( !supplierData.isEnd() )
				{
					getManager().m_OrderOverviewVC->matchToSupplierData( supplierData );
					return basar::appl::EventReturnStruct();
				}
				else 
				{
					// message box that no hits are found
					os	<< QApplication::translate("OrderOverviewUC", "No record for supplierNo ").toLocal8Bit().data() 
						<< customersupplierno
						<< QApplication::translate("OrderOverviewUC", " found!").toLocal8Bit().data();
					return basar::appl::EventReturnStruct(basar::appl::HANDLER_INFO, os.str().c_str());
				}
			}
		}
	}
	return basar::appl::EventReturnStruct( basar::appl::HANDLER_INFO );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
SYSTEM_EVENT_HANDLER_DEFINITION( OrderOverviewUC, CancelBookings )
{
	using basar::db::aspect::AccessorPropertyTable_YIterator;
	
	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings
	basar::ConstString fun = "BookingCatalogUC::CancelBookings()";

	basar::appl::EventReturnStruct retVal;

	if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) ){
		LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, fun, __FILE__, __LINE__);
	}

	getManager().m_OrderOverviewVC->matchFromNarcoticBookings();

	AccessorPropertyTableRef orderDetails = getManager().m_NarcoticsDM->getAccNarCatalogPropTab();
	AccessorPropertyTable_YIterator orderDetailsYit = orderDetails.begin();

	bool atLeastOnePosSelected = false;
	
	while( !orderDetailsYit.isNull() && !orderDetailsYit.isEnd() )
	{
		if (orderDetailsYit.getString(constants::SELECTION) == "1")
			atLeastOnePosSelected = true;
		
		basar::VarString msg;
		msg.format("enabled: %s, catalogid: %d, narcname: %s",
			orderDetailsYit.getString(constants::SELECTION).c_str(),
			orderDetailsYit.getInt32(constants::CATALOGID),
			orderDetailsYit.getString(constants::ARTICLENAME).c_str());
		LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
		++orderDetailsYit;
	}

	if (atLeastOnePosSelected == false)
	{
		retVal.ret = basar::appl::HANDLER_INFO;
		retVal.message = QApplication::translate("OrderOverviewUC", "Please tick at least one position you want to cancel!").toLocal8Bit().data();
		return retVal;
	}
		
	if( orderDetailsYit.isEnd() ){
		orderDetailsYit = orderDetails.begin();
	}

	basar::Int32 contributionVoucherNo = 0;
	if( !orderDetailsYit.isNull() && !orderDetailsYit.isEnd() )
	{
		contributionVoucherNo = orderDetailsYit.getInt32("contributionvoucherno");
		
		if( orderDetailsYit.getString( constants::TRANSACTIONTYPE ) == TransactionType::CUSTOMER_ORDER ||
			orderDetailsYit.getString( constants::TRANSACTIONTYPE ) == TransactionType::SUPPLIER_RETURNS )
		{
			if( contributionVoucherNo != 0 )
			{
				if( ftpCancelOrder(contributionVoucherNo) )
				{
					if( getManager().getOrderDM()->cancelSelectedOrderPositions( orderDetails ) && 
						getManager().getOrderDM()->setOrderStatusToForPrint( orderDetails.begin() ) )
					{
						retVal.ret = basar::appl::HANDLER_OK;
					}
					else
					{
						getManager().m_OrderOverviewVC->error( getManager().m_OrderOverviewVC.getWidgetPtr(),QApplication::translate("OrderOverviewUC","Contribution voucher removed but got error during cancelation of positions. Please try to delete positions and generate a new voucher manually").toLocal8Bit().constData() );
						retVal.ret = basar::appl::HANDLER_ERROR;
						retVal.message = QApplication::translate("OrderOverviewUC", "Contribution voucher removed but got error during cancelation of positions. Please try to delete positions and generate a new voucher manually").toLocal8Bit().data();
					}
				}
				else
				{
					getManager().m_OrderOverviewVC->error( getManager().m_OrderOverviewVC.getWidgetPtr(),QApplication::translate("OrderOverviewUC","No Contribution voucher found!! Maybe it has already been transmitted?").toLocal8Bit().constData() );
					retVal.ret = basar::appl::HANDLER_ERROR;
					retVal.message = QApplication::translate( "OrderOverviewUC", "No Contribution voucher found!! Maybe it has already been transmitted?" ).toLocal8Bit().data();
				}
			}	
			else
			{
				retVal.ret = basar::appl::HANDLER_INFO;
				retVal.message = QApplication::translate("OrderOverviewUC", "No Contribution voucher has been generated yet!").toLocal8Bit().data();
			}
		}
	}

	return retVal;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
SYSTEM_EVENT_HANDLER_DEFINITION( OrderOverviewUC, CancelFullOrder )
{
	using basar::db::aspect::AccessorPropertyTable_YIterator;
	
	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings
	basar::ConstString fun = "BookingCatalogUC::CancelBooking()";
	
	basar::appl::EventReturnStruct retVal;
	
	if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) ){
		LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, fun, __FILE__, __LINE__);
	}
	
	if( getManager().m_NarcoticsDM->getAccNarCatalogPropTab().begin() != getManager().m_NarcoticsDM->getAccNarCatalogPropTab().end() )
	{
		AccessorPropertyTable_YIterator orderDetails = getManager().m_NarcoticsDM->getAccNarCatalogPropTab().begin();

		basar::Int32 contrVoucherNo = orderDetails.getInt32("contributionvoucherno");

		getManager().m_OrderOverviewVC->matchToOrderData( orderDetails );
		
		if( orderDetails.getString( constants::TRANSACTIONTYPE ) == TransactionType::CUSTOMER_ORDER ||
			orderDetails.getString( constants::TRANSACTIONTYPE ) == TransactionType::SUPPLIER_RETURNS )
		{
			if( contrVoucherNo != 0 )		
			{
				if( ftpCancelOrder( contrVoucherNo ) )
				{
					if( getManager().getOrderDM()->deleteOrder( orderDetails ) )
					{
						retVal.ret = basar::appl::HANDLER_OK;
					}
					else
					{
						getManager().m_OrderOverviewVC->error( getManager().m_OrderOverviewVC.getWidgetPtr(),QApplication::translate("OrderOverviewUC","Contribution voucher removed but got error during cancelation of order. Please try to delete positions and generate a new voucher manually.").toLocal8Bit().constData() );
						retVal.ret = basar::appl::HANDLER_ERROR;
						retVal.message = QApplication::translate("OrderOverviewUC", "Contribution voucher removed but got error during cancelation of order. Please try to delete positions and generate a new voucher manually.").toLocal8Bit().data();
					}
				}
				else
				{
					getManager().m_OrderOverviewVC->error( getManager().m_OrderOverviewVC.getWidgetPtr(),QApplication::translate("OrderOverviewUC","No Contribution voucher found!! Maybe it has already been transmitted?").toLocal8Bit().constData() );
					retVal.ret = basar::appl::HANDLER_ERROR;
					retVal.message = QApplication::translate("OrderOverviewUC", "No Contribution voucher found!! Maybe it has already been transmitted?").toLocal8Bit().data();
				}
			}
			else
			{
				retVal.ret = basar::appl::HANDLER_INFO;
				retVal.message = QApplication::translate("OrderOverviewUC", "No Contribution voucher has been generated yet!").toLocal8Bit().data();
			}
		}
	}
	else
	{
		getManager().m_OrderOverviewVC->error( getManager().m_OrderOverviewVC.getWidgetPtr(),QApplication::translate("OrderOverviewUC","No data selected").toLocal8Bit().constData() );
	}
	return retVal;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//!	\retval	false if an error has occured or no datasets have been affected 
//!	\retval	true otherwise
/*!	\throw	no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
SYSTEM_EVENT_HANDLER_DEFINITION( OrderOverviewUC, SaveBooking )
{
    rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings
	
	if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "Event SAVE caught in OrderOverviewUC.";
		LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	basar::I18nString transactionTypeOriginalValue="";

	// aus NA12 sollte es doch überhaupt keine Inserts geben können !?
	if( basar::FOR_INSERT != getManager().m_currentBooking.getInsertType() )
	{
		// back up the old values of the current Iterator in a ParameterList-object
		if( false == m_DeepCopied )
		{
			getManager().m_NarcoticsDM->deepCopy( getManager().m_currentBooking );
			m_DeepCopied = true;
		}
	}	
// no matching of narcotic specific fields into yit necessary as booking being related to an order can only be 
// updated here. Inserts of new bookings are not possible. And the update-statement does only support changes of 
// article specific data, but not narcotic specific data. 

	getManager().m_OrderOverviewVC->matchFromCurrentBooking( getManager().m_currentBooking );

	checkCustomerSupplierData( getManager().m_currentBooking );

	basar::I18nString transactiontype = getManager().m_currentBooking.getString( constants::TRANSACTIONTYPE );

// IMPORTANTCHANGES

    //set acknowledgementstatus to INSERTED - dataset will not be printed - user is asked later, if she will print a contributionvoucher
    if( getManager().m_currentBooking.contains(constants::ACKNOWLEDGEMENTSTATUS) && 
        ( getManager().m_currentBooking.getState(constants::ACKNOWLEDGEMENTSTATUS) == basar::SS_UNSET || getManager().m_currentBooking.getInt16( constants::ACKNOWLEDGEMENTSTATUS ) == constants::FOR_PRINT )
        )
    {
        getManager().m_currentBooking.setInt16( constants::ACKNOWLEDGEMENTSTATUS, constants::INSERTED );
    }

	//! \todo: make clearer: this saveBooking call just checks if any records will get negative quantity, no update is done in datebase
    ExecuteReturnType ret = getManager().m_NarcoticsDM->saveBooking( getManager().m_currentBooking, true, true );
	basar::db::aspect::ExecuteResultInfo::RowNumber affectedRows = 0; // according to basar implementation 
																  // affectedRows will be initialized with 0

	if( !ret.hasError() ){
		affectedRows = ret.getAffectedRows();
	}

	if( ret.getError()	== ExecuteReturnType::INVALID_VALUE )
	{
		basar::Int32 quantitySave = 
				basar::gui::tie::questionMsgBox( getManager().m_OrderOverviewVC.getWidgetPtr(),
				QApplication::translate( "OrderOverviewUC", "One parameter isn't valid.").toLocal8Bit().data(),
				basar::gui::tie::ButtonCancel | basar::gui::tie::ButtonSave,
				basar::gui::tie::ButtonNoButton,
				QApplication::translate( "OrderOverviewUC", "Narcotics" ).toLocal8Bit().data()	);

		rollbackWorkaroundSaving();

		if( quantitySave == basar::gui::tie::ButtonSave )
		{
			ret = getManager().m_NarcoticsDM->saveBooking( getManager().m_currentBooking, true, false );
			affectedRows = 0; // reset value for affectedRows
			if( !ret.hasError() ){
				affectedRows = ret.getAffectedRows();
			}
		}
		else{
			return basar::appl::EventReturnStruct( basar::appl::HANDLER_INFO, QApplication::translate( "OrderOverviewUC", "booking has not been saved").toLocal8Bit().data() );
		}
	}

	if( ExecuteReturnType::NEGATIVE_QTY_IN_STOCK == ret.getError() )
	{
		basar::Int32 quantitySave = 
			basar::gui::tie::questionMsgBox( getManager().m_OrderOverviewVC.getWidgetPtr(),
			QApplication::translate( "OrderOverviewUC", "Your last action has caused a negative quantity in stock!").toLocal8Bit().data(),
			basar::gui::tie::ButtonCancel | basar::gui::tie::ButtonSave,
			basar::gui::tie::ButtonNoButton,
			QApplication::translate( "OrderOverviewUC", "Narcotics" ).toLocal8Bit().data() );

		rollbackWorkaroundSaving();
		
		if( quantitySave == basar::gui::tie::ButtonSave )
		{			
			ret = getManager().m_NarcoticsDM->saveBooking( getManager().m_currentBooking, false, false );
			affectedRows = 0; // reset value for affectedRows
			if( !ret.hasError() ){
				affectedRows = ret.getAffectedRows();
			}
		}
		else{
			return basar::appl::EventReturnStruct(basar::appl::HANDLER_INFO, QApplication::translate( "OrderOverviewUC", "booking has not been saved").toLocal8Bit().data());
		}
	}

	if( ExecuteReturnType::OK_BY_SELECT_BUT_NOTHING_SAVED_BY_MANIPULATION == affectedRows )
	{
		std::ostringstream os;
		switch( ret.getError() )
		{
			case ExecuteReturnType::OK_BY_SELECT_BUT_NOTHING_SAVED_BY_MANIPULATION:
									os	<< QApplication::translate( "OrderOverviewUC", 
											"No saving for current narcotics booking!").toLocal8Bit().data();
									break;
		
			case ExecuteReturnType::BASAR_EXCEPTION_THROWN:	
									os	<< QApplication::translate( "OrderOverviewUC", 
										"An error has been occurred for narcotics booking!").toLocal8Bit().data();
									break;
			case ExecuteReturnType::LOCK_ERROR:
									os	<< QApplication::translate( "OrderOverviewUC", 
										"An error has been occurred with locking the article!").toLocal8Bit().data();
									break;
			case ExecuteReturnType::ATTRIBUTES_UNSET: 	
									os	<< QApplication::translate( "OrderOverviewUC", 
										"All necessary attributes of the dataset are not set!\n"
										"Please, check if all data required are filled in the textfields!").toLocal8Bit().data();
									break;
			case ExecuteReturnType::CURRENT_DATASET_UNEQUAL_DB_DATASET: 	
									os	<< QApplication::translate( "OrderOverviewUC", 
											"The dataset has been changed since it has been read out of the database!\n"
											"Please read in the dataset(s) once more!").toLocal8Bit().data();
						break;
			case ExecuteReturnType::ARTICLE_LOCKED: 	
									os	<< QApplication::translate( "OrderOverviewUC", 
											"The article is locked for processing.\n"
											"Please try it at a later date!").toLocal8Bit().data();
						break;
			default:	break;
		}
		return basar::appl::EventReturnStruct( basar::appl::HANDLER_ERROR, os.str().c_str() );
	}

	if( transactiontype != transactionTypeOriginalValue &&
		( ( transactiontype == TransactionType::CUSTOMER_ORDER ) ||( transactiontype == TransactionType::SUPPLIER_RETURNS ) ) )// only for selloffs (Abverkäufe) and returns to supplier (Lieferantenretoure) a contribution voucher shall be printed
	{
		basar::gui::tie::MessageBoxButtonEnum questionResult = basar::gui::tie::questionMsgBox(
			getManager().m_NarcoticBookVC->getRecordEditVC().getWidgetPtr(),
			QApplication::translate( "OrderOverviewUC","Do you want a contributionvoucher being created when printprocess is running?").toLocal8Bit().data() );

		if( questionResult == basar::gui::tie::ButtonYes )
		{
			getManager().m_ContributionVoucherDM->init(		
											basar::login::Manager::getInstance().getAreaID(), 
											getManager().m_Connection.getCurrentConnection());

			// set acknowledgementstatus to FOR_PRINT
			basar::db::aspect::AccessorPropertyTable_YIterator yit = getManager().m_currentBooking;//orderPositions.begin();
			
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
					oldValue.itos(yit.getInt16( constants::ACKNOWLEDGEMENTSTATUS ) );
				}

				yit.setInt16( constants::ACKNOWLEDGEMENTSTATUS, constants::FOR_PRINT );
				changedAttribute = constants::ACKNOWLEDGEMENTSTATUS;
				newValue.itos(constants::FOR_PRINT);
				getManager().getLogDM()->saveLogEntryUpdate( catalogId, changedAttribute, oldValue, newValue );
			}

			narcotics::ExecuteReturnType retrn = getManager().m_ContributionVoucherDM->setAcknowledgementstatus( yit );

			if( retrn.hasError() )
			{
				basar::appl::EventReturnEnum info = basar::appl::HANDLER_INFO;
				basar::I18nString msg = QApplication::translate( "OrderOverviewUC", 
															"Acknowledgementstatus could not be updated!" ).toLocal8Bit().data();
				return basar::appl::EventReturnStruct( info, msg );
			}
		}
	}
		
	getManager().m_OrderOverviewVC->matchToNarcoticBookings( static_cast<basar::Int32>( getManager().m_NarcoticsDM->getAccNarCatalogPropTab().size() ) );	
	return basar::appl::EventReturnStruct();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//!	\return	void
//!	\throw	no-throw
/////////////////////////////////////////////////////////////////////////////////////////////////////
void OrderOverviewUC::checkCustomerSupplierData( basar::db::aspect::AccessorPropertyTable_YIterator currentBooking )
{
	if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "Event checkCustomerSupplierData caught in OrderOverviewUC.";
		LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	if( isCurrentBookingNotEqualToCurrentIterator( currentBooking ) )
	{
		// to read in once more the customer/supplier data if name or city is missed or a new customersupplierno is set
		const basar::VarString transactionType = currentBooking.getString(constants::TRANSACTIONTYPE);
		basar::db::aspect::AccessorPropertyTable_YIterator yit;
		const basar::Int32 bgaNo = currentBooking.getInt32( "customersupplierno" );
		if( transactionType == TransactionType::GOODSIN_DELIVERY || transactionType == TransactionType::SUPPLIER_RETURNS )
		{
			yit = getManager().m_NarcoticsDM->findNarcoticsSupplierByBgaNo( bgaNo );
			if( !yit.isEnd() )
			{
				currentBooking.setString( "customersuppliername", yit.getString( "suppliername" ) );
				currentBooking.setString( "customersupplierstreet", yit.getString( "street" ) );
				currentBooking.setString( "customersupplierlocation", yit.getString( "supplierlocation" ) );
			}
		} 
		else if( transactionType == TransactionType::CUSTOMER_ORDER || transactionType == TransactionType::CUSTOMER_RETURNS )
		{
			basar::I18nString pl;
			pl.format( "branchno=%d;customerno=%d;", basar::login::Manager::getInstance().getAreaID(), bgaNo );
			yit = getManager().m_CustomerDM->findCustomerByPattern( pl );
			if( !yit.isEnd() )
			{
				currentBooking.setString( constants::CUSTOMER_SUPPLIER_NAME, yit.getString("customername") );
				currentBooking.setString( constants::CUSTOMER_SUPPLIER_LOCATION, yit.getString("customerlocation") );
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------//
//
//-------------------------------------------------------------------------------------------------//
bool OrderOverviewUC::isCurrentBookingNotEqualToCurrentIterator( const basar::db::aspect::AccessorPropertyTable_YIterator currentBooking )
{
	bool execute = false;

	execute |= ( currentBooking.getString( "customersuppliername" ).length() <= 1 );
	execute |= ( currentBooking.getString( "customersupplierlocation" ).length() <= 1 );
	execute |= ( getManager().m_NarcoticsDM->getBackUpCurrentIterator().getString("customersuppliername") != currentBooking.getString( "customersuppliername" ) );
	execute |= ( getManager().m_NarcoticsDM->getBackUpCurrentIterator().getString( "customersupplierlocation" ) != currentBooking.getString( "customersupplierlocation" ) );
	execute |= ( getManager().m_NarcoticsDM->getBackUpCurrentIterator().getInt32( "customersupplierno" ) != currentBooking.getInt32( "customersupplierno" ) );

	return execute;
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
SYSTEM_EVENT_HANDLER_DEFINITION( OrderOverviewUC, SaveOrderChanges )
{
	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
	{
		basar::ConstString msg = "Event Save Order Changes caught in OrderOverviewUC.";
		LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	basar::db::aspect::AccessorPropertyTableRef saveOrderPropTab = getManager().m_NarcoticsDM->getAccNarCatalogPropTab();

	getManager().m_OrderOverviewVC->matchFromOrderData( saveOrderPropTab.begin() );
	
	AccessorPropertyTable_YIterator orderPosition = saveOrderPropTab.begin();
	
	if( !orderPosition.isEnd() && orderPosition.isContainedAndSet(constants::NEWTRANSACTIONTYPE) )
	{
		if(	orderPosition.getString(constants::NEWTRANSACTIONTYPE) == TransactionType::CUSTOMER_ORDER ||
			orderPosition.getString(constants::NEWTRANSACTIONTYPE) == TransactionType::SUPPLIER_RETURNS	)
		{
			basar::gui::tie::MessageBoxButtonEnum questionResult = basar::gui::tie::questionMsgBox(	getManager().m_OrderOverviewVC.getWidgetPtr(),
					QApplication::translate("OrderOverviewUC","Do you want a contributionvoucher being created when printprocess is running?").toLocal8Bit().data());

			if( questionResult == basar::gui::tie::ButtonYes )
			{
				basar::db::aspect::AccessorPropertyTable_YIterator yit = saveOrderPropTab.begin();

				while( !yit.isEnd() )
				{
					yit.setInt16( constants::ACKNOWLEDGEMENTSTATUS, constants::FOR_PRINT );
					yit++;
				}
			}
			else
			{
				basar::db::aspect::AccessorPropertyTable_YIterator yit = saveOrderPropTab.begin();
							
				while( !yit.isEnd() )
				{
					basar::Int16 ackStat = yit.getInt16( constants::ACKNOWLEDGEMENTSTATUS );
				
					if( ackStat == constants::FOR_PRINT )
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

					yit++;
				}
			}
		}
		else if (orderPosition.getString(constants::NEWTRANSACTIONTYPE) == TransactionType::DESTRUCTION)
		{
			// reset ack-status of destructions, so that these are not transferred to BfArm in any case!
			basar::db::aspect::AccessorPropertyTable_YIterator yit = saveOrderPropTab.begin();

			while( !yit.isEnd() )
			{
				yit.setInt16( constants::ACKNOWLEDGEMENTSTATUS, constants::INSERTED );
				yit++;
			}
		}
	}

	// IMPORTANTCHANGES
	ExecuteReturnType ret = getManager().m_NarcoticsDM->saveOrderChanges( saveOrderPropTab, m_FirstPosYit );
	basar::db::aspect::ExecuteResultInfo::RowNumber affectedRows = 0; // according to basar implementation 
																	  // affectedRows will be initialized with 0
	if( !ret.hasError() ){
		affectedRows = ret.getAffectedRows();
	}

	if( ExecuteReturnType::OK_BY_SELECT_BUT_NOTHING_SAVED_BY_MANIPULATION == affectedRows )
	{
		std::ostringstream os;
		switch( ret.getError() )
		{
			case ExecuteReturnType::OK_BY_SELECT_BUT_NOTHING_SAVED_BY_MANIPULATION:
									os	<< QApplication::translate( "OrderOverviewUC", 
										"No saving of Order Changes possible!").toLocal8Bit().data();
									break;
		
			case ExecuteReturnType::BASAR_EXCEPTION_THROWN:	
									os	<< QApplication::translate( "OrderOverviewUC", 
										"An error has occurred trying to save Order Changes!").toLocal8Bit().data();
									break;

			case ExecuteReturnType::ATTRIBUTES_UNSET: 	
									os	<< QApplication::translate( "OrderOverviewUC", 
										"Not all necessary attributes of the dataset are set!\n"
										"Please, check if all required data is filled in the textfields!").toLocal8Bit().data();
									break;

			default:	break;
		}
		return basar::appl::EventReturnStruct(basar::appl::HANDLER_ERROR, os.str().c_str());
	}

	getManager().m_OrderOverviewVC->matchToOrderData( getManager().m_NarcoticsDM->getAccNarCatalogPropTab().begin() );	
	// tableWidget has to be updated so that changes are also shown in booking overview	
	// ToDo: not needed anymore when transactiondate and transactiontime fields are removed from tableWidget
	basar::appl::EventReturnStruct retValue = basar::appl::SystemEventManager::getInstance().fire( "SearchOrderBooking" );

	return basar::appl::EventReturnStruct();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
SYSTEM_EVENT_HANDLER_DEFINITION( OrderOverviewUC, UsedBooking )
{
    rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "Event Current booking use caught in OrderOverviewUC.";
		LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	//	Independent on an existing deep copy in the narcotics module, 
	//	here via a bool value is set that there is no deep copy, 
	//	so that is made a new one by invoking the method deepCopy() of the narcotics module.
	//	Actually the deep copy has to be reset if the user do any cancel action, 
	//	but there is no adequate method in the view connectors to do this. 
	//	So it is done implicit if a new narcotic booking is intended to be processed.
	m_DeepCopied = false;
	getManager().m_currentBooking = getManager().m_OrderOverviewVC->getCurrentAccessorIterator();

	return basar::appl::EventReturnStruct();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
SYSTEM_EVENT_HANDLER_DEFINITION( OrderOverviewUC, DeleteBooking )
{
	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "Event Delete booking caught in OrderOverviewUC.";
		LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
	
	getManager().m_currentBooking = getManager().m_OrderOverviewVC->getCurrentAccessorIterator();
	getManager().m_NarcoticsDM->deepCopy( getManager().m_currentBooking ); // deep copy could always be done
	
	ExecuteReturnType ret = getManager().m_NarcoticsDM->strikeOut( getManager().m_currentBooking, true );
	basar::db::aspect::ExecuteResultInfo::RowNumber AFFECTED_ROWS = 0;	
	if( !ret.hasError() ){
		AFFECTED_ROWS = ret.getAffectedRows();
	}

	//! ßtodo magic numbers
	//IMPORTANTCHANGE
	if( ExecuteReturnType::BASAR_EXCEPTION_THROWN == ret.getError() || 
		( (AFFECTED_ROWS == ExecuteReturnType::OK_BY_SELECT_BUT_NOTHING_SAVED_BY_MANIPULATION) && !ret.hasError() ) )
	{
		std::ostringstream os;
		os	<< QApplication::translate( "OrderOverviewUC", "No strike out for narcotics booking with catalogid: ").toLocal8Bit().data() 
			<< getManager().m_currentBooking.getInt32(constants::CATALOGID)
			<< QApplication::translate( "OrderOverviewUC", " found!").toLocal8Bit().data();
		return basar::appl::EventReturnStruct(basar::appl::HANDLER_INFO, os.str().c_str()) ;
	}
	
	//! \todo magic number
	if( ExecuteReturnType::NEGATIVE_QTY_IN_STOCK == ret.getError() )
	{
		basar::Int32 quantitySave = basar::gui::tie::questionMsgBox(
							getManager().m_OrderOverviewVC.getWidgetPtr(),
							QApplication::translate( "OrderOverviewUC", "Your last action has caused a negative quantity in stock! Do you want to store this action / quantity?").toLocal8Bit().data());
		
		rollbackWorkaroundDeleting();
		
		if( quantitySave == basar::gui::tie::ButtonYes ){
			getManager().m_NarcoticsDM->strikeOut( getManager().m_currentBooking );
		}
		else{ 
			return basar::appl::EventReturnStruct();
		}
	}

	basar::appl::EventReturnStruct returnStruct = basar::appl::SystemEventManager::getInstance().fire( "SearchOrderBooking" );

	return returnStruct;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! \retval	true
//! \retval	otherwise false
/*!	\throw	no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
SYSTEM_EVENT_HANDLER_DEFINITION( OrderOverviewUC, FindCustomer )
{
	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "Event FindCustomer caught in OrderOverviewUC.";
		LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
    
    if( basar::FOR_INSERT != getManager().m_currentBooking.getInsertType() )
	{
		// back up the old values of the current Iterator in a ParameterList-object
		if( false == m_DeepCopied )
		{
			getManager().m_NarcoticsDM->deepCopy( getManager().m_currentBooking );
			m_DeepCopied = true;
		}
	}
    getManager().m_OrderOverviewVC->matchFromCurrentBooking( getManager().m_currentBooking );

	basar::VarString pl;
	pl.format(	"%s=%ld; %s=%ld;", 
				constants::BRANCHNO,
				basar::login::Manager::getInstance().getAreaID(),
				constants::CUSTOMERNO,
				getManager().m_currentBooking.getInt32( constants::CUSTOMERSUPPLIERNO ) );

	basar::db::aspect::AccessorPropertyTable_YIterator yit = getManager().m_CustomerDM->findCustomerByPattern( basar::cmnutil::ParameterList(pl) );
	if( !yit.isEnd() )
	{
		getManager().m_OrderOverviewVC->matchToCustomer( yit );
		return basar::appl::EventReturnStruct();
	}
	else{
		return basar::appl::EventReturnStruct( basar::appl::HANDLER_INFO, 
											   QApplication::translate( "OrderOverviewUC", "No Records for given customer number found!" ).toLocal8Bit().data() );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! \retval	true
//! \retval	otherwise false
/*!	\throw	no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
SYSTEM_EVENT_HANDLER_DEFINITION( OrderOverviewUC, FindBranchData )
{
	if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "Event findBranchData() in OrderOverviewUC.";
		LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings
	basar::db::aspect::AccessorPropertyTable_YIterator yit;

	// initialize branchDM - could be reason for INC-16008444 (connected to multi- threading: perhaps printthread shuts down branchDM before !?)
	getManager().m_BranchDM->init(	basar::login::Manager::getInstance().getAreaID(),
									getManager().m_Connection.getCurrentConnection() );
	
	yit = getManager().m_BranchDM->findBranch();
	if( !yit.isEnd() ){
		getManager().m_OrderOverviewVC->matchToBranch( yit );	
	}

	// shutdown
	getManager().m_BranchDM->shutdown();

	return basar::appl::EventReturnStruct();	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
SYSTEM_EVENT_HANDLER_DEFINITION( OrderOverviewUC, CheckTimeChanges )
{
	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "Event CheckTimeChanges caught in OrderOverviewUC.";
		LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	if( checkChangesApplied( constants::TRANSACTIONTIME ) ){
		return basar::appl::EventReturnStruct();
	} 
	else{
		return basar::appl::EventReturnStruct( basar::appl::HANDLER_INFO );
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\check if changes concerning fields customersupplierno and transactiondate have really been applied
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
bool OrderOverviewUC::checkChangesApplied( basar::I18nString fieldconstant )
{
	basar::db::aspect::AccessorPropertyTable_YIterator initialOrderDataIt = getManager().m_NarcoticsDM->getAccNarCatalogPropTab().begin();

	if( fieldconstant == constants::TRANSACTIONTIME ) 
	{
		basar::Int32 newTransactionTime = getManager().m_OrderOverviewVC->matchFromTransactionTime();
		if( newTransactionTime == initialOrderDataIt.getInt32( constants::TRANSACTIONTIME ) ){
			return false;
		}
	}
	if( fieldconstant == constants::CUSTOMERSUPPLIERNO ) 
	{
		basar::Int32 newCustomerSupplierNo = getManager().m_OrderOverviewVC->matchFromCustomerSupplierNo();
		if( newCustomerSupplierNo == initialOrderDataIt.getInt32( constants::CUSTOMERSUPPLIERNO ) ){
			return false;
		}
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! \no retval
/*!	\throw	no-throw */
/*! Workaround: clear m_currentBooking and match again, as iterator might have been destroyed due to rollback problems */
/////////////////////////////////////////////////////////////////////////////////////////////////////
void OrderOverviewUC::rollbackWorkaroundSaving()
{
	if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "Event rollbackWorkaroundSaving caught in OrderOverviewUC.";
		LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	// clear iterator
	getManager().m_currentBooking = basar::db::aspect::AccessorPropertyTable_YIterator();
	// reinitialize m_currentBooking
	getManager().m_currentBooking = getManager().m_OrderOverviewVC->getCurrentAccessorIterator();
	// refill iterator with recommended values
	getManager().m_OrderOverviewVC->matchFromCurrentBooking( getManager().m_currentBooking );
	checkCustomerSupplierData( getManager().m_currentBooking );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//! \no retval
/*!	\throw	no-throw */
/*!	\Workaround: clear m_currentBooking and match again, as iterator might have been destroyed due to rollback problems */
/////////////////////////////////////////////////////////////////////////////////////////////////////
void OrderOverviewUC::rollbackWorkaroundDeleting()
{	
	if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "Event rollbackWorkaroundDeleting caught in OrderOverviewUC.";
		LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	// clear iterator and match
	getManager().m_currentBooking = basar::db::aspect::AccessorPropertyTable_YIterator();
	getManager().m_currentBooking = getManager().m_OrderOverviewVC->getCurrentAccessorIterator();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool OrderOverviewUC::ftpCancelOrder( basar::Int32 contributionVoucherNo )
{
	basar::I18nString hostname;
	basar::I18nString user;
	basar::I18nString pw;
	basar::I18nString localTmpDirectory;
	basar::I18nString directoryname;

	basar::I18nString startDatetxt;
	basar::I18nString branchnotxt;

	basar::login::Manager::getInstance().getParaValue( UADM_CONTRIBUTIONVOUCHERDIR, directoryname	);				
	basar::login::Manager::getInstance().getParaValue( UADM_FTP_CITRIX_HOST, hostname			);
	basar::login::Manager::getInstance().getParaValue( UADM_FTP_CITRIX_USER, user				);	
	basar::login::Manager::getInstance().getParaValue( UADM_FTP_CITRIX_PW, pw					);	
	basar::login::Manager::getInstance().getParaValue( UADM_FTP_LOCAL_TMP, localTmpDirectory	);	

	branchnotxt.itos( basar::login::Manager::getInstance().getAreaID() );
	
	basar::login::Manager::getInstance().getParaValue( branchnotxt, UADM_STARTDATE, startDatetxt );

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
		
		throw basar::cmnutil::BasarInvalidStateException( excInfo );	
	}
	
	if( pw.empty() )
	{
		basar::cmnutil::ExceptionInfoStruct excInfo;
		excInfo.context = "PrintThread::init";
		excInfo .file =__FILE__;
		excInfo.line=__LINE__;
		excInfo.reason = "Parametervalue constants::FTP_CITRIX_PW is empty!!";
		
		throw basar::cmnutil::BasarInvalidStateException( excInfo );	
	}	
	
	if( localTmpDirectory.empty() )
	{
		basar::cmnutil::ExceptionInfoStruct excInfo;
		excInfo.context = "PrintThread::init";
		excInfo .file =__FILE__;
		excInfo.line=__LINE__;
		excInfo.reason = "Parametervalue constants::FTP_LOCAL_TMP is empty!!";
		
		throw basar::cmnutil::BasarInvalidStateException( excInfo );	
	}	

	getManager().m_BranchDM->init(	basar::login::Manager::getInstance().getAreaID(), 
											getManager().m_Connection.getCurrentConnection() );

	basar::db::aspect::AccessorPropertyTable_YIterator yitBranch = getManager().m_BranchDM->findBranch();

	basar::Int32 bgano = 0;

	if( !yitBranch.isNull() && !yitBranch.isEnd() ){
		bgano = yitBranch.getInt32( "bgano" );
	}

	getManager().m_BranchDM->shutdown();

	ContributionVoucherFTPTransfer ftpTrans( hostname, user, pw, directoryname, localTmpDirectory );

	return ftpTrans.cancelOrder( bgano, contributionVoucherNo, startDatetxt );
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
SYSTEM_EVENT_HANDLER_DEFINITION( OrderOverviewUC, PrintDissolveReceipt )
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "OrderOverviewUC::PrintDissolveReceipt()");

	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	getManager().m_NarcoticsDM->resetSearchIterator();
	getManager().m_OrderOverviewVC->matchFromSelection( getManager().m_NarcoticsDM->getSearchIterator() );
	basar::db::aspect::AccessorPropertyTable_YIterator	yitSearch = getManager().m_NarcoticsDM->getSearchIterator();

	basar::db::aspect::AccessorPropertyTableRef			prTabDissolveReceiptList;
	basar::db::aspect::AccessorPropertyTable_YIterator	yitBranch;

	yitSearch.setString( constants::TRANSACTIONTYPE, TransactionType::DESTRUCTION );
	yitSearch.setString( constants::DELETEDFLAG, "0" );
	yitSearch.setString( constants::PROCESSEDFLAG, "1" );

	prTabDissolveReceiptList = getManager().m_NarcoticsDM->findCatalogByPattern( yitSearch );

	if( prTabDissolveReceiptList.empty() )
	{
		return basar::appl::EventReturnStruct( basar::appl::HANDLER_INFO, 
			QApplication::translate("OrderOverviewUC","No bookings with transactiontype 'z' found for this order!").toLocal8Bit().data() );
	}

	// initialize branchDM
	getManager().m_BranchDM->init(	basar::login::Manager::getInstance().getAreaID(),
									getManager().m_Connection.getCurrentConnection() );
	yitBranch = getManager().m_BranchDM->findBranch();
	
	DissolveReceiptPrintData printData( "DissolveReceipt.ps", "DissolveReceipt", 
										prTabDissolveReceiptList, yitBranch, yitSearch );
	
	basar::gui::qt::PrintPreview preview( printData, 0, 
					QApplication::translate( "OrderOverviewUC", "Narcotics - Dissolve Receipt" ).toLocal8Bit().data() );
	preview.setOutputFormats( static_cast<basar::gui::qt::OutputEnum>( basar::gui::qt::PDF | basar::gui::qt::WIN_PRINT ) );
	preview.show();

	// shutdown
	getManager().m_BranchDM->shutdown();
	return basar::appl::EventReturnStruct();
}

} //namespace useCase
} //namespace narcotics