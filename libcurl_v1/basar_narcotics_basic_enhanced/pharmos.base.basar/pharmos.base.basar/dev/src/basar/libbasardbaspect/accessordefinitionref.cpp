//-------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  all functions implementation of AccessorDefinitionRef
 *  \author Roland Kiefert
 *  \date   18.07.2005
 */
//-------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------//

#include "libbasardbaspect_accessor.h"

#include "accessor.h"
#include "libbasardbaspect_accessmethod.h"

//-------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------//
namespace basar {
namespace db {
namespace aspect {

////////////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw  NullReferenceException
//!	\return accessor instance reference 
//!	\param  connRef Reference to Connection for the accessor instance
//!	\param  eCachePolicy caching policy, default ON_DEMAND_CACHING
////////////////////////////////////////////////////////////////////////////////////////////////////
const AccessorInstanceRef AccessorDefinitionRef::createInstance(	const ConnectionRef connRef, 
															const CachingPolicyEnum eCachePolicy /*= ON_DEMAND_CACHING */ ) const
{
	return AccessorInstanceRef( getImpl()->createInstance(connRef, eCachePolicy) );
}
/*!	\throw  NullReferenceException
	\return Reference to the property description list */
basar::property::PropertyDescriptionListRef AccessorDefinitionRef::getPropertyDescriptionList() 
{
	return getImpl()->getPropertyDescriptionList();
}

/*!	\throw ExistAccessorMethodException, UnknownAccessMethodException, NullReferenceException
	\return Reference to an AccessMethod 
	\param rName name of an accessmethod */
AccessMethodRef AccessorDefinitionRef::createAccessMethod( const AccessMethodName& rName )
{
	return getImpl()->createAccessMethod(rName); 
}

/*!	\throw UnknownAccessMethodException, NullReferenceException
	\return Reference to an AccessMethod 
	\param rName name of an accessmethod */
AccessMethodRef AccessorDefinitionRef::getAccessMethod(const AccessMethodName& rName) 
{
	return getImpl()->getAccessMethod(rName); 
}

/*!	\throw  NullReferenceException
	\return Reference to the AccessMethod list */
const AccessMethodListRef AccessorDefinitionRef::getAccessMethodList() const
{
	return getImpl()->getAccessMethodList(); 
}

/*! \throw  no-throw
	\return name of the accessor */
const VarString & AccessorDefinitionRef::getAccessorName() const
{
	return getImpl()->getAccessorName();
}

/*! \throw  no-throw
	\param name name of the accessor */
void AccessorDefinitionRef::setAccessorName( const VarString name )
{
	getImpl()->setAccessorName(name);
}

/*!	\throw  NullReferenceException
	\return pointer to Accessor */
const boost::shared_ptr<IAccessor>& AccessorDefinitionRef::getImpl() const
{
	if (m_pAccessor.get() == 0)
	{
		ExceptInfo sInfo("basar.db.aspect.AccessorDefinitionRef.getImpl", "Reference of Accessor in the accessor definition ref - no pointer to impl-class", __FILE__, __LINE__);
		throw NullReferenceException(sInfo);
	}
	return m_pAccessor;
}

/*!	\throw  no-throw
	\return pointer to Reference of Accessor Definition 
	\param pAccessor a shared pointer to its implementation class */
AccessorDefinitionRef::AccessorDefinitionRef(boost::shared_ptr<IAccessor> pAccessor) : m_pAccessor(pAccessor)
{
}

/*!	\throw no-throw */
AccessorDefinitionRef::AccessorDefinitionRef()
{
}

/*!	\throw no-throw
	\retval true  if shared pointer to implementation object is null 
	\return otherwise returns false */
bool AccessorDefinitionRef::isNull() const
{
	return (0 == m_pAccessor.get());
}

/*!	\throw no-throw */
void AccessorDefinitionRef::reset()
{
	m_pAccessor.reset();
}

} // aspect
} // db
} // basar

