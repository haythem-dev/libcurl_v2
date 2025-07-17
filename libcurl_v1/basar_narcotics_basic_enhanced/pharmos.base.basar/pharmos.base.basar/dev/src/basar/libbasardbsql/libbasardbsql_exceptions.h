//------------------------------------------------------------------------------
/*! \file
 *  \brief  public library interface: exceptions
 *  \author Michael Eichenlaub
 *  \date   14.08.2005
 */
//------------------------------------------------------------------------------

#ifndef GUARD_LIBBASARDBSQL_EXCEPTIONS_H
#define GUARD_LIBBASARDBSQL_EXCEPTIONS_H

//------------------------------------------------------------------------------
#include "libbasarcmnutil_exceptions.h"

//------------------------------------------------------------------------------
class QSqlDatabase;
class QSqlQuery;

//------------------------------------------------------------------------------
namespace basar {
namespace db	{
namespace sql	{

//------------------------------------------------------------------------------
//! thrown if physical database errors occur \n base class
class DbException : public basar::Exception
{
public:
	//! constructs exception object and initialises text description string
	DbException	(const ExceptInfo & sInfo	                      ,		//!< exception info struct
					const VarString  & exceptionType = "DbException" ,  //!< type of exception which is thrown
					const VarString  & addReason     = ""				//!< additional reason, extends m_InfoStruct.reason
				);
	//! destructor
	virtual	~DbException	();

protected:
	DbException	();					//!< forbidden
};

//------------------------------------------------------------------------------
//! thrown if Informix database errors occur
class DbExceptionInfx : public DbException
{
public:
	//! constructs exception object and initialises text description string
	DbExceptionInfx	(const ExceptInfo & sInfo	//!< exception info struct
					);
	//! destructor  \n     no-throw
	virtual ~DbExceptionInfx	();

private:
	DbExceptionInfx	();	//!< forbidden

	//! get additional reason and extend base member reason
	VarString getAddReason();
};

//------------------------------------------------------------------------------
//! thrown if ODBC errors occur
class DbExceptionOdbc : public DbException
{
public:
	//! constructs exception object and initialises text description string
	DbExceptionOdbc	(const ExceptInfo & sInfo,	 //!< exception info struct
					 QSqlDatabase     & rQSqlDb, //!< Qt connection
					 QSqlQuery        * pQQuery	 //!< Qt statement/query
					);
	//! destructor
	virtual ~DbExceptionOdbc	();	

private:
	DbExceptionOdbc	(); //!< forbidden

	//! get additional reason and extend base member reason
	VarString getAddReason	(QSqlDatabase & rQSqlDb, //!< Qt database
							 QSqlQuery    * pQQuery  //!< Qt statement/query
                            );
};

//------------------------------------------------------------------------------
//! thrown by CICS errors
class DbExceptionCics : public DbException
{
public:
	//! c'tor
	DbExceptionCics(BULong            errorcode, //!< Error Code from tcACCESS API
					const VarString & message,	 //!< Error Message from tcACCESS API
					const VarString & file,		 //!< source code file for thrown exception
					const UInt32      line		 //!< source code file line for thrown exception
				   );

private:
	//! returns the Except Info, composed by parameters
	static ExceptInfo getExceptInfo(BULong            errorcode, //!< Error Code from tcACCESS API
									const VarString & message,	 //!< Error Message from tcACCESS API
									const VarString & file   ,	 //!< source code file for thrown exception
									const UInt32      line		 //!< source code file line for thrown exception
								   );
};

//------------------------------------------------------------------------------
//! thrown by CICS connection if port listener is not available ("connection refused")
class DbExceptionCicsUnavail : public DbExceptionCics
{
public:
	//! c'tor
	DbExceptionCicsUnavail(BULong            errorcode, //!< Error Code from tcACCESS API
					       const VarString & message,   //!< Error Message from tcACCESS API
					       const VarString & file,      //!< source code file for thrown exception
					       const UInt32      line       //!< source code file line for thrown exception
				          );
};

//------------------------------------------------------------------------------
//! thrown if allowed statement length is exceeded
class DbExceptionStatementLengthExceeded : public DbException
{
public:
	//! constructs exception object and initialises text description string
	DbExceptionStatementLengthExceeded	(const ExceptInfo & sInfo	                      ,		//!< exception info struct
					const VarString  & exceptionType = "DbExceptionStatementLengthExceeded" ,  //!< type of exception which is thrown
					const VarString  & addReason     = ""				//!< additional reason, extends m_InfoStruct.reason
				);

private:
	DbExceptionStatementLengthExceeded	();	//!< forbidden

	//! get additional reason and extend base member reason
	VarString getAddReason();
};

//------------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

//------------------------------------------------------------------------------
#endif	// GUARD
