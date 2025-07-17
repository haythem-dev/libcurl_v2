//----------------------------------------------------------------------------
/*! \file
 *  \brief  database implementation class
 *  \author Michael Eichenlaub
 *  \date   26.07.2005
 */
//----------------------------------------------------------------------------

#include "database.h"

#include "libbasarcmnutil_exceptions.h"

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//----------------------------------------------------------------------------
//! \throw  no-throw 
//! \param  rDbInfo database specific connection data
Database::Database ( const DatabaseInfo & rDbInfo ) 
                   : m_DbInfo(rDbInfo)
{
}

//----------------------------------------------------------------------------
//!	\return type of connection values in m_DbInfo
//! \throw  InvalidParameterException
Database::ConnTypeEnum Database::getConnType() const
{
	switch (m_DbInfo.eConnectType)
	{
	case INFORMIX:
		if ( m_DbInfo.dbServer.empty() || m_DbInfo.database.empty() )
		{
			VarString val;
			val.format("Informix: m_DbInfo.dbServer <%s>, m_DbInfo.database <%s>",
						m_DbInfo.dbServer.c_str(),
						m_DbInfo.database.c_str());

			throw InvalidParameterException(ExceptInfo ("basar.db.sql.Database.getConnType", val, __FILE__, __LINE__));
		}

		break;

	case ODBC:
		if ( m_DbInfo.database.empty() )
		{
			VarString val;
			val.format("ODBC: m_DbInfo.database not set");

			throw InvalidParameterException(ExceptInfo ("basar.db.sql.Database.getConnType", val, __FILE__, __LINE__));
		}

		break;

	default:
		break;
	}

	return (true == m_DbInfo.user.empty()) ? CONNTYPE_USER_DEFAULT 
		                                   : CONNTYPE_USER_SET;
}

//----------------------------------------------------------------------------
//! \return	constant string containing database server
//! \throw  no-throw
const DatabaseInfo & Database::getDbInfo() const
{
	return m_DbInfo;
}

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar
