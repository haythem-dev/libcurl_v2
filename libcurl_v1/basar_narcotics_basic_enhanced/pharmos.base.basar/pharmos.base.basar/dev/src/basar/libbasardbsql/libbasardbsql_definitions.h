//------------------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  public library interface: definitions
 *  \author Michael Eichenlaub
 *  \date   14.08.2005
 */
//------------------------------------------------------------------------------------------------------------//

#ifndef GUARD_LIBBASARDBSQL_DEFINITIONS_H
#define GUARD_LIBBASARDBSQL_DEFINITIONS_H

//------------------------------------------------------------------------------------------------------------//
// includes
//------------------------------------------------------------------------------------------------------------//
#include <map>
#include <vector>
#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "libbasar_definitions.h"
#include "libbasarcmnutil_bstring.h"
//------------------------------------------------------------------------------------------------------------//
// namespace section
//------------------------------------------------------------------------------------------------------------//
namespace basar {
namespace db	{
namespace sql	//! database data access via sql
{

//------------------------------------------------------------------------------------------------------------//
// forward declarations
//------------------------------------------------------------------------------------------------------------//
class SqlDiagnostics;
class Resultset;
class Insertset;
class Statement;
class PreparedStatement;
class Connection;

//------------------------------------------------------------------------------------------------------------//
// typedef section
//------------------------------------------------------------------------------------------------------------//

typedef boost::weak_ptr   < Connection        > ConnectionWeakPtr;			//!< weak reference to connection implementation object that's already managed by a shared_ptr (ConnectionRef)


typedef std::map < Connection *, 
                   ConnectionWeakPtr          > CollConnection;				//!< \brief definition for connection collection
																			//!< \n     key  : implementation object's address
																			//!< \n     value: weak pointer to implementation object
typedef CollConnection::iterator				CollConnectionIt;			//!< iterator for connection collection
typedef CollConnection::value_type				CollConnectionPair;			//!< value pair of connection collection
typedef CollConnection::size_type				ConnectionListSize;			//!< size of connection collection


typedef boost::shared_ptr < SqlDiagnostics	  >	SqlDiagSharedPtr   ;		//!< reference to SQL diagnostics implementation object
typedef boost::shared_ptr < Resultset		  >	ResultsetSharedPtr ;		//!< reference to resultset  implementation object
typedef boost::shared_ptr < Insertset		  >	InsertsetSharedPtr ;		//!< reference to insertset  implementation object
typedef boost::shared_ptr < Statement		  >	StatementSharedPtr ;		//!< reference to statement  implementation object
typedef boost::shared_ptr < PreparedStatement >	PreparedStatementSharedPtr;	//!< reference to prepared statement  implementation object
typedef boost::shared_ptr < Connection		  >	ConnectionSharedPtr;		//!< reference to connection implementation object

typedef boost::shared_ptr < CollConnection    >	CollConnectionSharedPtr;	//!< reference to connection collection implementation object

typedef std::vector< VarString >                CollColNames;               //!< definition for vector with selected column names
typedef std::vector< basar::SupportedTypeEnum > CollColTypes;               //!< definition for vector with selected column types
//------------------------------------------------------------------------------------------------------------//
// enum section
//------------------------------------------------------------------------------------------------------------//
enum CursorTypeEnum		//! cursor types
{
	CURS_DEFAULT          = 0x00,	//!< default cursor behaviour: cursor should be closed after transaction
	CURS_HOLD_OVER_COMMIT = 0x01	//!< cursor will not be closed when transaction has finished (commit/rollback)
};

//------------------------------------------------------------------------------------------------------------//
//! special Error codes
//------------------------------------------------------------------------------------------------------------//
struct DBErrorCode
{
    //! special Error codes
    enum Code
    {    
        SUCCESS  = 0 ,  //!< no error        
        UNKNOWN,        //!< unknown error, might be INSERT_DUPL or NONSPECIFIC
        INSERT_DUPL,    //!< "duplicate insert" error
        NONSPECIFIC,    //!< non-specific error, "rest/remainder" of errors, at present all not INSERT_DUPL errors
		LENGHT_EXCEEDED //!< passed sql statement is too large for library backend (primarily a CICS error)
    };
};

//------------------------------------------------------------------------------------------------------------//
//! non-interpreted database error codes
//------------------------------------------------------------------------------------------------------------//
struct DBError
{
    //! default values
    DBError() : m_rawMainError(0), m_rawSubError(0)
    {};

    basar::Int32       m_rawMainError;      //!< main error number of used db api (infx: main, odbc: query error code, tcAccess: error code)
    basar::Int32       m_rawSubError;       //!< sub error number of used db api, if applicable (infx: isam, odbc: db error code, tcAccess: always 0)
    basar::VarString   m_errorMsg;          //!< error message(s) of used db api, if applicalbe
};

//------------------------------------------------------------------------------------------------------------//
//! Return Info of a execute Statement 
//------------------------------------------------------------------------------------------------------------//
struct ExecuteReturnInfo
{
	typedef BULong RowNumber;				//!< type for row counting

    DBErrorCode::Code  m_error;				//!< returned Error
	RowNumber          m_affectedRows;	 	//!< number of affected Rows
    std::vector<Int64> m_lastSerial;		//!< serial numbers of return statements
    DBError            m_DBError;           //!< db specific error codes and messages

    //! Default Values
    ExecuteReturnInfo():m_error       (DBErrorCode::SUCCESS),
		                m_affectedRows(0)
	{
	};

    //! \brief  checks error
	//! \retval true  executing statement failed
	//! \retval false executing statement succeeded
    bool hasError() const
    {
        return (DBErrorCode::SUCCESS != m_error);
    };

	//! \brief  get error value
	//! \return error value of executing statement
	DBErrorCode::Code getError() const
	{
		return m_error;
	};

	//! \brief get number of affected rows
	//! \return number of affected rows
	RowNumber getAffectedRows() const
	{
		return m_affectedRows;
	};

    //! \brief get main error code of used db api
    //! \return main error code of used db (infx: main, odbc: query error code, tcAccess: error code)
    basar::Int32 getRawMainError() const
    {
        return m_DBError.m_rawMainError;
    };

    //! \brief get sub error code of used db api
    //! \return sub error code of used db (infx: isam, odbc: db error code, tcAccess: always 0)
    basar::Int32 getRawSubError() const
    {
        return m_DBError.m_rawSubError;
    };

    //! \brief get error message(s) of used db api
    //! \return error message(s) of used db api
    const basar::VarString& getErrorMessage() const
    {
        return m_DBError.m_errorMsg;
    };
};

//------------------------------------------------------------------------------------------------------------//
}	// namespace sql
}	// namespace db
}	// namespace basar

//------------------------------------------------------------------------------------------------------------//
#endif	// GUARD
