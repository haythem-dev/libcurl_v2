//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  implemtation of use case "narcotics category"
 *  \author	Roland Kiefert
 *  \date   21.02.2006
 */
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "narcoticsappl.h"
#include "narcoticscategory.h"
#include "loggerpool.h"

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
NarcoticsCategoryUC::NarcoticsCategoryUC()
{
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
NarcoticsCategoryUC::~NarcoticsCategoryUC()
{
}

//-------------------------------------------------------------------------------------------------//
// macro for registrating and deregistrating EventHandlers!
// 1st para: name of UC
// 2nd para: name of handle-Method
BEGIN_HANDLERS_REGISTRATION( NarcoticsCategoryUC )
	SYSTEM_EVENT_HANDLER_REGISTRATION( NarcoticsCategoryUC, SearchNarcoticsCategory )
END_HANDLERS_REGISTRATION()

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
basar::appl::EventReturnType NarcoticsCategoryUC::run()
{
	registerEventHandlers( true );

	getManager().m_NarcoticsGroupDM->init(	getManager().m_Connection.getCurrentConnection() );

	getManager().m_NarcoticCategoryVC->init( getManager().m_NarcoticsGroupDM->getAccNarCatalogPropTab() );
	
	getManager().m_NarcoticCategoryVC->show();

	getManager().m_NarcoticCategoryVC->shutdown();

	getManager().m_NarcoticCategoryVC.reset();
	
	getManager().m_currentBooking.clear();
	getManager().m_NarcoticsGroupDM->shutdown();

	registerEventHandlers( false );

	return basar::appl::EventReturnStruct(); //std with ret = OK, msg = ""
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
SYSTEM_EVENT_HANDLER_DEFINITION( NarcoticsCategoryUC, SearchNarcoticsCategory )
{
	rSource = rSource; //if source not needed otherwise (e.g. for comparing); to avoid warnings

	if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "Event search caught in NarcoticsCategoryUC.";
		LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
	matchToSearchCriteria();

	basar::appl::EventReturnStruct ret = getManager().m_ResultsetSearch.findResultset();

	if( ret.ret != basar::appl::HANDLER_INFO ){
		ret = searchCategory();
	}
	if( ret.ret != basar::appl::HANDLER_INFO ){
		matchFromCategory();
	}
	matchFromSearchCriteria();
	
	return ret;
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
void NarcoticsCategoryUC::matchFromCategory()
{
	getManager().m_NarcoticCategoryVC->matchToCategory();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
basar::appl::EventReturnStruct NarcoticsCategoryUC::searchCategory()
{
	AccessorPropertyTableRef resultTable = getManager().m_NarcoticsGroupDM->findNarcGroupByPattern( getManager().m_NarcoticsDM->getSearchIterator() );
	
	if( resultTable.empty() )
	{
		return basar::appl::EventReturnStruct( basar::appl::HANDLER_INFO, QApplication::translate( "NarcoticsCategoryUC", 
												"No results for given group or name found!" ).toLocal8Bit().data() );
	}

	return basar::appl::EventReturnStruct();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
void NarcoticsCategoryUC::matchToSearchCriteria()
{
	getManager().m_NarcoticsDM->resetSearchIterator();
	getManager().m_NarcoticCategoryVC->matchFromSelection( getManager().m_NarcoticsDM->getSearchIterator() );
	
	// test fields of searchIterator
	basar::db::aspect::AccessorPropertyTable_YIterator	yit = getManager().m_NarcoticsDM->getSearchIterator();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
void NarcoticsCategoryUC::matchFromSearchCriteria()
{
	if( getManager().m_ResultsetSearch.matchingFromSearchCriteriaAllowed() )
	{
		basar::I18nString searchCriteria = getManager().m_ResultsetSearch.getSearchCriteria();
		if( searchCriteria == resultsetdefines::SEARCH_FOR_NARCOTICGROUP ){
			getManager().m_NarcoticCategoryVC->matchToGroup( getManager().m_NarcoticsDM->getSearchIterator().getString( constants::GROUP ) );
		}
	}
	getManager().m_ResultsetSearch.resetSearchCriteria();
}

} //namespace useCase
} //namespace narcotics