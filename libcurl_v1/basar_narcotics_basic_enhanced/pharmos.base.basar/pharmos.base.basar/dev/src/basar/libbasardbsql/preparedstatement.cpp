//----------------------------------------------------------------------------
/*! \file
*  \brief  prepared statement implementation class
*  \author Roland Kiefert
*  \date   02.11.2005
*/
//----------------------------------------------------------------------------

#include "preparedstatement.h"
#include "connection.h"
#include "libbasarcmnutil_exceptions.h"

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//------------------------------------------------------------------------------
ConstString PreparedStatement::s_METHOD_INVALID		= "invalid/unknown SQL type";
ConstString PreparedStatement::s_METHOD_SETINT32	= "basar.db.sql.PreparedStatementRef.setInt32"  ;
ConstString PreparedStatement::s_METHOD_SETINT16	= "basar.db.sql.PreparedStatementRef.setInt16"  ;
ConstString PreparedStatement::s_METHOD_SETSTRING	= "basar.db.sql.PreparedStatementRef.setString" ;
ConstString PreparedStatement::s_METHOD_SETFLOAT64	= "basar.db.sql.PreparedStatementRef.setFloat64";
ConstString PreparedStatement::s_METHOD_SETFLOAT32	= "basar.db.sql.PreparedStatementRef.setFloat32";
ConstString PreparedStatement::s_METHOD_SETDECIMAL	= "basar.db.sql.PreparedStatementRef.setDecimal";
ConstString PreparedStatement::s_METHOD_SETTIME		= "basar.db.sql.PreparedStatementRef.setTime";
ConstString PreparedStatement::s_METHOD_SETDATE		= "basar.db.sql.PreparedStatementRef.setDate";
ConstString PreparedStatement::s_METHOD_SETDATETIME	= "basar.db.sql.PreparedStatementRef.setDateTime";
ConstString PreparedStatement::s_METHOD_SETTIMESPAN	= "basar.db.sql.PreparedStatementRef.setTimeSpan";
ConstString PreparedStatement::s_METHOD_SETBINARY   = "basar.db.sql.PreparedStatementRef.setBinary";
ConstString PreparedStatement::s_METHOD_SETINT64    = "basar.db.sql.PreparedStatementRef.setInt64";
ConstString PreparedStatement::s_METHOD_UNKNOWN		= "unknown SQL type";


//------------------------------------------------------------------------------
//! check statement for number of occuring '?'
//! \return number of occuring '?'
//! \throw no-throw
UInt32 PreparedStatement::getCountParameter(const VarString & rStmt)
{
#define PREPAREDSTATEMENT_PARAMETER_SYMBOL	'?'				// parameter to be checked in statement

    return static_cast<UInt32>(rStmt.getCount(PREPAREDSTATEMENT_PARAMETER_SYMBOL));
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
PreparedStatement::PreparedStatement ( ConnectionSharedPtr pConn, const VarString& rSql )
    : m_pConn         (pConn),
    m_Open          (false),
    m_CountParameter(getCountParameter(rSql))
{
    m_SqlExceptHnd = m_pConn->m_SqlExceptHnd; //copy shared pointer to ExceptHandler to know if inf / odbc !

    for (UInt32 i = 0; i < m_CountParameter; i++)
        m_CollParamFlags.push_back(false);
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
PreparedStatement::~PreparedStatement ()
{
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void PreparedStatement::close()
{
    if (!m_ResultsetRef.isNull())
    {
        m_ResultsetRef.close();		// close cursor
        m_ResultsetRef.reset();		// delete impl if last reference
    }

    if (!m_InsertsetRef.isNull())
    {
        m_InsertsetRef.close();		// close cursor
        m_InsertsetRef.reset();		// delete impl if last reference
    }
}

//------------------------------------------------------------------------------
//!	\retval true  if statement is opened/prepared
//! \retval false if statement is closed/freed
//! \throw no-throw
bool PreparedStatement::isOpen() const
{
    return m_Open;
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void PreparedStatement::checkConnState (const VarString & rWhat,
    const VarString & rFile,
    UInt32            line) const
{
    m_pConn->checkConnState(rWhat, rFile, line);
}

//------------------------------------------------------------------------------
//! \throw no-throw
//! \retval true if all parameters are set
//! \retval false if not all parameters are set
bool PreparedStatement::isParamsSet()
{
    for (UInt32 i = 0; i < m_CountParameter; ++i)
    {
        if (m_CollParamFlags[i] == false)
            return false;
    }

    return true;
}

//----------------------------------------------------------------------------
//! \throw  OutOfRangeIndexException
//!
void PreparedStatement::checkRange (UInt32      index,
    ConstString func ,
    ConstString file ,
    UInt32		line ) const
{
    if (m_CountParameter <= index)
    {
        VarString val;
        val.format("index %ld >= m_CountParameter %ld", 
            index,
            m_CountParameter);

        throw OutOfRangeIndexException(ExceptInfo (func, val, file, line));
    }
}

//------------------------------------------------------------------------------
//! \throw no-throw
//! \retval true if fetch mode is raw fetching
//! \retval false if fetch mode is not raw fetching
bool PreparedStatement::isFetchModeRaw() const
{
    return m_pConn->isFetchModeRaw();
}

//------------------------------------------------------------------------------
//! \throw  InvalidStateException
//! \return reference to created insertset
InsertsetRef PreparedStatement::executeInsert()
{
	bool f = true;

    //! if this virtual is called then there doesn't exist any RDBMS specific implementation
	if (f)
	{
	throw InvalidStateException(ExceptInfo(	"basar.db.sql.PreparedStatement.executeInsert",
											"method not implemented in RDBMS specific context",
                    						__FILE__, __LINE__));
	}

	return InsertsetRef();
}

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar
