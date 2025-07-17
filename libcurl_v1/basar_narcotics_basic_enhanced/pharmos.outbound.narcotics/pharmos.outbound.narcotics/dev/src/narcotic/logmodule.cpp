//----------------------------------------------------------------------------
/*! \file
 *  \brief  implementation of domain module "log"
 *  \author	Roland Kiefert
 *  \date   22.02.2006
 */
//----------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------//
// include sections
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "logmodule.h"
#include "loggerpool.h"

//-------------------------------------------------------------------------------------------------//
// namespace sections
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
LogDM::LogDM()
:m_Initialized( 0 )
{
}

//--------------------------------------------------------------------------------------------------------------//
/*!	\return	void
	\param	activeConn	represents the active connection which has to be passed to create the two AccessorInstanceRef members.
	\param	branchNo	represents the branch no. Every domain module is initialized with the branch no which it is running for.
	\param	userName	is assigned to a member - important for logging entries.
	\throw	no-throw */
//--------------------------------------------------------------------------------------------------------------//
void LogDM::init(	const basar::db::aspect::ConnectionRef activeConn, 
					const basar::Int16 branchNo, 
					const basar::VarString userName )
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::VarString msg;
		msg.format("init of LogDM. Parameter: branchNo : %d; userName : %s", branchNo, userName.c_str());
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}

	if( 0 == m_Initialized )
	{
		m_BranchNo = branchNo;
		m_userName = userName;
		basar::I18nString branch;
		branch.format( "%s=%i; %s; %s; %s; %s; %s; %s; %s;",
			constants::BRANCHNO,
			m_BranchNo, 
			constants::FROMDATE, 
			constants::TODATE, 
			constants::USERNAME, 
			constants::CHANGEDATTRIBUTE, 
			constants::ARTICLENO, 
			constants::ARTICLECODE, 
			constants::CATALOGID );

		m_searchIterator = basar::db::aspect::Manager::getInstance().createAccessorYIterator(branch);

		m_accNarProtocol = basar::db::aspect::Manager::getInstance().createAccessorInstance( 
			"Acc_NarcoticsProtocol", "NarcoticsProtocol", activeConn );

		m_accNarTransaction = basar::db::aspect::Manager::getInstance().createAccessorInstance( 
							"Acc_NarcoticsTransactionData", "NarcoticsTransactionData", activeConn );

	}
	++m_Initialized;
}

//--------------------------------------------------------------------------------------------------------------//
bool LogDM::isInitialized()
{
    return m_Initialized > 0;
}

//--------------------------------------------------------------------------------------------------------------//
/*!	\return	void
	\throw	no-throw */
//--------------------------------------------------------------------------------------------------------------//
void LogDM::shutdown()
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "shutdown of LogDM.";
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	if( m_Initialized == 0 ){
		return;
	}
	--m_Initialized;
	if( 0 == m_Initialized )
	{
		m_accNarProtocol.reset();
		m_accNarTransaction.reset();
	}
}

//--------------------------------------------------------------------------------------------------------------//
/*!	\return	void
	\throw	no-throw */
//--------------------------------------------------------------------------------------------------------------//
void LogDM::clearSearchIterator()
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "clearSearchIterator of LogDM.";
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	m_searchIterator.setPropertyState( constants::FROMDATE,			basar::SS_UNSET	);
	m_searchIterator.setPropertyState( constants::TODATE,			basar::SS_UNSET	);
	m_searchIterator.setPropertyState( constants::ARTICLENO,		basar::SS_UNSET	);
	m_searchIterator.setPropertyState( constants::ARTICLECODE,		basar::SS_UNSET	);
	m_searchIterator.setPropertyState( constants::USERNAME,			basar::SS_UNSET	);
	m_searchIterator.setPropertyState( constants::CHANGEDATTRIBUTE, basar::SS_UNSET	);
	m_searchIterator.setPropertyState( constants::CATALOGID,		basar::SS_UNSET	);
}

//--------------------------------------------------------------------------------------------------------------//
/*!	\return	AccessorPropertyTable_YIterator represents the search iterator
	\throw	no-throw */
//--------------------------------------------------------------------------------------------------------------//
basar::db::aspect::AccessorPropertyTable_YIterator LogDM::getSearchIterator()
{
	return m_searchIterator;
}

//--------------------------------------------------------------------------------------------------------------//
/*!	\return	AccessorPropertyTableRef represents the search hits
	\param	activeConn	represents the active connection and is used here for an lokal instance of a AccessorInstanceRef
	\param	yit contains the parameters affecting the search result of logging entries.
	\throw	no-throw */
//--------------------------------------------------------------------------------------------------------------//
basar::db::aspect::AccessorPropertyTableRef LogDM::findLogsByPattern(	const basar::db::aspect::ConnectionRef activeConn,	
																		AccessorPropertyTable_YIterator yit )
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::VarString msg = "findLogsByPattern of LogDM. Parameter: \n ";
		for( basar::db::aspect::AccessorPropertyTable_XIterator xit = yit.begin(); xit != yit.end(); ++xit )
		{
			if( !(xit.getState() == basar::SS_UNSET) ){
				msg = msg + xit.getName() + ":" + xit.getSQLString() + "\n";
			}
		}
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}
	basar::db::aspect::AccessorInstanceRef accPrintProt	= 
		basar::db::aspect::Manager::getInstance().createAccessorInstance( "Acc_PrintProtocol", "NarcoticsProtocol", activeConn);

	m_accPrintProtList.push_back(accPrintProt);
	accPrintProt.execute( "SelProtocol", yit );
	return accPrintProt.getPropertyTable();
}

//--------------------------------------------------------------------------------------------------------------//
/*!	\return	AccessorPropertyTableRef represents the search hits
	\param	yit contains the parameters affecting the search result of logging entries.
	\throw	no-throw */
//--------------------------------------------------------------------------------------------------------------//
basar::db::aspect::AccessorPropertyTableRef LogDM::findLogEntryByPattern( AccessorPropertyTable_YIterator yit )
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::VarString msg = "findLogEntryByPattern of LogDM. Parameter: \n ";
		for( basar::db::aspect::AccessorPropertyTable_XIterator xit = yit.begin(); xit != yit.end(); ++xit )
		{
			if( !(xit.getState() == basar::SS_UNSET) ){
				msg = msg + xit.getName() + ":" + xit.getSQLString() + "\n";
			}
		}
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}
	m_accNarProtocol.execute("SelProtocol", yit);

	return m_accNarProtocol.getPropertyTable();
}

//--------------------------------------------------------------------------------------------------------------//
/*!	\return	AccessorPropertyTableRef represents the search hits
	\param	yit	contains the parameters affecting the search result of narcotic bookings.
	\throw	no-throw */
//--------------------------------------------------------------------------------------------------------------//
basar::db::aspect::AccessorPropertyTableRef	LogDM::findCatalogByPattern( basar::db::aspect::AccessorPropertyTable_YIterator yit )
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::VarString msg = "findCatalogByPattern of LogDM. Parameter: \n ";
		for( basar::db::aspect::AccessorPropertyTable_XIterator xit = yit.begin(); xit != yit.end(); ++xit )
		{
			if( !(xit.getState() == basar::SS_UNSET) ){
				msg = msg + xit.getName() + ":" + xit.getSQLString() + "\n";
			}
		}
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}
	m_accNarTransaction.execute("SelTransactionCatalog", yit);

	return m_accNarTransaction.getPropertyTable();
}

//--------------------------------------------------------------------------------------------------------------//
/*!	\retval	 >0	signalizes the number of datasets are affected. \n
	\retval	 0	signalizes that nothing was saved. \n
	\retval	 <0	signalizes that something went wrong. \n
	\param	catalogid			represents the unique key of the changed narcotic booking. \n
	\param	changedAttribute	represents the changed attribute of the concerned narcotic booking. \n
	\param	valueOld			represents the old value of the changed attribute of the concerned narcotic booking. \n
	\param	valueNew			represents the new value of the changed attribute of the concerned narcotic booking. \n
	\throw	no-throw */
//--------------------------------------------------------------------------------------------------------------//
bool LogDM::saveLogEntryUpdate(	Int32 catalogid, basar::VarString changedAttribute,
								basar::VarString valueOld,	basar::VarString valueNew )
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::VarString msg;
		msg.format( "saveLogEntryUpdate of LogDM. Parameter: catalogid : %ld; changedAttribute : %s; valueOld : %s; valueNew : %s ", 
						catalogid, 
						changedAttribute.c_str(),
						valueOld.c_str(),
						valueNew.c_str() );
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}

	m_insertIterator = addEmptyProtocol();
	m_insertIterator.setInt32(	constants::CATALOGID,			catalogid);
	m_insertIterator.setString(	constants::USERNAME,			m_userName);
	m_insertIterator.setInt32(	constants::PROTOCOLDATE,		basar::cmnutil::Date::getCurrent().getDate());
	m_insertIterator.setInt32(	constants::PROTOCOLTIME,		basar::cmnutil::DateTime::getCurrent().getTime()/1000);
	m_insertIterator.setString(	constants::ACTIVITYTYPE,		"U");
	m_insertIterator.setString(	constants::CHANGEDATTRIBUTE,	changedAttribute);
	m_insertIterator.setString(	constants::VALUEOLD,			valueOld);
	m_insertIterator.setString(	constants::VALUENEW,			valueNew);

	return (m_accNarProtocol.execute("InsProtocolUpdate", m_insertIterator)).hasError(); //m_affectedRows;
}
//--------------------------------------------------------------------------------------------------------------//
/*!	\retval	 >0	signalizes the number of datasets are affected. \n
	\retval	 0	signalizes that nothing was saved. \n
	\retval	 <0	signalizes that something went wrong. \n
	\param	catalogid is the unique key of a narcotic booking which has been deleted.
	\throw	no-throw */
//--------------------------------------------------------------------------------------------------------------//
bool LogDM::saveLogEntryDelete( Int32 catalogid )
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::VarString msg;
		msg.format("saveLogEntryDelete of LogDM. Parameter: catalogid : %ld", catalogid);
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}

	m_insertIterator = addEmptyProtocol();
	m_insertIterator.setInt32(	constants::CATALOGID,		catalogid);
	m_insertIterator.setString(	constants::USERNAME,		m_userName);
	m_insertIterator.setInt32(	constants::PROTOCOLDATE,	basar::cmnutil::Date::getCurrent().getDate() );
	m_insertIterator.setInt32(	constants::PROTOCOLTIME,	basar::cmnutil::DateTime::getCurrent().getTime()/1000);
	m_insertIterator.setString(	constants::ACTIVITYTYPE,	"D");

	return (m_accNarProtocol.execute("InsProtocolDelete", m_insertIterator)).hasError(); //m_affectedRows;
}
//--------------------------------------------------------------------------------------------------------------//
/*!	\retval	 >0	signalizes the number of datasets are affected. \n
	\retval	 0	signalizes that nothing was saved. \n
	\retval	 <0	signalizes that something went wrong. \n
	\param	catalogid is the unique key of a narcotic booking which has been restored.
	\throw	no-throw */
//--------------------------------------------------------------------------------------------------------------//
bool LogDM::saveLogEntryRestore( Int32 catalogid )
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::VarString msg;
		msg.format("saveLogEntryRestore of LogDM. Parameter: catalogid : %ld", catalogid);
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}

	m_insertIterator = addEmptyProtocol();
	m_insertIterator.setInt32(	constants::CATALOGID,		catalogid);
	m_insertIterator.setString(	constants::USERNAME,		m_userName);
	m_insertIterator.setInt32(	constants::PROTOCOLDATE,	basar::cmnutil::Date::getCurrent().getDate() );
	m_insertIterator.setInt32(	constants::PROTOCOLTIME,	basar::cmnutil::DateTime::getCurrent().getTime()/1000);
	m_insertIterator.setString(	constants::ACTIVITYTYPE,	"R");
	
	return (m_accNarProtocol.execute("InsProtocolRestore", m_insertIterator)).hasError(); //m_affectedRows;
}

//--------------------------------------------------------------------------------------------------------------//
/*!	\return	AccessorPropertyTableRef represents the narcotic protocols depending on the search criteria
	\throw	no-throw */
//--------------------------------------------------------------------------------------------------------------//
AccessorPropertyTableRef LogDM::getAccNarProtocolPropTab()
{
	return m_accNarProtocol.getPropertyTable();
}

//--------------------------------------------------------------------------------------------------------------//
/*!	\return	AccessorPropertyTableRef represents the narcotic bookings depending on the search criteria
	\throw	no-throw */
//--------------------------------------------------------------------------------------------------------------//
AccessorPropertyTableRef LogDM::getAccNarTransactionPropTab()
{
	return m_accNarTransaction.getPropertyTable();
}

//--------------------------------------------------------------------------------------------------------------//
/*!	\return	AccessorPropertyTable_YIterator representing the newly inserted dataset in the before cleared PropertyTable
	\throw	no-throw */
//--------------------------------------------------------------------------------------------------------------//
AccessorPropertyTable_YIterator LogDM::addEmptyProtocol()
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "addEmptyProtocol of LogDM.";
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	getAccNarProtocolPropTab().clear();
	return getAccNarProtocolPropTab().insert( basar::FOR_INSERT );
}

} //namespace domMod