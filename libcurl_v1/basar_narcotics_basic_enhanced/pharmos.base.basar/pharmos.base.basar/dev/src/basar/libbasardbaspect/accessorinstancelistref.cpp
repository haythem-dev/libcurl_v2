//----------------------------------------------------------------------------
/*! \file
 *  \brief  accessor instance list
 *  \author Roland Kiefert
 *  \date   19.07.2005
 */
//----------------------------------------------------------------------------

#include "libbasardbaspect_accessor.h"
#include "managerimpl.h"

//--------------------------------------------------------------------------------------------------
namespace basar {
namespace db {
namespace aspect {

//--------------------------------------------------------------------------------------------------
/*!	\throw no-throw 
	\param Manager a reference to the impl class ManagerImpl */
AccessorInstanceListRef::AccessorInstanceListRef( const boost::shared_ptr<ManagerImpl> Manager ) 
: m_Manager(Manager)
{
}

//--------------------------------------------------------------------------------------------------
/*!	\throw no-throw 
	\return the first iterator of the list */
AccessorInstanceList_Iterator AccessorInstanceListRef::begin()
{
	AccessorInstanceList_Iterator ret (m_Manager, m_Manager->beginInstance());
	return ret;
}

//--------------------------------------------------------------------------------------------------
/*!	\throw no-throw 
	\return the last iterator of the list */
AccessorInstanceList_Iterator AccessorInstanceListRef::end()
{
	AccessorInstanceList_Iterator ret (m_Manager, m_Manager->endInstance());
	return ret;
}

//--------------------------------------------------------------------------------------------------
/*!	\throw no-throw 
	\return the size of the list */
AccessorListSize AccessorInstanceListRef::size() const
{
	return m_Manager->sizeInstance();
}

//--------------------------------------------------------------------------------------------------
} // aspect
} // db
} // basar
