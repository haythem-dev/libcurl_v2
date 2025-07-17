//----------------------------------------------------------------------------
/*! \file
 *  \brief  insertset reference class
 *  \author Roland Kiefert
 *  \date   08.11.2005
 */
//----------------------------------------------------------------------------

#include "libbasardbsql_insertsetref.h"

#include "libbasarcmnutil_exceptions.h"
#include "insertset.h"

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//----------------------------------------------------------------------------
//! \throw  no-throw 
//!
InsertsetRef::InsertsetRef()
{
}

//----------------------------------------------------------------------------
//! \throw  no-throw 
//! 
InsertsetRef::InsertsetRef ( InsertsetSharedPtr pImpl)
                           : m_pImpl(pImpl)
{
}

//----------------------------------------------------------------------------
//! \throw  no-throw 
//!
InsertsetRef::~InsertsetRef()
{
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//!
void InsertsetRef::reset()
{
	m_pImpl.reset();
}

//----------------------------------------------------------------------------
//!	\retval true  if shared pointer to implementation object is null
//! \retval false if shared pointer to implementation object is valid
//! \throw  no-throw
bool InsertsetRef::isNull() const
{
	return (0 == m_pImpl.get());
}

//----------------------------------------------------------------------------
//!	\return shared pointer to implementation object
//! \throw  NullReferenceException 
const InsertsetSharedPtr & InsertsetRef::getImpl() const
{
	if (0 == m_pImpl.get())
		throw NullReferenceException(ExceptInfo("basar.db.sql.InsertsetRef.getImpl", 
												"m_pImpl.get() == 0", __FILE__, __LINE__));

	return m_pImpl;
}

//------------------------------------------------------------------------------
//!	\retval true  if insertset is opened
//! \retval false if insertset is closed/not opened
//! \throw no-throw
bool InsertsetRef::isOpen() const
{
	return getImpl()->isOpen();
}

//------------------------------------------------------------------------------
//! \throw no-throw
//!
void InsertsetRef::close()
{
	getImpl()->close();
}

//----------------------------------------------------------------------------
//! \return number of inserted rows
//! \throw no-throw
UInt32 InsertsetRef::getInsertedRows() const
{
	return getImpl()->getInsertedRows();
}

//----------------------------------------------------------------------------
//! \throw no-throw
void InsertsetRef::insertRow()
{
	getImpl()->insertRow();
}

//----------------------------------------------------------------------------
//! \throw no-throw
//! \return number of inserted rows to database
UInt32 InsertsetRef::flushRows()
{
	return getImpl()->flushRows();
}

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar
