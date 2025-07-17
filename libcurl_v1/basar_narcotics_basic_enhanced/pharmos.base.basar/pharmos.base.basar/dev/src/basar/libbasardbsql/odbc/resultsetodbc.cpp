//----------------------------------------------------------------------------
/*! \file
 *  \brief  resultset odbc implementation class
 *  \author Michael Eichenlaub
 *  \date   21.08.2005
 */
//----------------------------------------------------------------------------

#include "resultsetodbc.h"

#ifdef _WIN32
#pragma warning (push)
#pragma warning(disable: 4127) // qstring.h - C4127: conditional expression is constant
#pragma warning(disable: 4251) // qshareddata.h C4251: 'QSharedData::ref' : class 'QAtomicInt' needs to have dll-interface to be used by clients of class 'QSharedData'
#endif
#include <QtSql/QSqlQuery>
#include <QtCore/QDateTime>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#ifdef _WIN32
#pragma warning (pop)
#endif



//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//------------------------------------------------------------------------------
//! \return recommended method name
//! \throw  no-throw
ConstString ResultsetOdbc::getMethodName(Int32 type)
{
	switch (type)
	{
	case QVariant::Int:
		return s_METHOD_GETINT32;

	case QVariant::ByteArray:
	case QVariant::String:
		return s_METHOD_GETSTRING;

	case QVariant::Double:
		return s_METHOD_GETFLOAT64;

	case QVariant::Time:
		return s_METHOD_GETTIME;

	case QVariant::Date:
		return s_METHOD_GETDATE;

	case QVariant::DateTime:
		return s_METHOD_GETDATETIME;

	default:
		assert(0);
		return s_METHOD_INVALID;
	}
}

//------------------------------------------------------------------------------
//! \return	shared pointer to resultset base implementation object
//! \throw  no-throw
ResultsetSharedPtr ResultsetOdbc::create(ConnectionSharedPtr pConn  ,
										  QSqlQuery &        rQQuery)
{
	return ResultsetSharedPtr(new ResultsetOdbc(pConn, rQQuery));
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
ResultsetOdbc::ResultsetOdbc ( ConnectionSharedPtr pConn  ,
							   QSqlQuery         & rQQuery)
							 : Resultset    (pConn  ), 
							   m_rQSqlQuery (rQQuery),
							   m_rSqlExceptHndOdbc(*(static_cast<SqlExceptHandlerOdbc*>(m_SqlExceptHnd.get())))
{
	m_RetValTime.setRange(Time::DTUNIT_HOUR, Time::DTUNIT_MSEC);

	describeStmt();		// describe statement, get column names

	m_rQSqlQuery.setForwardOnly(true);	// if false (=default) no success at first next()!

	m_eState      = STATE_OPENED;
	m_ePos        = POS_BEFORE_FIRST;
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
ResultsetOdbc::~ResultsetOdbc()
{
	close();
}

//------------------------------------------------------------------------------
//! \throw no-throw
void ResultsetOdbc::close()
{
	m_eState      = STATE_INVALID;
	m_ePos        = POS_INVALID;
	m_FetchedRows = 0;
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  NotImplementedException
const Int64 & ResultsetOdbc::getInt64(UInt32) const
{
    throw NotImplementedException(ExceptInfo("basar.db.sql.ResultsetOdbc::getInt64", "Not implemented (yet?)", __FILE__, __LINE__));
}

//------------------------------------------------------------------------------
//! \throw InvalidParameterException
//!
void ResultsetOdbc::describeStmt()
{
	ConstString func = "basar.db.sql.ResultsetOdbc.describeStmt";

	// get number of columns
	Int32 cols = m_rQSqlQuery.record().count();

	// get column names
	for (Int32 i = 0; i < cols; ++i)		
	{
		storeColNames(m_rQSqlQuery.record().fieldName(i).toLatin1().constData(), 
		              i, 
					  func, 
					  __FILE__, 
					  __LINE__);

        storeColTypes( getBasarTypeForODBCType( m_rQSqlQuery.record().field(i).type() ) );

	}
}

//----------------------------------------------------------------------------
//! \note  time-critical; avoid unnecessary statement, instantiations, etc.!
//! \retval true  if new current row is valid
//! \retval false if there are no more rows
//! \throw no-throw
bool ResultsetOdbc::next()
{
	switch (m_ePos)
	{
	case POS_AFTER_LAST:	// new fetch not necessary
		m_ePos  = POS_AFTER_LAST;
		return false;

	default:
		break;
	}

	ConstString func = "basar.db.sql.ResultsetOdbc.next";

	checkConnState(func, __FILE__, __LINE__);
	
	if ( false == m_rQSqlQuery.next() )
	{ 
		m_rSqlExceptHndOdbc.setQSqlQuery(&m_rQSqlQuery);
		if ( true == isSqlStateNotFound(func, __FILE__, __LINE__) )
		{
			m_ePos = POS_AFTER_LAST;
			return false;
		}
	}

	++m_FetchedRows;
	m_ePos = POS_VALID;

	return true;
}

//------------------------------------------------------------------------------
//! \throw  InvalidParameterException
//!
void ResultsetOdbc::checkType (UInt32         index,
							   QVariant::Type cmpType,
							   ConstString    func,
							   ConstString    file,
							   UInt32		  line) const
{
	QVariant::Type eType = m_rQSqlQuery.value(index).type();

	if (cmpType != eType)
	{
		if ( cmpType == QVariant::ByteArray && eType == QVariant::String )
			return;	// in ODBC-mode there are two "string-return-types" possible: QVariant::ByteArray + QVariant::String

		VarString val;
		val.format("index %ld, name \"%s\", type \"%s\" : use method %s!", 
					index,
					m_rQSqlQuery.record().fieldName(index).toLatin1().constData(),
					m_rQSqlQuery.value(index).typeName(),
					getMethodName(eType));

		throw InvalidParameterException(ExceptInfo (func, val, file, line));
	}
}

inline basar::SupportedTypeEnum ResultsetOdbc::getBasarTypeForODBCType( const QVariant::Type odbcType ) const
{
    switch ( odbcType )
    {
        case QVariant::Int:
            return basar::INT32;
            break;

        case QVariant::ByteArray:
        case QVariant::String:
            return basar::STRING;
            break;

        case QVariant::Double:
            return basar::FLOAT64;
            break;

        case QVariant::Time:
            return basar::TIME;
            break;

        case QVariant::Date:
            return basar::DATE;
            break;

        case QVariant::DateTime:
            return basar::DATETIME;
            break;

        default:
            return basar::UNKNOWN;
            break;
    };
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
const Int32 & ResultsetOdbc::getInt32(UInt32 index) const
{
	ConstString func = "basar.db.sql.ResultsetOdbc.getInt32";

	checkRange(index, func, __FILE__, __LINE__);
    checkNextCalled();
	checkType (index, QVariant::Int, func, __FILE__, __LINE__);

	m_RetValInt32 = m_rQSqlQuery.isNull(index) ? 0 : m_rQSqlQuery.value(index).toInt(); 
	
	return m_RetValInt32;
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
const Int16 & ResultsetOdbc::getInt16(UInt32 index) const
{
	m_RetValInt16 = static_cast<Int16>(getInt32(index));	// consciously shortened 
	
	return m_RetValInt16;
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
ConstBuffer ResultsetOdbc::getString(UInt32 index) const
{
	ConstString func = "basar.db.sql.ResultsetOdbc.getString";

	checkRange(index, func, __FILE__, __LINE__);
    checkNextCalled();
	checkType (index, QVariant::ByteArray, func, __FILE__, __LINE__);

	m_RetValStr = m_rQSqlQuery.isNull(index) ? "" : m_rQSqlQuery.value(index).toString().toLocal8Bit().constData();
	
	if ( ! isFetchModeRaw() )
		m_RetValStr.trim();
	
	return m_RetValStr.c_str();
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
const Binary & ResultsetOdbc::getBinary(UInt32 /*index*/) const
{
    throw NotImplementedException(ExceptInfo("basar.db.sql.ResultsetOdbc::getBinary", "Not implemented (yet?)", __FILE__, __LINE__));
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
const Float64 &	ResultsetOdbc::getFloat64(UInt32 index) const 
{
	ConstString func = "basar.db.sql.ResultsetOdbc.getFloat64";

	checkRange(index, func, __FILE__, __LINE__);
    checkNextCalled();
	checkType (index, QVariant::Double, func, __FILE__, __LINE__);

	m_RetValFloat64 = m_rQSqlQuery.isNull(index) ? 0.0 : m_rQSqlQuery.value(index).toDouble(); 
	
	return m_RetValFloat64;
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  mo-throw
const Float32 &	ResultsetOdbc::getFloat32 (UInt32 index)  const
{
	m_RetValFloat32 = static_cast<Float32>(getFloat64(index));		// consciously shortened 

	return m_RetValFloat32;
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
const Decimal & ResultsetOdbc::getDecimal (UInt32 index)  const
{
	ConstString func = "basar.db.sql.ResultsetOdbc.getDecimal";

	checkRange(index, func, __FILE__, __LINE__);
    checkNextCalled();
	checkType (index, QVariant::Double, func, __FILE__, __LINE__);

	if ( m_rQSqlQuery.isNull(index) )
		m_RetValDecimal = Decimal(0.0);
	else
		m_RetValDecimal.fromNonLocString(m_rQSqlQuery.value(index).toString().toLatin1().constData());

	return m_RetValDecimal;
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
const Date & ResultsetOdbc::getDate (UInt32 index)  const
{
	ConstString func = "basar.db.sql.ResultsetOdbc.getDate";

	checkRange(index, func, __FILE__, __LINE__);
    checkNextCalled();
	checkType (index, QVariant::Date, func, __FILE__, __LINE__);

	QDate dt = m_rQSqlQuery.isNull(index) ? QDate(9999, 12, 31) : m_rQSqlQuery.value(index).toDate();

	m_RetValDate.setDate(dt.year(), dt.month(), dt.day());

	return m_RetValDate;
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
const Time & ResultsetOdbc::getTime (UInt32 index)  const
{
	ConstString func = "basar.db.sql.ResultsetOdbc.getTime";

	checkRange(index, func, __FILE__, __LINE__);
    checkNextCalled();
	checkType (index, QVariant::Time, func, __FILE__, __LINE__);

	QTime tm = m_rQSqlQuery.isNull(index) ? QTime(23, 59, 59, 999) : m_rQSqlQuery.value(index).toTime();

	m_RetValTime.setTime(tm.hour(), tm.minute(), tm.second(), tm.msec());

	return m_RetValTime;
}

//----------------------------------------------------------------------------
//! \return column value
//! \note   Informix DateTime "year to day" is evaluated by Qt to QDate!
//! \throw  no-throw
const DateTime & ResultsetOdbc::getDateTime (UInt32 index)  const
{
	ConstString func = "basar.db.sql.ResultsetOdbc.getDateTime";

	checkRange(index, func, __FILE__, __LINE__);
    checkNextCalled();
	checkType (index, QVariant::DateTime, func, __FILE__, __LINE__);

	QDateTime dt = m_rQSqlQuery.isNull(index) ? QDateTime(QDate(9999, 12, 31), QTime(23, 59, 59, 999))
		                                      : m_rQSqlQuery.value(index).toDateTime();

	if (dt.date() == QDate(1200, 1, 1))		// magic invalid date for Qt ODBC
		m_RetValDateTime.setInvalidDate();
	else
        m_RetValDateTime.setDate(dt.date().year(), dt.date().month(), dt.date().day());

	m_RetValDateTime.setTime(dt.time().hour(), dt.time().minute(), dt.time().second(), dt.time().msec());

	return m_RetValDateTime;
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  NotImplementedException
const TimeSpan & ResultsetOdbc::getTimeSpan (UInt32)  const
{
    throw NotImplementedException (ExceptInfo("basar.db.sql.ResultsetOdbc::getDateTime","Not implemented, DataType not supported by Qt",__FILE__, __LINE__));
}

//----------------------------------------------------------------------------
//! \return column type
//! \throw  no-throw
const QVariant::Type ResultsetOdbc::getTypeByName(const VarString & rColName	) const
{
	return getType(getIndex(rColName));
}

//----------------------------------------------------------------------------
//! \return column type
//! \throw  no-throw
const QVariant::Type ResultsetOdbc::getType		(const UInt32 index	) const
{
	return m_rQSqlQuery.value(index).type();
}

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar
