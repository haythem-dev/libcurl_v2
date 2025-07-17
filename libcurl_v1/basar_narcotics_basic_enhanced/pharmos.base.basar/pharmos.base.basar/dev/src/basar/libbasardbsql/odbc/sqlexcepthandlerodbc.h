//----------------------------------------------------------------------------
/*! \file
 *  \brief  handler for SQL exceptions (odbc)
 *  \author Michael Eichenlaub
 *  \date   11.08.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_SQLEXCEPTHANDLERODBC_H
#define GUARD_SQLEXCEPTHANDLERODBC_H

//----------------------------------------------------------------------------
#include <sqlexcepthandler.h>

//----------------------------------------------------------------------------
class QSqlDatabase;
class QSqlQuery;

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//----------------------------------------------------------------------------
//! handler for SQL exceptions \n final class
class SqlExceptHandlerOdbc : public SqlExceptHandler
{
public:

	//! create sql except handler implementation object		\n no-throw
	static boost::shared_ptr < SqlExceptHandler >	create					(QSqlDatabase & rQSqlDb		//!< reference to Qt-Connection, so that exception knows to which connection it belongs;
																			);

	//! constructs implementation object			\n no-throw
													SqlExceptHandlerOdbc	(QSqlDatabase & rQSqlDb		//!< reference to Qt-Connection, so that exception knows to which connection it belongs;
												);	
	
	//! destructs implementation object			\n no-throw
	virtual										   ~SqlExceptHandlerOdbc	();	

	//! check and handle SQL STATE				\n throw  DbExceptionOdbc
	virtual bool									checkSqlState			(const VarString & rWhat,		//!< text description 
																			 const VarString & rFile,		//!< file where exception is thrown
																			 UInt32			   line,		//!< line of file where exception is thrown 
																			 bool              flagExcept	//!< if error: DB exception / return code
																			);	
	//! retrieve ODBC's last occurred error		\n no-throw
    virtual DBErrorCode::Code						getLastError();
	

    virtual DBError                                 getRawDBError();

	//! check SQL state for "NO DATA FOUND"		\n throw DbExceptionOdbc
	virtual bool									isSqlStateNotFound		(const VarString & rWhat,	//!< text description 
																			 const VarString & rFile,	//!< file where exception is thrown
																			 UInt32			   line		//!< line of file where exception is thrown 
																			) ;
	
	//! set member m_pQQuery	\n no-throw
	void											setQSqlQuery			(QSqlQuery* pQQuery	//!< pointer to set to m_pQQuery
																			);

	//! reset member m_pQQuery	\n no-throw
	void											resetQSqlQuery			();


protected:

													SqlExceptHandlerOdbc	(const SqlExceptHandlerOdbc & rCopy);	//!< forbidden
	SqlExceptHandlerOdbc &							operator =				(const SqlExceptHandlerOdbc & rCopy);	//!< forbidden

	//! clear existing warning object and create new one		\n no-throw
	virtual void									createWarnings			();


	QSqlDatabase &				m_rQSqlDb;		//!< QSqlDatabase from QT: connection where errors occur
	QSqlQuery	 *				m_pQQuery;		//!< QSqlQuery from QT: statement where errors occur

};

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

#endif	// GUARD
