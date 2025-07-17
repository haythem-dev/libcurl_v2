//-------------------------------------------------------------------------------------------------//
/*! \file 
 *  \brief  Domain module for handling searches based on an empty accessor
 *  \author Benjamin Endlicher
 *  \date   24.08.2010
 */
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "searchmodule.h"
#include "loggerpool.h"

//-------------------------------------------------------------------------------------------------//
// using declarations section
//-------------------------------------------------------------------------------------------------//
using basar::db::aspect::AccessorInstanceRef;
using basar::db::aspect::AccessorPropertyTableRef;

//-------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------//
namespace domMod
{
    SearchDM::SearchDM() 
    : m_Initialized( 0 )
	{
	}

    //-------------------------------------------------------------------------------------------------//	
	SearchDM::~SearchDM()
	{
		shutdown();
	}

    //-------------------------------------------------------------------------------------------------//
//	all Accessors needing an active conn are initialized here	
	void SearchDM::init( const basar::db::aspect::ConnectionRef activeConn )
	{
		BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerDomModules, "SearchDM::init()");
		
		if( 0 == m_Initialized ){
			m_ConnectionRef = activeConn;
		}
		++m_Initialized;
	}

	//-------------------------------------------------------------------------------------------------//
	// Lazy Instantiation of Accessors in this Section: Getter-Methods defined here, won't be executed before really being needed
	// Consequently, Accessors won't be initialized before being needed
	CREATE_ACCESSOR_INSTANCE_DEF( SearchDM, Search, Search, basar::db::aspect::ON_DEMAND_CACHING );

    //-------------------------------------------------------------------------------------------------//
    void SearchDM::shutdown()
    {
 		if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
		{
			basar::ConstString msg = "shutdown of SearchDM.";
			narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
		}

		if( m_Initialized == 0 ){
			return;
		}
		--m_Initialized;
		if( 0 == m_Initialized ){
	// resetting accessor by using macro FREE_ACCESSOR of accessor_definitions.h
			FREE_ACCESSOR( m_SearchACC );
		}
    }

	//-------------------------------------------------------------------------------------------------
	// get Search propertytable
	//-------------------------------------------------------------------------------------------------
    AccessorPropertyTableRef SearchDM::getAccSearchPropTab()
    {
		return getSearchACC().getPropertyTable();
    }

	//-------------------------------------------------------------------------------------------------
	// clear Search propertytable
	//-------------------------------------------------------------------------------------------------
    void SearchDM::clearSearchPropTab()
    {
		getSearchACC().getPropertyTable().clear();
    }

} //namespace domMod