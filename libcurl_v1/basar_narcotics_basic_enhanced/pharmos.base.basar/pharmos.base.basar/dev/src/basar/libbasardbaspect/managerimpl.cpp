//---------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  all functions implemented of ManagerImpl
 *  \author Roland Kiefert
 *  \date   18.07.2005
 */
//---------------------------------------------------------------------------------------------------//

//---------------------------------------------------------------------------------------------------//
// includes
//---------------------------------------------------------------------------------------------------//

#include "managerimpl.h"

#include "loggerpool.h"
#include "libbasardbsql_manager.h"
#include "libbasardbsql_connectionref.h"
#include "libbasardbsql_databaseinfo.h"
#include "libbasardbsql_connectionlist_iterator.h"
#include "libbasarproperty_propertytable_xiterator.h"
#include "connection.h"
#include "libbasardbaspect_accessor.h"
#include "accessor.h"
#include "cachecontroller.h"
#include "libbasardbaspect_exceptions.h"
#include "../libbasarproperty/propertydescriptionlist.h"

//---------------------------------------------------------------------------------------------------//
// namespace section
//---------------------------------------------------------------------------------------------------//
namespace basar  {
namespace db	 {
namespace aspect {
//---------------------------------------------------------------------------------------------------//
// create methods
//---------------------------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw  no-throw
//!	\return reference to a connection 
//!	\param rDbInfo database infos  
//!	\param openConn tells if the database should be opened
///////////////////////////////////////////////////////////////////////////////////////////////////////
ConnectionRef ManagerImpl::createConnect( const ConnectionRef::DatabaseInfo& rDbInfo, bool openConn )
{
	sql::ConnectionRef sqlConnRef = sql::Manager::getInstance().createConnect(sql::DatabaseInfo(rDbInfo), 
																			  openConn ); 
	
	return ConnectionRef( Connection::create(sqlConnRef) );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw UnknownAccessorDefinitonException, ExistAccessorInstanceException
//!	\return reference to an accessor instance 
//!	\param rInsName name of the accessor instance
//!	\param rDefName name of the accessor definition
//!	\param connRef connection reference
//!	\param eCachePolicy caching policy, default ON_DEMAND_CACHING
//! \param storeInInstanceList should created instance be stored in instance list? (default false)
///////////////////////////////////////////////////////////////////////////////////////////////////////
AccessorInstanceRef ManagerImpl::createAccessorInstance(	const AccessorName    & rInsName, 
															const AccessorName    & rDefName, 
															const ConnectionRef     connRef,
															const CachingPolicyEnum eCachePolicy        /* = ON_DEMAND_CACHING */,
															bool                    storeInInstanceList /* = false */)
{
	static ConstString fun = "basar.db.aspect.ManagerImpl.createAccessorInstance";

	if (LoggerPool::manager().isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
	{
	    basar::VarString msg;
	    msg.format("creating AccessorInstance: %s", rInsName.c_str());
	    LoggerPool::manager().forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	CollAccessorDefinitionRef::iterator it = m_AccessorDefinitionList.find( rDefName );

	if( it == m_AccessorDefinitionList.end() )
	{
		VarString msg = "The accessor definition name <";
		msg.append(rDefName);
		msg.append("> is unknown in the accessor definition list.");
		
		throw UnknownAccessorDefinitionException( ExceptInfo ( fun, msg, __FILE__, __LINE__) );
	}

	AccessorInstanceRef air = it->second.createInstance( connRef, eCachePolicy );
	air.setAccessorName( rInsName );

	if ( storeInInstanceList )
	{
		if ( !m_AccessorInstanceList.insert( CollAccessorInstanceRef::value_type(rInsName, air) ).second )
		{
			VarString msg;
			msg.format( "The following accessor instance name <%s> already exists!", rInsName.c_str() ); 
			throw ExistAccessorInstanceException( ExceptInfo( fun, msg, __FILE__, __LINE__ ) );
		}
	}

	return air;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  ExistAccessorDefinitionException
	\return reference to an accessor definition 
	\param  rName name of the accessor definition */
///////////////////////////////////////////////////////////////////////////////////////////////////////
AccessorDefinitionRef ManagerImpl::createAccessorDefinition( const AccessorName & rName )
{
	static const char* fun = "basar.db.aspect.ManagerImpl.createAccessorDefinition()";

	AccessorDefinitionRef adr( Accessor::create() );

	if ( !m_AccessorDefinitionList.insert(CollAccessorDefinitionRef::value_type(rName, adr)).second )
	{
		VarString errMsg;
		errMsg.format( "The following accessor definition name <%s> already exists!", rName.c_str() ); 
		throw ExistAccessorDefinitionException( ExceptInfo( fun, errMsg.c_str(), __FILE__, __LINE__ ) );
	}

	adr.setAccessorName( rName );

	return adr;
}

///////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	created accessorpropertytable yiterator 
	\param	rParaList parameter list
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTable_YIterator ManagerImpl::createAccessorYIterator( const cmnutil::ParameterList& rParaList )
{
	using cmnutil::ParameterList_Iterator;
	using property::PropertyDescriptionList;

	PropertyDescriptionListRef descriptList = PropertyDescriptionListRef( PropertyDescriptionList::create() );

	const ParameterList_Iterator itEnd = rParaList.end();

	for( ParameterList_Iterator it = rParaList.begin(); it != itEnd; ++it )
	{
		descriptList.push_back( it->first, basar::STRING );
	}

	AccessorPropertyTable_YIterator yIt = AccessorPropertyTableRef(
											 CacheController::create(descriptList)).insert(FOR_UNKNOWN);
	AccessorPropertyTable_XIterator xIt = yIt.begin();

	for( ParameterList_Iterator it = rParaList.begin(); it != itEnd; ++it, ++xIt )
	{
		if( ! it.isValueEmpty() )
			xIt.setString( it->second ); 
	}

	return 	yIt;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw
	\return pointer to AccessMethod itself */
///////////////////////////////////////////////////////////////////////////////////////////////////////
ManagerImpl::ManagerImplSharedPtr ManagerImpl::create()
{

	if (LoggerPool::manager().isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
	{
	    LoggerPool::manager().forcedLog(log4cplus::TRACE_LOG_LEVEL, "statically creating db::aspect::managerimpl", __FILE__, __LINE__);
	}

	ManagerImplSharedPtr t = ManagerImplSharedPtr(new ManagerImpl);
	t->setWeakPtr(t);
	return t;
}
//---------------------------------------------------------------------------------------------------//
// c'tor, d'tor and self references 
//---------------------------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////
ManagerImpl::ManagerImpl()
{
	if (LoggerPool::manager().isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
	{
	    LoggerPool::manager().forcedLog(log4cplus::TRACE_LOG_LEVEL, "creating db::aspect::managerimpl", __FILE__, __LINE__);
	}

}
///////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////
ManagerImpl::~ManagerImpl()
{
	if (LoggerPool::manager().isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
	{
	    LoggerPool::manager().forcedLog(log4cplus::TRACE_LOG_LEVEL, "destroying db::aspect::managerimpl", __FILE__, __LINE__);
	}

	m_AccessorDefinitionList.clear();
	m_AccessorInstanceList  .clear();
	sql::Manager           ::clear();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return (only one group of) AccessMethod shared pointer 
	\throw  no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////
ManagerImpl::ManagerImplSharedPtr ManagerImpl::getThis()
{
	return ManagerImplSharedPtr(m_WeakPtr);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \throw  no-throw
	\param weakPtr weak pointer to the class ManagerImpl */
///////////////////////////////////////////////////////////////////////////////////////////////////////
void ManagerImpl::setWeakPtr(ManagerImplWeakPtr weakPtr)
{
	m_WeakPtr = weakPtr;
}
//---------------------------------------------------------------------------------------------------//
// get methods
//---------------------------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw UnknownAccessorDefinitonException 
	\return reference to an accessor definition 
	\param rName name of the accessor definition */
///////////////////////////////////////////////////////////////////////////////////////////////////////
AccessorDefinitionRef ManagerImpl::getAccessorDefinition(const AccessorName& rName)
{
	CollAccessorDefinitionRef::iterator it = m_AccessorDefinitionList.find( rName );

	if( it == m_AccessorDefinitionList.end() )
	{
		VarString msg;
		msg.format("The accessor definition name <%s> does not exist.", rName.c_str());
		throw	UnknownAccessorDefinitionException(ExceptInfo( "basar.db.aspect.ManagerImpl.getAccessorDefinition", 
												   msg, 
												   __FILE__, __LINE__ ));
	}

	return it->second;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw UnknownAccessorInstanceException 
	\return reference to an accessor instance 
	\param rName name of the accessor instance */
///////////////////////////////////////////////////////////////////////////////////////////////////////
AccessorInstanceRef ManagerImpl::getAccessorInstance(const AccessorName& rName)
{
	CollAccessorInstanceRef::iterator It = m_AccessorInstanceList.find(rName);

	if (It == m_AccessorInstanceList.end())
	{
		VarString msg;
		msg.format("The accessor instance name <%s> is unknown in the accessor instance list.", rName.c_str());
		ExceptInfo sInfo( "basar.db.aspect.ManagerImpl.accessorInstance", msg, __FILE__, __LINE__ );
		throw UnknownAccessorInstanceException(sInfo);
	}

	return It->second;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return list of accessor definitions */
///////////////////////////////////////////////////////////////////////////////////////////////////////
const AccessorDefinitionListRef ManagerImpl::getAccessorDefinitionList()
{
	return AccessorDefinitionListRef(getThis());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return list of accessor instances */
///////////////////////////////////////////////////////////////////////////////////////////////////////
const AccessorInstanceListRef ManagerImpl::getAccessorInstanceList()
{
	return AccessorInstanceListRef(getThis());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw UnknownAccessorDefinitionException 
	\param rName name of the accessor definition  */
///////////////////////////////////////////////////////////////////////////////////////////////////////
void ManagerImpl::releaseAccessorDefinition(const AccessorName& rName ) 
{
	if (0 == m_AccessorDefinitionList.erase(rName))
	{
		VarString msg;
		msg.format("The accessor definition name <%s> is unknown in the accessor definition list.", rName.c_str());
		ExceptInfo sInfo( "basar.db.aspect.ManagerImpl.releaseAccessorDefinition", msg, __FILE__, __LINE__ );
		throw UnknownAccessorDefinitionException(sInfo);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\param rName name of the accessor definition
	\return bool indicating if accessor definition is known by manager  */
///////////////////////////////////////////////////////////////////////////////////////////////////////

bool ManagerImpl::hasAccessorDefinition(const AccessorName& rName) const
{
    CollAccessorDefinitionRef::const_iterator It = m_AccessorDefinitionList.find(rName);
    
    return (m_AccessorDefinitionList.end() != It);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\param rName name of the accessor instance
	\return bool indicating if accessor instance is stored in manager's instance list  */
///////////////////////////////////////////////////////////////////////////////////////////////////////

bool ManagerImpl::hasAccessorInstance(const AccessorName& rName) const
{
    CollAccessorInstanceRef::const_iterator It = m_AccessorInstanceList.find(rName);
    
    return (m_AccessorInstanceList.end() != It);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw UnknownAccessorDefinitionException 
	\param rName name of the accessor instance */
///////////////////////////////////////////////////////////////////////////////////////////////////////
void ManagerImpl::releaseAccessorInstance(const AccessorName& rName) 
{
	if ( 0 == m_AccessorInstanceList.erase(rName) )
	{
		VarString msg;
		msg.format("The accessor instance name <%s> is unknown in the accessor instance list.", rName.c_str());
		ExceptInfo sInfo( "basar.db.aspect.ManagerImpl.releaseAccessorInstance", msg.c_str(),
						  __FILE__, __LINE__ );
		throw UnknownAccessorInstanceException(sInfo);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw
	\return iterator to the first element of the accessor instance list */
///////////////////////////////////////////////////////////////////////////////////////////////////////
CollAccessorInstanceRefIt ManagerImpl::beginInstance()
{
	return m_AccessorInstanceList.begin();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw
	\return iterator to the last element of the accessor instance list */
///////////////////////////////////////////////////////////////////////////////////////////////////////
CollAccessorInstanceRefIt ManagerImpl::endInstance()
{
	return m_AccessorInstanceList.end();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw
	\return the size of the accessor instance list */
//////////////////////////////////////////////////////////////////////////////////////////////////////
AccessorListSize ManagerImpl::sizeInstance() const
{
	return m_AccessorInstanceList.size();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw
	\retval true instance list is empty 
	\return otherwise returns false */
//////////////////////////////////////////////////////////////////////////////////////////////////////
bool ManagerImpl::emptyInstance() const
{
	return m_AccessorInstanceList.empty();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw
	\return iterator to the first element of the accessor definition list */
///////////////////////////////////////////////////////////////////////////////////////////////////////
CollAccessorDefinitionRefIt ManagerImpl::beginDefinition()
{
	return m_AccessorDefinitionList.begin();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw
	\return iterator to the last element of the accessor definition list */
///////////////////////////////////////////////////////////////////////////////////////////////////////
CollAccessorDefinitionRefIt ManagerImpl::endDefinition()
{
	return m_AccessorDefinitionList.end();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw
	\return the size of the accessor definition list */
////////////////////////////////////////////////////////////////////////////////////////////////////////
AccessorListSize ManagerImpl::sizeDefinition() const
{
	return m_AccessorDefinitionList.size();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw
	\retval true if definition list is empty 
	\return otherwise returns false */
////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ManagerImpl::emptyDefinition() const
{
	return m_AccessorDefinitionList.empty();
}
///////////////////////////////////////////////////////////////////////////////////////////////
/*! \retval	true if the connection the passed iterator is pointing to is open
	\return otherwise returns false
	\param	it iterator to a conection
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////
bool ManagerImpl::isOpen( const sql::ConnectionList_Iterator it ) const
{
	return sql::Manager::getInstance().isOpen( it );
}
///////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	iterator pointing to the first connection element
 	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////
const sql::ConnectionList_Iterator	ManagerImpl::beginConn() const
{
	return sql::Manager::getInstance().begin();
}
///////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	iterator pointing to one position after the last connection element
 	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////
const sql::ConnectionList_Iterator ManagerImpl::endConn() const
{
	return sql::Manager::getInstance().end();
}
///////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	size of the Connection List
 	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////
sql::ConnectionListSize ManagerImpl::sizeConn() const
{
	return sql::Manager::getInstance().size();
}
///////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	number of open connections
 	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////
/*! \brief	The method returns the number of all open connections in CollConnection. \n no-throw */
sql::ConnectionListSize ManagerImpl::sizeOpenConn() const
{
	return sql::Manager::getInstance().sizeOpen();
}
///////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	the database info to given iterator and sets connId to connections unique id 
	\param	it iterator of CollConnection
	\param	connId reference to VarString, set to application's unique connection id
 	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////
const ConnectionRef::DatabaseInfo & ManagerImpl::getConnInfo( const sql::ConnectionList_Iterator it ,
															 VarString                         & connId ) const
{
	return sql::Manager::getInstance().getConnInfo( it, connId );
}

//---------------------------------------------------------------------------------------------------//
} // aspect
} // db
} // basar
