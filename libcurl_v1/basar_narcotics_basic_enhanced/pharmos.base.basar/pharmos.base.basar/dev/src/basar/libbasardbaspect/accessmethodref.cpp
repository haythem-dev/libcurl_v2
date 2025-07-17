//----------------------------------------------------------------------------
/*! \file
 *  \brief  reference to all functions of Access method 
 *  \author Roland Kiefert
 *  \date   15.07.2005
 */
//----------------------------------------------------------------------------

#include "libbasardbaspect_accessmethod.h"

#include "accessmethod.h"
#include "libbasardbaspect_sqlstringbuilder.h"

//--------------------------------------------------------------------------------------------------
namespace basar {
namespace db {
namespace aspect {


//--------------------------------------------------------------------------------------------------
/*!	\throw OutOfRangeIteratorException
	\return Reference to a SQLStringBuilder 
	\param rBuilder a reference to an existing SQLStringBuilder */
SQLStringBuilderIndex AccessMethodRef::push_back(SQLStringBuilder& rBuilder)
{
	return getImpl()->push_back(rBuilder);
}
	
//--------------------------------------------------------------------------------------------------
/*!	\throw OutOfRangeIteratorException, NullReferenceException 
	\return Reference to a SQLStringBuilder 
	\param index index starting with 0 */
SQLStringBuilderRef AccessMethodRef::getBuilder(SQLStringBuilderListSize index)
{
	return getImpl()->getBuilder(index);
}

//--------------------------------------------------------------------------------------------------
SQLStringBuilderRef AccessMethodRef::getBuilder(SQLStringBuilderListSize index) const
{
	return getImpl()->getBuilder(index);
}

//--------------------------------------------------------------------------------------------------
/*!	\throw  NullReferenceException
	\return Reference to the SQLStringBuilder list */
SQLStringBuilderListRef AccessMethodRef::getBuilderList()
{
	return getImpl()->getBuilderList();
}

//--------------------------------------------------------------------------------------------------
/*!	\throw  no-throw
	\return Reference to an AccessMethod */
AccessMethodRef AccessMethodRef::createInstance()
{
	return AccessMethodRef(getImpl()->createInstance());
}

//--------------------------------------------------------------------------------------------------
/*! \throw  no-throw
	\return name of the accessor method*/
const VarString & AccessMethodRef::getAccessorMethodName()
{
	return getImpl()->getAccessorMethodName();
}

//--------------------------------------------------------------------------------------------------
/*! \throw  no-throw
	\param name name of the accessor method*/
void AccessMethodRef::setAccessorMethodName(VarString name)
{
	getImpl()->setAccessorMethodName(name);
}

//--------------------------------------------------------------------------------------------------
/*!	\throw  NullReferenceException
	\return pointer to AccessMethod */
const boost::shared_ptr<AccessMethod>& AccessMethodRef::getImpl() const
{
	if (m_pMethod.get() == 0)
	{
		ExceptInfo sInfo( "basar.db.aspect.AccessMethodRef.getImpl", 
						  "Reference of AccessMethod - no pointer to impl-class", 
						  __FILE__, __LINE__);
		throw NullReferenceException(sInfo);
	}
	return m_pMethod;
}

//--------------------------------------------------------------------------------------------------
/*!	\throw  no-throw
	\return pointer to Reference of AccessMethod 
	\param pMethod a shared pointer to its implementation class */
AccessMethodRef::AccessMethodRef(boost::shared_ptr<AccessMethod> pMethod) : m_pMethod(pMethod)
{
}

//--------------------------------------------------------------------------------------------------
/*!	\throw no-throw */
AccessMethodRef::AccessMethodRef()
{
}

//--------------------------------------------------------------------------------------------------
/*!	\throw no-throw
	\retval true  if shared pointer to implementation object is null
	\return otherwise returns false */
bool AccessMethodRef::isNull() const
{
	return (0 == m_pMethod.get());
}

//--------------------------------------------------------------------------------------------------
/*!	\throw no-throw */
void AccessMethodRef::reset()
{
	m_pMethod.reset();
}

//--------------------------------------------------------------------------------------------------
/*!	\throw  no-throw
	\return the size of the list */
SQLStringBuilderListSize AccessMethodRef::size() const
{
	return getImpl()->size();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw
	\retval true if there are no elements in the SQLStringBuilderList 
	\return otherwise returns false */
/////////////////////////////////////////////////////////////////////////////////////////////////
bool AccessMethodRef::empty() const
{
	return getImpl()->empty();
}

//--------------------------------------------------------------------------------------------------
} // aspect
} // db
} // basar
