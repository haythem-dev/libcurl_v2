//----------------------------------------------------------------------------
/*! \file
 *  \brief  SQL diagnostics area's info implementation class
 *  \author Michael Eichenlaub
 *  \date   11.08.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_RESULTSETODBC_H
#define GUARD_RESULTSETODBC_H

//----------------------------------------------------------------------------
#include "libbasarcmnutil_decimal.h"
#include "libbasarcmnutil_datetime.h"
#include "libbasarcmnutil_date.h"
#include "sqlexcepthandlerodbc.h"
#include "resultset.h"

#ifdef _WIN32
#pragma warning (push)
#pragma warning(disable: 4127) // qhash.h - C4127: conditional expression is constant
#pragma warning(disable: 4512) // qobject_impl.h - C4512: 'QtPrivate::QSlotObjectBase' : assignment operator could not be generated
#pragma warning(disable: 4800) // qmetatype.h - C4800: forcing value to bool 'true' or 'false' (performance warning)
#endif
#include <QtCore/qvariant.h>
#ifdef _WIN32
#pragma warning (pop)
#endif
//----------------------------------------------------------------------------
class QSqlQuery;

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//----------------------------------------------------------------------------
//! \brief resultset odbc implementation class 
//! \n     final class
//! 
class ResultsetOdbc : public Resultset
{
public:

	// --- method section ----------------------------------------------------

	//! create resultset implementation object								\n no-throw
	static ResultsetSharedPtr	create				(ConnectionSharedPtr pConn,		//!< shared pointer to connection implementation object
													 QSqlQuery       &   rQQuery    //!< reference to Qt-SqlQuery, so that ResultSet knows to which Statement it belongs
													);
	
	//! close and destruct resultset implementation object					\n no-throw
	virtual					   ~ResultsetOdbc		();

	//! move to the next row in the result set								\n no-throw
	virtual bool				next				();
	//! \brief release resultset resources											\n no-throw
	virtual void				close				();

    //! get a column's value as an Int64									\n no-throw
    virtual const Int64 &       getInt64            (UInt32            index        //!< column index (0, ..., n-1)
                                                    ) const;
	//! get a column's value as an Int32									\n no-throw
	virtual const Int32 &		getInt32			(UInt32            index		//!< column index (0, ..., n-1)
													) const;
	//! get a column's value as an Int16									\n no-throw
	virtual const Int16 &		getInt16			(UInt32            index		//!< column index (0, ..., n-1)
													) const;
	//! get a column's value as a string									\n no-throw
	virtual ConstBuffer 		getString			(UInt32            index		//!< column index (0, ..., n-1)
													) const;
    //! get a column's value as a binary									\n no-throw
    virtual const Binary &      getBinary           (UInt32            index        //!< column index (0, ..., n-1)
                                                    ) const;
	//! get a column's value as a Float64									\n no-throw
	virtual const Float64 &		getFloat64			(UInt32            index		//!< column index (0, ..., n-1)
													) const;
	//! get a column's value as a Float32									\n no-throw
	virtual const Float32 &		getFloat32			(UInt32            index		//!< column index (0, ..., n-1)
													) const;
	//! get a column's value as a decimal									\n no-throw
	virtual const Decimal &		getDecimal			(UInt32            index		//!< column index (0, ..., n-1)
													) const;
	//! get a column's value as a Date										\n no-throw
	virtual const Date &		getDate				(UInt32            index		//!< column index (0, ..., n-1)
													) const;
	//! get a column's value as a Time										\n no-throw
	virtual const Time &		getTime				(UInt32 index		//!< column index (0, ..., n-1)
													) const;
	//! get a column's value as a DateTime									\n no-throw
	virtual const DateTime &	getDateTime			(UInt32            index		//!< column index (0, ..., n-1)
													) const;
	//! get a column's value as a TimeSpan									\n no-throw
	virtual const TimeSpan &	getTimeSpan			(UInt32            index		//!< column index (0, ..., n-1)
													) const;

	//! get a column's return type											\n no-throw
	const QVariant::Type		getType				(UInt32            index		//!< column index (0, ..., n-1)
													) const;
	//! get a column's return type											\n no-throw
	const QVariant::Type		getTypeByName		(const VarString & rColName		//!< column name
													) const;

private:

	// --- method section ----------------------------------------------------

	//! describe statement and create map with column names		\n throw InvalidParameterException
	void						describeStmt		();
	
	//! for exceptions: get recommended method name for give SQL type
	static ConstString 			getMethodName		(Int32 type					//!< Qt column type
													);

	//! construct implementation object by connection reference, declare and open cursor \n no-throw
								ResultsetOdbc		(ConnectionSharedPtr pConn,		//!< shared pointer to connection implementation object
													 QSqlQuery& rQQuery				//!< reference to Qt-SqlQuery, so that ResultSet knows to which Statement it belongs
													);

								ResultsetOdbc		();								//!< forbidden
								ResultsetOdbc		(const ResultsetOdbc & rCopy);	//!< forbidden
	ResultsetOdbc &    			operator =			(const ResultsetOdbc & rCopy);	//!< forbidden


	//! check if column type is compatible with called method	\n throw InvalidParameterException
	void						checkType			(UInt32          index,		//!< column index (0, ..., n-1)
													 QVariant::Type  cmpType,	//!< Qt SQL type, compared with indexed column type
													 ConstString     func,		//!< calling method
													 ConstString     file,		//!< calling file
													 UInt32		     line		//!< calling file line
													) const;

    inline basar::SupportedTypeEnum    getBasarTypeForODBCType( const QVariant::Type odbcType ) const;

	QSqlQuery &					m_rQSqlQuery;		//!< QSqlQuery from Qt where all statement calls are sent to
	SqlExceptHandlerOdbc &      m_rSqlExceptHndOdbc;//!< reference to ODBC exception handler

	mutable Int32				m_RetValInt32;		//!< to return reference to Int32 values
	mutable Int16				m_RetValInt16;		//!< to return reference to Int16 values
	mutable Float64				m_RetValFloat64;	//!< to return reference to Float64 values
	mutable Float32				m_RetValFloat32;	//!< to return reference to Float32 values
	mutable Decimal				m_RetValDecimal;	//!< to return reference to Decimal values
	mutable VarString			m_RetValStr;		//!< to return reference to String values
	mutable DateTime			m_RetValDateTime;	//!< to return reference to DateTime values
	mutable Date				m_RetValDate;		//!< to return reference to Date values
	mutable Time				m_RetValTime;		//!< to return reference to Time values
};

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

#endif	// GUARD
