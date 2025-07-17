//----------------------------------------------------------------------------
/*! \file
 *  \brief  SQL diagnostics area's info implementation class
 *  \author Michael Eichenlaub
 *  \date   11.08.2005
 */
//----------------------------------------------------------------------------

#include "sqldiagnostics.h"
#include "libbasarcmnutil_exceptions.h"

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//----------------------------------------------------------------------------
//! \throw  no-throw
//!
SqlDiagnostics::SqlDiagnostics()
{
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//!
SqlDiagnostics::~SqlDiagnostics()
{
}

//------------------------------------------------------------------------------
//! \throw no-throw
//! 
void SqlDiagnostics::getAllExcepts(VarString & outString) const
{
	outString.format ("  DBSERVER <%s>, CONNECTION NAME <%s>, count %d", 
					  getDbServer().c_str(),
					  getConnId  ().c_str(),
					  m_CountExcepts);

	VarString tmp;

	for (Int32 i = 0; i < m_CountExcepts; ++i)
	{
		absolute(i);

		tmp.format ("\n  [%02d]  SQLSTATE %s"  \
					"\n        SQLCODE  %d" \
					"\n        MESSAGE  %s",
					i,
					getSqlState().c_str(),
					getSqlCode (),
					getMsgTxt  ().c_str());

		outString += tmp;
	}
}

//----------------------------------------------------------------------------
//! \throw  OutOfRangeIndexException
//!
void SqlDiagnostics::absolute(Int32 exceptIndex) const
{
	if ((0 > exceptIndex) || (m_CountExcepts <= exceptIndex) )
	{
		VarString val;
		val.format("!(0 <= exceptIndex (%ld) < m_CountExcepts (%ld)", 
			       exceptIndex,
				   m_CountExcepts);

		throw OutOfRangeIndexException(ExceptInfo ("SqlDiagnostics::absolute()", val, __FILE__, __LINE__));
	}

	m_ItDiagInfo = m_CollDiagInfo.begin() + exceptIndex;
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//! \return number of occured exceptions
Int32 SqlDiagnostics::getCountExcepts() const
{
	return m_CountExcepts;
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//! \return name of dbserver
const VarString & SqlDiagnostics::getDbServer() const
{
	return m_DbServer;
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//! \return unique connection id
const VarString & SqlDiagnostics::getConnId() const
{
	return m_ConnId;
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//! \return sql state
const VarString & SqlDiagnostics::getSqlState() const
{
	return (*m_ItDiagInfo).sqlState;
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//! \return sql error code
BLong SqlDiagnostics::getSqlCode() const
{
	return (*m_ItDiagInfo).sqlCode;
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//! \return sql message
const VarString & SqlDiagnostics::getMsgTxt() const
{
	return (*m_ItDiagInfo).msgTxt;
}

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar
