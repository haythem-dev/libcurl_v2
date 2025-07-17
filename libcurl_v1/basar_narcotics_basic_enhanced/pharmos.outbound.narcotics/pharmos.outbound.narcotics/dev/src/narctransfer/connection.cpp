//----------------------------------------------------------------------------
/*! \file
 *  \brief  connection class implementation
 *  \author	
 *  \date   
 */
//----------------------------------------------------------------------------
//includes
//----------------------------------------------------------------------------
#include "commonheader.h"
#include "connection.h"
#include "loggerpool.h"
#include "version.h"
#include "narctransfer_definitions.h"

//----------------------------------------------------------------------------
//namespaces
//----------------------------------------------------------------------------
namespace narcotics
{

//----------------------------------------------------------------------------
/*!	\throw no-throw */
Connection::Connection()
{
	BLOG_TRACE_METHOD(LoggerPool::loggernarctransfer, "Connection::Connection()");
}

//----------------------------------------------------------------------------
/*!	\throw no-throw */
Connection::~Connection()
{
	BLOG_TRACE_METHOD(LoggerPool::loggernarctransfer, "Connection::~Connection()");
}

//----------------------------------------------------------------------------
/*!	\throw BasarException */
void Connection::connect()
{
	BLOG_TRACE_METHOD(LoggerPool::loggernarctransfer, "Connection::connect()");

	setDbInfo();
	m_ActivConn = basar::db::aspect::Manager::getInstance().createConnect( m_DbInfo );
	m_ActivConn.setLockModeWait( basar::WAITSECS, 30 );   

	if( LoggerPool::loggernarctransfer.isEnabledFor(log4cplus::INFO_LOG_LEVEL) ){
		basar::VarString msg;
		msg.format( "%s connected successfully < %s, %s > for branch < %d >, user < %s >.", 
			PROGRAM_NAME, m_DbInfo.dbServer.c_str(), m_DbInfo.database.c_str(),
			basar::login::Manager::getInstance().getAreaID(), basar::login::Manager::getInstance().getUserName().c_str());
		LoggerPool::loggernarctransfer.forcedLog( log4cplus::INFO_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__ );
	}
}

//----------------------------------------------------------------------------
/*!	\throw no-throw */
void Connection::setDbInfo()
{
	BLOG_TRACE_METHOD(LoggerPool::loggernarctransfer, "Connection::setDbInfo()");

	basar::I18nString databaseVal, hostVal, branchnoVal;

	branchnoVal.itos(basar::login::Manager::getInstance().getAreaID());
	basar::login::Manager::getInstance().getParaValue(branchnoVal, uadm::UADM_HOST, hostVal);
	basar::login::Manager::getInstance().getParaValue(branchnoVal, uadm::UADM_DB, databaseVal);

	m_DbInfo.eConnectType		 = basar::INFORMIX;
	m_DbInfo.dbServer			 = hostVal;
	m_DbInfo.database			 = databaseVal;
}

//----------------------------------------------------------------------------
/*!	\throw BasarException */
void Connection::disconnect()
{
	BLOG_TRACE_METHOD(LoggerPool::loggernarctransfer, "Connection::disconnect()");

	if (!m_ActivConn.isNull())
	{
		m_ActivConn.close();
		if (LoggerPool::loggernarctransfer.isEnabledFor(log4cplus::INFO_LOG_LEVEL))
		{
			basar::VarString msg;
			msg.format( "%s disconnected successfully < %s, %s >.", PROGRAM_NAME, m_DbInfo.dbServer.c_str(), m_DbInfo.database.c_str() );
			LoggerPool::loggernarctransfer.forcedLog( log4cplus::INFO_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__ );
		}
	}
}

//----------------------------------------------------------------------------
/*! \throw no-throw 
	\return Reference to current connection; used to create an instance of an accessor definition */
const ConnectionRef Connection::getCurrentConnection() const
{
	return m_ActivConn;
}

} //namespace obst