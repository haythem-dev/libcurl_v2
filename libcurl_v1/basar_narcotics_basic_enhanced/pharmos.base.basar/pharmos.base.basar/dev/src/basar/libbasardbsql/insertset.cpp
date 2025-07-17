//----------------------------------------------------------------------------
/*! \file
 *  \brief  insertset implementation class
 *  \author Roland Kiefert
 *  \date   08.11.2005
 */
//----------------------------------------------------------------------------

#include "insertset.h"
#include "connection.h"

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//------------------------------------------------------------------------------
//! \throw no-throw
//!
Insertset::Insertset ( ConnectionSharedPtr pConn )
					 : m_eState			(STATE_INVALID),
					   m_InsertedRows	(0            ),
					   m_pConn			(pConn        )
{
	m_SqlExceptHnd = m_pConn->m_SqlExceptHnd; //copy shared pointer to ExceptHandler to know if inf / odbc !
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
Insertset::~Insertset()
{
}

//------------------------------------------------------------------------------
//!	\retval true  if insertset is opened
//! \retval false if insertset is closed/not opened
//! \throw no-throw
bool Insertset::isOpen() const
{
	return (STATE_OPENED == m_eState);
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void Insertset::checkConnState(const VarString & rWhat,
							   const VarString & rFile,
							   UInt32			 line ) const
{
	m_pConn->checkConnState(rWhat, rFile, line);
}

//----------------------------------------------------------------------------
//! \return number of inserted rows
//! \throw  no-throw
UInt32 Insertset::getInsertedRows() const
{
	return m_InsertedRows;
}

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar
