//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  implemtation of use case "report print"
 *  \author	Thomas Hörath
 *  \date   11.05.2006
 */
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "reportprint.h"
#include "narcoticsappl.h"
#include "inventoryprintdata.h"
#include "dissolvereceiptprintdata.h"
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

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
ReportPrintUC::ReportPrintUC()
{
	registerEventHandlers( true );
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
ReportPrintUC::~ReportPrintUC()
{
	registerEventHandlers( false );
}

//-------------------------------------------------------------------------------------------------//
// macro for registrating and deregistrating EventHandlers!
// 1st para: name of UC
// 2nd para: name of handle-Method
BEGIN_HANDLERS_REGISTRATION(ReportPrintUC)
	SYSTEM_EVENT_HANDLER_REGISTRATION( ReportPrintUC, PrintCountList		)
	SYSTEM_EVENT_HANDLER_REGISTRATION( ReportPrintUC, PrintStockList		)
    SYSTEM_EVENT_HANDLER_REGISTRATION( ReportPrintUC, PrintStockListQuick	)
	SYSTEM_EVENT_HANDLER_REGISTRATION( ReportPrintUC, PrintDissolveReceipt	)
END_HANDLERS_REGISTRATION()

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
basar::appl::EventReturnType ReportPrintUC::run()
{
	return basar::appl::EventReturnStruct(); //std with ret = OK, msg = ""
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
SYSTEM_EVENT_HANDLER_DEFINITION( ReportPrintUC, PrintCountList )
{
	//pseudo-dialog
	getManager().m_InputDateVc->init();
	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	QApplication::setOverrideCursor( QCursor(Qt::WaitCursor) );
	
	if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "Event print count list caught in ReportPrintUC.";
		LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
	
	AccessorPropertyTableRef prTabInventoryCountList
		= getManager().m_NarcoticsDM->findData4InventoryCountPrint();

	if( prTabInventoryCountList.empty() )
	{
		QApplication::restoreOverrideCursor();
		return basar::appl::EventReturnStruct( basar::appl::HANDLER_INFO, 
											   QApplication::translate( "ReportPrintUC", "No Inventory Data found for counting list!" ).toLocal8Bit().data() );
	}
	
	InventoryPrintData printData( "StockCountList", "StockCountList", prTabInventoryCountList, true );

	basar::gui::qt::PrintPreview preview( printData, 0, 
					QApplication::translate( "ReportPrintUC", "Narcotics - Counting List" ).toLocal8Bit().data() );
	
	preview.setOutputFormats( basar::gui::qt::PDF );
	
	QApplication::restoreOverrideCursor();
		
	preview.show();

	//pseudo-dialog
	getManager().m_InputDateVc.reset();	
	return basar::appl::EventReturnStruct();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
SYSTEM_EVENT_HANDLER_DEFINITION( ReportPrintUC, PrintStockList )
{
	//pseudo-dialog
	getManager().m_InputDateVc->init();
	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	QApplication::setOverrideCursor( QCursor(Qt::WaitCursor) );
		
	if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "Event print stock list caught in ReportPrintUC.";
		LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
	
	getManager().m_NarcoticsDM->resetSearchIterator();
	QApplication::restoreOverrideCursor();
	getManager().m_InputDateVc->init();
	basar::cmnutil::DateTime dt;
	dt.setDate( dt.getCurrent().getDate() );
	dt.addDays(-1);
	getManager().m_InputDateVc->setDate( dt );
	getManager().m_InputDateVc->show();
	QApplication::setOverrideCursor( QCursor(Qt::WaitCursor) );

	AccessorPropertyTable_YIterator yit = getManager().m_NarcoticsDM->getSearchIterator();

	getManager().m_InputDateVc->matchFromSelection( yit );

	AccessorPropertyTableRef prTabInventoryStockList
		= getManager().m_NarcoticsDM->findData4InventoryCountPrint();

	if( prTabInventoryStockList.empty() )
	{
		QApplication::restoreOverrideCursor();
		return basar::appl::EventReturnStruct( basar::appl::HANDLER_INFO, 
											  QApplication::translate( "ReportPrintUC", "No Inventory Data found for stock list!" ).toLocal8Bit().data() );
	}

//--- reading of stocks:
//	 print contribution vouchers for ALL articles of current date!

	basar::I18nString where;
	where.format( "branchno=%d; transactiondate=%s; transactiontime=%ld; articleno;", 
					basar::login::Manager::getInstance().getAreaID(),
					yit.getString( constants::FROMDATE ).c_str(), 235959 );

	basar::db::aspect::AccessorPropertyTable_YIterator yit1 = 
			basar::db::aspect::Manager::getInstance().createAccessorYIterator( where );

	yit = prTabInventoryStockList.begin();

	for( ; !yit.isEnd(); yit++ )
	{
		where.format( "%ld", yit.getInt32( constants::ARTICLENO ) );
		yit1.setString( constants::ARTICLENO, where );
		AccessorPropertyTable_YIterator yitStock = getManager().m_NarcoticsDM->findPreviousBooking( yit1 );

		if( yitStock.isEnd() ){
			yit.setInt32( "qtyinstock", 0 );
		}
		else{
			yit.setInt32( "qtyinstock", yitStock.getInt32( "qtyinstock" ) );
		}
	}

	InventoryPrintData printData( "StockCountList", "StockCountList", prTabInventoryStockList, false );

	basar::gui::qt::PrintPreview preview( printData, 0, 
					QApplication::translate( "ReportPrintUC", "Narcotics - Stock List" ).toLocal8Bit().data() );
	
	preview.setOutputFormats( basar::gui::qt::PDF );
	
	QApplication::restoreOverrideCursor();

	preview.show();

	//pseudo-dialog
	getManager().m_InputDateVc.reset();	
	return basar::appl::EventReturnStruct();
}

//-------------------------------------------------------------------------------------------------//
SYSTEM_EVENT_HANDLER_DEFINITION( ReportPrintUC, PrintStockListQuick )
{
	//pseudo-dialog
	getManager().m_InputDateVc->init();
	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	QApplication::setOverrideCursor( QCursor(Qt::WaitCursor) );
		
	if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "Event print stock list caught in ReportPrintUC.";
		LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
	
	getManager().m_NarcoticsDM->resetSearchIterator();
	QApplication::restoreOverrideCursor();
	getManager().m_InputDateVc->init();
	basar::cmnutil::DateTime dt;
	dt.setDate( dt.getCurrent().getDate() );
	dt.addDays(-1);
	getManager().m_InputDateVc->setDate( dt );
	getManager().m_InputDateVc->show();
	QApplication::setOverrideCursor( QCursor(Qt::WaitCursor) );
	AccessorPropertyTable_YIterator yit = getManager().m_NarcoticsDM->getSearchIterator();

	getManager().m_InputDateVc->matchFromSelection( yit );

    basar::VarString where;
	where.format( "branchno=%d; transactiondate=%s; transactiontime=%ld; articleno;", basar::login::Manager::getInstance().getAreaID(), yit.getString( constants::FROMDATE ).c_str(), 235959 );

    basar::db::aspect::AccessorPropertyTable_YIterator yit1 = basar::db::aspect::Manager::getInstance().createAccessorYIterator( static_cast<basar::I18nString>( where ) );

	AccessorPropertyTableRef prTabInventoryStockList = getManager().m_NarcoticsDM->findData4InventoryStockPrint( yit1 );

	if( prTabInventoryStockList.empty() )
	{
		QApplication::restoreOverrideCursor();
		return basar::appl::EventReturnStruct( basar::appl::HANDLER_INFO, 
											  QApplication::translate( "ReportPrintUC", "No Inventory Data found for stock list!" ).toLocal8Bit().data() );
	}

	InventoryPrintData printData( "StockCountList", "StockCountList", prTabInventoryStockList, false );

	basar::gui::qt::PrintPreview preview( printData, 0, 
					QApplication::translate( "ReportPrintUC", "Narcotics - Stock List" ).toLocal8Bit().data() );
	
	preview.setOutputFormats( basar::gui::qt::PDF );
	
	QApplication::restoreOverrideCursor();

	preview.show();

	//pseudo-dialog
	getManager().m_InputDateVc.reset();	
	return basar::appl::EventReturnStruct();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
SYSTEM_EVENT_HANDLER_DEFINITION( ReportPrintUC, PrintDissolveReceipt )
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "ReportPrintUC::PrintDissolveReceipt()");

	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	basar::db::aspect::AccessorPropertyTable_YIterator	yitSearch;
	basar::db::aspect::AccessorPropertyTableRef			prTabDissolveReceiptList;
	basar::db::aspect::AccessorPropertyTable_YIterator	yitBranch;

	// print contribution vouchers for ALL articles of current date!
	getManager().m_NarcoticsDM->resetSearchIterator();
	getManager().m_InputDateVc->init();
	getManager().m_InputDateVc->show();
	yitSearch = getManager().m_NarcoticsDM->getSearchIterator();

	yitSearch.setString( constants::TRANSACTIONTYPE, TransactionType::DESTRUCTION );
	
	getManager().m_InputDateVc->matchFromSelection( yitSearch );
	yitSearch.setString( constants::DELETEDFLAG, "0" );
	yitSearch.setString( constants::PROCESSEDFLAG, "1" );

	prTabDissolveReceiptList = getManager().m_NarcoticsDM->findCatalogByPattern( yitSearch );

	if( prTabDissolveReceiptList.empty() )
	{
		basar::DateTime printDate( static_cast<basar::Int32>(yitSearch.getString(constants::TODATE).stol()), 0 );
		basar::VarString msg; 
		msg.format( QApplication::translate("ReportPrintUC",
			"No bookings with transactiontype 'z' found!\nFor this date %s, no data available for printing dissolve receipt!").toLocal8Bit().data(), 
			printDate.toStrDate().c_str() );
		return basar::appl::EventReturnStruct( basar::appl::HANDLER_INFO, msg );
	}

	// initialize branchDM
	getManager().m_BranchDM->init(	basar::login::Manager::getInstance().getAreaID(),
									getManager().m_Connection.getCurrentConnection() );
	yitBranch = getManager().m_BranchDM->findBranch();
	
	printDissolveReceipt( prTabDissolveReceiptList, yitBranch, yitSearch );
	// shutdown
	getManager().m_BranchDM->shutdown();
	getManager().m_InputDateVc.reset();
	return basar::appl::EventReturnStruct();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
void ReportPrintUC::printDissolveReceipt( AccessorPropertyTableRef prTabDissolveReceiptList, AccessorPropertyTable_YIterator yitBranch, AccessorPropertyTable_YIterator yitDate )
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "ReportPrintUC::printDissolveReceipt()");
	
	DissolveReceiptPrintData printData( "DissolveReceipt.ps", "DissolveReceipt", prTabDissolveReceiptList, yitBranch, yitDate );
	
	basar::gui::qt::PrintPreview preview( printData, 0, 
		QApplication::translate( "ReportPrintUC", "Narcotics - Dissolve Receipt" ).toLocal8Bit().data() );
	preview.setOutputFormats( static_cast<basar::gui::qt::OutputEnum>(basar::gui::qt::PDF | basar::gui::qt::WIN_PRINT) );
	preview.show();
}

} //namespace useCase
} //namespace narcotics