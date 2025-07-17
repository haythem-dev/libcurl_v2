//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  implemtation of domain module "openorder"
 *  \author	Roland Kiefert
 *  \date   22.02.2006
 */
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "openordermodule.h"
#include "loggerpool.h"

//-------------------------------------------------------------------------------------------------//
// namespace section
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
OpenOrderDM::OpenOrderDM() 
: m_Initialized( 0 )
{
	BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerDomModules, "OpenOrderDM::OpenOrderDM()");
}

//-------------------------------------------------------------------------------------------------//
//! \return		void 
//! \param		branchNo is used in several method so it is set to a member. \n
//! \param		activeConn important for the creating process of every AccessorInstanceRef. \n
/*!	\throw		no-throw */
//-------------------------------------------------------------------------------------------------//
void OpenOrderDM::init( const basar::Int16 branchNo, const basar::db::aspect::ConnectionRef activeConn )
{
	BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerDomModules, "OpenOrderDM::init()");

	if( 0 == m_Initialized )
	{
		m_accNarOpenOrder = basar::db::aspect::Manager::getInstance().createAccessorInstance(
			"Acc_NarcoticsOpenOrders", "NarcoticsOpenOrders", activeConn);
		basar::VarString defParams;
		defParams.format("%s=%d;", constants::BRANCHNO, branchNo);
		m_accNarOpenOrder.setDefaultParameter(defParams);
	}
	++m_Initialized;
}

//-------------------------------------------------------------------------------------------------//
//! \return		void 
/*!	\throw		no-throw */
//-------------------------------------------------------------------------------------------------//
void OpenOrderDM::shutdown()
{
	BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerDomModules, "OpenOrderDM::shutdown()");

	if( m_Initialized == 0 ){
		return;
	}
	--m_Initialized;
	if( 0 == m_Initialized ){
		m_accNarOpenOrder.reset();
	}
}

//-------------------------------------------------------------------------------------------------//
//! \return		AccessorPropertyTableRef represents the search result.
//! \param		yit holds all criteria for searching open orders.
/*!	\throw		no-throw */
//-------------------------------------------------------------------------------------------------//
AccessorPropertyTableRef OpenOrderDM::findAllOpenOrderByPattern( AccessorPropertyTable_YIterator yit )
{
	BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerDomModules, "OpenOrderDM::findAllOpenOrderByPattern()");

	m_accNarOpenOrder.execute( "SelOpenOrdersByPattern", yit );
	return m_accNarOpenOrder.getPropertyTable();
}

//-------------------------------------------------------------------------------------------------//
//! \return		AccessorPropertyTableRef returns the current datasets in the PropertyTable.
/*!	\throw		no-throw */
//-------------------------------------------------------------------------------------------------//
AccessorPropertyTableRef OpenOrderDM::getAccOpenOrderPropTab()
{
	return m_accNarOpenOrder.getPropertyTable();
}

//-------------------------------------------------------------------------------------------------//
//! \return		Int32 represents the number of the affected datasets.
//! \param		yit holds all criteria for deleting open orders.
/*!	\throw		no-throw */
//-------------------------------------------------------------------------------------------------//
basar::db::aspect::ExecuteResultInfo OpenOrderDM::deleteOpenOrder( AccessorPropertyTable_YIterator yit )
{
	BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerDomModules, "OpenOrderDM::deleteOpenOrder()");

	return m_accNarOpenOrder.execute( "DelOpenOrder", yit );
}

} //namespace domMod