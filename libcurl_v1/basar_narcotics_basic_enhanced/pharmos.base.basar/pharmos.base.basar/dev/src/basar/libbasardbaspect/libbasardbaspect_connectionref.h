#ifndef GUARD_LIBBASARDBASPECT_CONNECTIONREF_H
#define GUARD_LIBBASARDBASPECT_CONNECTIONREF_H
//------------------------------------------------------------------------------
/*! \file
 *  \brief  public library interface: reference class for database connection
 *  \author Björn Bischof
 *  \date   15.10.2005
 */
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------//
// dll exports and imports
//------------------------------------------------------------------------------//
#ifndef LIBBASARDBASPECT_API
	#ifdef _WIN32
		#ifdef LIBBASARDBASPECT_EXPORTS
			#define LIBBASARDBASPECT_API __declspec(dllexport)	//!< dll exported
		#else
			#define LIBBASARDBASPECT_API __declspec(dllimport)	//!< dll imported
		#endif 
	#else 
		#define LIBBASARDBASPECT_API
	#endif 
#endif 


//----------------------------------------------------------------------------//
// includes
//----------------------------------------------------------------------------//
#include "libbasardbaspect_definitions.h"
#include "libbasardbsql_definitions.h"

//----------------------------------------------------------------------------//
// namespaces
//----------------------------------------------------------------------------//
namespace basar  {
namespace db	 {
	namespace sql 
	{
		class DatabaseInfo;
		class ConnectionRef;
		class StatementRef;
		class PreparedStatementRef;
	}
namespace aspect {

//----------------------------------------------------------------------------//
// forward declarations
//----------------------------------------------------------------------------//
class Connection;

//------------------------------------------------------------------------------
//! reference class representing connection (session) with database \n final class 
class ConnectionRef
{
	friend class ManagerImpl;
	friend class Accessor;
public:
	//------------------------------------------------------------------------------//
	// typedef section
	//------------------------------------------------------------------------------//
	typedef db::sql::DatabaseInfo           DatabaseInfo;	      //!< struct containing database infos
	typedef db::sql::StatementRef           StatementRef;	      //!< reference to db::sql statement
	typedef db::sql::PreparedStatementRef   PreparedStatementRef; //!< reference to db::sql preparedStatement

	//------------------------------------------------------------------------------//
	// object section
	//------------------------------------------------------------------------------//
	//! \brief constructs reference object and initialises shared pointer to implementation object to null \n no-throw
	LIBBASARDBASPECT_API						ConnectionRef();

private:
	//! \brief constructs reference object  \n no-throw
	LIBBASARDBASPECT_API						ConnectionRef( boost::shared_ptr<Connection> connImpl );

public:
	//! \brief destructs reference object and unregisters connection by manager \n no-throw
	LIBBASARDBASPECT_API						~ConnectionRef();

	//!	\brief sets pointer to implementation object to null	 \n no-throw
	LIBBASARDBASPECT_API void					reset();		
	
	//!	\brief checks if shared pointer to implementation class is null \n no-throw
	LIBBASARDBASPECT_API bool					isNull() const;	

	//-----------------------------------------------------------------------------------//
	// transaction methods
	//-----------------------------------------------------------------------------------//
	//! \brief	starts transaction. \n NullReferenceException.
	LIBBASARDBASPECT_API void				beginTransaction();
	//! \brief	realizes transaction. \n NullReferenceException.
	LIBBASARDBASPECT_API void				commitTransaction();
	//! \brief	cancels transaction. \n NullReferenceException.
	LIBBASARDBASPECT_API void				rollbackTransaction();
	//!	\brief indicates if a transaction is active or not. \n throws NullReferenceException.
	LIBBASARDBASPECT_API bool				isInTransaction() const;

	//! \brief	opens connection to database     \n no-throw
	LIBBASARDBASPECT_API void				open();

	//! \brief  closes connection to database    \n no-throw
	LIBBASARDBASPECT_API void				close();

	//! \brief checks if connection to database is open? \n no-throw
	LIBBASARDBASPECT_API bool				isOpen() const;

    //!< check if connection to database is technically open (by SQL query)		\n no-throw
    LIBBASARDBASPECT_API bool				isValid					();

	//! \brief	sets transaction isolation level					\n no-throw
	LIBBASARDBASPECT_API void				setIsolationLevel( IsolationLevelEnum eLevel );

	//! \brief	sets mode for accessing locked rows or tables	\n no-throw
	LIBBASARDBASPECT_API void				setLockModeWait( LockModeEnum eMode, UInt32 secs = 0 );

	//! \brief  sets fetch buffer size			\n no-throw
	LIBBASARDBASPECT_API void				setFetBufSize	(int size	//!< new fetch buffer size in bytes
															);
	//! \brief  gets fetch buffer size			\n no-throw
	LIBBASARDBASPECT_API int				getFetBufSize	();

	//! \brief	gets database info. \n no-throw */
	LIBBASARDBASPECT_API 
	const DatabaseInfo &					getDbInfo() const;

	//! \brief	get connectionref from db::sql for using sql-functions directly (e.g. prepared statements).		\n no-throw */
	LIBBASARDBASPECT_API 
	const db::sql::ConnectionRef			getSqlConnection() const;

private:
	//! \brief checks if accessor is registered		\n no-throw
	bool									isRegistered( const AccessorWeakPtr wpAcc ) const;

    //! \brief checks if accessor is registered		\n no-throw
    bool                                    isRegistered(Accessor& acc ) const;

	//! \brief registers accessor					\n no-throw
	void									doRegister( const AccessorWeakPtr wpAcc );
	//! \brief deregisters accessor					\n no-throw
	void									unRegister( Accessor & acc );

	//! \brief checks and returns shared pointer to implementation object \n NullReferenceException
	const boost::shared_ptr<Connection>&	getImpl() const;

	//! \brief	creates statement object. \n no-throw
	StatementRef							createStatement();	
	
    //! \brief	creates prepared statement object. \n no-throw
	PreparedStatementRef					createPreparedStatement(const VarString& rSql, sql::CursorTypeEnum eCursorType = sql::CURS_DEFAULT);			

	boost::shared_ptr<Connection>			m_pConnImpl;	//!< shared pointer to aspect::connection implementation object
	
};

//------------------------------------------------------------------------------
}	// namespace aspect
}	// namespace db
}	// namespace basar

//------------------------------------------------------------------------------
#endif	// GUARD_LIBBASARDBASPECT_CONNECTIONREF_H
