//------------------------------------------------------------------------------
/*! \file
 *  \brief  connection implementation class for odbc
 *  \author Michael Eichenlaub
 *  \date   06.10.2005
 */
//------------------------------------------------------------------------------

#include "connectionodbc.h"

#include "sqlexcepthandlerodbc.h"
#include "loggerpool.h"
#include "libbasarcmnutil_logging.h"
#include "libbasardbsql_exceptions.h"
#include "libbasardbsql_preparedstatementref.h"
#include "libbasardbsql_statementref.h"
#include "statementodbc.h"
#include "preparedstatementodbc.h"
#include "setenvinfx.h"

#ifdef _WIN32
#pragma warning (push)
#pragma warning(disable: 4800) // qmetatype.h - C4800: forcing value to bool 'true' or 'false' (performance warning)
#pragma warning(disable: 4512) // qobject_impl.h - C4512: 'QtPrivate::QSlotObjectBase' : assignment operator could not be generated
#endif
#include <QtCore/QDir>
#ifdef _WIN32
#pragma warning (pop)
#endif

#include <QtCore/QCoreApplication>
#include <QtSql/QSqlDatabase>

//------------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//------------------------------------------------------------------------------
//! \return	shared pointer to Database implementation object
//! \throw  no-throw
ConnectionSharedPtr	ConnectionOdbc::create( const DatabaseInfo & rDbInfo,
									        bool                 openConn)
{
	ConnectionSharedPtr pShared( new ConnectionOdbc(rDbInfo, openConn));	// create implementation object from heap
	pShared->setWeakPtr(pShared);											// save in weak pointer

	return pShared;
}

//------------------------------------------------------------------------------
//!        must be called before first call of QSqlDatabase::isDriverAvailable().
//!        call is necessary only once,
//!        driver is searched in <currentPath()>/sqldrivers (Qt hardcoded!)
//! \throw no-throw
//! \return 0 (always) - needed for assignment to local static variable
Int32 ConnectionOdbc::addDriverPath()
{
	QCoreApplication::instance()->addLibraryPath(QDir::currentPath());

	return 0;	// dummy; only needed for assignment to local static variable
}

//------------------------------------------------------------------------------
//! \throw InvalidStateException
//! before QSqlDatabase::addDatabase can be called successfully, 
//! QApplication object has to be initialized !!!
ConnectionOdbc::ConnectionOdbc ( const DatabaseInfo & rDbInfo , 
								 bool                 openConn)	
							   : Connection       (rDbInfo)
{
	static ConstString DRIVER = "QODBC";
	static Int32       dummy  = addDriverPath();		// do it once(!) and
														// before(!) calling QSqlDatabase::isDriverAvailable() !

	if ( ! QSqlDatabase::isDriverAvailable(DRIVER) )
	{
		static ConstString func = "basar.db.sql.ConnectionOdbc.ConnectionOdbc";

		VarString msg;
		msg.format("QODBC driver is not available in current path <%s>\n"
			       "or Q(Core)Application has not been initialized!",
				   QDir::currentPath().toLatin1().constData());
		
		throw InvalidStateException(ExceptInfo(func, msg, __FILE__, __LINE__));
	}

	QSqlDatabase* db = new QSqlDatabase(QSqlDatabase::addDatabase(DRIVER, m_ConnId.c_str()));

	db->setConnectOptions("SQL_ATTR_ODBC_VERSION=SQL_OV_ODBC3");

	m_QSqlDb       = DatabaseSharedPtr( db );
	m_SqlExceptHnd = SqlExceptHandlerOdbc::create(*m_QSqlDb);

	if (true == openConn)
		open(); 
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
ConnectionOdbc::~ConnectionOdbc()
{
	try
	{
		close();
		m_SqlExceptHnd.reset();
		m_QSqlDb      .reset();		// QSqlDatabase must be deleted before removeDatabase()
		QSqlDatabase::removeDatabase(m_ConnId.c_str());
	}
	catch (const basar::Exception &)
	{
		assert(0);
	}
	catch (...)
	{
		BLOG_ERROR(LoggerPool::connOdbc(), "unknown exception");
		assert(0);
	}
}

//------------------------------------------------------------------------------
//! \throw InvalidStateException
void ConnectionOdbc::open()
{
	ConstString func = "basar.db.sql.ConnectionOdbc.open";

	if (true == isOpen())
		throw InvalidStateException(ExceptInfo (func, "isOpen() == true", __FILE__, __LINE__));

	switch (m_Database.getConnType())
	{
	case Database::CONNTYPE_USER_DEFAULT:
		m_QSqlDb->setHostName	 (m_Database.getDbInfo().dbServer.c_str());
        m_QSqlDb->setDatabaseName(m_Database.getDbInfo().database.c_str());
        break;

	case Database::CONNTYPE_USER_SET:
	default:
		m_QSqlDb->setHostName	 (m_Database.getDbInfo().dbServer.c_str());
        m_QSqlDb->setDatabaseName(m_Database.getDbInfo().database.c_str());
		        
		m_QSqlDb->setUserName	 (m_Database.getDbInfo().user  .c_str());
        m_QSqlDb->setPassword	 (m_Database.getDbInfo().passwd.c_str());
		break;
	}

	// narrow scope for instance settings => short lifetime and quick restore of old environment values
	{
		SetEnvInfx settings;    // saves Informix environment and restores at destructing
        SetEnvInfx::clear();    // clear Informix environment: needed for Informix ODBC!

		m_QSqlDb->open();
	}
	
	Connection::open();
	
	if (LoggerPool::connOdbc().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("%s opened: %s connected to %s@%s",
				m_ConnId.c_str(),
				m_Database.getDbInfo().user    .c_str(),
				m_Database.getDbInfo().database.c_str(),
				m_Database.getDbInfo().dbServer.c_str());
		LoggerPool::connOdbc().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
		
	checkConnState(func, __FILE__, __LINE__);
}

//------------------------------------------------------------------------------
//! \throw no-throw
void ConnectionOdbc::close()
{	
	if (false == isOpen())	// nothing to do
		return;

	// for open transaction
	if (m_InTransaction)
		rollback(); 

	m_QSqlDb->close	();
	
	Connection::close();

	if (LoggerPool::connOdbc().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("%s closed: %s disconnected from %s@%s",
			       m_ConnId.c_str(),
				   m_Database.getDbInfo().user    .c_str(),
				   m_Database.getDbInfo().database.c_str(),
				   m_Database.getDbInfo().dbServer.c_str());
		LoggerPool::connOdbc().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}

//----------------------------------------------------------------------------
//!	\retval true  if connection is technically opened
//! \retval false if connection is technically close/not opened
//! \throw  no-throw
bool ConnectionOdbc::isValid()
{
	if (!isOpen())
		return false;

	ConstString SQL = "SELECT 1 FROM syscolumns WHERE 0 = 1;";

	StatementRef  stmt   = createStatement();

	try
	{
		ResultsetRef result = stmt.executeSingleQuery(SQL);	
	}
	catch (const DbException &)
	{
        if (LoggerPool::connOdbc().isEnabledFor(log4cplus::ERROR_LOG_LEVEL))
        {
            VarString msg;
            msg.format("connection %s to %s not longer valid: checked with <%s>", 
						m_ConnId       .c_str(),
						getConnectStr().c_str(),
						SQL);
            LoggerPool::connOdbc().forcedLog(log4cplus::ERROR_LOG_LEVEL, msg, __FILE__, __LINE__);
        }

		return false;
	}

	return true;
}

//------------------------------------------------------------------------------
//!	\return created statement reference object
//! \throw no-throw
StatementRef ConnectionOdbc::createStatement()
{
	return StatementRef(StatementOdbc::create(getThis(), *m_QSqlDb)); 
}

//------------------------------------------------------------------------------
//!	\return created prepared statement reference object
//! \throw no-throw
PreparedStatementRef ConnectionOdbc::createPreparedStatement(const VarString & rSql,
															 CursorTypeEnum	       )
{
	return PreparedStatementRef(PreparedStatementOdbc::create(getThis(), rSql, *m_QSqlDb)); 
}

//------------------------------------------------------------------------------
//! \throw no-throw
void ConnectionOdbc::begin()
{
	ConstString func = "basar.db.sql.ConnectionOdbc.begin";

	checkConnState(func, __FILE__, __LINE__);
	
	m_QSqlDb->transaction();

	checkSqlState (func, __FILE__, __LINE__);

	Connection::begin();

	if (LoggerPool::connOdbc().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("%s begin transaction",
			       m_ConnId.c_str());
		LoggerPool::connOdbc().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}

//------------------------------------------------------------------------------
//! \throw no-throw
void ConnectionOdbc::commit()
{
	ConstString func = "basar.db.sql.ConnectionOdbc.commit";

	checkConnState(func, __FILE__, __LINE__);
	
	m_QSqlDb->commit();
	
	checkSqlState (func, __FILE__, __LINE__);

	Connection::commit();

	if (LoggerPool::connOdbc().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("%s commit transaction", m_ConnId.c_str());
		LoggerPool::connOdbc().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}

//------------------------------------------------------------------------------
//! \throw no-throw
void ConnectionOdbc::rollback()
{
	ConstString func = "basar.db.sql.ConnectionOdbc.rollback";

	checkConnState(func, __FILE__, __LINE__);
	
	m_QSqlDb->rollback();

	checkSqlState   (func, __FILE__, __LINE__);

	Connection::rollback();

	if (LoggerPool::connOdbc().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("%s rollback transaction", m_ConnId.c_str());
		LoggerPool::connOdbc().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}

//------------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar
