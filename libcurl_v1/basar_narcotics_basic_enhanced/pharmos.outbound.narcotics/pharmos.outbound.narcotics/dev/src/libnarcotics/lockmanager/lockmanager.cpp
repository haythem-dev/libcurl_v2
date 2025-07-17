//------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  class for locking business objects
 *  \author Benjamin Endlicher
 *  \date   18.05.2012
 */
//------------------------------------------------------------------------------------------------//
// #######################################################################################################################
//
// Informix
// =======================================================================================================================
/*! \brief Informix definition of narclock table, adapt as needed

Note:	- replace "narclock" with a table name fitting your application
		- also: businesstype is only 20 chars long, while locking will work, you won't be able to delete that lock using businesstype if your string is longer

BEGIN WORK;

-- Creating table
CREATE TABLE "informix".narclock
	(
	branchno smallint DEFAULT 0 NOT NULL,
	articleno integer DEFAULT 0 NOT NULL,
	username character(30) DEFAULT '' NOT NULL,
	lockdate integer DEFAULT 0 NOT NULL,
	locktime integer DEFAULT 0 NOT NULL,
	businesstype character(20) DEFAULT '' NOT NULL,
	idbusinessobject integer DEFAULT 0 NOT NULL,
	counter integer DEFAULT 0 NOT NULL
	)
	IN dbsdata2
	EXTENT SIZE 16 NEXT SIZE 16
	LOCK MODE ROW;

-- Creating index
CREATE DISTINCT INDEX "informix".i_narclock_1 ON "informix".narclock
	(
	branchno "informix".btree_ops, 
	counter "informix".btree_ops, 
	businesstype "informix".btree_ops, 
	idbusinessobject "informix".btree_ops
	)
	USING "informix".btree
	IN dbsindex1
	ENABLED;

COMMIT WORK;
*/

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "lockmanager.h"
#include "statements.h"

//------------------------------------------------------------------------------
//namespaces
//------------------------------------------------------------------------------
namespace libnarcotics {
namespace lockmanager {

//-------------------------------------------------------------------------------------------------//
// using declarations
//-------------------------------------------------------------------------------------------------//
using basar::db::sql::ConnectionRef;
using basar::db::sql::PreparedStatementRef;
using basar::db::sql::StatementRef;
using basar::db::sql::ResultsetRef;
using basar::db::sql::InsertsetRef;
using basar::db::sql::ExecuteReturnInfo;


	//-------------------------------------------------------------------------------------------------//
	/*!	\return	bool indicating success
		\param	businessType for which a lock is released
		\param	idbusinessobject - id of business object for which lock is released
		\brief	releases a lock - method is transaction-aware (starts/commits it's own transaction only if no transaction has started yet)
		\throw	no-throw */
	//-------------------------------------------------------------------------------------------------//
	bool LockManager::releaseLock(const basar::Int16 branchNo, const basar::VarString& businessType, basar::Int32 idBusinessObject)
	{
		
		if (m_Logger.isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
		{
			basar::VarString msg;
			msg.format("unlock of LockManager. Parameter: branchno : %d, type: %s, idbusinessobject: %d", 
				branchNo, businessType.c_str(), idBusinessObject);
			m_Logger.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
		}

		ExecuteReturnInfo ret;
		bool ownsTransaction = false;

		try
		{
			PreparedStatementRef stmtSelectForRenumber	= m_SqlConnectionRef.createPreparedStatement(m_Statements->getStmtSelectForRenumber());

			PreparedStatementRef stmtDeleteLock			= m_SqlConnectionRef.createPreparedStatement(m_Statements->getStmtDeleteLock());

			PreparedStatementRef stmtSelectLockByUser	= m_SqlConnectionRef.createPreparedStatement(m_Statements->getStmtSelectLockByUser());

			PreparedStatementRef stmtUpdateRenumber		= m_SqlConnectionRef.createPreparedStatement(m_Statements->getStmtUpdateForRenumber());

			stmtSelectLockByUser.setString		(0, m_UserName.c_str());
			stmtSelectLockByUser.setInt16		(1, branchNo);
			stmtSelectLockByUser.setString		(2, businessType.c_str());
			stmtSelectLockByUser.setInt32		(3, idBusinessObject);
			
			stmtSelectForRenumber.setString		(0, businessType.c_str());
			stmtSelectForRenumber.setInt32		(1, idBusinessObject);
			stmtSelectForRenumber.setInt16		(2, branchNo);

			stmtDeleteLock.setString			(0, m_UserName.c_str());
			stmtDeleteLock.setString			(1, businessType.c_str());
			stmtDeleteLock.setInt32				(2, idBusinessObject);
			stmtDeleteLock.setInt16				(3, branchNo);
			

			stmtUpdateRenumber.setInt16			(1, branchNo);
			stmtUpdateRenumber.setString		(2, businessType.c_str());
			stmtUpdateRenumber.setInt32			(3, idBusinessObject);
			
			if (!m_SqlConnectionRef.isInTransaction())
			{
				m_SqlConnectionRef.begin();
				ownsTransaction = true;
			}

			ResultsetRef retStmtSelectLockByUser = stmtSelectLockByUser.executeQuery();

			if (retStmtSelectLockByUser.next())
			{
				stmtDeleteLock.setInt32	(4, retStmtSelectLockByUser.getInt32("counter"));
			}

			ret = stmtDeleteLock.execute();

			ResultsetRef resRenumber = stmtSelectForRenumber.executeQuery();

			int i = 0;

			while(resRenumber.next())
			{
				if (resRenumber.getInt32("counter") > ++i)
				{
					//update 
					stmtUpdateRenumber.setInt32		(0, i);
					stmtUpdateRenumber.setString	(4, resRenumber.getString("username"));
					stmtUpdateRenumber.setInt32		(5, resRenumber.getInt32("counter"));

					stmtUpdateRenumber.execute();
				}
				
			}
			
			if (ownsTransaction)
			{
				m_SqlConnectionRef.commit();
			}

		}
		catch(basar::Exception& ex)
		{
			if (m_Logger.isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
			{
				basar::VarString msg;
				msg.format("releaseLock of LockManager. Exception: %s, Parameters: Branchno: %d, Businesstype: %s, IdBusinessObject: %d ", 
					ex.what().c_str(), branchNo, businessType.c_str(), idBusinessObject);
				m_Logger.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
			}
			if (m_SqlConnectionRef.isInTransaction() && ownsTransaction)
			{
				m_SqlConnectionRef.rollback();
			}
			
			ret.m_error = basar::db::sql::DBErrorCode::UNKNOWN;
			ret.m_affectedRows = 0;
		}

		return !ret.hasError();
	}

	//-------------------------------------------------------------------------------------------------//
	/*!	\return	bool indicating success
		\param	businesstype - name of type of business object, eg 'article', should be same for same kind of lock
		\param  idbusinessobject - id identifying a single business object (eg article number), use constant number if businesstype offern no inherent id
		\param	maximumCounterValue - maximum of concurrent locks (default 1)
		\brief	requests a lock of a businesstype-idbusinessobject id, do not call inside a transaction, for lock will not be visible until commit
		\throw	no-throw */
	//-------------------------------------------------------------------------------------------------//
	bool LockManager::requestLock(const basar::Int16 branchNo, const basar::VarString& businessType, const basar::Int32 idBusinessObject, const basar::Int16 maximumCounterValue = 1)
	{
		
		bool lockGranted = false;

		if (m_Logger.isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
		{
			basar::VarString msg;
			msg.format("lock requested with: BusinessObject-Id : %d, maximumCounterValue: %d", idBusinessObject, maximumCounterValue);
			m_Logger.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
		}
		
		ExecuteReturnInfo ret;

		try
		{
			StatementRef statement;

			statement = m_SqlConnectionRef.createStatement();

			basar::DateTime now = basar::DateTime::getCurrent();

			//stmtInsertLockRaw inserts only if number of locks for given businessType/idBusinessObject combo isn't exceeded by maximumCounterValue
			//maybe masking of sql metachars in username and businesstype is needed
			basar::VarString stmtInsertLock = "";
			stmtInsertLock.format(m_Statements->getStmtInsertLock().c_str(), branchNo, m_UserName.c_str(), businessType.c_str(), idBusinessObject, now.getDate(),
				now.getTime(), branchNo, businessType.c_str(), idBusinessObject, maximumCounterValue, branchNo, businessType.c_str(), idBusinessObject);

			ret  = statement.execute(stmtInsertLock);
		}
		catch(basar::Exception& ex)
		{
			if (m_Logger.isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
			{
				basar::VarString msg;
				msg.format("requestLock of LockManager. Exception: %s, Parameters: Branchno: %d, Businesstype: %s, IdBusinessObject: %d ", 
					ex.what().c_str(), branchNo, businessType.c_str(), idBusinessObject);
				m_Logger.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
			}		
			ret.m_error = basar::db::sql::DBErrorCode::UNKNOWN;
			ret.m_affectedRows = 0;
		}

		if (!ret.hasError() && ret.getAffectedRows() == 1)
		{
			lockGranted = true;
		}

		return lockGranted;
	}

	//-------------------------------------------------------------------------------------------------//
	/*!	\return	bool indicating success
		\param	(none)
		\throw	no-throw */
	//-------------------------------------------------------------------------------------------------//
	bool LockManager::releaseAllLocksByUser(const basar::Int16 branchNo)
	{
		ExecuteReturnInfo ret;

	//	//1: alle locks von username holen
	//	//2: pro lock alle locks der anderen user holen
	//	//	1: lock des users löschen
	//	//	2: counter der anderen user um 1 ernierdigen
		if (m_Logger.isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
		{
			basar::VarString msg;
			msg.format("unlockAllLocksByUserName of LockManager. Parameter: username: %s", 
				m_UserName.c_str());
			m_Logger.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
		}
		bool ownsTransaction = false; //fix to allow to be called inside transactions of other objects, other objects must also handle transaction commit/rollback
		
		try
		{
			PreparedStatementRef stmtDeleteLock			= m_SqlConnectionRef.createPreparedStatement(m_Statements->getStmtDeleteLock());

			PreparedStatementRef stmtSelect				= m_SqlConnectionRef.createPreparedStatement(m_Statements->getStmtSelectAllLocksByUser());

			PreparedStatementRef stmtUpdateRenumber		= m_SqlConnectionRef.createPreparedStatement(m_Statements->getStmtUpdateForRenumber());

			if (!m_SqlConnectionRef.isInTransaction())
			{
				m_SqlConnectionRef.begin();
				ownsTransaction = true;
			}

			stmtSelect.setString(0, m_UserName.c_str());

			ResultsetRef resAllLocksByUser = stmtSelect.executeQuery();
			
			while(resAllLocksByUser.next())
			{
				stmtDeleteLock.setString	(0, m_UserName.c_str());
				stmtDeleteLock.setString	(1, resAllLocksByUser.getString	("businesstype") );
				stmtDeleteLock.setInt32		(2, resAllLocksByUser.getInt32	("idbusinessobject") );
				stmtDeleteLock.setInt16		(3, resAllLocksByUser.getInt16	("branchno") );
				stmtDeleteLock.setInt32		(4, resAllLocksByUser.getInt32	("counter"));


				stmtUpdateRenumber.setInt16		(1, branchNo);
				stmtUpdateRenumber.setString	(2, resAllLocksByUser.getString	("businesstype") );
				stmtUpdateRenumber.setInt32		(3, resAllLocksByUser.getInt32	("idbusinessobject") );

				ret = stmtDeleteLock.execute();

				PreparedStatementRef stmtSelectForRenumber	= m_SqlConnectionRef.createPreparedStatement(m_Statements->getStmtSelectForRenumber());

				stmtSelectForRenumber.setString	(0, resAllLocksByUser.getString	("businesstype") );
				stmtSelectForRenumber.setInt32  (1, resAllLocksByUser.getInt32	("idbusinessobject") );
				stmtSelectForRenumber.setInt16	(2, resAllLocksByUser.getInt16	("branchno") );

				ResultsetRef resRenumber = stmtSelectForRenumber.executeQuery();

				int i = 0;

				while(resRenumber.next())
				{
						stmtUpdateRenumber.setInt32		(0, ++i);
						stmtUpdateRenumber.setString	(4, resRenumber.getString("username"));
						stmtUpdateRenumber.setInt32		(5, resRenumber.getInt32("counter"));

						stmtUpdateRenumber.execute();
				}
			}
			if (ownsTransaction)
			{
				m_SqlConnectionRef.commit();
			}

		}
		catch(basar::Exception& ex)
		{
			if (m_Logger.isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
			{
				basar::VarString msg;
				msg.format("unlockAllLocksByUserName of LockManager. Exception: %s, Parameters: Branchno: %d, UserName: %s ", 
					ex.what().c_str(), branchNo, m_UserName.c_str());
				m_Logger.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
			}
			
			if (m_SqlConnectionRef.isInTransaction() && ownsTransaction)
			{
				m_SqlConnectionRef.rollback();
			}

			ret.m_error = basar::db::sql::DBErrorCode::UNKNOWN;
			ret.m_affectedRows = 0;
		}
	
		return !ret.hasError();
	}

	//-------------------------------------------------------------------------------------------------//
	/*!	\return	bool indicating success
		\param	(none)
		\throw	no-throw */
	//-------------------------------------------------------------------------------------------------//
	bool LockManager::releaseAllLocksByBranch(const basar::Int16 branchNo)
	{
		if (m_Logger.isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
		{
			basar::VarString msg;
			msg.format("unlockAllLocksByBranch of LockManager. Parameter: branchno: %d", 
				branchNo);
			m_Logger.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
		}

		PreparedStatementRef stmtDeleteAllLocksByBranchNo = m_SqlConnectionRef.createPreparedStatement(m_Statements->getStmtDeleteAllLocksByBranch());

		stmtDeleteAllLocksByBranchNo.setInt16	(0, branchNo);

		ExecuteReturnInfo ret = stmtDeleteAllLocksByBranchNo.execute();
	
		return !ret.hasError();
	}

	//-------------------------------------------------------------------------------------------------//
	bool LockManager::releaseAllLocksByBusinessType(const basar::Int16 branchNo, const basar::VarString& businessType)
	{
		if (m_Logger.isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
		{
			basar::VarString msg;
			msg.format("unlockAllLocksByBusinessType of LockManager. Parameter: branchno: %d, businesstype: %s", 
				branchNo, businessType.c_str());
			m_Logger.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
		}

		PreparedStatementRef stmtDeleteAllLocksByBusinessType = m_SqlConnectionRef.createPreparedStatement(m_Statements->getStmtDeleteAllLocksByBusinessType());

		stmtDeleteAllLocksByBusinessType.setInt16	(0, branchNo);
		stmtDeleteAllLocksByBusinessType.setString	(1, businessType.c_str());

		ExecuteReturnInfo ret = stmtDeleteAllLocksByBusinessType.execute();

		return !ret.hasError();
	}

	//-------------------------------------------------------------------------------------------------//
	bool LockManager::releaseAllLocksByBusinessTypeSpecialUserExcluded(const basar::Int16 branchNo, const basar::VarString& businessType)
	{
		if (m_Logger.isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
		{
			basar::VarString msg;
			msg.format("releaseAllLocksByBusinessTypeSpecialUserExcluded of LockManager. Parameter: branchno: %d, businesstype: %s", 
				branchNo, businessType.c_str());
			m_Logger.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
		}

		PreparedStatementRef stmtDeleteAllLocksByBusinessTypeSpecialUserExcluded = 
			m_SqlConnectionRef.createPreparedStatement(m_Statements->getStmtDeleteAllLocksByBusinessTypeSpecialUserExcluded());

		stmtDeleteAllLocksByBusinessTypeSpecialUserExcluded.setInt16	(0, branchNo);
		stmtDeleteAllLocksByBusinessTypeSpecialUserExcluded.setString	(1, businessType.c_str());

		ExecuteReturnInfo ret = stmtDeleteAllLocksByBusinessTypeSpecialUserExcluded.execute();

		return !ret.hasError();
	}

	//-------------------------------------------------------------------------------------------------//
	bool LockManager::releaseAllLocksByBranchBusinessTypeAndUser(const basar::Int16 branchNo, const basar::VarString& businessType)
	{
		if (m_Logger.isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
		{
			basar::VarString msg;
			msg.format("releaseAllLocksByBusinessTypeAndUser of LockManager. Parameter: branchNo: %d, userName: %s, businesstype: %s", 
				branchNo, m_UserName.c_str(), businessType.c_str());
			m_Logger.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
		}

		PreparedStatementRef stmtDeleteAllLocksByBranchBusinessTypeAndUser = m_SqlConnectionRef.createPreparedStatement(m_Statements->getStmtDeleteAllLocksByBranchBusinessTypeAndUser());

		stmtDeleteAllLocksByBranchBusinessTypeAndUser.setInt16(0, branchNo);
		stmtDeleteAllLocksByBranchBusinessTypeAndUser.setString(1, businessType.c_str());
		stmtDeleteAllLocksByBranchBusinessTypeAndUser.setString(2, m_UserName.c_str());

		ExecuteReturnInfo ret = stmtDeleteAllLocksByBranchBusinessTypeAndUser.execute();

		return !ret.hasError();
	}

	//-------------------------------------------------------------------------------------------------//
	bool LockManager::releaseAllLocksByBusinessTypeAndUser( const basar::VarString& businessType)
	{
		if (m_Logger.isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
		{
			basar::VarString msg;
			msg.format("releaseAllLocksByBusinessTypeAndUser of LockManager. Parameter: userName: %s, businesstype: %s", 
				m_UserName.c_str(), businessType.c_str());
			m_Logger.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
		}

		PreparedStatementRef stmtDeleteAllLocksByBusinessTypeAndUser = m_SqlConnectionRef.createPreparedStatement(m_Statements->getStmtDeleteAllLocksByBusinessTypeAndUser());

		stmtDeleteAllLocksByBusinessTypeAndUser.setString(0, businessType.c_str());
		stmtDeleteAllLocksByBusinessTypeAndUser.setString(1, m_UserName.c_str());

		ExecuteReturnInfo ret = stmtDeleteAllLocksByBusinessTypeAndUser.execute();

		return !ret.hasError();
	}

	//-------------------------------------------------------------------------------------------------//
	/*!	\return	
		\param	branch number, reference to active database connection, name of logged-in user, name of lock-storing table
		\throw	no-throw */
	//-------------------------------------------------------------------------------------------------//
	LockManager::LockManager(
		const basar::db::aspect::ConnectionRef activeConn,
		const basar::VarString& userName,
		const basar::VarString& tableName,
		const log4cplus::Logger& logger) :
			m_SqlConnectionRef(activeConn.getSqlConnection()),
			m_UserName(userName),
			m_Logger(logger)
	{
		m_Statements = boost::shared_ptr<IStatements>(new Statements(tableName));
	}

	//-------------------------------------------------------------------------------------------------//
	LockManager::~LockManager()
	{
	}

//------------------------------------------------------------------------------

} //namespace lockmanager
} //namespace libnarcotics

//------------------------------------------------------------------------------
