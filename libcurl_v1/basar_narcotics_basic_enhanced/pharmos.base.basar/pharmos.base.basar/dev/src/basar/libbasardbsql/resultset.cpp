//----------------------------------------------------------------------------
/*! \file
 *  \brief  resultset implementation class
 *  \author Michael Eichenlaub
 *  \date   21.07.2005
 */
//----------------------------------------------------------------------------

#include "resultset.h"
#include "connection.h"
#include "libbasarcmnutil_exceptions.h"

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//------------------------------------------------------------------------------
ConstString Resultset::s_METHOD_INVALID		= "invalid SQL type";
ConstString Resultset::s_METHOD_GETINT32	= "basar.db.sql.ResultsetRef.getInt32"  ;
ConstString Resultset::s_METHOD_GETINT16	= "basar.db.sql.ResultsetRef.getInt16"  ;
ConstString Resultset::s_METHOD_GETSTRING	= "basar.db.sql.ResultsetRef.getString" ;
ConstString Resultset::s_METHOD_GETFLOAT64	= "basar.db.sql.ResultsetRef.getFloat64";
ConstString Resultset::s_METHOD_GETFLOAT32	= "basar.db.sql.ResultsetRef.getFloat32";
ConstString Resultset::s_METHOD_GETDECIMAL	= "basar.db.sql.ResultsetRef.getDecimal";
ConstString Resultset::s_METHOD_GETTIME		= "basar.db.sql.ResultsetRef.getTime";
ConstString Resultset::s_METHOD_GETDATE		= "basar.db.sql.ResultsetRef.getDate";
ConstString Resultset::s_METHOD_GETDATETIME	= "basar.db.sql.ResultsetRef.getDateTime";
ConstString Resultset::s_METHOD_GETTIMESPAN	= "basar.db.sql.ResultsetRef.getTimeSpan";
ConstString Resultset::s_METHOD_GETBYTE     = "basar.db.sql.ResultsetRef.getBinary";
ConstString Resultset::s_METHOD_GETINT64    = "basar.db.sql.ResultsetRef.getInt64";
ConstString Resultset::s_METHOD_UNKNOWN		= "unknown SQL type";

//------------------------------------------------------------------------------
//! \throw no-throw
//!
Resultset::Resultset ( ConnectionSharedPtr pConn)
					 : m_eState		(STATE_INVALID),
                       m_ePos       (POS_INVALID  ),
					   m_FetchedRows(0            ),
					   m_pConn		(pConn        )
{
	m_SqlExceptHnd = m_pConn->m_SqlExceptHnd; //copy shared pointer to ExceptHandler to know if inf / odbc !
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
Resultset::~Resultset()
{
}

//------------------------------------------------------------------------------
//!	\retval true  if resultset is opened
//! \retval false if resultset is closed/not opened
//! \throw no-throw
bool Resultset::isOpen() const
{
	return (STATE_OPENED == m_eState);
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void Resultset::checkConnState(const VarString & rWhat,
							   const VarString & rFile,
							   UInt32			 line ) const
{
	m_pConn->checkConnState(rWhat, rFile, line);
}

//------------------------------------------------------------------------------
//! \throw no-throw
//! \retval true if fetch mode is raw fetching
//! \retval false if fetch mode is not raw fetching
bool Resultset::isFetchModeRaw() const
{
	return m_pConn->isFetchModeRaw();
}

//----------------------------------------------------------------------------
//! \return retrieve number of fetched rows
//! \throw  no-throw
UInt32 Resultset::getFetchedRows() const
{
	return m_FetchedRows;
}

//----------------------------------------------------------------------------
//! \return column names of executed query, same order as in statement
//! \throw  no-throw
const CollColNames& Resultset::getColumnNames() const
{
    return m_CollColNames;
}

const CollColTypes& Resultset::getColumnTypes() const
{
    return m_CollColTypes;
}

//------------------------------------------------------------------------------
//! \throw  InvalidStateException
//! \return number of rows affected by delete/update statement
Int32 Resultset::execCurrentRow(const VarString &)
{
	bool f = true;

    //! if this virtual method is called then there doesn't exist any RDBMS specific implementation
	if (f)
	{
		throw InvalidStateException(ExceptInfo("basar.db.sql.Resultset.execCurrentRow",
												"method not implemented in RDBMS specific context",
												__FILE__, __LINE__));
	}

	return 0;
}

//------------------------------------------------------------------------------
//! \throw  InvalidStateException
//! 
void Resultset::reopen()
{
	//! if this virtual method is called then there doesn't exist any RDBMS specific implementation
	throw InvalidStateException(ExceptInfo("basar.db.sql.Resultset.reopen",
										   "method not implemented in RDBMS specific context",
										   __FILE__, __LINE__));
}

//----------------------------------------------------------------------------
//! \retval true if cursor is  before first row
//! \retval false if cursor is at any other position or the result set contains no rows
//! \throw no-throw
bool Resultset::isBeforeFirst() const
{
	return (POS_BEFORE_FIRST == m_ePos);
}

//----------------------------------------------------------------------------
//! \retval true if cursor is after last row
//! \retval false if cursor is at any other position or resultset contains no rows
//! \throw no-throw
bool Resultset::isAfterLast() const
{
	return (POS_AFTER_LAST == m_ePos);
}

//----------------------------------------------------------------------------
//! \return number of selected columns
//! \throw  no-throw
UInt32 Resultset::getSelectedCols() const
{
	return static_cast<UInt32>(m_CollColItems.size());
}

//----------------------------------------------------------------------------
//! \throw no-throw
//! \retval true if column name exists in resultset
//! \retval false if column name doesn't exist in resultset
bool Resultset::isExist(const VarString & rColName) const
{
	return (m_CollColItems.end() != m_CollColItems.find(rColName));
}

//----------------------------------------------------------------------------
//! \throw  OutOfRangeIndexException
//!
void Resultset::checkRange (UInt32      index,
							ConstString func,
							ConstString file,
							UInt32		line) const
{
	if (getSelectedCols() <= index)
	{
		VarString val;
		val.format("index %ld >= getSelectedCols() %ld", 
			       index,
				   getSelectedCols());

		throw OutOfRangeIndexException(ExceptInfo (func, val, file, line));
	}
}

//----------------------------------------------------------------------------
//! \throw  InvalidParameterException
//!
void Resultset::storeColNames(	ConstString name ,
								UInt32      index,
								ConstString func ,
								ConstString file ,
								UInt32		line )
{
	VarString colName = name;
	colName.lower();

	if ( ! m_CollColItems.insert(CollColItems::value_type(colName, index)).second )
	{
		VarString msg;
		msg.format("column (%d, %s) is not unique in SQL statement", 
			       index, 
				   name);

		throw InvalidParameterException(ExceptInfo(func, msg, file, line));
	}

    m_CollColNames.push_back(colName);
}

void Resultset::storeColTypes ( basar::SupportedTypeEnum colType )
{
    m_CollColTypes.push_back( colType );
}

void Resultset::checkNextCalled() const
{
    if (POS_INVALID == m_ePos || POS_BEFORE_FIRST == m_ePos)
    {
        throw InvalidStateException(ExceptInfo("basar.db.sql.Resultset.checkNextCalled",
            "please call Resultset.next() before accessing any of the values",
            __FILE__, __LINE__));
    }
}

//----------------------------------------------------------------------------
//! \throw  OutOfRangeIndexException
//! \return index (0, ..., n-1) of column name in resultset
UInt32 Resultset::getIndex(const VarString & rColName) const
{
	VarString colName = rColName;
	colName.lower();

    CollColItems::const_iterator itItem = m_CollColItems.find(colName);

	if (m_CollColItems.end() == itItem)
	{
		VarString msg;
		msg.format("table column <%s> not found in selected columns", rColName.c_str());
		throw OutOfRangeIndexException(ExceptInfo (	"basar.db.sql.Resultset.getIndex", msg, __FILE__, __LINE__));
	}

    return itItem->second;
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
const Int64 & Resultset::getInt64ByName(const VarString & rColName) const
{
    return getInt64(getIndex(rColName));
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
const Int32 & Resultset::getInt32ByName(const VarString & rColName) const
{
	return getInt32(getIndex(rColName));
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
const Int16 & Resultset::getInt16ByName(const VarString & rColName) const
{
	return getInt16(getIndex(rColName));
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
ConstBuffer Resultset::getStringByName(const VarString & rColName) const
{
	return getString (getIndex(rColName));
}
//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
const Binary & Resultset::getBinaryByName(const VarString & rColName) const
{
    return getBinary(getIndex(rColName));
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
const Float64 &	Resultset::getFloat64ByName(const VarString & rColName) const
{
	return getFloat64(getIndex(rColName));
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
const Float32 &	Resultset::getFloat32ByName (const VarString & rColName) const
{
	return getFloat32 (getIndex(rColName));
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
const Decimal & Resultset::getDecimalByName (const VarString & rColName) const
{
	return getDecimal (getIndex(rColName));
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
const Date & Resultset::getDateByName(const VarString & rColName) const
{
	return getDate(getIndex(rColName));
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
const Time & Resultset::getTimeByName(const VarString & rColName) const
{
	return getTime(getIndex(rColName));
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
const DateTime & Resultset::getDateTimeByName(const VarString & rColName) const
{
	return getDateTime(getIndex(rColName));
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
const TimeSpan & Resultset::getTimeSpanByName(const VarString & rColName) const
{
	return getTimeSpan(getIndex(rColName));
}

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar
