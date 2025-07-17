//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  implemtation of domain module "narcotics"
 *  \author	Roland Kiefert
 *  \date   22.02.2006
 */
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "narcoticsgroupmodule.h"
#include "loggerpool.h"

//-------------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------------//
namespace domMod 
{

//-------------------------------------------------------------------------------------------------//
// using declarations
//-------------------------------------------------------------------------------------------------//
using basar::Int16;
using basar::Int32;
using basar::cmnutil::ParameterList;
using basar::db::aspect::ConnectionRef;
using basar::db::aspect::AccessorPropertyTableRef;
using basar::db::aspect::AccessorPropertyTable_YIterator;

//-------------------------------------------------------------------------------------------------//
/*!	\throw		no-throw */
//-------------------------------------------------------------------------------------------------//
NarcoticsGroupDM::NarcoticsGroupDM()
:m_Initialized( 0 )
{
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw		no-throw */
//-------------------------------------------------------------------------------------------------//
NarcoticsGroupDM::~NarcoticsGroupDM()
{
	shutdown();
}

//-------------------------------------------------------------------------------------------------//
//! \return		void
//! \param		activeConn important for the creating process of every AccessorInstanceRef. \n
/*!	\throw		no-throw */
//-------------------------------------------------------------------------------------------------//
void NarcoticsGroupDM::init( const ConnectionRef activeConn )
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "init of NarcoticsGroupDM.";
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	if( 0 == m_Initialized )
	{
		m_accNarGroup = basar::db::aspect::Manager::getInstance().createAccessorInstance( 
			"Acc_NarcoticsGroup", "NarcoticsGroup", activeConn);
	}

	++m_Initialized;
}

//-------------------------------------------------------------------------------------------------//
//! \return		void
/*!	\throw		no-throw */
//-------------------------------------------------------------------------------------------------//
void NarcoticsGroupDM::shutdown()
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "shutdown of NarcoticsGroupDM.";
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	if( m_Initialized == 0 ){
		return;
	}
	--m_Initialized;
	if( 0 == m_Initialized ){
		m_accNarGroup.reset();
	}
}

//-------------------------------------------------------------------------------------------------//
//! \return		AccessorPropertyTableRef 
//! \param		yit holds the search criteria
/*!	\throw		no-throw */
//-------------------------------------------------------------------------------------------------//
AccessorPropertyTableRef NarcoticsGroupDM::findNarcGroupByPattern( AccessorPropertyTable_YIterator yit )
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::VarString msg = "findNarcGroupByPattern of NarcoticsGroupDM. Parameter: \n ";
		for( basar::db::aspect::AccessorPropertyTable_XIterator xit = yit.begin(); xit != yit.end(); ++xit )
		{
			if( !(xit.getState() == basar::SS_UNSET) ){
				msg = msg + xit.getName() + ":" + xit.getSQLString() + "\n";
			}
		}
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}
	m_accNarGroup.execute("SelGroup",yit);
	return m_accNarGroup.getPropertyTable();
}

//-------------------------------------------------------------------------------------------------//
//! \return		AccessorPropertyTableRef represents the search result.
//! \param		searchIt is the yiterator holding the search criteria.
/*!	\throw		no-throw */
//-------------------------------------------------------------------------------------------------//
AccessorPropertyTableRef NarcoticsGroupDM::findGroupByName( AccessorPropertyTable_YIterator searchIt )
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::VarString msg = "findByName of NarcoticsGroupDM. Parameter: \n ";
		for( basar::db::aspect::AccessorPropertyTable_XIterator xit = searchIt.begin(); xit != searchIt.end(); ++xit )
		{
			if( !(xit.getState() == basar::SS_UNSET) ){
				msg = msg + xit.getName() + ":" + xit.getSQLString() + "\n";
			}
		}
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}
	m_accNarGroup.execute("SelGroupByName", searchIt);

	return m_accNarGroup.getPropertyTable();
}

//-------------------------------------------------------------------------------------------------//
//! \return		AccessorPropertyTable_YIterator represents the result search.
//! \param		articleNo which the article should be searched for.
//! \param		branchNo which the article should be searched for.
/*!	\throw		no-throw */
//-------------------------------------------------------------------------------------------------//
AccessorPropertyTable_YIterator NarcoticsGroupDM::findStockLocationByArticleNo( const basar::Int32 articleNo,
																				const basar::Int16 branchNo )
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::VarString msg;
		msg.format("findStockLocationByArticleNo of NarcoticsGroupDM. Parameter: articleNo : %d; branchNo : %d", articleNo, branchNo);
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}

	basar::I18nString where;
	where.format( "articleno=%d; branchno=%d;", articleNo, branchNo );
	basar::db::aspect::AccessorPropertyTable_YIterator yit = 
			basar::db::aspect::Manager::getInstance().createAccessorYIterator(where);
	m_accNarGroup.execute( "SelStockLocationByArticleNo", yit );

	if( m_accNarGroup.getPropertyTable().empty() ){
		m_accNarGroup.execute( "SelStockLocation2ByArticleNo", yit );
	}

	return m_accNarGroup.getPropertyTable().begin();
}

//-------------------------------------------------------------------------------------------------//
//! \return		AccessorPropertyTableRef returns the container of the current datasets in the PropertyTable.
/*!	\throw		no-throw */
//-------------------------------------------------------------------------------------------------//
AccessorPropertyTableRef NarcoticsGroupDM::getAccNarCatalogPropTab()
{
	return m_accNarGroup.getPropertyTable();
}

} //namespace domMod