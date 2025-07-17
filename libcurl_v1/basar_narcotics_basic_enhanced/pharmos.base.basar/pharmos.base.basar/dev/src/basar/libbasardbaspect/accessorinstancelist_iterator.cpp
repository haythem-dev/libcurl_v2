//----------------------------------------------------------------------------
/*! \file
 *  \brief  iterator of accessor instance list
 *  \author Roland Kiefert
 *  \date   24.08.2005
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
	\param Manager a reference to the father class Manager 
	\param It iterator to its map */
AccessorInstanceList_Iterator::AccessorInstanceList_Iterator(	const boost::shared_ptr<ManagerImpl> Manager, 
																const std::map<AccessorName,AccessorInstanceRef>::iterator It )
{
	m_It = It;
	m_Manager = Manager;
}

//--------------------------------------------------------------------------------------------------
/*!	\throw no-throw */
AccessorInstanceList_Iterator::AccessorInstanceList_Iterator ()
{
}

////////////////////////////////////////////////////////////////////////////////
/*! \retval	true if the inner map contains at least one item and the iterator 
			is pointing to the first.
	\return	otherwise returns false
	\throw	InvalidIteratorException */
////////////////////////////////////////////////////////////////////////////////
bool AccessorInstanceList_Iterator::isBegin() const
{
	checkValid();
	return (!m_Manager->emptyInstance()) && (m_Manager->beginInstance() == m_It);
}

////////////////////////////////////////////////////////////////////////////////
/*! \retval	true if the inner map contains at least one item and the iterator 
			is pointing to the position one beyond the last item.
	\return	otherwise returns false
	\throw	InvalidIteratorException */
////////////////////////////////////////////////////////////////////////////////
bool AccessorInstanceList_Iterator::isEnd() const
{
	checkValid();
	return (!m_Manager->emptyInstance()) && (m_Manager->endInstance() == m_It);
}

//--------------------------------------------------------------------------------------------------
/*!	\throw DifferentContainersIteratorException 
	\retval true if the iterator is equal to rhp 
	\return otherwise returns false
	\param rhp iterator on list to compare with m_It / m_Manager */
bool AccessorInstanceList_Iterator::operator == (const AccessorInstanceList_Iterator& rhp) const
{
	checkValid();
	if (rhp.m_Manager != m_Manager)
	{
		ExceptInfo sInfo("basar.db.aspect.AccessorInstanceList_Iterator.operator==", "Iterator from different Container.", __FILE__, __LINE__);
		throw DifferentContainersIteratorException(sInfo);
	}
	return (this->m_It == rhp.m_It);
}

//--------------------------------------------------------------------------------------------------
/*!	\throw DifferentContainersIteratorException 
	\retval true if the iterator isn't equal to rhp 
	\return otherwise returns false
	\param rhp iterator on list to compare with m_It / m_Manager */
bool AccessorInstanceList_Iterator::operator != (const AccessorInstanceList_Iterator& rhp) const
{
	if (rhp.m_Manager != m_Manager)
	{
		ExceptInfo sInfo("basar.db.aspect.AccessorInstanceList_Iterator.operator!=", "Iterator from different Container.", __FILE__, __LINE__);
		throw DifferentContainersIteratorException(sInfo);
	}
	return (!operator==(rhp));
}

//--------------------------------------------------------------------------------------------------
/*!	\throw OutOfRangeIteratorException 
	\return the next iterator */
AccessorInstanceList_Iterator AccessorInstanceList_Iterator::operator ++ ()
{
	checkValid();
	if (m_It == m_Manager->endInstance())
	{
		ExceptInfo sInfo("basar.db.aspect.AccessorInstanceList_Iterator.operator++", "Iterator of accessor instance list is at the end", __FILE__, __LINE__);
		throw OutOfRangeIteratorException(sInfo);
	}
	++m_It;
	return *this;
}

//--------------------------------------------------------------------------------------------------
/*!	\throw OutOfRangeIteratorException 
	\return the current iterator */
AccessorInstanceList_Iterator AccessorInstanceList_Iterator::operator ++ (int)
{
	checkValid();
	if (m_It == m_Manager->endInstance())
	{
		ExceptInfo sInfo("basar.db.aspect.AccessorInstanceList_Iterator.operator++(int)", "Iterator of accessor instance list is at the end", __FILE__, __LINE__);
		throw OutOfRangeIteratorException(sInfo);
	}
	AccessorInstanceList_Iterator temp(*this);
	++m_It;
	return temp;
}

//--------------------------------------------------------------------------------------------------
/*!	\throw OutOfRangeIteratorException 
	\return accessor instance reference of the current iterator */
AccessorInstanceRef AccessorInstanceList_Iterator::operator*()
{
	checkValid();
	if (m_It == m_Manager->endInstance())
	{
		ExceptInfo sInfo("basar.db.aspect.AccessorInstanceList_Iterator.operator*", "Iterator of accessor instance list is at the end", __FILE__, __LINE__);
		throw OutOfRangeIteratorException(sInfo);
	}
	return m_It->second;
}

//--------------------------------------------------------------------------------------------------
/*!	\throw InvalidIteratorException */
void AccessorInstanceList_Iterator::checkValid() const
{
	if( m_Manager.get() == 0 )
	{
		ExceptInfo sInfo("basar.db.aspect.AccessMethodList_Iterator.checkValid", "Iterator of accessor method isn't valid.", __FILE__, __LINE__);
		throw InvalidIteratorException(sInfo);
	}
}

//--------------------------------------------------------------------------------------------------
bool AccessorInstanceList_Iterator::isNull() const
{
	return ( m_Manager.get() == 0 );
}

//--------------------------------------------------------------------------------------------------
void  AccessorInstanceList_Iterator::reset()
{
 m_Manager.reset();
}

//--------------------------------------------------------------------------------------------------
} // aspect
} // db
} // basar
