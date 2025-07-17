//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  implemtation of domain module "original"
 *  \author	Roland Kiefert
 *  \date   23.02.2006
 */
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// include declarations
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "originalmodule.h"
#include "loggerpool.h"

//-------------------------------------------------------------------------------------------------//
// namespace declarations
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
OriginalDM::OriginalDM()
:m_Initialized( 0 )
{
}

//-------------------------------------------------------------------------------------------------//
//! \return		void
//! \param		branchNo is used in several method so it is set to a member. \n
//! \param		activeConn important for the creating process of every AccessorInstanceRef. \n
/*!	\throw		no-throw */
//-------------------------------------------------------------------------------------------------//
void OriginalDM::init( const basar::Int16 branchNo, const basar::db::aspect::ConnectionRef activeConn )
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::VarString msg;
		msg.format("init of OriginalDM. Parameter: branchNo : %d", branchNo);
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}

	if( 0 == m_Initialized ){
		m_BranchNo = branchNo;

		basar::I18nString branch;
		branch.format( "%s=%i; %s; %s; %s; %s; %s;",
						constants::BRANCHNO,
						m_BranchNo, 
						constants::FROMDATE, 
						constants::TODATE, 
						constants::ARTICLENO, 
						constants::PURCHASEORDERNO, 
						constants::TRANSACTIONTYPE );

		m_searchIterator = basar::db::aspect::Manager::getInstance().createAccessorYIterator(branch);

		m_accOrgCatalog	= basar::db::aspect::Manager::getInstance().createAccessorInstance( 
			"Acc_OriginalCatalog", "OriginalCatalog", activeConn );
	}
	m_Initialized++;
}

//-------------------------------------------------------------------------------------------------//
//! \return		void
/*!	\throw		no-throw */
//-------------------------------------------------------------------------------------------------//
void OriginalDM::shutdown()
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "shutdown of OriginalDM.";
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	if( m_Initialized == 0 ){
		return;
	}
	--m_Initialized;
	if( 0 == m_Initialized ){
		m_accOrgCatalog.reset();
	}
}

//-------------------------------------------------------------------------------------------------//
//! \return		AccessorPropertyTable_YIterator is the YIterator holding the search criteria.
/*!	\throw		no-throw */
//-------------------------------------------------------------------------------------------------//
basar::db::aspect::AccessorPropertyTable_YIterator OriginalDM::getSearchIterator()
{
	return m_searchIterator;
}

//-------------------------------------------------------------------------------------------------//
//! \return		void
/*!	\throw		no-throw */
//-------------------------------------------------------------------------------------------------//
void OriginalDM::clearSearchIterator()
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "clearSearchIterator of OriginalDM.";
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	m_searchIterator.setPropertyState( constants::FROMDATE,			basar::SS_UNSET );
	m_searchIterator.setPropertyState( constants::TODATE,			basar::SS_UNSET );
	m_searchIterator.setPropertyState( constants::ARTICLENO,		basar::SS_UNSET );
	m_searchIterator.setPropertyState( constants::PURCHASEORDERNO,	basar::SS_UNSET );
	m_searchIterator.setPropertyState( constants::TRANSACTIONTYPE,	basar::SS_UNSET );
}

//-------------------------------------------------------------------------------------------------//
//! \return		AccessorPropertyTableRef represents the search result.
//! \param		filterParams is the Parameterlist holding the search criteria.
/*!	\throw		no-throw */
//-------------------------------------------------------------------------------------------------//
AccessorPropertyTableRef OriginalDM::findCatalogByPattern( ParameterList filterParams )
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::VarString msg = "findCatalogByPattern of OriginalDM. Parameter: \n ";
		for( basar::cmnutil::ParameterList_Iterator it = filterParams.begin(); it != filterParams.end(); ++it )
		{
			msg = msg + it->first + ":" + it->second + "\n";
		}
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}

	basar::db::aspect::AccessorPropertyTable_YIterator yit = 
			basar::db::aspect::Manager::getInstance().createAccessorYIterator(filterParams);
	m_accOrgCatalog.execute("SelCatalog", yit);

	return m_accOrgCatalog.getPropertyTable();
}

//-------------------------------------------------------------------------------------------------//
//! \return		AccessorPropertyTableRef represents the search result.
//! \param		yit holds the search criteria
/*!	\throw		no-throw */
//-------------------------------------------------------------------------------------------------//
AccessorPropertyTableRef OriginalDM::findCatalogByPattern( AccessorPropertyTable_YIterator yit )
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::VarString msg = "findCatalogByPattern of OriginalDM. Parameter: \n ";
		for( basar::db::aspect::AccessorPropertyTable_XIterator xit = yit.begin(); xit != yit.end(); ++xit )
		{
			if( !(xit.getState() == basar::SS_UNSET) ){
				msg = msg + xit.getName() + ":" + xit.getSQLString() + "\n";
			}
		}
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}

	m_accOrgCatalog.execute("SelCatalog",yit);
	return m_accOrgCatalog.getPropertyTable();
}

//-------------------------------------------------------------------------------------------------//
//! \return		AccessorPropertyTableRef returns the container of the current datasets in the PropertyTable.
/*!	\throw		no-throw */
//-------------------------------------------------------------------------------------------------//
AccessorPropertyTableRef OriginalDM::getAccOrgCatalogPropTab()
{
	return m_accOrgCatalog.getPropertyTable();
}

} //namespace domMod