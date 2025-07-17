//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  
 *  \author	
 *  \date   
 */
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "numberassignmentdm.h"
#include "numberacc.h"
#include "numberassignmentdm_definitions.h"
#include "loggerpool/loggerpool.h"

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
using basar::db::aspect::AccessorInstanceRef;
using basar::db::aspect::ExecuteResultInfo;

//-------------------------------------------------------------------------------------------------//
/*!	\throw		no-throw */
//-------------------------------------------------------------------------------------------------//
NumberAssignmentDM::NumberAssignmentDM() //: DomainModuleBase()
: m_Initialized( 0 )
{
  BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerDomModules, "NumberAssignmentDM::NumberAssignmentDM().");

	m_defNameNumberAssignment = "AccNumber";
	m_instNameNumberAssignment = "AccInstanceNumber";
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw		no-throw */
//-------------------------------------------------------------------------------------------------//
NumberAssignmentDM::~NumberAssignmentDM() 
{
  BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerDomModules, "NumberAssignmentDM::~NumberAssignmentDM().");
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
void NumberAssignmentDM::init( const ConnectionRef activeConn, const basar::Int16 branchNo )
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::DEBUG_LOG_LEVEL) )
	{
		basar::VarString msg;
		msg.format("NumberAssignmentDM::init(). Branch:<%d>.", branchNo);
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}
	
   if( m_Initialized == 0 ) 
   {
	   acc_number::SelectNoForContext a;
	   acc_number::UpdateNoForContext b;

	   m_NumberACC = basar::db::aspect::Manager::getInstance().createAccessorInstance( 
										m_instNameNumberAssignment, m_defNameNumberAssignment, activeConn, basar::db::aspect::FULL_CACHING );
   }
   m_Initialized++;

   // set default parameter branchno for numberacc
   m_defaultParameter.format( "%s=%d;", lit::pdm_numberassignment::BRANCH_NO.getName().c_str(), branchNo );
   m_NumberACC.setDefaultParameter( m_defaultParameter );
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
void NumberAssignmentDM::shutdown()
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "shutdown of NumberAssignmentDM.";
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	if( m_Initialized == 0 ){
		return;
	}
	--m_Initialized;
	if( 0 == m_Initialized )
	{
        if( false == m_NumberACC.getPropertyTable().isNull() )
		{
            m_NumberACC.getPropertyTable().clear();
            m_NumberACC.getPropertyTable().reset();
        }
        m_NumberACC.reset();
	}
}

//----------------------------------------------------------------------------
/*!	\throw no-throw 
	\return current internal purchase order no */
basar::Int32 NumberAssignmentDM::findAndIncrementNoForContext( const AccessorPropertyTable_YIterator yit )
{
  BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerDomModules, "NumberAssignmentDM::findAndIncrementNoForContext().");

  ExecuteResultInfo               resInfo;
  AccessorPropertyTable_YIterator yitCurrentEntry;
  basar::Int32                    currentNo  = 0;
  basar::Int32                    startNo    = 0;
  basar::Int32                    maxNo      = 0;
  basar::Int16                    overflowAction = pdm_numberassignment::OVERFLOW_ACTION_NONE;
  basar::Int32					  nextNo = 0;
  bool							  transactionActive = false;

  if( false == transactionActive )
  {
    // if we don't run inside an already opened transaction, we start our own
    m_NumberACC.beginTransaction();
	transactionActive = true;
  }

  resInfo = m_NumberACC.execute("amSelectNoForContext", yit);
  yitCurrentEntry = m_NumberACC.getPropertyTable().begin();

  // return error if select failed
  if( resInfo.hasError() || yitCurrentEntry.isEnd() )
  {
      m_NumberACC.rollbackTransaction();
      return basar::db::aspect::SQL_ERROR_UNKNOWN;	// error occured 
  }

  if (yitCurrentEntry.isContainedAndSet(lit::pdm_numberassignment::NUMBER.getName()))
    currentNo = yitCurrentEntry.getInt32(lit::pdm_numberassignment::NUMBER.getName());
  if (yitCurrentEntry.isContainedAndSet(lit::pdm_numberassignment::NUMBER_START.getName()))
    startNo = yitCurrentEntry.getInt32(lit::pdm_numberassignment::NUMBER_START.getName());
  if (yitCurrentEntry.isContainedAndSet(lit::pdm_numberassignment::NUMBER_MAXIMUM.getName()))
    maxNo = yitCurrentEntry.getInt32(lit::pdm_numberassignment::NUMBER_MAXIMUM.getName());
  if (yitCurrentEntry.isContainedAndSet(lit::pdm_numberassignment::OVERFLOW_ACTION.getName()))
    overflowAction = yitCurrentEntry.getInt16(lit::pdm_numberassignment::OVERFLOW_ACTION.getName());

  nextNo = currentNo;
  // increment or restart current number
  if( maxNo > 0 && nextNo == maxNo && overflowAction == pdm_numberassignment::OVERFLOW_ACTION_RESTART )
    nextNo = startNo;
  else
    nextNo++;
  yitCurrentEntry.setInt32(lit::pdm_numberassignment::NUMBER.getName(), nextNo);

  resInfo = m_NumberACC.execute("amUpdateNoForContext", yitCurrentEntry);

  // commit only if select and update were successful
  if( resInfo.hasError() || resInfo.m_affectedRows != pdm_numberassignment::ONE_ROW_AFFECTED )
  {
      m_NumberACC.rollbackTransaction();
      return basar::db::aspect::SQL_ERROR_UNKNOWN;	// error occured 
  }

  m_NumberACC.commitTransaction();
  return currentNo;	// new number is returned or 0 if no record was found
}

} //namespace domMod
