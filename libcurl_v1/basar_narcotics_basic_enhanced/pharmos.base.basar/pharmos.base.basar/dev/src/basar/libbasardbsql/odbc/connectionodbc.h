//----------------------------------------------------------------------------
/*! \file
 *  \brief  connection implementation class for odbc
 *  \author Thomas Hörath
 *  \date   06.10.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_CONNECTIONODBC_H
#define GUARD_CONNECTIONODBC_H

//----------------------------------------------------------------------------
#include "connection.h"

//----------------------------------------------------------------------------
class QSqlDatabase;

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//----------------------------------------------------------------------------
//! connection implementation class \n final class
class ConnectionOdbc : public Connection
{
	friend class StatementOdbc;				//!< for accessing setCurrent()
	friend class PreparedStatementOdbc;		//!< for accessing setCurrent()
	friend class ResultsetOdbc;				//!< for accessing setCurrent()

public:

	// --- method section ----------------------------------------------------

	//! create Connection implementation object by database specific connection data	\n no-throw
	static ConnectionSharedPtr		create					(const DatabaseInfo & rDbInfo ,	//!< database specific connection data
															 bool                 openConn	//!< open connection to database
															);

	virtual						   ~ConnectionOdbc			();		//!< close connection		\n no-throw

	//! \brief open connection to database		\n throw InvalidStateException
	virtual void					open					();
	//! \brief close connection to database	\n no-throw
	virtual void					close					();

	//! \brief check if connection is technically open	\n no-throw
	virtual bool					isValid					();	

	//! \brief begin transaction		\n no-throw
	virtual void					begin					();
	//! \brief commit transaction		\n no-throw
	virtual void					commit					();	
	//! \brief rollback transaction	\n no-throw
	virtual void					rollback				();		

	//! create statement reference object			\n no-throw
	virtual StatementRef			createStatement			();		
	
	//! create prepared statement reference object	\n no-throw
	virtual PreparedStatementRef	createPreparedStatement	(const VarString & rSql,		//!< Sql-String for preparing statement
															 CursorTypeEnum	   eCursorType	//!< cursor type
															);


private:

	// --- method section ----------------------------------------------------

	//! \brief construct implementation object by database specific connection data	\n throw InvalidStateException
									ConnectionOdbc			(const DatabaseInfo & rDbInfo ,		//!< database specific connection data NULL
															 bool                 openConn		//!< open connection to database
															);

									ConnectionOdbc			();										//!< forbidden
									ConnectionOdbc			(const ConnectionOdbc & rCopy);			//!< forbidden
	ConnectionOdbc &    			operator =				(const ConnectionOdbc & rCopy);			//!< forbidden

	//! add Qt database driver search path				\n no-throw
	static Int32					addDriverPath			();

	// --- member section ----------------------------------------------------

	typedef boost::shared_ptr < QSqlDatabase >	DatabaseSharedPtr;	//!< reference to Qt database object

	DatabaseSharedPtr				m_QSqlDb;						//!< QSqlDatabase from QT, all calls delegated to this member
};

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

#endif	// GUARD
