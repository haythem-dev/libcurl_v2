//----------------------------------------------------------------------------
/*! \file
 *  \brief  database implementation class
 *  \author Michael Eichenlaub
 *  \date   26.07.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_DATABASE_H
#define GUARD_DATABASE_H

//----------------------------------------------------------------------------
#include "libbasardbsql_databaseinfo.h"

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//----------------------------------------------------------------------------
//! database implementation class \n final class
class Database
{
	friend class Connection;	
	friend class ConnectionCics;

public:
	//! \brief type of connection values in m_DbInfo
	enum ConnTypeEnum			
	{
		CONNTYPE_USER_SET,		//!< user data are set
		CONNTYPE_USER_DEFAULT	//!< user data are not set -> use default host user (see setnet32)
	};

	ConnTypeEnum				getConnType		() const;	//!< type of connection values	\n no-throw

	const DatabaseInfo &        getDbInfo		() const;	//!< get database info			\n no-throw


private:
	//! construct database implementation object by database specific connection data	\n no-throw
								Database		(const DatabaseInfo & rDbInfo);

	//! default constructor				\n no-throw
								Database		();
	//! copy constructor				\n no-throw
								Database		(const Database     & rCopy  );
	
	//! assignment operator				\n no-throw
	Database &    				operator =		(const Database     & rCopy  );


	DatabaseInfo	m_DbInfo;	//!< database specific connection data
};

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

#endif	// GUARD
