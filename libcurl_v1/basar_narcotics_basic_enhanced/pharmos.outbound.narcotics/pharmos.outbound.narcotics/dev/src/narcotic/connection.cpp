//----------------------------------------------------------------------------
/*! \file
 *  \brief  connection class implementation
 *  \author	Thomas Hörath
 *  \date   22.02.2006
 */
//----------------------------------------------------------------------------
//includes
//----------------------------------------------------------------------------
#include "commonheader.h"
#include "connection.h"
#include "loggerpool.h"
#include "version.h"

//----------------------------------------------------------------------------
//namespaces
//----------------------------------------------------------------------------
namespace narcotics 
{

//----------------------------------------------------------------------------
/*!	\throw no-throw */
Connection::Connection()
{
}

//----------------------------------------------------------------------------
/*!	\throw no-throw */
Connection::~Connection()
{
}

//----------------------------------------------------------------------------
void Connection::initConnection()
{
	setDbInfo();
}

//----------------------------------------------------------------------------
/*!	\throw BasarException */
void Connection::connect()
{
	m_ActivConn = basar::db::aspect::Manager::getInstance().createConnect(m_DbInfo);
	
	if( LoggerPool::loggerNarcotics.isEnabledFor(log4cplus::INFO_LOG_LEVEL) )
	{
		basar::VarString msg;
		msg.format("narcotics(%s) connected successfully < %s, %s >, user < %s >.", VERSION_NUMBER, m_DbInfo.dbServer.c_str(), m_DbInfo.database.c_str(), basar::login::Manager::getInstance().getUserName().c_str());
		LoggerPool::loggerNarcotics.forcedLog(log4cplus::INFO_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	m_ActivConn.setLockModeWait( basar::WAITSECS, 120 );   
}

//----------------------------------------------------------------------------
/*!	\throw no-throw */
void Connection::setDbInfo()
{
	basar::VarString host	= UADM_HOST;
	basar::VarString db		= UADM_DB;

	basar::VarString branch;
	basar::VarString user;
	basar::I18nString hostVal;			
	basar::I18nString databaseVal;		

	branch.itos( basar::login::Manager::getInstance().getAreaID() );
	user = basar::login::Manager::getInstance().getUserName();

	if( basar::login::Manager::getInstance().getParaValue( branch, user, UADM_HOST, hostVal ) == false )
	{
		// no user-specific host parameter found (for individual testing!)
		basar::login::Manager::getInstance().getParaValue( branch, UADM_HOST, hostVal );
	}
	
	if( basar::login::Manager::getInstance().getParaValue( branch, user, UADM_DB, databaseVal ) == false )
	{
		// no user-specific database parameter found (for individual testing!)
		basar::login::Manager::getInstance().getParaValue( branch, UADM_DB, databaseVal );
	}
	
	m_DbInfo.eConnectType		= basar::INFORMIX;
	m_DbInfo.dbServer			= hostVal;
	m_DbInfo.database			= databaseVal;
	// open concurrent transaction
	m_DbInfo.concurrTransaction = true;
}

//----------------------------------------------------------------------------
/*!	\throw BasarException */
void Connection::disconnect()
{
	if( !m_ActivConn.isNull() )
	{
		m_ActivConn.close();

		if( LoggerPool::loggerNarcotics.isEnabledFor(log4cplus::INFO_LOG_LEVEL) )
		{
			basar::VarString msg;
			msg.format("narcotics disconnected successfully < %s, %s >, user < %s >.", m_DbInfo.dbServer.c_str(), m_DbInfo.database.c_str(), basar::login::Manager::getInstance().getUserName().c_str());
			LoggerPool::loggerNarcotics.forcedLog(log4cplus::INFO_LOG_LEVEL, msg, __FILE__, __LINE__);
		}
	}
}

//----------------------------------------------------------------------------
/*! \throw no-throw */
basar::db::aspect::ConnectionRef Connection::getCurrentConnection()
{
	return m_ActivConn;
}

} //namespaces narcotics