//----------------------------------------------------------------------------
/*! \file
 *  \brief  prepared statement odbc implementation class
 *  \author Thomas Hörath
 *  \date   07.11.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_PREPAREDSTATEMENTODBC_H
#define GUARD_PREPAREDSTATEMENTODBC_H

//----------------------------------------------------------------------------
#include "preparedstatement.h"
#include "sqlexcepthandlerodbc.h"

#ifdef _WIN32
#pragma warning (push)
#pragma warning(disable: 4127) // qstring.h - C4127: conditional expression is constant
#endif
#include <QtSql/QSqlQuery.h>
#ifdef _WIN32
#pragma warning (pop)
#endif


//----------------------------------------------------------------------------
class QSqlDatabase;

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//----------------------------------------------------------------------------
//! \brief prepared statement implementation class for odbc\n final class
//!
class PreparedStatementOdbc : public PreparedStatement
{
public:

	// --- method section ----------------------------------------------------

	//! create statement implementation object								\n no-throw
	static PreparedStatementSharedPtr	create			(ConnectionSharedPtr pConn,		//!< shared pointer to connection implementation object
														 const VarString &   rSql ,		//!< SQL statement
														 QSqlDatabase &      rQSqlDb	//!< reference to Qt-Connection, so that QSqlQuery knows to which connection it belongs
														);

	//! close and destruct statement implementation object					\n no-throw
	virtual					   ~PreparedStatementOdbc	();

	//! execute a SQL select statement, return a resultset					\n InvalidParameterException
	virtual ResultsetRef		executeQuery			();
	//! execute a SQL single select statement, return a one row resultset	\n InvalidParameterException
	virtual ResultsetRef		executeSingleQuery		();
	//! execute a given SQL statement (non-select)							\n InvalidParameterException
	virtual ExecuteReturnInfo	execute					(bool  flagExcept	//!< if error: DB exception / return code
														);

	//! \brief execute a SQL single select statement containing count(*) or sum, avg, min, max on numeric fields	\n InvalidParameterException
	//! \return result of aggregate select
	virtual basar::Decimal		executeAggregate		();

	//! release statement resources											\n no-throw
	virtual void				close					();

    //! \brief set a parameter value as an Int64 - not implemented
    //!	\n NotImplementedException
    virtual void                setInt64                (UInt32 pos,                //!< position of the parameter
                                                         const Int64 & rValue       //!< value of the parameter
                                                        );
	//! \brief set a parameter value as an Int32, the value isn't saved in the structure, there are only references to memory
	//!	\n no-throw
	virtual void				setInt32				(UInt32        pos,			//!< position of the parameter
														 const Int32 & rValue		//!< value of the parameter
														);
	//! \brief set a parameter value as an Int16, the value isn't saved in the structure, there are only references to memory
	//!	\n no-throw
	virtual void				setInt16				(UInt32        pos,			//!< position of the parameter
														 const Int16 & rValue		//!< value of the parameter
														);
	//! \brief set a parameter value as a string, the value isn't saved in the structure, there are only references to memory
	//!	\n no-throw
	virtual void				setString				(UInt32      pos,			//!< position of the parameter
														 ConstString value			//!< value of the parameter
														);
    //! \brief set a parameter value as a binary - not implemented
    //!	\n NotImplementedException
    virtual void                setBinary               (UInt32 pos,                //!< position of the parameter
                                                         const Binary& value        //!< value of the parameter
                                                        );
	//! \brief set a parameter value as a Float64, the value isn't saved in the structure, there are only references to memory
	//!	\n no-throw
	virtual void				setFloat64				(UInt32          pos,		//!< position of the parameter
														 const Float64 & rValue		//!< value of the parameter
														);
	//! \brief set a parameter value as a Float32, the value isn't saved in the structure, there are only references to memory
	//!	\n no-throw
	virtual void				setFloat32				(UInt32          pos,		//!< position of the parameter
														 const Float32 & rValue		//!< value of the parameter
														);
	//! \brief set a parameter value as a decimal, the value isn't saved in the structure, there are only references to memory
	//!	\n no-throw
	virtual void				setDecimal				(UInt32	         pos,		//!< position of the parameter
														 const Decimal & rValue		//!< value of the parameter
														);
	//! \brief set a parameter value as a Time, the value isn't saved in the structure, there are only references to memory
	//!	\n no-throw
	virtual void				setTime					(UInt32           pos ,		//!< position of the parameter
														 const Time     & rValue	//!< value of the parameter
														);
	//! \brief set a parameter value as a Date, the value isn't saved in the structure, there are only references to memory
	//!	\n no-throw
	virtual void				setDate					(UInt32           pos ,		//!< position of the parameter
														 const Date     & rValue	//!< value of the parameter
														);
	//! \brief set a parameter value as a DateTime, the value isn't saved in the structure, there are only references to memory
	//!	\n no-throw
	virtual void				setDateTime				(UInt32           pos,		//!< position of the parameter
														 const DateTime & rValue	//!< value of parameter
														);
	//! \brief set a parameter value as a TimeSpan, the value isn't saved in the structure, there are only references to memory, not implemented, not supported by Qt
    //!	\n BasarNotImplementedException */
	virtual void				setTimeSpan				(UInt32           pos,		//!< position of the parameter
														 const TimeSpan & rValue	//!< value of parameter
														);


private:

	// --- method section ----------------------------------------------------

	//! constructs implementation object by connection reference			\n no-throw
								PreparedStatementOdbc	(ConnectionSharedPtr pConn,				//!<  shared pointer to connection implementation object
														 const VarString &   rSql,				//!< SQL statement
														 QSqlDatabase &      rQSqlDb			//!< reference to Qt-Connection, so that QSqlQuery knows to which connection it belongs
														);

								PreparedStatementOdbc	();										//!< forbidden
								PreparedStatementOdbc	(const PreparedStatementOdbc & rCopy);	//!< forbidden
	PreparedStatementOdbc &    	operator =				(const PreparedStatementOdbc & rCopy);	//!< forbidden


	//! execute a SQL prepared statement, return a resultsetref	\n InvalidParameterException
	ResultsetRef				execQuery				(const VarString & func			//!< caller method - for exception!
														);

	QSqlQuery					m_QSqlQuery;				//!< QSqlQuery from QT where all statement calls are sent to
	SqlExceptHandlerOdbc &      m_rSqlExceptHndOdbc;		//!< reference to ODBC exception handler
};

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

#endif	// GUARD
