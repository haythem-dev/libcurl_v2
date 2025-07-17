#ifndef GUARD_ASPECT_CONNECTION_H
#define GUARD_ASPECT_CONNECTION_H
//----------------------------------------------------------------------------//
/*! \file
 *  \brief  connection implementation class
 *  \author Björn Bischof
 *  \date   27.10.2005
 */
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// includes
//----------------------------------------------------------------------------//
#include "libbasardbsql_connectionref.h"
#include "libbasardbaspect_definitions.h"

//----------------------------------------------------------------------------//
// namespaces
//----------------------------------------------------------------------------//
namespace basar	 {
namespace db	 {
	namespace sql
	{
		class DatabaseInfo;
		class StatementRef;
		class PreparedStatementRef;
	}
namespace aspect {

//----------------------------------------------------------------------------//
// forward declarations
//----------------------------------------------------------------------------//
//class Accessor;
	
////////////////////////////////////////////////////////////////////////////////
//! \brief connection implementation class in dbaspect \n base class
////////////////////////////////////////////////////////////////////////////////
class Connection
{
public:
	//------------------------------------------------------------------------------------------------//
	// typedef section
	//------------------------------------------------------------------------------------------------//
	typedef db::sql::StatementRef StatementRef;		//!< reference of statement
	typedef db::sql::DatabaseInfo DatabaseInfo;		//!< struct containing database infos
	typedef db::sql::PreparedStatementRef  PreparedStatementRef; //!< reference to db::sql preparedStatement

	//! \brief static create-method		\n no-throw
	static boost::shared_ptr<Connection>	create( const db::sql::ConnectionRef connRef );

	//------------------------------------------------------------------------------------------------//
	// object section
	//------------------------------------------------------------------------------------------------//
	//! \brief constructor		\n no-throw
	Connection( db::sql::ConnectionRef );
	
	//! \brief destructor		\n no-throw
	virtual									~Connection			();
	
	//! \brief check if there is a transaction		\n no-throw
	bool									isInTransaction() const;
	
	//! \brief check if accessor is registered		\n InvalidValueException
	bool									isRegistered( const AccessorWeakPtr wpAcc ) const;
	
	//! \brief check if accessor is registered
	bool                                    isRegistered( Accessor& acc ) const;
	
	//! \brief register accessor					\n InvalidValueException
	void									doRegister( const AccessorWeakPtr wpAcc );
	//! \brief deregister accessor					\n NotInTransactionException, InvalidParameterException
	void									unRegister(Accessor & acc);
	
	//! \brief create a statement for this connection	\n no-throw
	Connection::StatementRef				createStatement();

    //! \brief create a prepared statement for this connection	\n no-throw
    Connection::PreparedStatementRef        createPreparedStatement(const VarString& rSql, sql::CursorTypeEnum eCursorType = sql::CURS_DEFAULT);

	//-----------------------------------------------------------------------------------//
	// transaction methods
	//-----------------------------------------------------------------------------------//
	//! \brief	start transaction. \n AlreadyInTransactionException.
	void									beginTransaction();
	//! \brief	realize transaction. \n NotInTransactionException.
	void									commitTransaction();
	//! \brief	cancel transaction. \n NotInTransactionException.
	void									rollbackTransaction();
	
	//! \brief	open connection to database     \n no-throw
	void									open();			
	
	//! \brief	close connection to database    \n no-throw
	void									close();

	//! \brief	check if connection to database is open \n no-throw
	bool									isOpen() const;

    //! \brief check if connection to database is technically open (by SQL query)		\n no-throw
    bool				isValid					();

	//! \brief	set transaction isolation level					\n no-throw
	void									setIsolationLevel( const IsolationLevelEnum eLevel );

	//! \brief	set mode for accessing locked rows or tables	\n no-throw
	void									setLockModeWait( const LockModeEnum eMode, const UInt32 secs = 0 );
	
	//! set fetch buffer size			\n throw  no-throw
	virtual void							setFetBufSize	(int				size				//!< new fetch buffer size in bytes
															);

	//! get fetch buffer size			\n throw  no-throw
	virtual int								getFetBufSize	();

	//! \brief	get database info.		\n no-throw */
	const Connection::DatabaseInfo&			getDbInfo() const;

	//! \brief	get connectionref from db::sql for using sql-functions directly (e.g. prepared statements).		\n no-throw */
	const db::sql::ConnectionRef			getSqlConnection() const;

private:
	//! \brief	set weak ptr					\n no-throw
	void									setThisPtr( const boost::weak_ptr<Connection> pWeak );
	
	//! \brief	reset registered accessors to snapshots	\n InvalidValueException
	void									resetAccessorsToSnapshots();
	/*! \brief	clear the list of accessors, but do previously clear the internal propertytablesnapshot. 
				\n InvalidValueException */
	void									clearRegisteredAccessors();


	db::sql::ConnectionRef							m_SqlConnectionRef;			//!< reference to db::sql connection class
	bool											m_InTransaction;			//!< is there a transaction ?
	boost::weak_ptr<Connection>						m_this_as_weak_ptr;			//!< weak pointer to this

	typedef std::map< Accessor *, AccessorWeakPtr >	AccessorList;				//!< typedef of list of accessors
	AccessorList									m_RegisteredAccessors;		//!< list of registered accessors
};

//----------------------------------------------------------------------------
}	// namespace sapect
}	// namespace db
}	// namespace basar

#endif	// GUARD_ASPECT_CONNECTION_H
