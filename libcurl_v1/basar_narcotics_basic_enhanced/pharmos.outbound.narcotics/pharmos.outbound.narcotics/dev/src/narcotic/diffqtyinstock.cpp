//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  implemtation of use case "booking catalog"
 *  \author	Bjoern Bischof
 *  \date   28.02.2006
 */
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "diffqtyinstockuc.h"
#include "narcoticsappl.h"
#include "loggerpool.h"

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
DiffQtyInStockUC::DiffQtyInStockUC()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
DiffQtyInStockUC::~DiffQtyInStockUC()
{
}

//-------------------------------------------------------------------------------------------------//
// macro for registrating and deregistrating EventHandlers!
// 1st para: name of UC
// 2nd para: name of handle-Method
BEGIN_HANDLERS_REGISTRATION( DiffQtyInStockUC )
	SYSTEM_EVENT_HANDLER_REGISTRATION( DiffQtyInStockUC, UpdateDiffQtyInStock )
	SYSTEM_EVENT_HANDLER_REGISTRATION( DiffQtyInStockUC, ShowComparingBookings )
END_HANDLERS_REGISTRATION()

/////////////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw	no-throw
/////////////////////////////////////////////////////////////////////////////////////////////////////
basar::appl::EventReturnType DiffQtyInStockUC::run()
{
	registerEventHandlers( true );

	getManager().m_NarcoticsDM->init(	basar::login::Manager::getInstance().getAreaID(), 
										getManager().m_Connection.getCurrentConnection(), 
										basar::login::Manager::getInstance().getUserName() );
	
	getManager().m_DiffQtyInStockVC->init( getManager().m_NarcoticsDM->getAccDiffQtyInStock() );
	getManager().m_NarcoticsDM->findDifferencesToOriginal();
	getManager().m_DiffQtyInStockVC->matchToData();

	getManager().m_DiffQtyInStockVC->show();

	getManager().m_DiffQtyInStockVC->shutdown();
	getManager().m_DiffQtyInStockVC.reset();
	
	getManager().m_currentBooking.clear();
	getManager().m_NarcoticsDM->shutdown();

	registerEventHandlers( false );

	return basar::appl::EventReturnStruct(); //std with ret = OK, msg = ""
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw no-throw
/////////////////////////////////////////////////////////////////////////////////////////////////////
SYSTEM_EVENT_HANDLER_DEFINITION( DiffQtyInStockUC, ShowComparingBookings )
{
	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "Event show caught in DiffQtyInStockUC.";
		LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	getManager().m_CompareBookingsUC.m_invokeContextDiffQtyInStock = true;
	getManager().m_CompareBookingsUC.run();
	getManager().m_CompareBookingsUC.m_invokeContextDiffQtyInStock = false;

	return basar::appl::EventReturnStruct();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw no-throw
/////////////////////////////////////////////////////////////////////////////////////////////////////
SYSTEM_EVENT_HANDLER_DEFINITION( DiffQtyInStockUC, UpdateDiffQtyInStock )
{
	BLOG_TRACE_METHOD( LoggerPool::loggerUseCases, "DiffQtyInStockUC::UpdateDiffQtyInStock" );

	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	basar::appl::EventReturnStruct ret;
	basar::db::aspect::ExecuteResultInfo resInfo;

	if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "Event update caught in DiffQtyInStockUC.";
		LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
	
	resInfo = getManager().m_NarcoticsDM->updateDiffQtyInStock();
	if( resInfo.hasError() )
	{
		ret.ret = basar::appl::HANDLER_INFO;
		ret.message = QApplication::translate("DiffQtyInStockUC", "The dialog/table couldn't be updated!").toLocal8Bit().data();
	}
	getManager().m_NarcoticsDM->findDifferencesToOriginal();

	getManager().m_DiffQtyInStockVC->matchToData();

	return ret;
}

} //namespace useCase
} //namespace narcotics