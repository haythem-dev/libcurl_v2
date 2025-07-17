//----------------------------------------------------------------------------
/*! \file
 *  \brief  handler for SQL exceptions (infx)
 *  \author Michael Eichenlaub
 *  \date   11.08.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_SQLEXCEPTHANDLERINFX_H
#define GUARD_SQLEXCEPTHANDLERINFX_H

//----------------------------------------------------------------------------
#include "sqlexcepthandler.h"

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//----------------------------------------------------------------------------
//! handler for SQL exceptions \n final class
class SqlExceptHandlerInfx : public SqlExceptHandler
{
public:

	//! create sql except handler implementation object	\n no-throw
	static boost::shared_ptr < SqlExceptHandler >	create				();		

	//! constructs implementation object		\n no-throw
													SqlExceptHandlerInfx();	
	
	//! destructs implementation object			\n no-throw
	virtual										   ~SqlExceptHandlerInfx();	

	//! check and handle Informix SQL STATE	\n throw  DbExceptionInfx
	virtual bool									checkSqlState		(const VarString & rWhat,		//!< text description 
																		 const VarString & rFile,		//!< file where exception is thrown
																		 UInt32			   line,		//!< line of file where exception is thrown 
																		 bool              flagExcept	//!< if error: DB exception / return code		
                                                                         );
    //! returns the last DBErrorCode::Code 
	virtual DBErrorCode::Code						getLastError		();

    virtual DBError                    getRawDBError();

    //! check Informix SQL state for "NO DATA FOUND"	\n no-throw
	virtual bool									isSqlStateNotFound	(const VarString & rWhat,	//!< text description 
																		 const VarString & rFile,	//!< file where exception is thrown
																		 UInt32			   line		//!< line of file where exception is thrown 
																		) ;

protected:
													SqlExceptHandlerInfx(const SqlExceptHandlerInfx & rCopy);	//!< forbidden
	SqlExceptHandlerInfx &							operator =			(const SqlExceptHandlerInfx & rCopy);	//!< forbidden

	//! clear existing warning object and create new one		\n no-throw
	virtual void									createWarnings		();
};

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

#endif	// GUARD

