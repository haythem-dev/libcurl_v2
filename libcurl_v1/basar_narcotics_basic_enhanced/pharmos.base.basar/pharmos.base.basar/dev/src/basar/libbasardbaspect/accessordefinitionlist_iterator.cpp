//----------------------------------------------------------------------------
/*! \file
 *  \brief  iterator of accessor definition list
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
	\param Manager a reference to the father class Manager 
	\param It iterator to its map */
AccessorDefinitionList_Iterator::AccessorDefinitionList_Iterator (const boost::shared_ptr<ManagerImpl> Manager, 
																  const std::map<AccessorName,AccessorDefinitionRef>::iterator It)
: m_It(It), m_Manager(Manager)
{
}

//--------------------------------------------------------------------------------------------------
/*!	\throw no-throw */
AccessorDefinitionList_Iterator::AccessorDefinitionList_Iterator ()
{
}

////////////////////////////////////////////////////////////////////////////////
/*! \retval	true if the inner map contains at least one item and the iterator 
			is pointing to the first.
	\return	otherwise returns false
	\throw	InvalidIteratorException */
////////////////////////////////////////////////////////////////////////////////
bool AccessorDefinitionList_Iterator::isBegin() const
{
	checkValid();
	return (!m_Manager->emptyDefinition()) && (m_Manager->beginDefinition() == m_It);
}

////////////////////////////////////////////////////////////////////////////////
/*! \retval	true if the inner map contains at least one item and the iterator 
			is pointing to the position one beyond the last item.
	\return	otherwise returns false
	\throw	InvalidIteratorException */
////////////////////////////////////////////////////////////////////////////////
bool AccessorDefinitionList_Iterator::isEnd() const
{
	checkValid();
	return (!m_Manager->emptyDefinition()) && (m_Manager->endDefinition() == m_It);
}

//--------------------------------------------------------------------------------------------------
/*!	\throw DifferentContainersIteratorException 
	\retval true if the iterator is equal to rhp 
	\return otherwise returns false
	\param rhp iterator on list to compare with m_It / m_Manager */
bool AccessorDefinitionList_Iterator::operator == (const AccessorDefinitionList_Iterator& rhp) const
{
	checkValid();
	if (rhp.m_Manager != m_Manager)
	{
		ExceptInfo sInfo("basar.db.aspect.AccessorDefinitionList_Iterator.operator==", "Iterator from different Container.", __FILE__, __LINE__);
		throw DifferentContainersIteratorException(sInfo);
	}
	return (this->m_It == rhp.m_It);
}

//--------------------------------------------------------------------------------------------------
/*!	\throw DifferentContainersIteratorException 
	\retval true if the iterator isn't equal to rhp 
	\return otherwise returns false
	\param rhp iterator on list to compare with m_It / m_Manager */
bool AccessorDefinitionList_Iterator::operator != (const AccessorDefinitionList_Iterator& rhp) const
{
	if (rhp.m_Manager != m_Manager)
	{
		ExceptInfo sInfo("basar.db.aspect.AccessorDefinitionList_Iterator.operator!=", "Iterator from different Container.", __FILE__, __LINE__);
		throw DifferentContainersIteratorException(sInfo);
	}
	return (!operator==(rhp));
}

//--------------------------------------------------------------------------------------------------
/*!	\throw OutOfRangeIteratorException 
	\return the next iterator */
AccessorDefinitionList_Iterator AccessorDefinitionList_Iterator::operator ++ ()
{
	checkValid();
	if (m_It == m_Manager->endDefinition())
	{
		ExceptInfo sInfo("basar.db.aspect.AccessorDefinitionList_Iterator.operator++", "Iterator of accessor definition list is at the end", __FILE__, __LINE__);
		throw OutOfRangeIteratorException(sInfo);
	}
	++m_It;
	return *this;
}

//--------------------------------------------------------------------------------------------------
/*!	\throw OutOfRangeIteratorException 
	\return the current iterator */
AccessorDefinitionList_Iterator AccessorDefinitionList_Iterator::operator ++ (int)
{
	checkValid();
	if (m_It == m_Manager->endDefinition())
	{
		ExceptInfo sInfo("basar.db.aspect.AccessorDefinitionList_Iterator.operator++(int)", "Iterator of accessor definition list is at the end", __FILE__, __LINE__);
		throw OutOfRangeIteratorException(sInfo);
	}
	AccessorDefinitionList_Iterator temp(*this);
	++m_It;
	return temp;
}

//--------------------------------------------------------------------------------------------------
/*!	\throw OutOfRangeIteratorException 
	\return accessor definition reference to the current iterator */
AccessorDefinitionRef AccessorDefinitionList_Iterator::operator*()
{
	checkValid();
	if (m_It == m_Manager->endDefinition())
	{
		ExceptInfo sInfo("basar.db.aspect.AccessorDefinitionList_Iterator.operator*", "Iterator of accessor definition list is at the end", __FILE__, __LINE__);
		throw OutOfRangeIteratorException(sInfo);
	}
	return m_It->second;
}

//--------------------------------------------------------------------------------------------------
/*!	\throw InvalidIteratorException */
void AccessorDefinitionList_Iterator::checkValid() const
{
	if( m_Manager.get() == 0 )
	{
		ExceptInfo sInfo("basar.db.aspect.AccessMethodList_Iterator.checkValid", "Iterator of accessor method isn't valid.", __FILE__, __LINE__);
		throw InvalidIteratorException(sInfo);
	}
}

//--------------------------------------------------------------------------------------------------
bool AccessorDefinitionList_Iterator::isNull() const
{
	return ( m_Manager.get() == 0 );
}

//--------------------------------------------------------------------------------------------------
void  AccessorDefinitionList_Iterator::reset()
{
	m_Manager.reset();
}

//--------------------------------------------------------------------------------------------------
} // aspect
} // db
} // basar
