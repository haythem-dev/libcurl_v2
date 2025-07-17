//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  implemtation of domain module "branch"
 *  \author	Bjoern Bischof
 *  \date   29.06.2006
 */
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "branchmodule.h"
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
BranchDM::BranchDM()
:m_BranchNo( 0 ), m_Initialized( 0 ), m_Loaded( false )
{
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
void BranchDM::init( const Int16 branchNo, const ConnectionRef activeConn )
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::VarString msg;
		msg.format("init of BranchDM. Parameter: branchNo : %d", branchNo);
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}
	if( 0 == m_Initialized )
	{
		m_BranchNo			= branchNo;
		m_accBranch			= basar::db::aspect::Manager::getInstance().createAccessorInstance( 
							"Acc_Branch", "Branch", activeConn );
		m_accBranchAddress	= basar::db::aspect::Manager::getInstance().createAccessorInstance( 
							"Acc_BranchAddress", "Branch", activeConn );
	}
	++m_Initialized;
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
void BranchDM::shutdown()
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "shutdown of BranchDM.";
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
	if( m_Initialized == 0 ){
		return;
	}
	--m_Initialized;
	if( 0 == m_Initialized )
	{
		m_accBranch.reset();
		m_accBranchAddress.reset();
		m_Loaded = false;
	}
}

//-------------------------------------------------------------------------------------------------//
/*!	\return an iterator of a property table line
	\throw	no-throw */
//-------------------------------------------------------------------------------------------------//
AccessorPropertyTable_YIterator BranchDM::findBranch()
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "findBranch of BranchDM.";
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	if( false == m_Loaded )
	{
		basar::VarString where;
		where.format( "branchno = %d;", m_BranchNo );
		basar::db::aspect::AccessorPropertyTable_YIterator yit = 
            basar::db::aspect::Manager::getInstance().createAccessorYIterator( static_cast<basar::I18nString>(where) );
		m_accBranch.execute( "SelBranch", yit );
		m_yitBranch = m_accBranch.getPropertyTable().begin();
		m_Loaded = true;
	}
	return m_yitBranch;
}

//-------------------------------------------------------------------------------------------------//
/*!	\return an iterator of a property table line
	\throw	no-throw */
//-------------------------------------------------------------------------------------------------//
AccessorPropertyTable_YIterator BranchDM::findBranchAddress()
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "findBranchAddress of BranchDM.";
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	basar::VarString where;
	where.format( "branchno = %d;", m_BranchNo );
	basar::db::aspect::AccessorPropertyTable_YIterator yit = 
        basar::db::aspect::Manager::getInstance().createAccessorYIterator( static_cast<basar::I18nString>(where) );
	m_accBranchAddress.execute( "SelBranchAddress", yit );
	m_yitBranchAddress = m_accBranchAddress.getPropertyTable().begin();
	
	return m_yitBranchAddress;
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
AccessorPropertyTableRef BranchDM::getAccBranchAddressPropTab()
{
	return m_accBranchAddress.getPropertyTable();
}

} //namespace domMod