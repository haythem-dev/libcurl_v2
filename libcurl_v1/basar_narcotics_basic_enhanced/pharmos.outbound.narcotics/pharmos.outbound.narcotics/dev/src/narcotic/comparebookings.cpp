//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  implementation of use case "incoming goods"
 *  \author	Roland Kiefert
 *  \date   14.02.2006
 */
//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "comparebookings.h"
#include "narcoticsappl.h"
#include "loggerpool.h"

//-------------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------------//
namespace narcotics 
{
namespace useCase 
{

using basar::VarString;
using basar::Int32;
using basar::db::aspect::AccessorPropertyTable_YIterator;
using basar::db::aspect::AccessorPropertyTableRef;

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
CompareBookingsUC::CompareBookingsUC()
: m_invokeContextDiffQtyInStock ( false )
{
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
CompareBookingsUC::~CompareBookingsUC()
{
}

//-------------------------------------------------------------------------------------------------//
// macro for registrating and deregistrating EventHandlers!
// 1st para: name of UC
// 2nd para: name of handle-Method
BEGIN_HANDLERS_REGISTRATION( CompareBookingsUC )
	SYSTEM_EVENT_HANDLER_REGISTRATION( CompareBookingsUC, SearchCompareBookings				)
	SYSTEM_EVENT_HANDLER_REGISTRATION( CompareBookingsUC, RebuildNarcoticsBooking			)
	SYSTEM_EVENT_HANDLER_REGISTRATION( CompareBookingsUC, DeleteNarcoticsBooking			)
	SYSTEM_EVENT_HANDLER_REGISTRATION( CompareBookingsUC, SearchArticleNoWithDiffQtyInStock )
END_HANDLERS_REGISTRATION()

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
basar::appl::EventReturnType CompareBookingsUC::run()
{
	registerEventHandlers( true );

	getManager().m_NarcoticsGroupDM->init	( getManager().m_Connection.getCurrentConnection() );
	getManager().m_NarcoticsDM->init		( basar::login::Manager::getInstance().getAreaID(), 
											  getManager().m_Connection.getCurrentConnection(), 
											  basar::login::Manager::getInstance().getUserName()
											);
	getManager().m_OriginalDM.init			( basar::login::Manager::getInstance().getAreaID(), 
											  getManager().m_Connection.getCurrentConnection() 
											);

	getManager().m_CompareBookingsVC->init( getManager().m_NarcoticsGroupDM->getAccNarCatalogPropTab(), 
											  getManager().m_NarcoticsDM->getAccNarCatalogPropTab(),
											  getManager().m_OriginalDM.getAccOrgCatalogPropTab(),
											  getManager().m_NarcoticsDM->getAccDiffQtyInStock(),
											  getManager().m_NarcoticsDM->getAccArticleNosDiffQtyInStock(),
											  m_invokeContextDiffQtyInStock
											  );
	
	getManager().m_CompareBookingsVC->show();

	getManager().m_CompareBookingsVC->shutdown();
	getManager().m_CompareBookingsVC.reset();
	
	getManager().m_currentBooking.clear();
	getManager().m_OriginalDM.shutdown();
	getManager().m_NarcoticsDM->shutdown();
	getManager().m_NarcoticsGroupDM->shutdown();

	registerEventHandlers( false );

	return basar::appl::EventReturnStruct(); //std with ret = OK, msg = ""
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
SYSTEM_EVENT_HANDLER_DEFINITION( CompareBookingsUC, SearchCompareBookings )
{
	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "Event search caught in CompareBookingsUC.";
		LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	//-----------------------------------------------//
	// to search / match the article specific data
	//-----------------------------------------------//
	getManager().m_NarcoticsDM->resetSearchIterator();
	getManager().m_CompareBookingsVC->matchFromSelection( getManager().m_NarcoticsDM->getSearchIterator() );
	AccessorPropertyTable_YIterator yit = getManager().m_NarcoticsDM->getSearchIterator();

	if( yit.isContainedAndSet( constants::ARTICLECODE ) )
	{
		yit.setString( constants::ARTICLECODE, removeLeadingZeros(yit.getString(constants::ARTICLECODE)) );
		getManager().m_ArticleDM.init( basar::login::Manager::getInstance().getAreaID(), getManager().m_Connection.getCurrentConnection() );
		basar::db::aspect::AccessorPropertyTable_YIterator yitArticle = getManager().m_ArticleDM.findArticleByPattern( yit );

		if( yitArticle.isEnd() )
		{
			basar::gui::tie::infoMsgBox(
								getManager().m_CompareBookingsVC.getWidgetPtr(),
								QApplication::translate( "GoodsTransactionUC", "Given articleno is not valid!" ).toLocal8Bit().data(),
								QApplication::translate( "CompareBookingsUC", "Narcotics" ).toLocal8Bit().constData() );

			getManager().m_NarcoticsDM->getAccNarCatalogPropTab().clear();
			getManager().m_OriginalDM.getAccOrgCatalogPropTab().clear();
			getManager().m_ArticleDM.shutdown();
		
			return basar::appl::EventReturnStruct( basar::appl::HANDLER_ERROR, QApplication::translate( "GoodsTransactionUC", "Given articleno is not valid!" ).toLocal8Bit().data());
		}
		else
		{
			yit.setString( constants::ARTICLENO, yitArticle.getSQLString( constants::ARTICLENO ) );
			getManager().m_ArticleDM.shutdown();
		}
	}

	Int32 articleNo = atoi( yit.getString( constants::ARTICLENO ).c_str() );
	
	AccessorPropertyTable_YIterator yitExecute 
		= getManager().m_NarcoticsGroupDM->findStockLocationByArticleNo( articleNo, basar::login::Manager::getInstance().getAreaID() );
	
	if( yitExecute.isEnd() )
	{
		std::ostringstream os;
		os	<< QApplication::translate( "CompareBookingsUC", "No records for article number: " ).toLocal8Bit().constData() 
			<< articleNo 
			<< QApplication::translate( "CompareBookingsUC", " found!" ).toLocal8Bit().constData();
		
		basar::gui::tie::infoMsgBox( getManager().m_CompareBookingsVC.getWidgetPtr(),
									 os.str().c_str(),
									 QApplication::translate( "CompareBookingsUC", "Narcotics" ).toLocal8Bit().constData() );

		getManager().m_NarcoticsDM->getAccNarCatalogPropTab().clear();
		getManager().m_OriginalDM.getAccOrgCatalogPropTab().clear();
	
		return basar::appl::EventReturnStruct( basar::appl::HANDLER_ERROR, QApplication::translate( "GoodsTransactionUC", "Given articleno is not valid!" ).toLocal8Bit().data());
	}
	getManager().m_CompareBookingsVC->matchToNarcoticData();

	//---------------------------------------------------//
	// to search / match the qtyinstock difference 
	//---------------------------------------------------//
	yitExecute = getManager().m_NarcoticsDM->findLatestDiffQtyByArticleNo( articleNo, basar::login::Manager::getInstance().getAreaID() );
	getManager().m_CompareBookingsVC->matchToArticleDifference();

	//----------------------------------//
	// to search / match the catalog data
	//----------------------------------//
	getManager().m_NarcoticsDM->getSearchIterator().setString( constants::PROCESSEDFLAG, constants::ISPROCESSED );
	getManager().m_NarcoticsDM->getSearchIterator().setString( constants::DELETEDFLAG, constants::ISNOTDELETED );
	AccessorPropertyTableRef table = 
		getManager().m_NarcoticsDM->findCatalogByPattern( getManager().m_NarcoticsDM->getSearchIterator() );
	if( table.empty() )
	{
		basar::gui::tie::infoMsgBox(
							getManager().m_CompareBookingsVC.getWidgetPtr(),
							QApplication::translate( "CompareBookingsUC", "No records for search criteria in narcotic bookings found!" ).toLocal8Bit().constData(),
							QApplication::translate( "CompareBookingsUC", "Narcotics").toLocal8Bit().constData());
	}
	getManager().m_CompareBookingsVC->matchToNarcoticBookings();

	//----------------------------------//
	// to search / match the original data
	//----------------------------------//
	table = getManager().m_OriginalDM.findCatalogByPattern( getManager().m_NarcoticsDM->getSearchIterator() );
	if( table.empty() )
	{
		basar::gui::tie::infoMsgBox(
							getManager().m_CompareBookingsVC.getWidgetPtr(),
							QApplication::translate( "CompareBookingsUC", "No records for search criteria in Pharmos bookings found!" ).toLocal8Bit().constData(),
							QApplication::translate( "CompareBookingsUC", "Narcotics" ).toLocal8Bit().constData());
	}
	getManager().m_CompareBookingsVC->matchToOriginalBookings();

	return basar::appl::EventReturnStruct();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
SYSTEM_EVENT_HANDLER_DEFINITION( CompareBookingsUC, RebuildNarcoticsBooking )
{
	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "Event rebuild caught in CompareBookingsUC.";
		LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	getManager().m_DeletedBookingsVC.setParent(getManager().m_CompareBookingsVC.getWidgetPtr());
	
	getManager().m_DeletedBookingsUC.run();

	if( getManager().m_CompareBookingsVC->isSelected() ){
		basar::appl::SystemEventManager::getInstance().fire( "SearchCompareBookings" );
	}

	return basar::appl::EventReturnStruct();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
SYSTEM_EVENT_HANDLER_DEFINITION( CompareBookingsUC, DeleteNarcoticsBooking )
{
	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "Event delete caught in CompareBookingsUC.";
		LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	getManager().m_currentBooking = getManager().m_CompareBookingsVC->getCurrentAccessorIterator();
	
	 basar::db::aspect::AccessorPropertyTable_YIterator yitCloned = // welchen Sinn hatte der Aufruf zuvor, wenn Yit (Return-Wert) dann nicht abgegriffen wurde ???
		getManager().m_NarcoticsDM->deepCopy( getManager().m_currentBooking );
	
	//! \todo magic number
	//magic number has been removed here: NEGATIVE_QTY_IN_STOCK	replaced -5
	if( ExecuteReturnType::NEGATIVE_QTY_IN_STOCK == ( getManager().m_NarcoticsDM->strikeOut( getManager().m_currentBooking, true ) ).getError() ) //! \todo magic number
	{
		Int32 quantitySave = basar::gui::tie::questionMsgBox(
							getManager().m_CompareBookingsVC.getWidgetPtr(),
							QApplication::translate( "CompareBookingsUC", "Your last action has caused a negative quantity in stock! Do you want to store this action / quantity?").toLocal8Bit().data() );
			
		if( quantitySave == basar::gui::tie::ButtonYes ){
			getManager().m_NarcoticsDM->strikeOut( yitCloned );
		}
	}

	if( getManager().m_CompareBookingsVC->isSelected() ){
		basar::appl::SystemEventManager::getInstance().fire( "SearchCompareBookings" );
	}

	return basar::appl::EventReturnStruct();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
SYSTEM_EVENT_HANDLER_DEFINITION( CompareBookingsUC, SearchArticleNoWithDiffQtyInStock )
{
	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "Event search article number with differenz qty in stock caught in CompareBookingsUC.";
		LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	AccessorPropertyTableRef table = getManager().m_NarcoticsDM->findDifferencesToOriginalArticleNosOnly();
	if( table.empty() ){
		return basar::appl::EventReturnStruct( basar::appl::HANDLER_INFO, 
												QApplication::translate("CompareBookingsUC", "no articles with different quantity in stock found!").toLocal8Bit().data() );
	}

	getManager().m_CompareBookingsVC->matchToArticleNosWithDiffQtyInStock();

	return basar::appl::EventReturnStruct();
}

} //namespace useCase
} //namespace narcotics