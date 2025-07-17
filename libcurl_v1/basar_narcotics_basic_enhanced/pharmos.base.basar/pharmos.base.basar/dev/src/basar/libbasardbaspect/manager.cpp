//-------------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  implementation of Manager and ManagerBase
 *  \author Roland Kiefert
 *  \date   18.07.2005
 */
//-------------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------------//
#include "loggerpool.h"
#include "managerimpl.h"
#include "libbasardbaspect_manager.h"
#include "libbasardbaspect_functions.h"
#include "libbasardbaspect_accessor.h"
#include "libbasardbsql_connectionlist_iterator.h"

//-------------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------------//
namespace basar  {

//------------------------------------------------------------------------------
namespace cmnutil {

//------------------------------------------------------------------------------
using db::aspect::ManagerBase;

/*!	specialised method */
//! \return shared_ptr to ManagerBase instance
template <>
typename Singleton<ManagerBase>::InstancePtr & Singleton<ManagerBase>::inst()
{
    // avoid init order fiasco
    db::aspect::init();

    static InstancePtr s_Inst;
    return s_Inst;
}

//------------------------------------------------------------------------------
}

//------------------------------------------------------------------------------
namespace db     {
namespace aspect {

///////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw  no-throw
//!	\return reference to a connection 
//!	\param rDbInfo database infos  
//!	\param openConn tells if the database should be opened
///////////////////////////////////////////////////////////////////////////////////////////////
ConnectionRef ManagerBase::createConnect( const ConnectionRef::DatabaseInfo& rDbInfo, 
										   bool openConn /*=true*/ )
{
	return getImpl()->createConnect( rDbInfo, openConn );
}
///////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw UnknownAccessorDefinitonException, NullReferenceException
//!	\return reference to an accessor instance 
//!	\param rInsName name of the accessor instance
//!	\param rDefName name of the accessor definition 
//!	\param connRef connection reference
//!	\param eCachePolicy caching policy, default ON_DEMAND_CACHING
//! \param storeInInstanceList should created instance be stored in instance list? (default false)
///////////////////////////////////////////////////////////////////////////////////////////////
AccessorInstanceRef ManagerBase::createAccessorInstance(	const AccessorName& rInsName, 
															const AccessorName& rDefName, 
															const ConnectionRef connRef, 
															const CachingPolicyEnum eCachePolicy /*= ON_DEMAND_CACHING*/,
															bool  storeInInstanceList /*= false*/)
{
	return getImpl()->createAccessorInstance( rInsName, rDefName, connRef, eCachePolicy, storeInInstanceList );
}

/*!	\throw NullReferenceException 
	\return the created reference to an accessor definition 
	\param rName name of the accessor definition */
AccessorDefinitionRef ManagerBase::createAccessorDefinition(const AccessorName& rName )
{
	return getImpl()->createAccessorDefinition(rName);
}

/*! \return	created accessorpropertytable yiterator 
	\param	rParaList parameter list
	\throw	no-throw */
AccessorPropertyTable_YIterator ManagerBase::createAccessorYIterator( const basar::cmnutil::ParameterList& rParaList )
{
	return getImpl()->createAccessorYIterator(rParaList);
}

/*!	\throw NullReferenceException 
	\return reference to an accessor definition 
	\param rName name of the accessor definition */
AccessorDefinitionRef ManagerBase::getAccessorDefinition(const AccessorName& rName )
{
	return getImpl()->getAccessorDefinition(rName);
}

/*!	\throw UnknownAccessorInstanceException, NullReferenceException 
	\return reference to an accessor instance 
	\param rName name of the accessor instance */
AccessorInstanceRef ManagerBase::getAccessorInstance(const AccessorName& rName) 
{
	return getImpl()->getAccessorInstance(rName);
}

/*!	\throw UnknownAccessorInstanceException 
	\param rName name of the accessor definition */
void ManagerBase::releaseAccessorDefinition(const AccessorName& rName ) 
{
	return getImpl()->releaseAccessorDefinition(rName);
}

/*!	\throw UnknownAccessorInstanceException 
	\param rName name of the accessor instance */
void ManagerBase::releaseAccessorInstance(const AccessorName& rName) 
{
	return getImpl()->releaseAccessorInstance(rName);
}

/*!	\throw NullReferenceException 
	\return list of accessor definitions */
AccessorDefinitionListRef ManagerBase::getAccessorDefinitionList()
{
	return getImpl()->getAccessorDefinitionList();
}

/*!	\throw NullReferenceException 
	\return list of accessor instances */
AccessorInstanceListRef ManagerBase::getAccessorInstanceList()
{
	return getImpl()->getAccessorInstanceList();
}
/*!	\throw NullReferenceException 
	\param rName name of the accessor definition
	\return bool indicating if accessor definition is known by manager */
bool ManagerBase::hasAccessorDefinition(const AccessorName& rName) const
{
	return getImpl()->hasAccessorDefinition(rName);
}
/*!	\throw NullReferenceException 
	\param rName name of the accessor instance
	\return bool indicating if accessor instance is stored in manager's instance list */
bool ManagerBase::hasAccessorInstance(const AccessorName& rName) const
{
	return getImpl()->hasAccessorInstance(rName);
}
/*!	\throw  NullReferenceException
	\return pointer to Manager */
const boost::shared_ptr<ManagerImpl>& ManagerBase:: getImpl() const
{
	if (m_pManager.get() == 0)
	{
		ExceptInfo sInfo( "basar.db.aspect.ManagerBase.check", 
						  "Reference of ManagerImpl is no pointer to impl-class", 
						  __FILE__, __LINE__);
		throw NullReferenceException(sInfo);
	}
	return m_pManager;
}
///////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////
ManagerBase::ManagerBase()
:m_pManager(ManagerImpl::create())
{
}
///////////////////////////////////////////////////////////////////////////////////////////////
/*!	\retval true if the passed connection the iterator is pointing to is open
	\return otherwise returns false
	\param	it iterator to a connection
	\throw  no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////
bool ManagerBase::isOpen( const ConnectionList_Iterator it ) const
{
	return getImpl()->isOpen( it );
}
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////
ManagerBase::ConnectionList_Iterator ManagerBase::beginConn() const
{
	return getImpl()->beginConn();
}	
///////////////////////////////////////////////////////////////////////////////////////////////
/*! \brief The method returns an iterator to the last element of the connection list. 
 	\n     no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////
ManagerBase::ConnectionList_Iterator ManagerBase::endConn() const
{
	return getImpl()->endConn();
}
///////////////////////////////////////////////////////////////////////////////////////////////
/*! \brief	The method returns the size of the connection list. \n no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////
ManagerBase::ConnectionListSize ManagerBase::sizeConn() const
{
	return getImpl()->sizeConn();
}
///////////////////////////////////////////////////////////////////////////////////////////////
/*! \brief	The method returns the number of all open connections in CollConnection. \n no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////
ManagerBase::ConnectionListSize ManagerBase::sizeOpenConn() const
{
	return getImpl()->sizeOpenConn();
}
///////////////////////////////////////////////////////////////////////////////////////////////
/*! \brief	The method returns the number of all open connections in CollConnection. \n no-throw
	\param	it iterator of CollConnection
	\param	connId reference to VarString, set to application's unique connection id
	\return	the database info to given iterator and sets connId to connections unique id 
*/
///////////////////////////////////////////////////////////////////////////////////////////////
const ConnectionRef::DatabaseInfo& ManagerBase::getConnInfo( const ConnectionList_Iterator it, 
															 VarString& connId ) const
{
	return getImpl()->getConnInfo( it, connId );
}

} // aspect
} // db
} // basar

