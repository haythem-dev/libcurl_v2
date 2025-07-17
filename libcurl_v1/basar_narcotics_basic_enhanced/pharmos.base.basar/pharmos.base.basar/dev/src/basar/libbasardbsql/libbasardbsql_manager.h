//------------------------------------------------------------------------------
/*! \file
 *  \brief  public library interface: manager singleton for database connections
 *  \author Michael Eichenlaub
 *  \date   15.07.2005
 */
//------------------------------------------------------------------------------

#ifndef GUARD_LIBBASARDBSQL_MANAGER_H
#define GUARD_LIBBASARDBSQL_MANAGER_H

//------------------------------------------------------------------------------
#include "libbasardbsql_api.h"
#include "libbasardbsql_managerimpl.h"
#include "libbasarcmnutil_pattern.h"

//------------------------------------------------------------------------------
namespace basar {
namespace db	{
namespace sql	{

//------------------------------------------------------------------------------
//! \brief singleton wrapper for ManagerImpl \n final class
//! \note  class is necessary for dll exporting Singleton\<ManagerImpl\> and 
//!        instantiating Manager::m_Instance
class LIBBASARDBSQL_API Manager : public cmnutil::Singleton<ManagerImpl>
{
private:
									Manager    		();							//!< forbidden
									Manager    		(const Manager & rCopy);	//!< forbidden
	Manager &    					operator =		(const Manager & rCopy);	//!< forbidden
};

//------------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

//------------------------------------------------------------------------------
#endif	// GUARD
