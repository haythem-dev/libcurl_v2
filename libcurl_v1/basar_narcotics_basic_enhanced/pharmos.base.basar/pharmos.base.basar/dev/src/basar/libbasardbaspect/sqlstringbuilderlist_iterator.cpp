//----------------------------------------------------------------------------
/*! \file
 *  \brief  Iterator class of build SQL - statements list
 *  \author Roland Kiefert
 *  \date   24.08.2005
 */
//----------------------------------------------------------------------------

#include "libbasardbaspect_sqlstringbuilder.h"
#include "accessmethod.h"

/*--------------------------------------------------------------------------------------------------*/
namespace basar {
namespace db {
namespace aspect {

/*--------------------------------------------------------------------------------------------------*/
/*!	\throw no-throw */
SQLStringBuilderList_Iterator::SQLStringBuilderList_Iterator()
{

}

/*--------------------------------------------------------------------------------------------------*/
/*!	\throw no-throw 
	\param method reference to the father class AccessMethod 
	\param it iterator to list */
SQLStringBuilderList_Iterator::SQLStringBuilderList_Iterator (boost::shared_ptr<AccessMethod> method, std::list<SQLStringBuilderRef>::iterator it)
: m_It(it), m_Method(method)
{
}

////////////////////////////////////////////////////////////////////////////////
/*! \retval	true if the inner map contains at least one item and the iterator 
			is pointing to the first.
	\return	otherwise returns false
	\throw	InvalidIteratorException */
////////////////////////////////////////////////////////////////////////////////
bool SQLStringBuilderList_Iterator::isBegin() const
{
	checkValid();
	return (m_Method->begin() == m_It);
}
////////////////////////////////////////////////////////////////////////////////
/*! \retval	true if the inner map contains at least one item and the iterator 
			is pointing to the position one beyond the last item.
	\return	otherwise returns false
	\throw	InvalidIteratorException */
////////////////////////////////////////////////////////////////////////////////
bool SQLStringBuilderList_Iterator::isEnd() const
{
	checkValid();
	return (!m_Method->empty()) && (m_Method->end() == m_It);
}
////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\retval true if the iterator is equal to rhp 
	\return otherwise returns false
	\param rhp iterator on list to compare with m_It / m_Method */
////////////////////////////////////////////////////////////////////////////////
bool SQLStringBuilderList_Iterator::operator == (const SQLStringBuilderList_Iterator& rhp) const
{
	checkValid();
	if (rhp.m_Method != m_Method)
	{
		ExceptInfo sInfo("basar.db.aspect.SQLStringBuilderList_Iterator.operator==", "Iterator from different Container.", __FILE__, __LINE__);
		throw DifferentContainersIteratorException(sInfo);
	}
	return (this->m_It == rhp.m_It);
}

/*--------------------------------------------------------------------------------------------------*/
/*!	\throw no-throw 
	\retval true if the iterator isn't equal to rhp 
	\return otherwise return false
	\param rhp iterator on list to compare with m_It / m_Method */
bool SQLStringBuilderList_Iterator::operator != (const SQLStringBuilderList_Iterator& rhp) const
{
	if (rhp.m_Method != m_Method)
	{
		ExceptInfo sInfo("basar.db.aspect.SQLStringBuilderList_Iterator.operator!=", "Iterator from different Container.", __FILE__, __LINE__);
		throw DifferentContainersIteratorException(sInfo);
	}
	return (!operator==(rhp));
}

/*--------------------------------------------------------------------------------------------------*/
/*!	\throw OutOfRangeIteratorException 
	\return the increased iterator */
SQLStringBuilderList_Iterator SQLStringBuilderList_Iterator::operator ++ ()
{
	checkValid();
	if (m_It == m_Method->end())
	{
		ExceptInfo sInfo("basar.db.aspect.SQLStringBuilderList_Iterator.operator++ ", "Iterator of sql string builder list is at the end.", __FILE__, __LINE__);
		throw OutOfRangeIteratorException(sInfo);
	}
	++m_It;
	return *this;
}

/*--------------------------------------------------------------------------------------------------*/
/*!	\throw OutOfRangeIteratorException 
	\return the current iterator */
SQLStringBuilderList_Iterator SQLStringBuilderList_Iterator::operator ++ (int)
{
	checkValid();
	if (m_It == m_Method->end())
	{
		ExceptInfo sInfo("basar.db.aspect.SQLStringBuilderList_Iterator.operator++(int)", "Iterator of sql string builder list is at the end.", __FILE__, __LINE__);
		throw OutOfRangeIteratorException(sInfo);
	}
	SQLStringBuilderList_Iterator temp(*this);
	++m_It;
	return temp;
}

/*--------------------------------------------------------------------------------------------------*/
/*!	\throw OutOfRangeIteratorException 
	\return stringbuilder reference of the current iterator */
SQLStringBuilderRef SQLStringBuilderList_Iterator::operator*()
{
	checkValid();
	if (m_It == m_Method->end())
	{
		ExceptInfo sInfo("basar.db.aspect.SQLStringBuilderList_Iterator.operator*", "Iterator of sql string builder list is at the end.", __FILE__, __LINE__);
		throw OutOfRangeIteratorException(sInfo);
	}
	return *m_It;
}
/*--------------------------------------------------------------------------------------------------*/
/*!	\throw OutOfRangeIteratorException 
	\return pointer to stringbuilder reference of the current iterator */
SQLStringBuilderRef* SQLStringBuilderList_Iterator::operator ->()
{
    checkValid();
    if (m_It == m_Method->end())
    {
 		ExceptInfo sInfo("basar.db.aspect.SQLStringBuilderList_Iterator.operator*", "Iterator of sql string builder list is at the end.", __FILE__, __LINE__);
		throw OutOfRangeIteratorException(sInfo);   
    }
    return &(*m_It);
}

/*--------------------------------------------------------------------------------------------------*/
/*!	\throw InvalidIteratorException */
void SQLStringBuilderList_Iterator::checkValid() const
{
	if( m_Method.get() == 0 )
	{
		ExceptInfo sInfo("basar.db.aspect.SQLStringBuilderList_Iterator.checkValid", "Iterator of accessor method isn't valid.", __FILE__, __LINE__);
		throw InvalidIteratorException(sInfo);
	}
}

/*--------------------------------------------------------------------------------------------------*/
bool SQLStringBuilderList_Iterator::isNull() const
{
	return ( m_Method.get() == 0 );
}

/*--------------------------------------------------------------------------------------------------*/
void SQLStringBuilderList_Iterator::reset()
{
	m_Method.reset();
}

/*--------------------------------------------------------------------------------------------------*/
} // aspect
} // db
} // basar
