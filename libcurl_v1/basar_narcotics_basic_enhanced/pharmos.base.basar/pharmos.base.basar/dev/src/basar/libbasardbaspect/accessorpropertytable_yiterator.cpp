//--------------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  class implementation of yiterator of accessor property table
 *  \author Roland Kiefert
 *  \date   24.08.2005
 */
//--------------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------------//
// include
//--------------------------------------------------------------------------------------------------------//

#include "libbasardbaspect_accessorpropertytable_yiterator.h"
#include "libbasardbaspect_accessorpropertytable.h"

#include "libbasardebug.h"
#include "cachecontroller.h"
#include "cachingpolicy.h"
#include "loggerpool.h"
#include "libbasarproperty_propertytable_xiterator.h"
#include "libbasarproperty_algorithm.h"
#include "libbasarproperty_propertystate.h"
#include "libbasarproperty_propertydescriptionlistref.h"
#include <libbasarcmnutil_logging.h>

#include "../macro.h"

//--------------------------------------------------------------------------------------------------------//
// namespace
//--------------------------------------------------------------------------------------------------------//
namespace basar {
namespace db {
namespace aspect {

//----------------------------------------------------------------------------
void dumpAPTYIt(size_t addr)
{
	if (0 == addr)
		return;

	AccessorPropertyTable_YIterator obj = *(reinterpret_cast<AccessorPropertyTable_YIterator *>(addr));

	if (obj.isNull())
		return;

	std::ostringstream outStr;

	obj.toStream(outStr);

	debug::printDbgMsg(outStr.str().c_str());
}

//--------------------------------------------------------------------------------------------------------//
// object implementation
//--------------------------------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\param pCache pointer to CacheController
	\param it YIterator to propertytable
	\param row position in resultset */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTable_YIterator::AccessorPropertyTable_YIterator(boost::shared_ptr< CacheController > pCache, 
																 property::PropertyTable_YIterator    it, 
																 const Int32                          row) 
: m_pCacheController (pCache), m_PtYit (it), m_RowNumber(row)
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTable_YIterator::AccessorPropertyTable_YIterator () 
: m_RowNumber( -1 )
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\param state property state to set to row (yiterator) */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTable_YIterator::AccessorPropertyTable_YIterator( const PropertyStateSet& state )
: m_PtYit( state ), m_RowNumber( -1 )
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\param pss property state set to set to row (yiterator) 
	\param rhs yiterator to copy */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTable_YIterator::AccessorPropertyTable_YIterator( const AccessorPropertyTable_YIterator& rhs, 
																  const PropertyStateSet& pss )
: m_pCacheController( rhs.m_pCacheController ), m_PtYit( rhs.m_PtYit, pss ), m_RowNumber( rhs.m_RowNumber )
{
}
//--------------------------------------------------------------------------------------------------------//
// operators
//--------------------------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////
/*! \retval	true if the inner map contains at least one item and the iterator 
			is pointing to the first.
	\return	otherwise returns false
	\throw	no-throw */
////////////////////////////////////////////////////////////////////////////////
bool AccessorPropertyTable_YIterator::isBegin() const
{
	checkValid();
	
	return m_pCacheController->m_Caching->isBegin(*this);
}
////////////////////////////////////////////////////////////////////////////////
/*! \retval	true if the inner map contains at least one item and the iterator 
			is pointing to the position one beyond the last item.
	\return	otherwise returns false
	\throw	no-throw */
////////////////////////////////////////////////////////////////////////////////
bool AccessorPropertyTable_YIterator::isEnd() const
{
	checkValid();
	
	return m_pCacheController->m_Caching->isEnd(*this);
}

////////////////////////////////////////////////////////////////////////////////
/*! \brief returning size of propertytable after resultset 
           has been read completely
    \return size of propertytable after resultset 
           has been read completely
	\throw no-throw */
////////////////////////////////////////////////////////////////////////////////
NumberRows	AccessorPropertyTable_YIterator::size() const
{
	checkValid();
	return m_PtYit.size();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////			
/*!	\throw no throw
	\retval true if equal 
	\return otherwise returns false
	\param  rhp yiterator to compare with own iterator */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
bool AccessorPropertyTable_YIterator::operator == ( const AccessorPropertyTable_YIterator& rhp ) const
{
	checkValid();
	if( rhp.m_pCacheController != m_pCacheController )
	{
/*! \ingroup DIFF_DBG_REL */ 
#ifndef NDEBUG
		BLOG_ERROR( LoggerPool::aptYIterator(), 
					"basar.db.aspect.AccessorPropertyTable_YIterator.operator==(): "
					"Iterator from different Container." );
#endif
		return false;
	}

	return (rhp.m_PtYit == m_PtYit);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no throw
	\retval true if not equal 
	\return otherwise returns false
	\param  rhp yiterator to compare with own iterator */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
bool AccessorPropertyTable_YIterator::operator != ( const AccessorPropertyTable_YIterator& rhp ) const
{
	if( rhp.m_pCacheController != m_pCacheController )
	{
/*! \ingroup DIFF_DBG_REL */ 
#ifndef NDEBUG
		BLOG_ERROR( LoggerPool::aptYIterator(), 
			        "basar.db.aspect.AccessorPropertyTable_YIterator.operator!=(): "
					"Iterator from different Container." );
#endif
		return true;
	}

	return !(operator == (rhp));
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw OutOfRangeIteratorException 
	\return YIterator, set to next element (prae) */
////////////////////////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTable_YIterator AccessorPropertyTable_YIterator::operator++()
{

	checkValid();
	if( m_RowNumber == ITERATOR_ENDPOS  )
	{
		throw OutOfRangeIteratorException(
				ExceptInfo( "basar.db.aspect.AccessorPropertyTable_YIterator.operator++()", 
							"Iterator of accessor property table equals end().", 
							__FILE__, __LINE__) );
	}
	
	bool replicated = false;
	if( ! (m_pCacheController->m_Resultset.isNull()) ){
		if ( m_RowNumber+1 >= static_cast<Int32>(m_pCacheController->m_Resultset.getFetchedRows()) ){
			if (m_PtYit.getFilter().contains(SS_CLEAN) || 
				m_PtYit.getFilter().contains(SS_UNKNOWN))
				replicated = m_pCacheController->m_Caching->replicate(m_RowNumber+1);
		}
	}
	
	m_pCacheController->m_Caching->preIncrement(*this);
	m_pCacheController->m_Caching->updateRowNumber( replicated, *this );

	BLOG_TRACE(LoggerPool::aptYIterator(), "++operator; iterator incremented.");

	return *this;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw OutOfRangeIteratorException 
	\return YIterator, set to next element (post) */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTable_YIterator AccessorPropertyTable_YIterator::operator++( int )
{
	checkValid();
	AccessorPropertyTable_YIterator temp = *this;
	
	if( m_RowNumber == ITERATOR_ENDPOS )
	{		
		throw OutOfRangeIteratorException(
				ExceptInfo( "basar.db.aspect.AccessorPropertyTable_YIterator.operator++(int)", 
							"Iterator of accessor property table is at the end.", 
							__FILE__, __LINE__) );
	}
	
	bool replicated = false;
	if( ! (m_pCacheController->m_Resultset.isNull()) ){
		if( m_RowNumber+1 >= static_cast<Int32>(m_pCacheController->m_Resultset.getFetchedRows()) ){
			if (m_PtYit.getFilter().contains(SS_CLEAN) || 
				m_PtYit.getFilter().contains(SS_UNKNOWN))
				replicated = m_pCacheController->m_Caching->replicate(m_RowNumber+1);
		}
	}

	m_pCacheController->m_Caching->preIncrement(*this);
	m_pCacheController->m_Caching->updateRowNumber( replicated, *this );

	BLOG_TRACE(LoggerPool::aptYIterator(), "operator++; iterator incremented.");

	return temp;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw OutOfRangeIteratorException 
	\return YIterator, set to previous element (prae) */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTable_YIterator AccessorPropertyTable_YIterator::operator--()
{
	FUNCTION_NAME( "basar.db.ascpect.AccessorPropertyTable_YIterator::operator--()" )		
	
	checkValid();
	if( 0 == m_RowNumber ){
		throw OutOfRangeIteratorException(
				ExceptInfo( fun, "Iterator of accessor property table is at the begin.", __FILE__, __LINE__) );
	}

	--m_PtYit;
	if( m_RowNumber == ITERATOR_ENDPOS ){		
		m_RowNumber = static_cast<Int32>(m_pCacheController->m_PropertyTableRef.size()) - 1;	
	} else {
		m_RowNumber -= m_PtYit.getFilteredStep();
	}

	BLOG_TRACE(LoggerPool::aptYIterator(), "--operator; iterator decremented.");
	
	return *this;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw OutOfRangeIteratorException 
	\return YIterator, set to previous element (post) */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTable_YIterator AccessorPropertyTable_YIterator::operator--( int )
{
	FUNCTION_NAME( "basar.db.ascpect.AccessorPropertyTable_YIterator.operator--(int)" )		

	checkValid();
	
	if( 0 == m_RowNumber ){
		throw OutOfRangeIteratorException(
				ExceptInfo(	fun, 
							"Iterator of accessor property table is at the begin.", 
							__FILE__, __LINE__) );
	}

	AccessorPropertyTable_YIterator temp = *this;

	m_PtYit--;
	if( m_RowNumber == ITERATOR_ENDPOS ){		
		m_RowNumber = static_cast<Int32>(m_pCacheController->m_PropertyTableRef.size()) - 1;	
	} else {
		m_RowNumber -= m_PtYit.getFilteredStep();
	}

	BLOG_TRACE(LoggerPool::aptYIterator(), "operator--; iterator decremented.");

	return temp;
}

//--------------------------------------------------------------------------------------------------------//
// object implementation
//--------------------------------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return reference to first PropertyTable_XIterator. */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTable_XIterator AccessorPropertyTable_YIterator::begin() const
{
	checkValid();
	
	return m_pCacheController->m_Caching->begin( (*this) );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return reference to first PropertyTableXIter that is in given state
	\param state states to search for in the columns */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTable_XIterator AccessorPropertyTable_YIterator::begin(const PropertyStateSet & state )
{
	checkValid();
	
	return m_pCacheController->m_Caching->begin( state, *this );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return reference to PropertyTableXIter one after last */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTable_XIterator AccessorPropertyTable_YIterator::end() const
{
	checkValid();
	
	return m_pCacheController->m_Caching->end(*this);
}

///////////////////////////////////////////////////////////////////////////////////////////
//! \throw  no-throw
///////////////////////////////////////////////////////////////////////////////////////////
void AccessorPropertyTable_YIterator::clear()
{
	if ( m_pCacheController.get() != 0 )
		m_pCacheController->clear();
	
	m_pCacheController.reset();
	m_RowNumber = -1;
	m_PtYit.clear();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return the rownumber of the iterator */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
Int32 AccessorPropertyTable_YIterator::getRowNumber() const
{
	return m_RowNumber;
}

//--------------------------------------------------------------------------------------------------------//
// object methods
//--------------------------------------------------------------------------------------------------------//

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	Checks if all given properties (rPropertyNames) are set in this yiterator
    \return	true if all given properties (rPropertyNames) are set in this yiterator
	\return	false, otherwise
	\param rPropertyNames list of column names that are checked
	\throw no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
bool AccessorPropertyTable_YIterator::arePropertiesSet( 
    const PropertyName& rPropertyNames ) const
{
	checkValid();
	return property::arePropertiesSet( m_PtYit, rPropertyNames );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	Checks if all given properties (rPropertyNames) are set in this yiterator
    \return	true, if all given properties (rParaList) are set in this yiterator
	\return	false, otherwise   
	\param	rParaList list of column names that are checked
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
bool AccessorPropertyTable_YIterator::arePropertiesSet( const cmnutil::ParameterList & rParaList ) const
{
	checkValid();
	return property::arePropertiesSet( m_PtYit, rParaList );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	Checks if all the given properties (rParaList) are set in this yiterator
    and at least one of those properties is in one of the given states (pss)
    \return	true if all the given properties (rParaList) are set in this yiterator
                 and at least one of those properties is in one of the given states (pss)
	\return false, otherwise   
	\param	rParaList list of column names that are checked
	\param	pss       list of states at least one property has to be in
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
bool AccessorPropertyTable_YIterator::arePropertiesSetAndOneMustBeInState(const cmnutil::ParameterList & rParaList, 
																				const PropertyStateSet       & pss      ) const
{
	checkValid();
	return property::arePropertiesSetAndOneMustBeInState( m_PtYit, rParaList, pss );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! Checks if all the given properties (rParaList) are set in this yiterator
    and at least one of those properties is in one of the given states (pss)
    \return	true if all the given properties (rPropertyNames) are set in this yiterator
                 and at least one of those properties is in one of the given states (pss)
	\return false, otherwise   
	\param	rPropertyNames list of column names that are checked
	\param	pss            list of states at least one property has to be in
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
bool AccessorPropertyTable_YIterator::arePropertiesSetAndOneMustBeInState( 
    const PropertyName& rPropertyNames, 
	const PropertyStateSet& pss ) const
{
	checkValid();
	return property::arePropertiesSetAndOneMustBeInState( m_PtYit, rPropertyNames, pss );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	Checks if all the given properties (rPropertyNames) are set in this yiterator
    and at least one of those properties is in one of the given states (pss)
    \return	true, if all the given properties (rPropertyNames) are set in this yiterator
                  and at least one of those properties is in one of the given states (pss)
	\return false, otherwise   
	\param	rPropertyNames list of column names that are checked
	\param	pss            list of states the properties have to be in
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
bool AccessorPropertyTable_YIterator::arePropertiesSetAndAllInState(
	 const PropertyName& rPropertyNames, 
	 const PropertyStateSet& pss ) const
{
	checkValid();
	return property::arePropertiesSetAndAllInState( m_PtYit, rPropertyNames, pss );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	Checks if all the given properties (rPropertyNames) are set in this yiterator
    and at least one of those properties is in one of the given states (pss)
    \return	true, if all the given properties (rPropertyNames) are set in this yiterator
                  and at least one of those properties is in one of the given states (pss)
	\return false, otherwise   
	\param	rParaList      list of column names that are checked
	\param	pss            list of states the properties have to be in
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
bool AccessorPropertyTable_YIterator::arePropertiesSetAndAllInState(const cmnutil::ParameterList & rParaList,
																	 const PropertyStateSet       & pss      ) const
{
	checkValid();
	return property::arePropertiesSetAndAllInState( m_PtYit, rParaList, pss );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! Checks if all given properties in the 1st parameter ( plShouldBeSet)
    are set and at least one of the given properties in the 
	2nd parameter (plOneMustBeInState) is in one of the given states (pss)
    \return	true, if all given properties in the 1st parameter ( plShouldBeSet)
                  are set and at least one of the given properties in the 
				  2nd parameter (plOneMustBeInState) is in one of the given states (pss)
	\return false, otherwise   
	\param	plShouldBeSet      represents the properties that are checked if they are  set 
	\param  plOneMustBeInState represents the properties that are checked if at least one is in state
			                   and have to be in one of the given states (pss).
	\param  pss                represents the possible states at least one of the properties 
                               of the 2nd parameter (propertyNamesOneMustBeInState)
			                   should be in.
	\throw no-throw */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool AccessorPropertyTable_YIterator::are1stPropertiesSetAnd2ndOneInState( 
    const cmnutil::ParameterList& plShouldBeSet, 
	const cmnutil::ParameterList& plOneMustBeInState, 
	const PropertyStateSet& pss ) const
{
	checkValid();
	return property::are1stPropertiesSetAnd2ndOneInState( m_PtYit, plShouldBeSet, plOneMustBeInState, pss );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! Checks if all given properties in the 1st parameter ( plShouldBeSet)
    are set and at least one of the given properties in the 
	2nd parameter (plOneMustBeInState) is in one of the given states (pss)
    \return	true, if all given properties in the 1st parameter ( plShouldBeSet)
                  are set and at least one of the given properties in the 
				  2nd parameter (plOneMustBeInState) is in one of the given states (pss)
	\return false, otherwise   
	\param	propertyNamesShouldBeSet      represents the properties that are checked 
                                          if they are  set 
	\param  propertyNamesOneMustBeInState represents the properties that are checked if 
                                          at least one is in state
			                              and have to be in one of the given states (pss).
	\param  pss                           represents the possible states at least one 
                                          of the properties of the 
                                          2nd parameter (propertyNamesOneMustBeInState)
			                              should be in.
	\throw no-throw */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool AccessorPropertyTable_YIterator::are1stPropertiesSetAnd2ndOneInState( 
    const PropertyName& propertyNamesShouldBeSet, 
	const PropertyName& propertyNamesOneMustBeInState, 
	const PropertyStateSet& pss ) const
{
	checkValid();
	return property::are1stPropertiesSetAnd2ndOneInState( m_PtYit, propertyNamesShouldBeSet, 
																 propertyNamesOneMustBeInState, pss );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return	true, if given property is set and yiterator is not end 
	\return false, otherwise   
	\param	rPropertyName property to check if it is set
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
bool AccessorPropertyTable_YIterator::isNotEndAndSet( const PropertyName& rPropertyName ) const
{
	checkValid();
	return m_PtYit.isNotEndAndSet( rPropertyName );
}
	
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\retval	true, if given property is set and contained in yiterator
	\return false, otherwise   
	\param	rPropertyName property to check
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
bool	AccessorPropertyTable_YIterator::isContainedAndSet( const PropertyName& rPropertyName ) const
{
	checkValid();
	return m_PtYit.isContainedAndSet( rPropertyName );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! Checks if at least one of the given properties (rParaList) of this 
            yiterator is in one of the given states (pss)
    \return true, if at least one of the given properties (rParaList) of this 
            yiterator is in one of the given states (pss)
    \return false, otherwise
    \param	rParaList list of column names that are checked
	\param	pss       list of states at least one property has to be in
	\throw	no-throw */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool AccessorPropertyTable_YIterator::isOnePropertyInState(const cmnutil::ParameterList & rParaList, 
														   const PropertyStateSet       & pss
														  ) const
{
    checkValid();
	return property::isOnePropertyInState( m_PtYit, rParaList, pss );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! Checks if all of the given properties (rParaList), that are 
               listed in this YIterator are in one of the given states (pss). 
    \return true, if all of the given properties (rParaList), that are 
               listed in this YIterator are in one of the given states (pss). 
    \return acceptNoProperty, when none of the parameters are
            listed and set in this Y-Iterator
    \return false, otherwise
    \param	rParaList        list of column names that are checked
	\param	pss              list of states at least one property has to be in
    \param  acceptNoProperty default return value, when none of the parameters are 
                             listed in this Y-Iterator
	\throw	no-throw */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////	
bool AccessorPropertyTable_YIterator::areKnownPropertiesInState(const cmnutil::ParameterList & rParaList, 
														        const PropertyStateSet       & pss,
																bool                           acceptNoProperty
															   ) const
{
    checkValid();
	return property::areKnownPropertiesInState( m_PtYit, rParaList, pss , acceptNoProperty);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! Checks, if all of the given properties (rParaList), that are 
            listed and set (not in state UNSET) in this YIterator are in 
            one of the given states (pss). 
    \return true, if all of the given properties (rParaList), that are 
            listed and set (not in state UNSET) in this YIterator are in 
            one of the given states (pss). 
    \return acceptNoProperty, when none of the parameters are
            listed and set in this Y-Iterator the return 
            value is acceptNoProperty 
    \return false, otherwise
    \param	rParaList        list of column names that are checked
	\param	pss              list of states the found properties have to be in
    \param  acceptNoProperty default return value, when none of the parameters are
                             listed and set in this Y-Iterator
	\throw	no-throw */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool AccessorPropertyTable_YIterator::areKnownAndSetPropertiesInState(const cmnutil::ParameterList & rParaList, 
																	  const PropertyStateSet       & pss,
																	  bool                           acceptNoProperty
																	 ) const
{
    checkValid();
	return property::areKnownAndSetPropertiesInState( m_PtYit, rParaList, pss , acceptNoProperty);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return type of given column 
	\param rColName column that is asked for its type */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
const PropertyType& AccessorPropertyTable_YIterator::getType( const ColumnName& rColName ) const
{
	checkValid();
	return m_PtYit.getType( rColName );
}

const PropertyType& AccessorPropertyTable_YIterator::getType( const ColumnIndex index ) const
{
    checkValid();
    return m_PtYit.getType( index );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return index of given column 
	\param rColName column that is asked for its index */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
ColumnIndex AccessorPropertyTable_YIterator::getIndex( const ColumnName& rColName ) const
{
	return m_PtYit.getIndex(rColName);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return PropertyStateSet containing the possible states stored in a member. 
	\throw	NoFilterAvailableException */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
const PropertyStateSet& AccessorPropertyTable_YIterator::getFilter() const
{
	checkValid();

	return m_pCacheController->m_Caching->getFilter(*this);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return insert type of iterator */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
InsertTypeEnum AccessorPropertyTable_YIterator::getInsertType( ) const
{
	checkValid();
	return m_PtYit.getInsertType();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw	NoStateAvailableException
	\return state of iterator */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
const PropertyStateSet AccessorPropertyTable_YIterator::getState() const
{
	checkValid();
	
	return m_pCacheController->m_Caching->getState(*this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return state of given column 
	\param rColName column that is asked for its state */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
const PropertyState& AccessorPropertyTable_YIterator::getState( const ColumnName& rColName ) const
{
	checkValid();
	return m_PtYit.getState(rColName);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return state of given index 
	\param Index Index that is asked for its state */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
const PropertyState& AccessorPropertyTable_YIterator::getState( const ColumnIndex Index ) const
{
	checkValid();
	return m_PtYit.getState(Index);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\retval true if given column is in propertytable 
	\return otherwise returns false
	\param rColName column to search for in row */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
bool AccessorPropertyTable_YIterator::contains( const ColumnName& rColName ) const 
{
	checkValid();
	return m_PtYit.contains( rColName );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\retval true if given type in column is in propertytable 
	\return otherwise returns true
	\param rColName column to search for in row 
	\param pt type which column should have */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
bool AccessorPropertyTable_YIterator::contains( const ColumnName & rColName, 
										        const PropertyType pt ) const
{
	checkValid();
	return m_PtYit.contains(rColName,pt);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\retval true if given pair is in propertytable 
	\return otherwise returns false
	\param  p pair of columnname and type which should be searched for */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
bool AccessorPropertyTable_YIterator::contains(	const std::pair<ColumnName, PropertyType>& p ) const
{
	checkValid();
	return m_PtYit.contains(p);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw NoStateAvailableException
	\retval true if given type with given state in column is in propertytable 
	\return otherwise returns false
	\param rColName column to search for in row 
	\param pt type which column should have 
	\param pss states of rows to search for */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
bool AccessorPropertyTable_YIterator::contains(	const ColumnName     & rColName, 
												const PropertyType     pt, 
												const PropertyStateSet pss ) const
{
	checkValid();
	return m_pCacheController->m_Caching->contains( rColName, pt, pss, *this );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw NoStateAvailableException
	\retval true if given pair with state is in propertytable 
	\return otherwise returns false
	\param  pss states of rows to search for 
	\param  p pair of columnname and type which should be searched for */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
bool AccessorPropertyTable_YIterator::contains(	const std::pair<ColumnName, PropertyType>& p, 
												const PropertyStateSet pss ) const
{
	checkValid();
	return m_pCacheController->m_Caching->contains( p, pss, *this );
}
//--------------------------------------------------------------------------------------------------------//
// get- and set-methods
//--------------------------------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return integer value of column 
	\param  rColName name of column to get int-value from */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
const Int16& AccessorPropertyTable_YIterator::getInt16(const ColumnName & rColName ) const
{
	checkValid();
	
	return m_pCacheController->m_Caching->getInt16( rColName, *this );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return integer value of column 
	\param  index index of column to get int-value from */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
const Int16& AccessorPropertyTable_YIterator::getInt16(const ColumnIndex index ) const
{
	checkValid();
	
	return m_pCacheController->m_Caching->getInt16(index, *this);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw
\param rColName name of column to set int-value to
\param value int-value to set to column */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void AccessorPropertyTable_YIterator::setInt16(const ColumnName & rColName, const Int16 value)
{
    checkValid();
    m_pCacheController->m_Caching->checkIsAllowedToSet();
    m_pCacheController->m_Caching->checkIsRowLongerAvailable(*this);

    m_PtYit.setInt16(rColName, value);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw
\param index index of column to set int-value to
\param value int-value to set to column */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void AccessorPropertyTable_YIterator::setInt16(const ColumnIndex index, const Int16 value)
{
    checkValid();
    m_pCacheController->m_Caching->checkIsAllowedToSet();
    m_pCacheController->m_Caching->checkIsRowLongerAvailable(*this);

    m_PtYit.setInt16(index, value);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw
\return integer value of column
\param  rColName name of column to get int-value from */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
const Int32& AccessorPropertyTable_YIterator::getInt32(const ColumnName& rColName) const
{
    checkValid();

    return m_pCacheController->m_Caching->getInt32(rColName, *this);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw	no-throw
\return integer value of column
\param  index position of column in the PropertyDescriptionList to get int-value from */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
const Int32& AccessorPropertyTable_YIterator::getInt32(const ColumnIndex index) const
{
    checkValid();

    return m_pCacheController->m_Caching->getInt32(index, *this);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\param rColName name of column to set int-value to
	\param value int-value to set to column */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void AccessorPropertyTable_YIterator::setInt32( const ColumnName& rColName, const Int32 value )
{
	checkValid();
	m_pCacheController->m_Caching->checkIsAllowedToSet();
	m_pCacheController->m_Caching->checkIsRowLongerAvailable(*this);
	
	m_PtYit.setInt32( rColName, value );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\param index index of column to set int-value to
	\param value int-value to set to column */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void AccessorPropertyTable_YIterator::setInt32( const ColumnIndex index, const Int32 value )
{
	checkValid();
	m_pCacheController->m_Caching->checkIsAllowedToSet();
	m_pCacheController->m_Caching->checkIsRowLongerAvailable(*this);

	m_PtYit.setInt32( index, value );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw
\return integer value of column
\param  rColName name of column to get int-value from */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
const Int64& AccessorPropertyTable_YIterator::getInt64(const ColumnName& rColName) const
{
    checkValid();

    return m_pCacheController->m_Caching->getInt64(rColName, *this);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw	no-throw
\return integer value of column
\param  index position of column in the PropertyDescriptionList to get int-value from */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
const Int64& AccessorPropertyTable_YIterator::getInt64(const ColumnIndex index) const
{
    checkValid();

    return m_pCacheController->m_Caching->getInt64(index, *this);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw
\param rColName name of column to set int-value to
\param value int-value to set to column */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void AccessorPropertyTable_YIterator::setInt64(const ColumnName& rColName, const Int64 value)
{
    checkValid();
    m_pCacheController->m_Caching->checkIsAllowedToSet();
    m_pCacheController->m_Caching->checkIsRowLongerAvailable(*this);

    m_PtYit.setInt64(rColName, value);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw
\param index index of column to set int-value to
\param value int-value to set to column */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void AccessorPropertyTable_YIterator::setInt64(const ColumnIndex index, const Int64 value)
{
    checkValid();
    m_pCacheController->m_Caching->checkIsAllowedToSet();
    m_pCacheController->m_Caching->checkIsRowLongerAvailable(*this);

    m_PtYit.setInt64(index, value);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return string value of column 
	\param  rColName name of column to get string-value from */ 
///////////////////////////////////////////////////////////////////////////////////////////////////////////
const SQLString AccessorPropertyTable_YIterator::getSQLString( const ColumnName& rColName ) const
{
	checkValid();
	return m_pCacheController->m_Caching->getSQLString( rColName, *this );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return string value of column 
	\param  rColName name of column to get string-value from */ 
///////////////////////////////////////////////////////////////////////////////////////////////////////////
ConstBuffer AccessorPropertyTable_YIterator::getConstString( const ColumnName& rColName ) const
{
	checkValid();
	return m_pCacheController->m_Caching->getConstString(rColName, *this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return string value of column 
	\param  index index of column to get string-value from */ 
///////////////////////////////////////////////////////////////////////////////////////////////////////////
ConstBuffer AccessorPropertyTable_YIterator::getConstString( const ColumnIndex index ) const
{
	checkValid();	
	return m_pCacheController->m_Caching->getConstString(index, *this);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return string value of column 
	\param  rColName name of column to get string-value from */ 
///////////////////////////////////////////////////////////////////////////////////////////////////////////
const I18nString& AccessorPropertyTable_YIterator::getString( const ColumnName& rColName ) const
{
	checkValid();
	return m_pCacheController->m_Caching->getString(rColName, *this);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return string value of column 
	\param  index index of column to get string-value from */ 
///////////////////////////////////////////////////////////////////////////////////////////////////////////
const I18nString& AccessorPropertyTable_YIterator::getString( const ColumnIndex index ) const
{
	checkValid();
	return m_pCacheController->m_Caching->getString(index, *this);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\param rColName name of column to set string-value to
	\param value string-value to set to column */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void AccessorPropertyTable_YIterator::setString( const ColumnName& rColName, const I18nString& value )
{
	checkValid();
	m_pCacheController->m_Caching->checkIsAllowedToSet();
	m_pCacheController->m_Caching->checkIsRowLongerAvailable(*this);

	m_PtYit.setString(rColName,value);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\param index index of column to set string-value to
	\param value string-value to set to column */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void AccessorPropertyTable_YIterator::setString( const ColumnIndex index, const I18nString & value )
{
	checkValid();
	m_pCacheController->m_Caching->checkIsAllowedToSet();
	m_pCacheController->m_Caching->checkIsRowLongerAvailable(*this);

	m_PtYit.setString(index,value);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return Float64 value of column 
	\param  rColName name of column to get Float64-value from */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
const Float64& AccessorPropertyTable_YIterator::getFloat64( const ColumnName & rColName ) const
{
	checkValid();
	return m_pCacheController->m_Caching->getFloat64(rColName, *this);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return Float64 value of column 
	\param  index index of column to get Float64-value from */ 
///////////////////////////////////////////////////////////////////////////////////////////////////////////
const Float64& AccessorPropertyTable_YIterator::getFloat64(const ColumnIndex index ) const
{
	checkValid();
	return m_pCacheController->m_Caching->getFloat64(index, *this);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return Float32 value of column 
	\param  rColName name of column to get Float32-value from */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
const Float32& AccessorPropertyTable_YIterator::getFloat32(const ColumnName & rColName ) const
{
	checkValid();
	return m_pCacheController->m_Caching->getFloat32(rColName, *this);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return Float32 value of column 
	\param  index index of column to get Float32-value from */ 
///////////////////////////////////////////////////////////////////////////////////////////////////////////
const Float32& AccessorPropertyTable_YIterator::getFloat32(const ColumnIndex index ) const
{
	checkValid();
	return m_pCacheController->m_Caching->getFloat32(index, *this);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw  
	\param rColName name of column to set Float64-value to
	\param value Float64-value to set to column */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void AccessorPropertyTable_YIterator::setFloat64( const ColumnName & rColName, const Float64 value )
{
	checkValid();
	m_pCacheController->m_Caching->checkIsAllowedToSet();
	m_pCacheController->m_Caching->checkIsRowLongerAvailable(*this);

	m_PtYit.setFloat64(rColName,value);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw  
	\param index index of column to set Float64-value to
	\param value Float64-value to set to column */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void AccessorPropertyTable_YIterator::setFloat64( const ColumnIndex index, const Float64 value )
{
	checkValid();
	m_pCacheController->m_Caching->checkIsAllowedToSet();
	m_pCacheController->m_Caching->checkIsRowLongerAvailable(*this);

	m_PtYit.setFloat64(index,value);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw  
	\param rColName name of column to set Float32-value to
	\param value Float32-value to set to column */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void AccessorPropertyTable_YIterator::setFloat32( const ColumnName & rColName, const Float32 value )
{
	checkValid();
	m_pCacheController->m_Caching->checkIsAllowedToSet();
	m_pCacheController->m_Caching->checkIsRowLongerAvailable(*this);

	m_PtYit.setFloat32(rColName,value);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw  
	\param index index of column to set Float32-value to
	\param value Float32-value to set to column */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void AccessorPropertyTable_YIterator::setFloat32( const ColumnIndex index, const Float32 value )
{
	checkValid();
	m_pCacheController->m_Caching->checkIsAllowedToSet();
	m_pCacheController->m_Caching->checkIsRowLongerAvailable(*this);

	m_PtYit.setFloat32(index,value);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw  
	\param rColName name of column to set decimal-value to
	\param value DateTime-value to set to column */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void AccessorPropertyTable_YIterator::setDateTime( const ColumnName& rColName, const DateTime& value )
{
	checkValid();
	m_pCacheController->m_Caching->checkIsAllowedToSet();
	m_pCacheController->m_Caching->checkIsRowLongerAvailable(*this);

	m_PtYit.setDateTime( rColName, value );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw  
	\param index index of column to set decimal-value to
	\param value DateTime-value to set to column */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void AccessorPropertyTable_YIterator::setDateTime( const ColumnIndex index, const DateTime& value )
{
	checkValid();
	m_pCacheController->m_Caching->checkIsAllowedToSet();
	m_pCacheController->m_Caching->checkIsRowLongerAvailable(*this);

	m_PtYit.setDateTime( index, value );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return DateTime value of column 
	\param  rColName name of column to get DateTime-value from */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
const DateTime& AccessorPropertyTable_YIterator::getDateTime( const ColumnName& rColName ) const
{
	checkValid();
	return m_pCacheController->m_Caching->getDateTime( rColName, *this );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return DateTime value of column 
	\param  index index of column to get DateTime-value from */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
const DateTime& AccessorPropertyTable_YIterator::getDateTime( const ColumnIndex index ) const
{
	checkValid();
	return m_pCacheController->m_Caching->getDateTime( index, *this );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw  
	\param rColName name of column to set TimeSpan-value to
	\param value TimeSpan-value to set to column */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void AccessorPropertyTable_YIterator::setTimeSpan( const ColumnName& rColName, const TimeSpan& value )
{
	checkValid();
	m_pCacheController->m_Caching->checkIsAllowedToSet();
	m_pCacheController->m_Caching->checkIsRowLongerAvailable(*this);

	m_PtYit.setTimeSpan( rColName, value );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw  
	\param index index of column to set TimeSpan-value to
	\param value TimeSpan-value to set to column */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void AccessorPropertyTable_YIterator::setTimeSpan( const ColumnIndex index, const TimeSpan& value )
{
	checkValid();
	m_pCacheController->m_Caching->checkIsAllowedToSet();
	m_pCacheController->m_Caching->checkIsRowLongerAvailable(*this);

	m_PtYit.setTimeSpan( index, value );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return TimeSpan value of column 
	\param  rColName name of column to get TimeSpan-value from */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
const TimeSpan& AccessorPropertyTable_YIterator::getTimeSpan( const ColumnName& rColName ) const
{
	checkValid();
	return m_pCacheController->m_Caching->getTimeSpan( rColName, *this );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return TimeSpan value of column 
	\param  index index of column to get TimeSpan-value from */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
const TimeSpan& AccessorPropertyTable_YIterator::getTimeSpan( const ColumnIndex index ) const
{
	checkValid();
	return m_pCacheController->m_Caching->getTimeSpan( index, *this );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw  
	\param rColName name of column to set decimal-value to
	\param value DateTime-value to set to column */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void AccessorPropertyTable_YIterator::setDate( const ColumnName& rColName, const Date& value )
{
	checkValid();
	m_pCacheController->m_Caching->checkIsAllowedToSet();
	m_pCacheController->m_Caching->checkIsRowLongerAvailable(*this);

	m_PtYit.setDate( rColName, value );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw  
	\param index index of column to set decimal-value to
	\param value DateTime-value to set to column */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void AccessorPropertyTable_YIterator::setDate( const ColumnIndex index, const Date& value )
{
	checkValid();
	m_pCacheController->m_Caching->checkIsAllowedToSet();
	m_pCacheController->m_Caching->checkIsRowLongerAvailable(*this);

	m_PtYit.setDate( index, value );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return DateTime value of column 
	\param  rColName name of column to get DateTime-value from */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
const Date& AccessorPropertyTable_YIterator::getDate( const ColumnName& rColName ) const
{
	checkValid();
	return m_pCacheController->m_Caching->getDate( rColName, *this );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return DateTime value of column 
	\param  index index of column to get DateTime-value from */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
const Date& AccessorPropertyTable_YIterator::getDate( const ColumnIndex index ) const
{
	checkValid();
	return m_pCacheController->m_Caching->getDate( index, *this );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw  
	\param rColName name of column to set decimal-value to
	\param value DateTime-value to set to column */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void AccessorPropertyTable_YIterator::setTime( const ColumnName& rColName, const Time& value )
{
	checkValid();
	m_pCacheController->m_Caching->checkIsAllowedToSet();
	m_pCacheController->m_Caching->checkIsRowLongerAvailable(*this);

	m_PtYit.setTime( rColName, value );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw  
	\param index index of column to set decimal-value to
	\param value DateTime-value to set to column */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void AccessorPropertyTable_YIterator::setTime( const ColumnIndex index, const Time& value )
{
	checkValid();
	m_pCacheController->m_Caching->checkIsAllowedToSet();
	m_pCacheController->m_Caching->checkIsRowLongerAvailable(*this);

	m_PtYit.setTime( index, value );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return DateTime value of column 
	\param  rColName name of column to get DateTime-value from */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
const Time& AccessorPropertyTable_YIterator::getTime( const ColumnName& rColName ) const
{
	checkValid();
	return m_pCacheController->m_Caching->getTime( rColName, *this );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return DateTime value of column 
	\param  index index of column to get DateTime-value from */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
const Time& AccessorPropertyTable_YIterator::getTime( const ColumnIndex index ) const
{
	checkValid();
	return m_pCacheController->m_Caching->getTime( index, *this );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw
\param index index of column to set binary-value to
\param value binary-value to set to column */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
    void AccessorPropertyTable_YIterator::setBinary(const ColumnIndex index, const Binary & value)
    {
        checkValid();
        m_pCacheController->m_Caching->checkIsAllowedToSet();
        m_pCacheController->m_Caching->checkIsRowLongerAvailable(*this);

        m_PtYit.setBinary(index, value);
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    /*!	\throw no-throw
    \param rColName name of column to set binary-value to
    \param value binary-value to set to column */
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    void AccessorPropertyTable_YIterator::setBinary(const ColumnName & rColName, const Binary & value)
    {
        checkValid();
        m_pCacheController->m_Caching->checkIsAllowedToSet();
        m_pCacheController->m_Caching->checkIsRowLongerAvailable(*this);

        m_PtYit.setBinary(rColName, value);
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    /*!	\throw no-throw
    \return binary value of column
    \param  rColName name of column to get decimal-value from */
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    const Binary & AccessorPropertyTable_YIterator::getBinary(const ColumnName & rColName) const
    {
        checkValid();
        return m_pCacheController->m_Caching->getBinary(rColName, *this);
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    /*!	\throw no-throw
    \return binary value of column
    \param  index index of column to get decimal-value from */
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    const Binary & AccessorPropertyTable_YIterator::getBinary(const ColumnIndex index) const
    {
        checkValid();
        return m_pCacheController->m_Caching->getBinary(index, *this);
    }
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw  
	\param rColName name of column to set decimal-value to
	\param value decimal-value to set to column */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void AccessorPropertyTable_YIterator::setDecimal(const ColumnName & rColName, const Decimal value )
{
	checkValid();
	m_pCacheController->m_Caching->checkIsAllowedToSet();
	m_pCacheController->m_Caching->checkIsRowLongerAvailable(*this);

	m_PtYit.setDecimal(rColName,value);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw  
	\param index index of column to set decimal-value to
	\param value decimal-value to set to column */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void AccessorPropertyTable_YIterator::setDecimal(const ColumnIndex index, const Decimal value )
{
	checkValid();
	m_pCacheController->m_Caching->checkIsAllowedToSet();
	m_pCacheController->m_Caching->checkIsRowLongerAvailable(*this);

	m_PtYit.setDecimal(index,value);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return decimal value of column 
	\param  rColName name of column to get decimal-value from */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
const Decimal& AccessorPropertyTable_YIterator::getDecimal(const ColumnName & rColName ) const
{
	checkValid();
	return m_pCacheController->m_Caching->getDecimal(rColName, *this);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return decimal value of column 
	\param  index index of column to get decimal-value from */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
const Decimal& AccessorPropertyTable_YIterator::getDecimal(const ColumnIndex index ) const
{
	checkValid();
	return m_pCacheController->m_Caching->getDecimal(index, *this);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw	InvalidIteratorException */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void AccessorPropertyTable_YIterator::checkValid() const
{
	FUNCTION_NAME( "basar.db.ascpect.AccessorPropertyTable_YIterator.checkValid() const" )	

	if( m_pCacheController.get() == 0 ){
		throw InvalidIteratorException(ExceptInfo( fun, 
												   "Iterator is not initialized/valid!", 
												   __FILE__, __LINE__) );
	}
}
/*! \param	rColName passed column name referring to the xiterator.
	\param	eState is the passed state which has to be set to the passed column name.
	\throw	no-throw */ 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AccessorPropertyTable_YIterator::setPropertyState( const ColumnName& rColName, const SupportedStateEnum eState )
{
	m_PtYit.setPropertyState( rColName, eState );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	strm std::ostream that is converted
	\throw	no-throw */ 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AccessorPropertyTable_YIterator::toStream( std::ostream& strm /*= std::cout*/ ) const
{
	m_PtYit.toStream( strm );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \retval true if property table belonging to yiterator is not initialzed 
	\return otherwise returns false
	\throw	no throw */ 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool	AccessorPropertyTable_YIterator::isNull(  ) const
{
	return m_PtYit.isNull();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	AccessorPropertyTable_YIterator is the iterator with the copied values.
	\throw	no throw */ 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTable_YIterator AccessorPropertyTable_YIterator::cloneToUnboundedPropertyRow()
{
	PropertyDescriptionListRef pdl( m_pCacheController->getPropertyDescriptionList() );

	// welchen Zustand haben die einzelnen xits des yit;
	// welche Konsequenz hat der insert mit FOR_UNKNOWN;
	AccessorPropertyTable_YIterator yit = 
		AccessorPropertyTableRef( CacheController::create(pdl) ).insert( FOR_UNKNOWN );

	for( property::PropertyTable_XIterator xitSrc = m_PtYit.begin(), 
		                                   xitDst = yit.m_PtYit.begin(); 
		 xitSrc != m_PtYit.end(); 
	     ++xitSrc, ++xitDst )
	{
		if( xitSrc.getState().getState() == SS_UNSET )
			continue;

		switch (xitSrc.getType().getType())
		{
		case INT16:
			xitDst.setInt16( xitSrc.getInt16() );
			break;

		case INT32:
		case INTTIME:
		case INTDATE:
			xitDst.setInt32( xitSrc.getInt32() ); 
			break;

		case FLOAT32:
			xitDst.setFloat32( xitSrc.getFloat32() );
			break;

		case FLOAT64:
			xitDst.setFloat64( xitSrc.getFloat64() );
			break;

		case DECIMAL:
			xitDst.setDecimal( xitSrc.getDecimal() ); 
			break;

		case STRING:
			xitDst.setString( xitSrc.getString() );
			break;

		case DATETIME:
			xitDst.setDateTime( xitSrc.getDateTime() ); 
			break;

		case DATE:
			xitDst.setDate( xitSrc.getDate() ); 
			break;

		case TIME:
			xitDst.setTime( xitSrc.getTime() ); 
			break;

        case TIMESPAN:
            xitDst.setTimeSpan(xitSrc.getTimeSpan());
            break;

        case BINARY:
            xitDst.setBinary(xitSrc.getBinary());
            break;

		default:
			break;
		}
	}

	return 	yit;
}

///////////////////////////////////////////////////////////////////////////////////////////
/*! \throw	no-throw 
 *  \return true if xit and yit have equal value for given attribute colname, false otherwise
 */
bool AccessorPropertyTable_YIterator::cmpVal(SupportedTypeEnum                         eType,
											 const VarString                         & colname,	
											 const property::PropertyTable_XIterator & xit,
											 const AccessorPropertyTable_YIterator   & yit) const
{
	switch (eType)
	{
	case INT16:
		return xit.getInt16() == yit.getInt16(colname);

	case INT32:
	case INTTIME:
	case INTDATE:
		return xit.getInt32() == yit.getInt32(colname);

	case FLOAT32:
		return xit.getFloat32() == yit.getFloat32(colname);

	case FLOAT64:
		return xit.getFloat64() == yit.getFloat64(colname);

	case DECIMAL:
		return xit.getDecimal() == yit.getDecimal(colname);

	case STRING:
		return xit.getString() == yit.getString(colname);

	case DATETIME:
		return xit.getDateTime() == yit.getDateTime(colname);

	case DATE:
		return xit.getDate() == yit.getDate(colname);

	case TIME:
		return xit.getTime() == yit.getTime(colname);

    case TIMESPAN:
        return xit.getTimeSpan() == yit.getTimeSpan(colname);

    case BINARY:
        return xit.getBinary() == yit.getBinary(colname);

	default:
		return false;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////
/*! 
    \retval true  if the values of the parameter rhs are equal to the original yiterator 
                  (this) with constraints of the passed parameterlist. See further information 
                  in the description of the method and the other parameter pl.
    \retval false otherwise
    \param  rhs   yiterator which values have to be compared with the values of the 
                  original yiterator (this).
    \param  pl    ParameterList containing the attributes which have to be considered 
	              for the comparison only. \n
			      passed YIterator's attributes not existing in ParameterList will be skipped. \n
			      Attributes in ParameterList not existing in the passed yiterator will also 
			      be skipped. \n
			      An empty parameterList is not taken into account.
    \throw	      InvalidIteratorException 
*/ 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::pair< bool, cmnutil::ParameterList > 
AccessorPropertyTable_YIterator::compareSetValues(const AccessorPropertyTable_YIterator rhs,
												  const cmnutil::ParameterList        & pl )
{
	FUNCTION_NAME( "basar.db.ascpect.AccessorPropertyTable_YIterator.compareSetValues()" )	

	if ( isEnd() )
	{
		throw InvalidIteratorException(ExceptInfo(fun, 
												  "YIterator (lhs) is end()", 
												  __FILE__, __LINE__) );
	}

	if ( rhs.isEnd() )
	{
		throw InvalidIteratorException(ExceptInfo(fun, 
												  "YIterator (rhs) is end()", 
												  __FILE__, __LINE__) );
	}

	static ConstString pv  = "values not equal!";
	static ConstString psl = "not contained and set for lhs YIterator!";
	static ConstString psr = "not contained and set for rhs YIterator!";
	static ConstString pt  = "types not equal!";

	cmnutil::ParameterList plErr;

	for (property::PropertyTable_XIterator xitRhs = rhs.m_PtYit.begin(); 
		 xitRhs != rhs.m_PtYit.end(); 
		 ++xitRhs )
	{
		ColumnName colname = xitRhs.getName();

		// evaluate parameterlist's attributes
		if( (0 < pl.size()) && ( !pl.isKeyContained(colname) ) )
			continue;

		if ( rhs.isContainedAndSet(colname)  )
		{
			if (isContainedAndSet(colname))
			{
				SupportedTypeEnum eType = xitRhs.getType().getType();

				if (eType == getType(colname).getType())
				{
					if ( ! cmpVal(eType, colname, xitRhs, *this) )
						plErr.push_back(colname, pv );	// value
				}
				else
					plErr.push_back( colname, pt );		// type
			}
			else
				plErr.push_back( colname, psl );		// contained and state
		}
		else
		{
			if (isContainedAndSet(colname))
				plErr.push_back( colname, psr );		// contained and state
		}
	}

	return std::make_pair(plErr.size() == 0, plErr);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \param	rhs is the yiterator which values have to be set to the original yiterator (this)
	\throw	no throw */ 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AccessorPropertyTable_YIterator::match( AccessorPropertyTable_YIterator rhs )
{
	for (property::PropertyTable_XIterator xitRhs = rhs.m_PtYit.begin();
	 	 xitRhs != rhs.m_PtYit.end();
		 ++xitRhs )
	{
		ColumnName colname = xitRhs.getName();

		if (rhs.isContainedAndSet(colname) )
		{
			for (property::PropertyTable_XIterator xitLhs = m_PtYit.begin(); 
				 xitLhs != m_PtYit.end(); 
				 ++xitLhs )
			{	
				if (colname == xitLhs.getName() )
				{
					switch (xitRhs.getType().getType())
					{
					case INT16:
						xitLhs.setInt16( xitRhs.getInt16() );
						break;

					case INT32:
					case INTDATE:
					case INTTIME:
						xitLhs.setInt32( xitRhs.getInt32() );
						break;

					case FLOAT32:
						xitLhs.setFloat32( xitRhs.getFloat32() );
						break;

					case FLOAT64:
						xitLhs.setFloat64( xitRhs.getFloat64() );
						break;

					case DECIMAL:
						xitLhs.setDecimal( xitRhs.getDecimal() );
						break;

					case STRING:
						xitLhs.setString( xitRhs.getString() );
						break;

					case DATETIME:
						xitLhs.setDateTime( xitRhs.getDateTime() );
						break;

					case DATE:
						xitLhs.setDate( xitRhs.getDate() );
						break;

					case TIME:
						xitLhs.setTime( xitRhs.getTime() );
						break;

                    case TIMESPAN:
                        xitLhs.setTimeSpan(xitRhs.getTimeSpan());
                        break;

                    case BINARY:
                        xitLhs.setBinary(xitRhs.getBinary());
                        break;

					default:
						break;
					} // switch

					break;	// xitLhs with colname found => break inner loop
				}	// if
			}// end for dst( this )
		}	// if 
	} // end for src( rhs )
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return const CacheControllerSharedPtr 
	\throw	no throw */ 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
const CacheControllerSharedPtr AccessorPropertyTable_YIterator::getCacheControllerSharedPtr() const
{
	return m_pCacheController;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return const CacheControllerSharedPtr as AccessorPropertyTableRef
	\throw	no throw */ 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
const AccessorPropertyTableRef AccessorPropertyTable_YIterator::getPropertyTable() const
{
    return AccessorPropertyTableRef(m_pCacheController);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return the number of columns 
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
ColumnIndex AccessorPropertyTable_YIterator::getNumberOfColumns() const 
{
    checkValid();
    return m_PtYit.size();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return the name of one column 
	\param	index columnIndex
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
ColumnName AccessorPropertyTable_YIterator::getName(ColumnIndex index) const 
{
	checkValid();
	return m_PtYit.getName(index);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void AccessorPropertyTable_YIterator::markForDelete()
{
    m_pCacheController->markForDelete( *this );
}

} // aspect
} // db
} // basar
