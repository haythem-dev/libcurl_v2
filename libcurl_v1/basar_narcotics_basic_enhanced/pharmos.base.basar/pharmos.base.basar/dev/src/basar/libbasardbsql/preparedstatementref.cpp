//----------------------------------------------------------------------------
/*! \file
*  \brief  prepared statement reference class
*  \author Roland Kiefert
*  \date   02.11.2005
*/
//----------------------------------------------------------------------------

#include "libbasardbsql_preparedstatementref.h"

#include "libbasarcmnutil_exceptions.h"
#include "libbasarcmnutil_decimal.h"
#include "preparedstatement.h"

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//----------------------------------------------------------------------------
//! \throw  no-throw 
PreparedStatementRef::PreparedStatementRef()
{
}

//----------------------------------------------------------------------------
//! \throw  no-throw 
//! \param  pImpl shared pointer to implementation object
PreparedStatementRef::PreparedStatementRef ( PreparedStatementSharedPtr pImpl )
    : m_pImpl(pImpl)
{
}

//----------------------------------------------------------------------------
//! \throw  no-throw 
PreparedStatementRef::~PreparedStatementRef()
{
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//!
void PreparedStatementRef::reset()
{
    m_pImpl.reset();
}

//----------------------------------------------------------------------------
//!	\retval true  if shared pointer to implementation object is null
//! \retval false if shared pointer to implementation object is valid
//! \throw  no-throw
bool PreparedStatementRef::isNull() const
{
    return (0 == m_pImpl.get());
}

//----------------------------------------------------------------------------
//!	\return shared pointer to implementation object
//! \throw  NullReferenceException 
const PreparedStatementSharedPtr & PreparedStatementRef::getImpl() const
{
    if (0 == m_pImpl.get())
        throw NullReferenceException(ExceptInfo("basar.db.sql.PreparedStatementRef.getImpl", 
        "m_pImpl.get() == 0", 
        __FILE__, __LINE__));

    return m_pImpl;
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void PreparedStatementRef::close()
{
    getImpl()->close();
}

//------------------------------------------------------------------------------
//!	\retval true  if statement is opened
//! \retval false if statement is closed/not opened
//! \throw no-throw
bool PreparedStatementRef::isOpen() const
{
    return getImpl()->isOpen();
}

//------------------------------------------------------------------------------
//! \throw no-throw
//! \return reference to created resultset
ResultsetRef PreparedStatementRef::executeQuery()
{
    return getImpl()->executeQuery();
}

//------------------------------------------------------------------------------
//! \throw no-throw
//! \return reference to created resultset
ResultsetRef PreparedStatementRef::executeSingleQuery()
{
    return getImpl()->executeSingleQuery();
}

//------------------------------------------------------------------------------
//! \throw  no-throw
//! \return reference to created insertset
InsertsetRef PreparedStatementRef::executeInsert()
{
    return getImpl()->executeInsert();
}

//------------------------------------------------------------------------------
//! \throw  no-throw
//! \return number of rows affected by statement (update, delete, insert)
ExecuteReturnInfo PreparedStatementRef::execute(bool flagExcept /* = true */)
{
    return getImpl()->execute(flagExcept);
}

//------------------------------------------------------------------------------
//! \throw  no-throw
//! \return result of aggregate select
basar::Decimal PreparedStatementRef::executeAggregate()
{
	return getImpl()->executeAggregate();
}

//----------------------------------------------------------------------------
//! \throw  no-throw
void PreparedStatementRef::setInt64(UInt32 pos, const Int64 & rValue)
{
    getImpl()->setInt64(pos, rValue);
}

//----------------------------------------------------------------------------
//! \throw  no-throw
void PreparedStatementRef::setInt32(UInt32 pos, const Int32 & rValue)
{
    getImpl()->setInt32(pos, rValue);
}

//----------------------------------------------------------------------------
//! \throw  no-throw
void PreparedStatementRef::setInt16(UInt32 pos, const Int16 & rValue)
{
    getImpl()->setInt16(pos, rValue);
}

//----------------------------------------------------------------------------
//! \throw  no-throw
void PreparedStatementRef::setString(UInt32 pos, ConstString value)
{
    getImpl()->setString(pos, value);
}

//----------------------------------------------------------------------------
//! \throw  no-throw
void PreparedStatementRef::setBinary(UInt32 pos, const Binary& value)
{
    getImpl()->setBinary(pos, value);
}

//----------------------------------------------------------------------------
//! \throw  no-throw
void PreparedStatementRef::setFloat64(UInt32 pos, const Float64 & rValue)
{
    getImpl()->setFloat64(pos, rValue);
}

//----------------------------------------------------------------------------
//! \throw  no-throw
void PreparedStatementRef::setFloat32(UInt32 pos, const Float32 & rValue)
{
    getImpl()->setFloat32(pos, rValue);
}

//----------------------------------------------------------------------------
//! \throw  no-throw
void PreparedStatementRef::setDecimal(UInt32 pos, const Decimal & rValue)
{
    getImpl()->setDecimal(pos, rValue);
}

//----------------------------------------------------------------------------
//! \throw  no-throw
void PreparedStatementRef::setTime(UInt32 pos, const Time & rValue)
{
    getImpl()->setTime(pos, rValue);
}

//----------------------------------------------------------------------------
//! \throw  no-throw
void PreparedStatementRef::setDate(UInt32 pos, const Date & rValue)
{
    getImpl()->setDate(pos, rValue);
}

//----------------------------------------------------------------------------
//! \throw  no-throw
void PreparedStatementRef::setDateTime(UInt32 pos, const DateTime & rValue)
{
    getImpl()->setDateTime(pos, rValue);
}

//----------------------------------------------------------------------------
//! \throw  no-throw
void PreparedStatementRef::setTimeSpan(UInt32 pos, const TimeSpan & rValue)
{
    getImpl()->setTimeSpan(pos, rValue);
}

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar
