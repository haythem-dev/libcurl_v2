//----------------------------------------------------------------------------
/*! \file
 *  \brief  domain module for user specific handling
 *  \author Thomas Hörath
 *  \date   15.12.2005
 */
//----------------------------------------------------------------------------
#ifndef GUARD_USERMANAGEMENTDM_H
#define GUARD_USERMANAGEMENTDM_H

//----------------------------------------------------------------------------
#include "libbasar_definitions.h"
#include "libbasardbaspect_accessor.h"
#include "baseusermgmtdm.h"

//----------------------------------------------------------------------------
namespace basar
{
	namespace cmnutil
	{
		class ParameterList;
	}

	namespace db 
	{
		namespace aspect
		{
			class  ConnectionRef;
			class  AccessorPropertyTable_YIterator;
			class  AccessorPropertyTableRef;
			struct ExecuteResultInfo;
		}
	}
}

//----------------------------------------------------------------------------
//! namespace for domain modules
namespace domMod {

//---------------------------------------------------------------------------
/*! \brief class for managing user specific access 
	\n final class
	\n throws no exceptions */
class UserManagement: public BaseUserManagement
{
public:	
	/*! \brief std-constructor
		\n no-throw */
	UserManagement();
	
	/*! \brief std-destructor
		\n no-throw */
	~UserManagement();

	/*! \brief initialize accessor instances
		\n no-throw */
	void init( const basar::db::aspect::ConnectionRef activeConn //!< reference to active connection
        );
};

//----------------------------------------------------------------------------
} // domMod

//----------------------------------------------------------------------------
#endif	// GUARD
