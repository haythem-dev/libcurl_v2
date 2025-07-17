//----------------------------------------------------------------------------
/*! \file
 *  \brief  resultset reference class
 *  \author Michael Eichenlaub
 *  \date   28.07.2005
 */
//----------------------------------------------------------------------------

#include "libbasardbsql_resultsetref.h"

#include "libbasarcmnutil_exceptions.h"
#include "resultset.h"

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//----------------------------------------------------------------------------
//! \throw  no-throw 
//!
ResultsetRef::ResultsetRef()
{
}

//----------------------------------------------------------------------------
//! \throw  no-throw 
//! 
ResultsetRef::ResultsetRef ( ResultsetSharedPtr pImpl)
                           : m_pImpl(pImpl)
{
}

//----------------------------------------------------------------------------
//! \throw  no-throw 
//!
ResultsetRef::~ResultsetRef()
{
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//!
void ResultsetRef::reset()
{
	m_pImpl.reset();
}

//----------------------------------------------------------------------------
//!	\retval true  if shared pointer to implementation object is null
//! \retval false if shared pointer to implementation object is valid
//! \throw  no-throw
bool ResultsetRef::isNull() const
{
	return (0 == m_pImpl.get());
}

//----------------------------------------------------------------------------
//!	\return shared pointer to implementation object
//! \throw  NullReferenceException 
inline const ResultsetSharedPtr & ResultsetRef::getImpl() const
{
	if (0 == m_pImpl.get())
		throw NullReferenceException(ExceptInfo("basar.db.sql.ResultsetRef.getImpl", 
												"m_pImpl.get() == 0", __FILE__, __LINE__));

	return m_pImpl;
}

//------------------------------------------------------------------------------
//!	\retval true  if resultset is opened
//! \retval false if resultset is closed/not opened
//! \throw no-throw
bool ResultsetRef::isOpen() const
{
	return getImpl()->isOpen();
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void ResultsetRef::reopen()
{
	getImpl()->reopen();
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void ResultsetRef::close()
{
	getImpl()->close();
}

//------------------------------------------------------------------------------
//! \return number of rows affected by statement (update, delete)
//! \throw  no-throw
Int32 ResultsetRef::execCurrentRow(const VarString & rSql)
{
	return getImpl()->execCurrentRow(rSql);
}

//----------------------------------------------------------------------------
//! \return retrieve number of fetched rows
//! \throw no-throw
UInt32 ResultsetRef::getFetchedRows() const
{
	return getImpl()->getFetchedRows();
}

//----------------------------------------------------------------------------
//! \return column names of executed query, same order as in statement
//! \throw  no-throw
const CollColNames& ResultsetRef::getColumnNames() const
{
    return getImpl()->getColumnNames();
}

//----------------------------------------------------------------------------
//! \return column types of executed query, same order as in statement
//! \throw  no-throw
const CollColTypes& ResultsetRef::getColumnTypes() const
{
    return getImpl()->getColumnTypes();
}

//----------------------------------------------------------------------------
//! \retval true  if new current row is valid
//! \retval false if there are no more rows
//! \throw no-throw
bool ResultsetRef::next()
{
	return getImpl()->next();
}

//----------------------------------------------------------------------------
//! \retval true if cursor is before first row
//! \retval false if cursor is at any other position or the result set contains no rows
//! \throw no-throw
bool ResultsetRef::isBeforeFirst() const
{
	return getImpl()->isBeforeFirst();
}

//----------------------------------------------------------------------------
//! \retval true if cursor is after last row
//! \retval false if cursor is at any other position or resultset contains no rows
//! \throw no-throw
bool ResultsetRef::isAfterLast() const
{
	return getImpl()->isAfterLast();
}

//----------------------------------------------------------------------------
//! \return number of selected columns
//! \throw  no-throw
UInt32 ResultsetRef::getSelectedCols() const
{
	return getImpl()->getSelectedCols();
}

//----------------------------------------------------------------------------
//! \throw no-throw
//! \retval true if column name exists in resultset
//! \return otherwise returns false
bool ResultsetRef::isExist(const VarString & rColName) const
{
	return getImpl()->isExist(rColName);
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//! \return index (0, ..., n-1) of column name in resultset
UInt32 ResultsetRef::getIndex(const VarString & rColName) const
{
	return getImpl()->getIndex(rColName);
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
const Int64 & ResultsetRef::getInt64(UInt32 index) const
{
    return getImpl()->getInt64(index);
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
const Int64 & ResultsetRef::getInt64(const VarString& rColName) const
{
    return getImpl()->getInt64ByName(rColName);
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
const Int32 & ResultsetRef::getInt32(UInt32 index) const
{
	return getImpl()->getInt32(index);
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
const Int32 & ResultsetRef::getInt32(const VarString & rColName) const
{
	return getImpl()->getInt32ByName(rColName);
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
const Int16 & ResultsetRef::getInt16(UInt32 index) const
{
	return getImpl()->getInt16(index);
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
const Int16 & ResultsetRef::getInt16(const VarString & rColName) const
{
	return getImpl()->getInt16ByName(rColName);
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
ConstBuffer ResultsetRef::getString(UInt32 index) const
{
	return getImpl()->getString(index);
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
ConstBuffer ResultsetRef::getString(const VarString & rColName) const
{
	return getImpl()->getStringByName(rColName);
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
const Binary & ResultsetRef::getBinary(UInt32 index) const
{
    return getImpl()->getBinary(index);
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
const Binary & ResultsetRef::getBinary(const VarString& rColName) const
{
    return getImpl()->getBinaryByName(rColName);
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
const Float64 & ResultsetRef::getFloat64(UInt32 index) const
{
	return getImpl()->getFloat64(index);
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
const Float64 & ResultsetRef::getFloat64(const VarString & rColName) const
{
	return getImpl()->getFloat64ByName(rColName);
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
const Float32 & ResultsetRef::getFloat32(UInt32 index) const
{
	return getImpl()->getFloat32(index);
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
const Float32 & ResultsetRef::getFloat32(const VarString & rColName) const
{
	return getImpl()->getFloat32ByName(rColName);
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
const Decimal & ResultsetRef::getDecimal (UInt32 index) const
{
	return getImpl()->getDecimal(index);
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
const Decimal & ResultsetRef::getDecimal (const VarString & rColName) const
{
	return getImpl()->getDecimalByName(rColName);
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
const Date & ResultsetRef::getDate(UInt32 index) const
{
	return getImpl()->getDate(index);
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
const Date & ResultsetRef::getDate(const VarString & rColName) const
{
	return getImpl()->getDateByName(rColName);
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
const Time & ResultsetRef::getTime(UInt32 index) const
{
	return getImpl()->getTime(index);
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
const Time & ResultsetRef::getTime(const VarString & rColName) const
{
	return getImpl()->getTimeByName(rColName);
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
const DateTime & ResultsetRef::getDateTime(UInt32 index) const
{
	return getImpl()->getDateTime(index);
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
const DateTime & ResultsetRef::getDateTime(const VarString & rColName) const
{
	return getImpl()->getDateTimeByName(rColName);
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
const TimeSpan & ResultsetRef::getTimeSpan(UInt32 index) const
{
	return getImpl()->getTimeSpan(index);
}

//----------------------------------------------------------------------------
//! \return column value
//! \throw  no-throw
const TimeSpan & ResultsetRef::getTimeSpan(const VarString & rColName) const
{
	return getImpl()->getTimeSpanByName(rColName);
}

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar
