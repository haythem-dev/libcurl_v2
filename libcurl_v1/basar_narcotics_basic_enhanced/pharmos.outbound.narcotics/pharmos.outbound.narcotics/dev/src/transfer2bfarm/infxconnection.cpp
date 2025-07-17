//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "infxconnection.h"
#include "loggerpool.h"
#include "version.h"

//--------------------------------------------------------------------------------------------------//
// namespace section
//--------------------------------------------------------------------------------------------------//
namespace narcotics
{
namespace connection
{

//--------------------------------------------------------------------------------------------------//
InfxConnection::InfxConnection()
{
}

//--------------------------------------------------------------------------------------------------//
InfxConnection::~InfxConnection()
{
}

//--------------------------------------------------------------------------------------------------//

void InfxConnection::initConnection( const basar::db::sql::DatabaseInfo& dbInfo)
{
	m_DbInfo = dbInfo;
}

//----------------------------------------------------------------------------

void InfxConnection::connect()
{
	m_ActivConn = basar::db::aspect::Manager::getInstance().createConnect(m_DbInfo);
	
	if( LoggerPool::getLoggerTransferBfarm().isEnabledFor(log4cplus::INFO_LOG_LEVEL) )
	{
		basar::VarString msg;
		msg.format("%s (%s) connected successfully < %s, %s >, user < %s >.",
			PROGRAM_NAME, VERSION_NUMBER, m_DbInfo.dbServer.c_str(), m_DbInfo.database.c_str(),
			basar::login::Manager::getInstance().getUserName().c_str());
		LoggerPool::getLoggerTransferBfarm().forcedLog(log4cplus::INFO_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	m_ActivConn.setLockModeWait( basar::WAITSECS, 120 );   
}

//----------------------------------------------------------------------------

void InfxConnection::disconnect()
{
	m_ActivConn.close();
}

//----------------------------------------------------------------------------

basar::db::aspect::ConnectionRef InfxConnection::getCurrentConnection()
{
	return m_ActivConn;
}

//----------------------------------------------------------------------------

bool InfxConnection::isOpen()
{
	if (m_ActivConn.isNull())
		return false;
	return m_ActivConn.isOpen();
}


} //namespace connection
} //namespace narcotics
