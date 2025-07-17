//----------------------------------------------------------------------------
/*! \file
 *  \brief  implementation of Access method 
 *  \author Roland Kiefert
 *  \date   15.07.2005
 */
//----------------------------------------------------------------------------

#include "accessmethod.h"
#include "libbasardbaspect_sqlstringbuilder.h"
#include "loggerpool.h"

//--------------------------------------------------------------------------------------------------
namespace basar  {
namespace db     {
namespace aspect {

//--------------------------------------------------------------------------------------------------
/*!	\throw OutOfRangeIndexException 
	\return Reference to the SQLStringBuilder 
	\param index index starting with 0 */
SQLStringBuilderRef AccessMethod::getBuilder( const SQLStringBuilderListSize index ) 
{
	if (index >= m_BuilderList.size())
	{
		basar::VarString msg;
		msg.format("The index <%d> does not exist / is greater than size of builder list.", index);
		ExceptInfo sInfo("basar.db.aspect.AccessMethod.builder", msg.c_str(), __FILE__, __LINE__);
		throw OutOfRangeIndexException(sInfo);
	}
	std::list<SQLStringBuilderRef>::iterator It = m_BuilderList.begin();
	for (SQLStringBuilderListSize i=0; i<index; ++i)
	{
		++It;
	}
	return *It;
}

//--------------------------------------------------------------------------------------------------
/*!	\throw OutOfRangeIndexException 
	\return Reference to the SQLStringBuilder 
	\param index index starting with 0 */
SQLStringBuilderRef AccessMethod::getBuilder( const SQLStringBuilderListSize index ) const
{
	if (index >= m_BuilderList.size())
	{
		basar::VarString msg;
		msg.format("The index <%d> does not exist / is greater than size of builder list.", index);
		ExceptInfo sInfo("basar.db.aspect.AccessMethod.builder", msg.c_str(), __FILE__, __LINE__);
		throw OutOfRangeIndexException(sInfo);
	}
	std::list<SQLStringBuilderRef>::const_iterator It = m_BuilderList.begin();
	for (SQLStringBuilderListSize i=0; i<index; ++i)
	{
		++It;
	}
	return *It;
}

//--------------------------------------------------------------------------------------------------
/*!	\throw no-throw
	\return Index of the SQLStringBuilder 
	\param rBuilder a reference to an existing SQLStringBuilder */
SQLStringBuilderIndex AccessMethod::push_back( const SQLStringBuilder& rBuilder )
{
	m_BuilderList.push_back( rBuilder.create() );
	return m_BuilderList.size() - 1 ;
}

//--------------------------------------------------------------------------------------------------
/*!	\throw  no-throw
	\return Reference to the SQLStringBuilder list */
const SQLStringBuilderListRef AccessMethod::getBuilderList()  
{
	return  SQLStringBuilderListRef(getThis());
}

//--------------------------------------------------------------------------------------------------
/*!	\throw  no-throw
	\return iterator to the first element of the list */
std::list<SQLStringBuilderRef>::iterator AccessMethod::begin()
{
	return m_BuilderList.begin();
}

//--------------------------------------------------------------------------------------------------
/*!	\throw  no-throw
	\return iterator to the last element of the list */
std::list<SQLStringBuilderRef>::iterator AccessMethod::end()
{
	return m_BuilderList.end();
}

//--------------------------------------------------------------------------------------------------
/*!	\throw  no-throw
	\return the size of the list */
SQLStringBuilderListSize AccessMethod::size() const
{
	return m_BuilderList.size();
}
/////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw
	\retval true if there are no elements in the SQLStringBuilderList 
	\return otherwise returns false */
/////////////////////////////////////////////////////////////////////////////////////////////////
bool AccessMethod::empty() const
{
	return m_BuilderList.empty();
}

//--------------------------------------------------------------------------------------------------
/*!	\throw  no-throw
	\retval true if AccessMethod is a definition
	\return otherwise returns false */	
bool AccessMethod::isDefinition() const
{
	return m_Definition;
}

//--------------------------------------------------------------------------------------------------
/*!	\throw  no-throw
	\return an accessor instance reference */
const AccessMethod::AccessMethodSharedPtr AccessMethod::createInstance() const
{
	AccessMethodSharedPtr t = AccessMethodSharedPtr( AccessMethod::create() );
	
	BLOG_TRACE( LoggerPool::accessMethod(), 
				"createInstance() - New Accessor method is created" );

	std::list<SQLStringBuilderRef>::iterator It;
	t->m_BuilderList.assign(m_BuilderList.begin(),m_BuilderList.end());
	
	BLOG_TRACE( LoggerPool::accessMethod(),
				"createInstance() - copying of all stringbuilders is done.");

	for( It = t->m_BuilderList.begin(); It != t->m_BuilderList.end(); ++It ){
			*It = SQLStringBuilderRef(It->create());
	}
	
	BLOG_TRACE( LoggerPool::accessMethod(), 
				"createInstance() - each stringbuilder created itself" );
	
	t->m_Definition = false;
	return t;
}

//--------------------------------------------------------------------------------------------------
/*!	\throw  no-throw
	\return pointer to the AccessMethod itself */
AccessMethod::AccessMethodSharedPtr AccessMethod::create()
{
	AccessMethodSharedPtr t = AccessMethodSharedPtr(new AccessMethod);
	t->setWeakPtr(t);
	t->m_Definition = true;
	return t;
}

//--------------------------------------------------------------------------------------------------
/*! \return (only one group of) AccessMethod shared pointer 
	\throw  no-throw */
AccessMethod::AccessMethodSharedPtr AccessMethod::getThis()
{
	return AccessMethodSharedPtr(m_WeakPtr);
}

//--------------------------------------------------------------------------------------------------
/*! \throw  no-throw
	\param weakPtr weak pointer to the class AccessMethod */
void AccessMethod::setWeakPtr(AccessMethodWeakPtr weakPtr)
{
	m_WeakPtr = weakPtr;
}

//--------------------------------------------------------------------------------------------------
/*! \throw  no-throw
	\param name name of the accessor method*/
void AccessMethod::setAccessorMethodName(const VarString& name )
{
	m_AccessorMethodName = name;
}

//--------------------------------------------------------------------------------------------------
/*! \throw  no-throw
	\return name of the accessor method as VarString */
const VarString& AccessMethod::getAccessorMethodName() const
{
	return m_AccessorMethodName;
}

} // aspect
} // db
} // basar
