//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  implemtation of domain module "customer"
 *  \author	Roland Kiefert
 *  \date   03.04.2006
 */
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "customermodule.h"
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
CustomerDM::CustomerDM()
:m_Initialized( 0 )
{
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw		no-throw */
//-------------------------------------------------------------------------------------------------//
CustomerDM::~CustomerDM()
{
	shutdown();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
void CustomerDM::init( const Int16 branchNo, const ConnectionRef activeConn )
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::VarString msg;
		msg.format("init of CustomerDM. Parameter: branchNo : %d", branchNo);
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}

	if( 0 == m_Initialized )
	{
		m_accCustomer	= basar::db::aspect::Manager::getInstance().createAccessorInstance( 
							"Acc_Customer", "Customer", activeConn );
		m_accCustMapping= basar::db::aspect::Manager::getInstance().createAccessorInstance( 
							"Acc_CustomerMapping", "Customer", activeConn );
		basar::VarString defParam;
		defParam.format("%s=%d;",
			constants::BRANCHNO,
			branchNo);
		m_accCustomer.setDefaultParameter( defParam );
		m_accCustMapping.setDefaultParameter( defParam );
	}
	++m_Initialized;
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
void CustomerDM::shutdown()
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "shutdown of CustomerDM.";
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	if( m_Initialized == 0 ){
		return;
	}
	--m_Initialized;
	if( 0 == m_Initialized ){
		m_accCustomer.reset();
		m_accCustMapping.reset();
	}
}

//-------------------------------------------------------------------------------------------------//
/*!	\return a iterator of a property table line
	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
AccessorPropertyTable_YIterator CustomerDM::findCustomerByPattern( AccessorPropertyTable_YIterator yit )
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
	{
		basar::VarString msg = "findCustomerByPattern of CustomerDM. Parameter: \n ";
		for( basar::db::aspect::AccessorPropertyTable_XIterator xit = yit.begin(); xit != yit.end(); ++xit )
		{
			if( !( xit.getState() == basar::SS_UNSET ) ){
				msg = msg + xit.getName() + ":" + xit.getSQLString() + "\n";
			}
		}
		narcotics::LoggerPool::loggerDomModules.forcedLog( log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__ );
	}

	m_accCustomer.execute( "SelCustomer", yit );
	return m_accCustomer.getPropertyTable().begin();
}

//-------------------------------------------------------------------------------------------------//
/*!	\return a iterator of a property table line
	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
AccessorPropertyTable_YIterator CustomerDM::findCustomerForReprint( basar::db::aspect::AccessorPropertyTable_YIterator yit)
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
	{
		basar::VarString msg = "findCustomerForReprint of CustomerDM. Parameter: \n ";
		for( basar::db::aspect::AccessorPropertyTable_XIterator xit = yit.begin(); xit != yit.end(); ++xit )
		{
			if( !( xit.getState() == basar::SS_UNSET ) ){
				msg = msg + xit.getName() + ":" + xit.getSQLString() + "\n";
			}
		}
		narcotics::LoggerPool::loggerDomModules.forcedLog( log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__ );
	}

	m_accCustomer.execute( "SelCustomerForReprint", yit );
	return m_accCustomer.getPropertyTable().begin();
}

//-------------------------------------------------------------------------------------------------//
/*!	\return a iterator of a property table line
	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
AccessorPropertyTableRef CustomerDM::findCustomerMapping( const basar::db::aspect::AccessorPropertyTable_YIterator yit )
{
	BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerDomModules, "CustomerDM::findCustomerMapping()");

	m_accCustMapping.execute("SelCustomerMapping", yit);
	return m_accCustMapping.getPropertyTable();
}

//-------------------------------------------------------------------------------------------------//
/*!	\return a iterator of a property table line
	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
basar::db::aspect::ExecuteResultInfo CustomerDM::deleteCustomerMapping( const basar::db::aspect::AccessorPropertyTable_YIterator yit )
{
	BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerDomModules, "CustomerDM::deleteCustomerMapping()");

	return m_accCustMapping.execute("DelCustomerMapping", yit);	
}

//-------------------------------------------------------------------------------------------------//
/*!	\return a iterator of a property table line
	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
basar::db::aspect::ExecuteResultInfo CustomerDM::saveCustomerMapping( const basar::db::aspect::AccessorPropertyTable_YIterator yit )
{
	BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerDomModules, "CustomerDM::saveCustomerMapping()");

	return m_accCustMapping.execute("SaveCustomerMapping", yit);	
}

//-------------------------------------------------------------------------------------------------//
/*!	\return a iterator of a property table line
	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
AccessorPropertyTable_YIterator CustomerDM::findCustomerByPattern( ParameterList where )
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::VarString msg = "findCustomerByPattern of CustomerDM. Parameter: \n ";
		for( basar::cmnutil::ParameterList_Iterator it = where.begin(); it != where.end(); ++it ){
			msg = msg + it->first + ":" + it->second + "\n";
		}
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}

	basar::db::aspect::AccessorPropertyTable_YIterator yit = basar::db::aspect::Manager::getInstance().createAccessorYIterator( where );
	m_accCustomer.execute("SelCustomer", yit);

	return m_accCustomer.getPropertyTable().begin();
}

//-------------------------------------------------------------------------------------------------//
/*!	\return a property table
	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
AccessorPropertyTableRef CustomerDM::findCustomerNoByName( const basar::db::aspect::AccessorPropertyTable_YIterator yit )
{
	BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerDomModules, "CustomerDM::findCustomerNoByName()");

	m_accCustomer.execute("SelCustomerByName", yit);
	return m_accCustomer.getPropertyTable();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
AccessorPropertyTableRef CustomerDM::getAccCustomerPropTab()
{
	return m_accCustomer.getPropertyTable();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return a iterator of a empty property table line
	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTable_YIterator	CustomerDM::addEmptyCustomer()
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "addEmptyCustomer of CustomerDM.";
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	m_accCustomer.getPropertyTable().clear();
	return m_accCustomer.getPropertyTable().insert( basar::FOR_INSERT );
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
AccessorPropertyTableRef CustomerDM::getAccCustMappingPropTab()
{
	return m_accCustMapping.getPropertyTable();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return a iterator of a empty property table line
	\throw no-throw */
/////////////////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTable_YIterator	CustomerDM::addEmptyCustMapping()
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "addEmptyCustMapping of CustomerDM.";
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	m_accCustMapping.getPropertyTable().clear();
	return m_accCustMapping.getPropertyTable().insert( basar::FOR_INSERT );
}

} //namespace domMod