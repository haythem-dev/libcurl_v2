//----------------------------------------------------------------------------
/*! \file
 *  \brief  iterator of accessor method list
 *  \author Roland Kiefert
 *  \date   24.08.2005
 */
//----------------------------------------------------------------------------

#include "libbasardbaspect_accessmethod.h"
#include "accessor.h"

//--------------------------------------------------------------------------------------------------
namespace basar  {
namespace db     {
namespace aspect {

//--------------------------------------------------------------------------------------------------
/*!	\throw no-throw */
AccessMethodList_Iterator::AccessMethodList_Iterator ()
{
}

//--------------------------------------------------------------------------------------------------
/*!	\throw no-throw */
AccessMethodList_Iterator::AccessMethodList_Iterator( const IAccessorSharedPtr               acc, 
													  const CollAccessorMethodRef::iterator itMethod)
{
	m_It       = itMethod;
	m_Accessor = acc;
}

////////////////////////////////////////////////////////////////////////////////
/*! \retval	true if the inner map contains at least one item and the iterator 
			is pointing to the first.
	\return	otherwise returns false
	\throw	InvalidIteratorException */
////////////////////////////////////////////////////////////////////////////////
bool AccessMethodList_Iterator::isBegin() const
{
	checkValid();
	return (m_Accessor->begin() == m_It);
}
////////////////////////////////////////////////////////////////////////////////
/*! \retval	true if the inner map contains at least one item and the iterator 
			is pointing to the position one beyond the last item.
	\return	otherwise returns false
	\throw	InvalidIteratorException */
////////////////////////////////////////////////////////////////////////////////
bool AccessMethodList_Iterator::isEnd() const
{
	checkValid();
	return (!m_Accessor->empty()) && (m_Accessor->end() == m_It);
}

//--------------------------------------------------------------------------------------------------
/*!	\throw DifferentContainersIteratorException 
	\retval true if the iterator is equal to rhp  
	\return otherwise returns false
	\param rhp iterator on list to compare with m_It / m_Accessor */
bool AccessMethodList_Iterator::operator == (const AccessMethodList_Iterator& rhp) const
{
	checkValid();
	if (rhp.m_Accessor != m_Accessor)
	{
		ExceptInfo sInfo("basar.db.aspect.AccessMethodList_Iterator.operator==", "Iterator from different Container.", __FILE__, __LINE__);
		throw DifferentContainersIteratorException(sInfo);
	}
	return (this->m_It == rhp.m_It);
}

//--------------------------------------------------------------------------------------------------
/*!	\throw DifferentContainersIteratorException 
	\retval true if the iterator isn't equal to rhp 
	\return otherwise returns false
	\param rhp iterator on list to compare with m_It / m_Accessor */
bool AccessMethodList_Iterator::operator != (const AccessMethodList_Iterator& rhp) const
{
	if (rhp.m_Accessor != m_Accessor)
	{
		ExceptInfo sInfo("basar.db.aspect.AccessMethodList_Iterator.operator!=", "Iterator from different Container.", __FILE__, __LINE__);
		throw DifferentContainersIteratorException(sInfo);
	}
	return (!operator==(rhp));
}

//--------------------------------------------------------------------------------------------------
/*!	\throw OutOfRangeIteratorException 
	\return the next iterator */
AccessMethodList_Iterator AccessMethodList_Iterator::operator ++ ()
{
	checkValid();
	if (m_It == m_Accessor->end())
	{
		ExceptInfo sInfo("basar.db.aspect.AccessMethodList_Iterator.operator++", "Iterator of accessor method list is at the end.", __FILE__, __LINE__);
		throw OutOfRangeIteratorException(sInfo);
	}
	++m_It;
	return *this;
}

//--------------------------------------------------------------------------------------------------
/*!	\throw OutOfRangeIteratorException 
	\return the current iterator */
AccessMethodList_Iterator AccessMethodList_Iterator::operator ++ (int)
{
	checkValid();
	if (m_It == m_Accessor->end())
	{
		ExceptInfo sInfo("basar.db.aspect.AccessMethodList_Iterator.operator++(int)", "Iterator of accessor method list is at the end.", __FILE__, __LINE__);
		throw OutOfRangeIteratorException(sInfo);
	}
	AccessMethodList_Iterator temp(*this);
	++m_It;
	return temp;
}

//--------------------------------------------------------------------------------------------------
/*!	\throw OutOfRangeIteratorException 
	\return accessmethod reference to the current iterator */
AccessMethodRef AccessMethodList_Iterator::operator*()
{
	checkValid();
	if (m_It == m_Accessor->end())
	{
		ExceptInfo sInfo("basar.db.aspect.AccessMethodList_Iterator.operator*", "Iterator of accessor method list is at the end.", __FILE__, __LINE__);
		throw OutOfRangeIteratorException(sInfo);
	}
	return m_It->second;
}

//--------------------------------------------------------------------------------------------------
/*!	\throw InvalidIteratorException */
void AccessMethodList_Iterator::checkValid() const
{
	if( m_Accessor.get() == 0 )
	{
		ExceptInfo sInfo("basar.db.aspect.AccessMethodList_Iterator.checkValid", "Iterator of accessor method isn't valid.", __FILE__, __LINE__);
		throw InvalidIteratorException(sInfo);
	}
}

//--------------------------------------------------------------------------------------------------
} // aspect
} // db
} // basar
