//----------------------------------------------------------------------------
/*! \file
 *  \brief  statement implementation class
 *  \author Michael Eichenlaub
 *  \date   21.07.2005
 */
//----------------------------------------------------------------------------


#include "statement.h"
#include "connection.h"
#include "libbasarcmnutil_exceptions.h"

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//------------------------------------------------------------------------------
//! \throw no-throw
//!
Statement::Statement ( ConnectionSharedPtr pConn )
					 : m_pConn(pConn),
					   m_Open (false)
{
    //copy shared pointer to ExceptHandler to know if inf / odbc !
	m_SqlExceptHnd = m_pConn->m_SqlExceptHnd; 
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
Statement::~Statement ()
{
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void Statement::close()
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
bool Statement::isOpen() const
{
	return m_Open;
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void Statement::checkConnState (const VarString & rWhat,
								const VarString & rFile,
								UInt32            line) const
{
	m_pConn->checkConnState(rWhat, rFile, line);
}

//------------------------------------------------------------------------------
//! \throw  InvalidStateException
//! \return reference to created insertset
InsertsetRef Statement::executeInsert(
    const VarString &, 
    CursorTypeEnum /* eCursorType */)
{
	//! if this virtual method is called then there doesn't exist any RDBMS specific implementation
    throw InvalidStateException(
            ExceptInfo(
                "basar.db.sql.Statement.executeInsert",
												"method not implemented in RDBMS specific context",
												__FILE__, __LINE__));
}

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar
