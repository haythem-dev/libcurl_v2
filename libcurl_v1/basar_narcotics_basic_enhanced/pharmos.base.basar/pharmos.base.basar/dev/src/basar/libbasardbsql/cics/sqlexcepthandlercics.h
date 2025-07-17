/** $Id$
 *
 * @file sqlexcepthandlercics.h

 * @brief ...
 * @author Anke Klink

 * @date 2007
 */
//----------------------------------------------------------------------------

#ifndef  __GUARD_BASAR_LIBBASARDBSQL_CICS_SQLEXCEPTHANDLERCICS_H__ 
#define  __GUARD_BASAR_LIBBASARDBSQL_CICS_SQLEXCEPTHANDLERCICS_H__ 

//----------------------------------------------------------------------------
#include "sqlexcepthandler.h"

//----------------------------------------------------------------------------
namespace basar {
namespace db	{
namespace sql   {

//! handles Exceptions
class SqlExceptHandlerCics : public SqlExceptHandler
{
public:
	//! create sql except handler implementation object		\n no-throw
	static boost::shared_ptr < SqlExceptHandler >	create					();

	//! constructs implementation object			\n no-throw
													SqlExceptHandlerCics	();

	//! destructs implementation object			\n no-throw
	virtual										   ~SqlExceptHandlerCics	();

	//! check and handle SQL STATE				\n no-throw 
	virtual bool									checkSqlState			(const VarString & rWhat,		//!< text description
																			 const VarString & rFile,		//!< file where exception is thrown
																			 UInt32			   line,		//!< line of file where exception is thrown
																			 bool              flagExcept	//!< if error: DB exception / return code
																			);

	//! returns the last DBErrorCode::Code	\n no-throw
	virtual   DBErrorCode::Code						getLastError();

    virtual DBError                                 getRawDBError();

	//! check SQL state for "NO DATA FOUND"		\n no-throw
	virtual bool									isSqlStateNotFound		(const VarString & rWhat,	//!< text description
																			 const VarString & rFile,	//!< file where exception is thrown
																			 UInt32			   line		//!< line of file where exception is thrown
																			) ;


	protected:

        											SqlExceptHandlerCics	(const SqlExceptHandlerCics & rCopy);	//!< forbidden
	SqlExceptHandlerCics &							operator =				(const SqlExceptHandlerCics & rCopy);	//!< forbidden

	//! clear existing warning object and create new one		\n no-throw
	virtual void									createWarnings			();

};

// namespaces
}
}
}

#endif 

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
