//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  
 *  \author	Harald Bayer
 *  \date   02.08.2012
 */
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "orderdm.h"
#include "loggerpool.h"
#include "accessor_definitions.h"
#include <lockmanager/branchlockmanager.h>

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
OrderDM::OrderDM()
: m_Initialized( 0 )
{
}

//-------------------------------------------------------------------------------------------------//
//! \return		void 
//! \param		branchNo is used in several method so it is set to a member. \n
//! \param		activeConn important for the creating process of every AccessorInstanceRef. \n
/*!	\throw		no-throw */
//-------------------------------------------------------------------------------------------------//
void OrderDM::init( const basar::Int16 branchNo, 
					const basar::db::aspect::ConnectionRef activeConn, 
					boost::shared_ptr<domMod::LogDM> logDM, 
					const basar::VarString& userName )
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::VarString msg;
		msg.format("init of orderDM. Parameter: branchNo : %d", branchNo);
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}

	if( 0 == m_Initialized )
	{
		m_BranchNo = branchNo;
		m_ActiveConn = activeConn;
		m_accNarOrder	= basar::db::aspect::Manager::getInstance().createAccessorInstance( 
			"accNarOrder", "NarcoticsOrders", activeConn );
		m_accNarOrderPositions = basar::db::aspect::Manager::getInstance().createAccessorInstance( 
			"accNarOrderPositions", "NarcoticsOrdersPositions", activeConn, basar::db::aspect::FULL_CACHING);
		m_LockManager = boost::shared_ptr<libnarcotics::lockmanager::BranchLockManager>(
			new libnarcotics::lockmanager::BranchLockManager(m_BranchNo, activeConn, userName, constants::TRANSFERLOCK_TABLE_NAME, narcotics::LoggerPool::loggerDomModules));
	}

	m_LogDM = logDM;

	++m_Initialized;
}

//-------------------------------------------------------------------------------------------------//
//! \return		void 
/*!	\throw		no-throw */
//-------------------------------------------------------------------------------------------------//
void OrderDM::shutdown()
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "shutdown of orderDM.";
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	if( m_Initialized == 0 ){
		return;
	}
	--m_Initialized;
	if( 0 == m_Initialized )
	{
		FREE_ACCESSOR( m_accNarOrder );
		FREE_ACCESSOR( m_accNarOrderPositions );
	}
	m_LogDM->shutdown();
}

//-------------------------------------------------------------------------------------------------//
const bool OrderDM::isInitialized()
{
	return m_Initialized > 0;
}

//-------------------------------------------------------------------------------------------------//
//! \return		AccessorPropertyTableRef returns the current datasets in the PropertyTable.
/*!	\throw		no-throw */
//-------------------------------------------------------------------------------------------------//
AccessorPropertyTableRef OrderDM::getAccOpenOrderPropTab()
{
	return m_accNarOrder.getPropertyTable();
}

//-------------------------------------------------------------------------------------------------//
AccessorPropertyTableRef OrderDM::getAccOpenOrderPositionsPropTab()
{
	return m_accNarOrderPositions.getPropertyTable();
}

//-------------------------------------------------------------------------------------------------//
AccessorPropertyTable_YIterator OrderDM::getPositionsForOrder( basar::db::aspect::AccessorPropertyTable_YIterator order )
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::VarString msg = "getPositionsForOrder of OrderDM. Parameter: \n ";
		for( basar::db::aspect::AccessorPropertyTable_XIterator xit = order.begin(); xit != order.end(); ++xit )
		{
			if( !(xit.getState() == basar::SS_UNSET) ){
				msg = msg + xit.getName() + ":" + xit.getSQLString() + "\n";
			}
		}
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}	
	
	m_accNarOrderPositions.execute("SelOrderPositionsByPattern", order);
	
	return m_accNarOrderPositions.getPropertyTable().begin(); 
}

//-------------------------------------------------------------------------------------------------//
bool OrderDM::setOrderStatusToForPrint( basar::db::aspect::AccessorPropertyTable_YIterator order )
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "setOrderStatusToForPrint of OrderDM.";
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return !(m_accNarOrder.execute("UpdateOrderAckStatusByPatternSetToForPrint", order)).hasError();
}

//-------------------------------------------------------------------------------------------------//
//! \return		Int32 represents the number of the affected datasets.
//! \param		yit holds all criteria for deleting open orders.
/*!	\throw		no-throw */
//-------------------------------------------------------------------------------------------------//
bool OrderDM::deleteOrder( AccessorPropertyTable_YIterator yit )
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::VarString msg = "deleteOrder of OrderDM. Parameter: \n ";
		for( basar::db::aspect::AccessorPropertyTable_XIterator xit = yit.begin(); xit != yit.end(); ++xit )
		{
			if( !(xit.getState() == basar::SS_UNSET) ){
				msg = msg + xit.getName() + ":" + xit.getSQLString() + "\n";
			}
		}
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}
	
	AccessorPropertyTable_YIterator positions = getPositionsForOrder(yit);

	basar::Int32 articleNo = -1;

	basar::Int16 rollbacksOccured = 0;

	bool retVal = true;

	try
	{
		if( !positions.isNull() && !positions.isEnd() )
		{
			while( !positions.isEnd() )
			{
				//accquire lock - int articleNo is needed for unlock after rollback (yit might be invalid)
				articleNo = positions.getInt32(constants::ARTICLENO);
				bool lockAquired = m_LockManager->requestLock(constants::TRANSFER_BUSINESS_TYPE, articleNo, constants::TRANSFER_MAX_LOCKS_PER_BOOKING);
				
				m_accNarOrderPositions.beginTransaction();
				
				if( lockAquired && cancelOrderPosition(positions) && doStockUpdate(positions) )
				{
					m_accNarOrderPositions.commitTransaction();
					//only log if deletion worked
					m_LogDM->saveLogEntryDelete( positions.getInt32(constants::CATALOGID) );

					rollbacksOccured = 0;
				}
				else
				{
					m_accNarOrderPositions.rollbackTransaction();
					++rollbacksOccured;
				}
				
				m_LockManager->releaseLock(constants::TRANSFER_BUSINESS_TYPE, articleNo);
				
				//simple retry
				if( rollbacksOccured > 0 )
				{
					if( rollbacksOccured > 1 )
					{
						retVal = false;
						//log message
						basar::VarString func = "cancelSelectedOrderPositions";
						basar::VarString msg = "retrys exhausted, leftover positions were: ";
						
						while( !positions.isEnd() )
						{
							basar::VarString param;
							param.format("%d", positions.getInt32(constants::CATALOGID));

							msg = msg + param + " ";
							++positions;
						}
						//exit loop -> execption
						throw basar::cmnutil::BasarRuntimeException(basar::ExceptInfo(func, msg, __FILE__, __LINE__));
					}
					Sleep(5 * 1000);
				}
				else{
					++positions;
				}
			}
		}
	}
	catch( basar::db::sql::DbException& dbEx )
	{
		retVal = false;

		basar::VarString msg;
		msg.format("deleteOrder of OrderDM. Caught Database Exeception: %s", dbEx.what().c_str());
		BLOG_ERROR(narcotics::LoggerPool::loggerDomModules, msg);
	
		if( !m_ActiveConn.isNull() && m_ActiveConn.isOpen() && m_ActiveConn.isInTransaction() ){
			m_ActiveConn.rollbackTransaction();
		}
	}
	catch( basar::cmnutil::BasarRuntimeException& ex )
	{
		retVal = false;

		basar::VarString msg;
		msg.format("deleteOrder of OrderDM. Caught Basar Exeception: %s", ex.what().c_str());
		BLOG_ERROR(narcotics::LoggerPool::loggerDomModules, msg);
	
		if( !m_ActiveConn.isNull() && m_ActiveConn.isOpen() && m_ActiveConn.isInTransaction() ){
			m_ActiveConn.rollbackTransaction();
		}
	}

	//here no articlelock should exist in database - making sure that it is so
	try
	{
		m_LockManager->releaseAllLocksByBranchBusinessTypeAndUser(constants::TRANSFER_BUSINESS_TYPE);
	}
	catch( basar::db::sql::DbException& dbEx )
	{
		basar::VarString msg;
		msg.format("deleteOrder of OrderDM. Caught Database Exeception while releaseing lock: %s", dbEx.what().c_str());
		BLOG_ERROR(narcotics::LoggerPool::loggerDomModules, msg);
	}

	return retVal; //m_affectedRows;
}

//-------------------------------------------------------------------------------------------------//
//! \return		
//! \param		orderPositions contains all order positions, positions to delete are marked with a 1 in selection column\n
/*!	\throw		no-throw */
//-------------------------------------------------------------------------------------------------//
bool OrderDM::cancelSelectedOrderPositions( AccessorPropertyTableRef orderPositions )
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "cancelMarkedOrderPositions of OpenOrderDM.";
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	AccessorPropertyTable_YIterator yit = orderPositions.begin();

	basar::Int32 articleNo = -1;

	basar::Int16 rollbacksOccured = 0;

	bool retVal = true;

	try
	{
		while( !yit.isNull() && !yit.isEnd() )
		{
			if( yit.getString(constants::SELECTION) == constants::SELECTION_ON )
			{
				//accquire lock - int articleNo is needed for unlock after rollback (yit might be invalid)
				articleNo = yit.getInt32(constants::ARTICLENO);
				bool lockAquired = m_LockManager->requestLock(constants::TRANSFER_BUSINESS_TYPE, articleNo, constants::TRANSFER_MAX_LOCKS_PER_BOOKING);
				
				m_accNarOrderPositions.beginTransaction();

				if( lockAquired && cancelOrderPosition(yit) && doStockUpdate(yit) )
				{
					m_accNarOrderPositions.commitTransaction();
					m_LogDM->saveLogEntryDelete(yit.getInt32(constants::CATALOGID));

					rollbacksOccured = 0;
				}
				else
				{
					m_accNarOrderPositions.rollbackTransaction();
					++rollbacksOccured;
				}
				m_LockManager->releaseLock(constants::TRANSFER_BUSINESS_TYPE, articleNo);
			}

			//simple retry
			if( rollbacksOccured > 0 )
			{
				if( rollbacksOccured > 1 )
				{
					retVal = false;
					//log message
					basar::VarString func = "cancelSelectedOrderPositions";
					basar::VarString msg = "retrys exhausted, leftover positions were: ";
					
					while( !yit.isEnd() )
					{
						basar::VarString param;
						param.format("%d", yit.getInt32(constants::CATALOGID));

						msg = msg + param + " ";

						++yit;
					}

					throw basar::cmnutil::BasarRuntimeException(basar::ExceptInfo(func, msg, __FILE__, __LINE__));
					//exit loop -> execption
				}
				Sleep(5 * 1000);
			}
			else{
				++yit;
			}
		}
	}
	catch( basar::db::sql::DbException& dbEx )
	{
		retVal = false;

		basar::VarString msg;
		msg.format("deleteOrder of OrderDM. Caught Exeception: %s", dbEx.what().c_str());
		BLOG_ERROR(narcotics::LoggerPool::loggerDomModules, msg);
	
		if (!m_ActiveConn.isNull() && m_ActiveConn.isOpen() && m_ActiveConn.isInTransaction())
		{
			m_ActiveConn.rollbackTransaction();
		}
	}
	catch( basar::cmnutil::BasarRuntimeException& ex )
	{
		retVal = false;

		basar::VarString msg;
		msg.format("deleteOrder of OrderDM. Caught Basar Exeception: %s", ex.what().c_str());
		BLOG_ERROR(narcotics::LoggerPool::loggerDomModules, msg);
	
		if( !m_ActiveConn.isNull() && m_ActiveConn.isOpen() && 
			m_ActiveConn.isInTransaction() )
		{
			m_ActiveConn.rollbackTransaction();
		}
	}

	//here no articlelock should exist in database - making sure that it is so
	try
	{
		m_LockManager->releaseAllLocksByBranchBusinessTypeAndUser( constants::TRANSFER_BUSINESS_TYPE );
	}
	catch( basar::db::sql::DbException& dbEx )
	{
		basar::VarString msg;
		msg.format("deleteOrder of OrderDM. Caught Database Exeception while releaseing lock: %s", dbEx.what().c_str());
		BLOG_ERROR(narcotics::LoggerPool::loggerDomModules, msg);
	}


	return retVal;
}

//-------------------------------------------------------------------------------------------------//
//! \return		
//! \param		orderPosition represents one database row of an order
/*!	\throw		no-throw */
//-------------------------------------------------------------------------------------------------//
const bool OrderDM::cancelOrderPosition( basar::db::aspect::AccessorPropertyTable_YIterator orderPosition )
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "cancelOrderPositions of OpenOrderDM.";
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return !m_accNarOrderPositions.execute("DelOrderPositionsBySelection", orderPosition).hasError();
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw 
	\return number of rows where qtyinstock was updated or value -99 if parameter "checkOnly" = true and 
	a negative quantity in stock would be appear at least in one booking.
	parameter is single changed booking which is already written to database,
	locking for this article has to be done before calling this method, there will be no check for lock in here! */
//-------------------------------------------------------------------------------------------------//
const bool OrderDM::doStockUpdate( AccessorPropertyTable_YIterator orderPosition )
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::VarString msg = "doStockUpdate of OrderDM. Parameter: \n";
		for( basar::db::aspect::AccessorPropertyTable_XIterator xit = orderPosition.begin(); xit != orderPosition.end(); ++xit )
		{
			if( !(xit.getState() == basar::SS_UNSET) ){
				msg = msg + xit.getName() + ":" + xit.getSQLString() + "\n";
			}
		}
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}
	//! \todo check if article is locked (especially for batch) !?
	//! \todo transaction necessary!
	
    m_accNarOrder.getPropertyTable().clear();
	
	//get first order position with same articleno in our branch earlier than our position
	m_accNarOrder.execute( "SelQuickPrevForUpdQtyInStock", orderPosition );

	AccessorPropertyTable_YIterator prevOrderPositionYit	= m_accNarOrder.getPropertyTable().begin();
	
	basar::Int32 prevQtyInStock = 0;
	
	if( !prevOrderPositionYit.isEnd() ){ 
		prevQtyInStock = prevOrderPositionYit.getInt32( "qtyinstock" );
	}
	
	m_accNarOrder.getPropertyTable().clear();

	//get all order positions with same articleno in our branch later than our position
	m_accNarOrder.execute( "SelFollowForUpdQtyInStock", orderPosition );

	AccessorPropertyTableRef followingOrdersPositions = m_accNarOrder.getPropertyTable();

	//check if followingOrdersPositions is empty -> no further articles to do a stock update (our article is the last in line)
	//nothing to do, so return true

	basar::db::aspect::ExecuteResultInfo retVal;

	if( !followingOrdersPositions.isNull() && 
		!followingOrdersPositions.empty() )
	{
		AccessorPropertyTable_YIterator currentOrdersPositionYit = followingOrdersPositions.begin();
		
		basar::Int16 stockUpdateFactor = 1;
		while( !currentOrdersPositionYit.isEnd() )
		{
			// the letter "a" represents selloff (deu: Kundenauftrag)
			// the letter "y" represents a separate debit (deu: gesonderte Abbuchung), 
			// the letter "z" represents a separate posting (deu: gesonderte Abbuchung)
			if( TransactionType::CUSTOMER_ORDER == currentOrdersPositionYit.getString(constants::TRANSACTIONTYPE) || 
				TransactionType::SUPPLIER_RETURNS == currentOrdersPositionYit.getString(constants::TRANSACTIONTYPE) || 
				TransactionType::STOCK_DIFFERENCE_DECREASE == currentOrdersPositionYit.getString(constants::TRANSACTIONTYPE) || 
				TransactionType::DESTRUCTION == currentOrdersPositionYit.getString(constants::TRANSACTIONTYPE) )
			{
				stockUpdateFactor = -1;
			} 
			else{
				stockUpdateFactor = 1; 
			}

			currentOrdersPositionYit.setInt32("qtyinstock", prevQtyInStock + ( stockUpdateFactor * currentOrdersPositionYit.getInt32("qtytransaction")) );
			prevQtyInStock = currentOrdersPositionYit.getInt32("qtyinstock");
			++currentOrdersPositionYit;
		}

		retVal = m_accNarOrder.execute("UpdateQtyInStock", followingOrdersPositions.begin(), followingOrdersPositions.end());
	}
	else
	{
		basar::VarString msg = "";
		msg.format("doStockUpdate of OrderDM: No later orders for article %d, reporting succeess for stock update", orderPosition.getInt32(constants::ARTICLENO));

		BLOG_INFO(narcotics::LoggerPool::loggerDomModules, msg);
		retVal.m_error = basar::db::aspect::SUCCESS;
		retVal.m_affectedRows = 1;
	}

	return !retVal.hasError();
}

} //namespace domMod