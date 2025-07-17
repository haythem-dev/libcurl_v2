//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  implemtation of use case "half yearly report"
 *  \author	Roland Kiefert
 *  \date   28.03.2006
 */
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "narcoticsappl.h"
#include "halfyearlyreportuc.h"
#include "openorderprintdata.h"
#include "loggerpool.h"

//-------------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------------//
namespace narcotics 
{
namespace useCase 
{

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
HalfYearlyReportUC::HalfYearlyReportUC()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "HalfYearlyReportUC::HalfYearlyReportUC");
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
HalfYearlyReportUC::~HalfYearlyReportUC()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "HalfYearlyReportUC::~HalfYearlyReportUC");
}

//-------------------------------------------------------------------------------------------------//
// macro for registrating and deregistrating EventHandlers!
// 1st para: name of UC
// 2nd para: name of handle-Method
BEGIN_HANDLERS_REGISTRATION( HalfYearlyReportUC )
	SYSTEM_EVENT_HANDLER_REGISTRATION( HalfYearlyReportUC, ShowHalfYearlyReport )
	SYSTEM_EVENT_HANDLER_REGISTRATION( HalfYearlyReportUC, PrintHalfYearlyReport )
	SYSTEM_EVENT_HANDLER_REGISTRATION( HalfYearlyReportUC, DeleteHalfYearlyReport )
END_HANDLERS_REGISTRATION()

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
basar::appl::EventReturnType HalfYearlyReportUC::run()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "HalfYearlyReportUC::run()");

	registerEventHandlers( true );

	getManager().m_NarcoticsDM->init		(	basar::login::Manager::getInstance().getAreaID(), 
												getManager().m_Connection.getCurrentConnection(), 
												basar::login::Manager::getInstance().getUserName() );
	getManager().m_OpenOrderDM.init			(	basar::login::Manager::getInstance().getAreaID(), 
												getManager().m_Connection.getCurrentConnection()  );
	getManager().m_BranchDM->init			(	basar::login::Manager::getInstance().getAreaID(), 
												getManager().m_Connection.getCurrentConnection() );
	getManager().m_HalfYearlyReportVC->init	(	getManager().m_OpenOrderDM.getAccOpenOrderPropTab() );
	
	getManager().m_HalfYearlyReportVC->show();

	getManager().m_HalfYearlyReportVC->shutdown();
	getManager().m_HalfYearlyReportVC.reset();
	getManager().m_BranchDM->shutdown();
	getManager().m_OpenOrderDM.shutdown();
	getManager().m_NarcoticsDM->shutdown();

	registerEventHandlers( false );

	return basar::appl::EventReturnStruct(); //std with ret = OK, msg = ""
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
SYSTEM_EVENT_HANDLER_DEFINITION( HalfYearlyReportUC, ShowHalfYearlyReport )
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "HalfYearlyReportUC::ShowHalfYearlyReport()");

	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings
	
	basar::DateTime from;
	basar::DateTime to;
	getManager().m_HalfYearlyReportVC->matchFromPeriod( from, to, m_reportType );

	basar::I18nString httpAdress;
	basar::login::Manager::getInstance().getParaValue(UADM_BI_HTTP_ADDRESS, httpAdress);
	
	if( m_reportType == viewConn::C4Report )
	{
		basar::VarString str;
		str.format( "datefrom=%ld; dateto=%ld;", from.getDate(), to.getDate() );
		basar::db::aspect::AccessorPropertyTable_YIterator yitSearch =
			basar::db::aspect::Manager::getInstance().createAccessorYIterator( basar::cmnutil::ParameterList(str) );
		getManager().m_OpenOrderDM.findAllOpenOrderByPattern( yitSearch );
		getManager().m_HalfYearlyReportVC->matchToData();
	}
	else if( m_reportType == viewConn::C1Report )
	{	
		basar::VarString strLink;
		strLink.format( "%sWFServlet?IBIF_ex=bdh002fex&LNGID=%s&DOMUSER=%s"
                        "&RUNTIMEID=%ld&parDDateFrom=%s&parDDateTo=%s&parNBranch=%d&parCCheckNarcUnit=1&WFFMT=PDF",
						httpAdress.c_str(),
                        basar::login::Manager::getInstance().getLanguageID().substr(0,1).c_str(),
                        basar::login::Manager::getInstance().getNtUser().c_str(),
                        basar::DateTime::getCurrent().getTime()/1000,
                        from.toStrDate().c_str(),
                        to.toStrDate().c_str(),
                        basar::login::Manager::getInstance().getAreaID());
		ShellExecute(NULL,"open",strLink.c_str(),NULL,NULL,SW_SHOWNORMAL);

	}
	else if( m_reportType == viewConn::C2Report )
	{
		basar::VarString strLink;
		strLink.format( "%sWFServlet?IBIF_ex=bdh002fex&LNGID=%s&DOMUSER=%s"
                        "&RUNTIMEID=%ld&parDDateFrom=%s&parDDateTo=%s&parNBranch=%d&parCCheckNarcUnit=0&WFFMT=PDF",
						httpAdress.c_str(),
                        basar::login::Manager::getInstance().getLanguageID().substr(0,1).c_str(),
                        basar::login::Manager::getInstance().getNtUser().c_str(),
                        basar::DateTime::getCurrent().getTime()/1000,
                        from.toStrDate().c_str(),
                        to.toStrDate().c_str(),
                        basar::login::Manager::getInstance().getAreaID());
		ShellExecute(NULL,"open",strLink.c_str(),NULL,NULL,SW_SHOWNORMAL);
	}

	return basar::appl::EventReturnStruct();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
SYSTEM_EVENT_HANDLER_DEFINITION( HalfYearlyReportUC, DeleteHalfYearlyReport )
{
	BLOG_TRACE_METHOD(LoggerPool::loggerUseCases, "HalfYearlyReportUC::DeleteHalfYearlyReport()");

	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	basar::db::aspect::AccessorPropertyTable_YIterator	yitCurrent;

	if( m_reportType == viewConn::C4Report )
	{
		yitCurrent = getManager().m_HalfYearlyReportVC->getCurrentAccessorIterator();
		
		if( yitCurrent.isEnd() ){ 
			return basar::appl::EventReturnStruct();
		}
		
		if( basar::gui::tie::ButtonYes == basar::gui::tie::questionMsgBox(
			getManager().m_HalfYearlyReportVC.getWidgetPtr(),
			QApplication::translate("HalfYearlyReportUC","Do you really want to delete this order? ").toLocal8Bit().data()) )
		{
			getManager().m_OpenOrderDM.deleteOpenOrder(yitCurrent);
			basar::appl::SystemEventManager::getInstance().fire("ShowHalfYearlyReport");
		}
	}

	return basar::appl::EventReturnStruct();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
SYSTEM_EVENT_HANDLER_DEFINITION( HalfYearlyReportUC, PrintHalfYearlyReport )
{
	BLOG_TRACE_METHOD( LoggerPool::loggerUseCases, "HalfYearlyReportUC::PrintHalfYearlyReport()" );

	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	basar::DateTime from;
	basar::DateTime to;
	getManager().m_HalfYearlyReportVC->matchFromPeriod( from, to, m_reportType );

	basar::VarString period = from.toStrDate() + " - " + to.toStrDate();

	basar::db::aspect::AccessorPropertyTable_YIterator yitBranch = getManager().m_BranchDM->findBranch();
	
	if( m_reportType == viewConn::C4Report )	
	{
		basar::VarString str;
		str.format("datefrom=%ld; dateto=%ld;", from.getDate(), to.getDate());
		basar::db::aspect::AccessorPropertyTable_YIterator yitSearch =
			basar::db::aspect::Manager::getInstance().createAccessorYIterator( basar::cmnutil::ParameterList(str) );
		basar::db::aspect::AccessorPropertyTableRef ptOpenOrders = 
			getManager().m_OpenOrderDM.findAllOpenOrderByPattern( yitSearch );

		OpenOrderPrintData printData( "NarcHYR4", "NarcHYR4", ptOpenOrders, yitBranch, period );

		basar::gui::qt::PrintPreview preview( printData, 0, 
						QApplication::translate( "HalfYearlyReportUC", "Narcotics - C IV" ).toLocal8Bit().data(),
						QPrinter::Landscape );

		preview.setOutputFormats( basar::gui::qt::PDF );
		preview.show();
	}
	else if( m_reportType == viewConn::C1Report ) 
	{	
	}
	else if( m_reportType == viewConn::C2Report ) 
	{
	}

	return basar::appl::EventReturnStruct();
}

} //namespace useCase
} //namespace narcotics