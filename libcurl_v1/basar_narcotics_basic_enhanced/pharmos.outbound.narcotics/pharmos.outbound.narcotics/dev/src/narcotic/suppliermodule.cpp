//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  implemtation of domain module "supplier"
 *  \author Frank Attmannspacher
 *  \date   15.05.2013
 */
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "suppliermodule.h"
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
	SupplierDM::SupplierDM()
	: m_Initialized( false )
	{
	}

	//-------------------------------------------------------------------------------------------------//
	/*!	\throw		no-throw */
	//-------------------------------------------------------------------------------------------------//
	SupplierDM::~SupplierDM()
	{
		shutdown();
	}

	//-------------------------------------------------------------------------------------------------//
	/*!	\throw no-throw */
	//-------------------------------------------------------------------------------------------------//
	void SupplierDM::init( const Int16 branchNo, const ConnectionRef activeConn )
	{
		if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
		{
			basar::VarString msg;
			msg.format("init of SupplierDM. Parameter: branchNo : %d", branchNo);
			narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
		}

		if( m_Initialized == false )
		{
			m_accSupplier = basar::db::aspect::Manager::getInstance().createAccessorInstance( "Acc_Supplier", "Supplier", activeConn );

			basar::VarString defParam;
			defParam.format( "%s=%d;",
							 constants::BRANCHNO,
							 branchNo);
			m_accSupplier.setDefaultParameter(defParam);
		}
		m_Initialized = true;
	}

	//-------------------------------------------------------------------------------------------------//
	/*!	\throw no-throw */
	//-------------------------------------------------------------------------------------------------//
	void SupplierDM::shutdown()
	{
		if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
		{
			basar::ConstString msg = "shutdown of SupplierDM.";
			narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
		}

		if( m_Initialized == false ){
			return;
		}
		
		m_accSupplier.reset();
		m_Initialized = false;
	}

	//-------------------------------------------------------------------------------------------------//
	/*!	\return a iterator of a property table line
		\throw no-throw */
	//-------------------------------------------------------------------------------------------------//
	AccessorPropertyTable_YIterator SupplierDM::findSupplierByPattern( AccessorPropertyTable_YIterator yit )
	{
		if( narcotics::LoggerPool::loggerDomModules.isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
		{
			basar::VarString msg = "findSupplierByPattern of SupplierDM. Parameter: \n ";
			for( basar::db::aspect::AccessorPropertyTable_XIterator xit = yit.begin(); xit != yit.end(); ++xit )
			{
				if( !( xit.getState() == basar::SS_UNSET ) ){
					msg = msg + xit.getName() + ":" + xit.getSQLString() + "\n";
				}
			}
			narcotics::LoggerPool::loggerDomModules.forcedLog( log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__ );
		}

		m_accSupplier.execute( "SelSupplier", yit );
		return m_accSupplier.getPropertyTable().begin();
	}

} //namespace domMod