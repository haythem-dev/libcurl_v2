//----------------------------------------------------------------------------
/*! \file
 *  \brief  class implementation of parameter list iterator
 *  \author Thomas Hörath
 *  \date   24.08.2005
 */
//----------------------------------------------------------------------------


#include "libbasarcmnutil_parameterlist.h"
#include "libbasarcmnutil_i18nstring.h"
#include "libbasarcmnutil_exceptions.h"

//----------------------------------------------------------------------------
namespace basar {
namespace cmnutil{

//----------------------------------------------------------------------------
/*!	\throw no-throw 
	\param pCol pointer to parameter-list
	\param it iterator of parameter-list */
ParameterList_Iterator::ParameterList_Iterator(ColParamListMapSharedPtr pCol, ColParamListMapIt it) 
: m_ColParamListMap (pCol), m_ColParamListMapIt (it)
{

}

//----------------------------------------------------------------------------
/*!	\throw no-throw */
ParameterList_Iterator::ParameterList_Iterator() 
{

}
////////////////////////////////////////////////////////////////////////////////
/*! \retval	true if the inner map contains at least one item and the iterator 
			is pointing to the first.
	\return	otherwise returns false
	\throw	InvalidIteratorException */
////////////////////////////////////////////////////////////////////////////////
bool ParameterList_Iterator::isBegin() const
{
	checkValid();
	return (m_ColParamListMap->begin() == m_ColParamListMapIt);
}
////////////////////////////////////////////////////////////////////////////////
/*! \retval	true if the inner map contains at least one item and the iterator 
			is pointing to the position one beyond the last item.
	\return	otherwise returns false
	\throw	InvalidIteratorException */
////////////////////////////////////////////////////////////////////////////////
bool ParameterList_Iterator::isEnd() const
{
	checkValid();
	return (m_ColParamListMap->end() == m_ColParamListMapIt);
}

//----------------------------------------------------------------------------
/*!	\throw UnknownPropertyNameException 
	\retval true if no value is set to given key */
bool  ParameterList_Iterator::isValueEmpty () const
{
	checkValid();
	return m_ColParamListMapIt->second.empty();
}

//----------------------------------------------------------------------------
/*!	\throw no-throw 
	\param rhp iterator on list to compare with m_ColParamListMapIt / m_ColParamListMap
	\retval true if equal 
	\return otherwise returns false */
bool ParameterList_Iterator::operator == (const ParameterList_Iterator& rhp) const
{
	checkValid();
	if (rhp.m_ColParamListMap != m_ColParamListMap)
	{
		ExceptInfo sInfo("basar.cmnutil.ParameterList_Iterator.operator==", "Iterator of different container.", __FILE__, __LINE__);
		throw DifferentContainersIteratorException(sInfo);
	}
	return ( (rhp.m_ColParamListMap == m_ColParamListMap) && (rhp.m_ColParamListMapIt == m_ColParamListMapIt) );
}

//----------------------------------------------------------------------------
/*!	\throw no-throw 
	\param rhp iterator on list to compare with m_ColParamListMapIt / m_ColParamListMap
	\retval true if not equal
	\return otherwise returns false */
bool ParameterList_Iterator::operator != (const ParameterList_Iterator& rhp) const
{
	if (rhp.m_ColParamListMap != m_ColParamListMap)
	{
		ExceptInfo sInfo("basar.cmnutil.ParameterList_Iterator.operator!=", "Iterator of different container.", __FILE__, __LINE__);
		throw DifferentContainersIteratorException(sInfo);
	}
	return !(operator == (rhp));
}

//----------------------------------------------------------------------------
/*!	\throw OutOfRangeIteratorException 
	\return iterator, set to next element (prae) */
ParameterList_Iterator ParameterList_Iterator::operator ++()
{
	checkValid();
	if ( m_ColParamListMap->end() == m_ColParamListMapIt)
	{
		ExceptInfo sInfo("basar.cmnutil.ParameterList_Iterator.operator++", "current position of iterator is end()", __FILE__, __LINE__);
		throw OutOfRangeIteratorException(sInfo);
	}
	++m_ColParamListMapIt;
	return *this;
}

//----------------------------------------------------------------------------
/*!	\throw OutOfRangeIteratorException 
	\return iterator, set to next element (post) */
ParameterList_Iterator ParameterList_Iterator::operator ++(int)
{
	checkValid();
	if ( m_ColParamListMap->end() == m_ColParamListMapIt)
	{
		ExceptInfo sInfo("basar.cmnutil.ParameterList_Iterator.operator++(int)", "current position of iterator is end()", __FILE__, __LINE__);
		throw OutOfRangeIteratorException(sInfo);
	}
	ParameterList_Iterator temp = *this;
	
	++m_ColParamListMapIt;

	return temp;
}

//----------------------------------------------------------------------------
/*!	\throw OutOfRangeIteratorException 
	\return reference to current iterator */
ParamListPair& ParameterList_Iterator::operator*()
{
	checkValid();
	if ( m_ColParamListMap->end() == m_ColParamListMapIt)
	{
		ExceptInfo sInfo("basar.cmnutil.ParameterList_Iterator.operator*", "current position of iterator is end()", __FILE__, __LINE__);
		throw OutOfRangeIteratorException(sInfo);
	}
	return *m_ColParamListMapIt;
}

//----------------------------------------------------------------------------
/*!	\throw OutOfRangeIteratorException 
	\return reference to current iterator */
const ParamListPair& ParameterList_Iterator::operator*() const
{
	checkValid();
	if ( m_ColParamListMap->end() == m_ColParamListMapIt)
	{
		ExceptInfo sInfo("basar.cmnutil.ParameterList_Iterator.operator*const", "current position of iterator is end()", __FILE__, __LINE__);
		throw OutOfRangeIteratorException(sInfo);
	}
	return *m_ColParamListMapIt;
}
 
//----------------------------------------------------------------------------
/*!	\throw OutOfRangeIteratorException 
	\return pointer to current iterator */
ParamListPair* ParameterList_Iterator::operator ->()
{
	checkValid();
	if ( m_ColParamListMap->end() == m_ColParamListMapIt)
	{
		ExceptInfo sInfo("basar.cmnutil.ParameterList_Iterator.operator->", "current position of iterator is end()", __FILE__, __LINE__);
		throw OutOfRangeIteratorException(sInfo);
	}
	return &(*m_ColParamListMapIt);
}

//----------------------------------------------------------------------------
/*!	\throw OutOfRangeIteratorException 
	\return pointer to current iterator */
const ParamListPair* ParameterList_Iterator::operator ->() const
{
	checkValid();
	if ( m_ColParamListMap->end() == m_ColParamListMapIt)
	{
		ExceptInfo sInfo("basar.cmnutil.ParameterList_Iterator.operator->const", "current position of iterator is end()", __FILE__, __LINE__);
		throw OutOfRangeIteratorException(sInfo);
	}
	return &(*m_ColParamListMapIt);
}

//----------------------------------------------------------------------------
/*!	\throw InvalidIteratorException */
void ParameterList_Iterator::checkValid() const
{
	if( m_ColParamListMap.get() == 0 )
	{
		ExceptInfo sInfo("basar.cmnutil.ParameterList_Iterator.checkValid", "Iterator of parameter list isn't valid.", __FILE__, __LINE__);
		throw InvalidIteratorException(sInfo);
	}
}

//----------------------------------------------------------------------------
bool ParameterList_Iterator::isNull() const
{
	return (m_ColParamListMap.get() == 0);
}

//----------------------------------------------------------------------------
void ParameterList_Iterator::reset() 
{
	m_ColParamListMap.reset();
}

//----------------------------------------------------------------------------
}//cmnutil
}//basar
