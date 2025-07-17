//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  implemtation of use case "deleted booking"
 *  \author	Roland Kiefert
 *  \date   28.03.2006
 */
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "deletedbookings.h"
#include "narcoticsappl.h"
#include "loggerpool.h"

//-------------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------------//
namespace narcotics 
{
namespace useCase 
{

using basar::db::aspect::AccessorPropertyTable_YIterator;
using basar::db::aspect::AccessorPropertyTableRef;
using basar::VarString;
using basar::gui::tie::questionMsgBox;
using basar::Int32;

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
DeletedBookingsUC::DeletedBookingsUC()
{
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
DeletedBookingsUC::~DeletedBookingsUC()
{
}

//-------------------------------------------------------------------------------------------------//
// macro for registrating and deregistrating EventHandlers!
// 1st para: name of UC
// 2nd para: name of handle-Method
BEGIN_HANDLERS_REGISTRATION( DeletedBookingsUC )
	SYSTEM_EVENT_HANDLER_REGISTRATION( DeletedBookingsUC, SearchDeletedBookings )
	SYSTEM_EVENT_HANDLER_REGISTRATION( DeletedBookingsUC, RestoreDeletedBookings )
END_HANDLERS_REGISTRATION()

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
basar::appl::EventReturnType DeletedBookingsUC::run()
{
	registerEventHandlers( true );

	getManager().m_DeletedDM.init	(	basar::login::Manager::getInstance().getAreaID(), 
										getManager().m_Connection.getCurrentConnection(),
										basar::login::Manager::getInstance().getUserName()
									);
	getManager().m_DeletedBookingsVC->init( getManager().m_DeletedDM.getAccNarCatalogPropTab() );
	getManager().m_DeletedBookingsVC->show();
		
	getManager().m_DeletedBookingsVC->shutdown();
	getManager().m_DeletedBookingsVC.reset();
	
	getManager().m_currentBooking.clear();
	getManager().m_DeletedDM.shutdown();

	registerEventHandlers( false );

	return basar::appl::EventReturnStruct(); //std with ret = OK, msg = ""
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
SYSTEM_EVENT_HANDLER_DEFINITION( DeletedBookingsUC, SearchDeletedBookings )
{
	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "Event search caught in DeletedBookingsUC.";
		LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
	
	getManager().m_DeletedDM.resetSearchIterator();
	getManager().m_DeletedBookingsVC->matchFromSelection( getManager().m_DeletedDM.getSearchIterator() );

	basar::I18nString msg = "";
	basar::appl::EventReturnEnum ret = basar::appl::HANDLER_OK; 

	if( getManager().m_DeletedDM.getSearchIterator().isContainedAndSet( constants::ARTICLECODE ) )
	{
		getManager().m_DeletedDM.getSearchIterator().setString( constants::ARTICLECODE, 
			removeLeadingZeros( getManager().m_DeletedDM.getSearchIterator().getString( constants::ARTICLECODE ) ) );
		getManager().m_ArticleDM.init(	basar::login::Manager::getInstance().getAreaID(), getManager().m_Connection.getCurrentConnection() );
		basar::db::aspect::AccessorPropertyTable_YIterator yitArticle = getManager().m_ArticleDM.findArticleByPattern( getManager().m_DeletedDM.getSearchIterator() );
		
		if( yitArticle.isEnd() )
		{
			ret = basar::appl::HANDLER_INFO;
			msg = QApplication::translate( "GoodsTransactionUC", "Given articleno is not valid!" ).toLocal8Bit().data();
			getManager().m_DeletedDM.getAccNarCatalogPropTab().clear();

			return basar::appl::EventReturnStruct( ret, msg );
		}
		else{
			getManager().m_DeletedDM.getSearchIterator().setString( constants::ARTICLENO, yitArticle.getSQLString( constants::ARTICLENO ) );
		}
		getManager().m_ArticleDM.shutdown();
	}

	getManager().m_DeletedDM.getSearchIterator().setString( constants::DELETEDFLAG, constants::ISDELETED );
	
	AccessorPropertyTableRef table = getManager().m_DeletedDM.findCatalogByPattern( getManager().m_DeletedDM.getSearchIterator() );
	
	if( table.empty() )
	{
		ret = basar::appl::HANDLER_INFO;
		msg = QApplication::translate( "DeletedBookingsUC", 
										"No records for search criteria found!" ).toLocal8Bit().data();
	}
	
	getManager().m_DeletedBookingsVC->matchToDeletedBookings();

	return basar::appl::EventReturnStruct( ret, msg );
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
SYSTEM_EVENT_HANDLER_DEFINITION( DeletedBookingsUC, RestoreDeletedBookings )
{
	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "Event restore caught in DeletedBookingsUC.";
		LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	AccessorPropertyTable_YIterator it = getManager().m_DeletedBookingsVC->getSelectedFirstIterator();
	// declares Iterator position for rematching
	basar::Int32 position = it.getRowNumber(); 

	while (false == it.isEnd())
	{
		if (it.getInt16(constants::ACKNOWLEDGEMENTSTATUS) != constants::DELIVERY_NOTE_COPY_CREATED || it.getString( constants::TRANSACTIONTYPE ) == TransactionType::SUPPLIER_RETURNS)
		{
			getManager().m_currentBooking = it;
			// deep copy is done in DeletedDM as m_YIteratorBackUp is destroyed at the beginning of the function restoreStrikeOut due to rollback problems

			if (getManager().m_DeletedDM.restoreStrikedOut(it, true).getError() == ExecuteReturnType::NEGATIVE_QTY_IN_STOCK) //! \todo magic number -5
			{
				// clear and rematch m_currentBooking
				rollbackWorkaroundRestoring(position);

				if (basar::gui::tie::ButtonYes == basar::gui::tie::questionMsgBox(
						getManager().m_DeletedBookingsVC.getWidgetPtr(),
						QApplication::translate( "DeletedBookingsUC", "Your last action has caused a negative quantity in stock! Do you want to store this action / quantity?" ).toLocal8Bit().data()))
				{
					getManager().m_NarcoticsDM->restoreStrikedOut(getManager().m_currentBooking);
				}
			}
		}
		it = getManager().m_DeletedBookingsVC->getSelectedNextAccIterator();
		// declares Iterator position for rematching
		position = it.getRowNumber(); 
	}

	AccessorPropertyTableRef table = getManager().m_DeletedDM.findCatalogByPattern( getManager().m_DeletedDM.getSearchIterator() );
	
	getManager().m_DeletedBookingsVC->matchToDeletedBookings();

	return basar::appl::EventReturnStruct();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw	no-throw */
/*!	\Workaround: clear m_currentBooking and match again, as iterator might have been destroyed due to rollback problems */
//-------------------------------------------------------------------------------------------------//
void DeletedBookingsUC::rollbackWorkaroundRestoring( basar::Int32 position )
{	
	if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "Event rollbackWorkaroundRestoring caught in DeletedBookingsUC.";
		LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	// clear iterator and match again
	getManager().m_currentBooking = basar::db::aspect::AccessorPropertyTable_YIterator();
	getManager().m_currentBooking = getManager().m_DeletedBookingsVC->getCurrentAccessorIterator( position );
}

} //namespace useCase
} //namespace narcotics