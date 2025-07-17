//----------------------------------------------------------------------------
/*! \file
 *  \brief  base class for data manipulation classes
 *  \author Michael Eichenlaub
 *  \date   12.08.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_SQLOPERATIVE_H
#define GUARD_SQLOPERATIVE_H

//----------------------------------------------------------------------------
#include "libbasar_definitions.h"
#include "libbasardbsql_definitions.h"

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//----------------------------------------------------------------------------
class SqlExceptHandler;	
class SqlDiagnosticsRef;

//----------------------------------------------------------------------------
//! data manipulation class \n base class
class SqlOperative
{
public:
	//! virtual destructor because of virtual method(s)			\n no-throw
	virtual			   ~SqlOperative			();

	//! clears warning object (i.e. reset SqlDiagnosticsRef)	\n no-throw 
	void				clearWarnings			();
	//! get SQL diagnostics reference object					\n no-throw
	SqlDiagnosticsRef	getWarnings				() const;


protected:

    /*! 
     * checks if the statement starts with an special tokenvalue, 
     * f.ex. "insert", the character after 
     * the tokenvalue has to be a space value, f.ex blank 
     */
	bool				checkStmtStart			(const VarString & rStmt,            //!< statement, to be searched
												 const VarString & searchvaluegiven  //!< token to search
												);

	//! check connection state                        			\n no-throw
	virtual void		checkConnState			(const VarString & rWhat,	//!< text description 
												 const VarString & rFile,	//!< file where exception is thrown
												 UInt32			   line		//!< line of file where exception is thrown 
												) const = 0;

	//! checks and handles Informix SQL STATE					\n no-throw
	bool				checkSqlState			(const VarString & rWhat,				//!< text description 
												 const VarString & rFile,				//!< file where exception is thrown
												 UInt32			   line,				//!< line of file where exception is thrown 
												 bool              flagExcept = true	//!< if error: DB exception / return code
												);	
    //! returns the last DB Error
    DBErrorCode::Code   getLastError();

    DBError             getRawDBError() const;

	//! check Informix SQL state for "NO DATA FOUND"			\n no-throw
	bool				isSqlStateNotFound		(const VarString & rWhat,	//!< text description 
												 const VarString & rFile,	//!< file where exception is thrown
												 UInt32			   line		//!< line of file where exception is thrown 
												);

    boost::shared_ptr < SqlExceptHandler >	m_SqlExceptHnd;		//!< handler for SQL exceptions
};

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

#endif	// GUARD
