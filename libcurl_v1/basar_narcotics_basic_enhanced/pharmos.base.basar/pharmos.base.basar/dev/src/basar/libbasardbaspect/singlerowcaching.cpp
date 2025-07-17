//---------------------------------------------------------------------------------------------//
//! \file
//! \brief  internal implementation for the caching policy 'no caching'
//! \author Bjoern Bischof
//! \date   10.10.2006
//---------------------------------------------------------------------------------------------//

//---------------------------------------------------------------------------------------//
// includes
//---------------------------------------------------------------------------------------//

#include "singlerowcaching.h"

#include "libbasardbaspect_accessorpropertytable_yiterator.h"
#include "cachecontroller.h"
#include "libbasardbaspect_exceptions.h"

//---------------------------------------------------------------------------------------------//
// namespaces
//---------------------------------------------------------------------------------------------//
namespace basar  {
namespace db	 {
namespace aspect {

//---------------------------------------------------------------------------------------------//
// object section
//---------------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////
//! \throw  no-throw
/*! \param  cacheController here a plain pointer to the concerning cacheController. 
			It is stored as a member to delegate severals methods. */
////////////////////////////////////////////////////////////////////////////////////////////////
SingleRowCaching::SingleRowCaching( CacheController* cacheController  )
:CachingPolicy( cacheController )
{
}
////////////////////////////////////////////////////////////////////////////////////////////////
//! \throw  no-throw
////////////////////////////////////////////////////////////////////////////////////////////////
SingleRowCaching::~SingleRowCaching()
{
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \retval	true if the passed AccessorPropertyTable_YIterator represents the first iterator (begin())
  ! \retval	otherwise false
//! \param  r is the AccessorPropertyTable_YIterator which internal cachController is asked for 
			the begin()-method.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
bool SingleRowCaching::isBegin( const AccessorPropertyTable_YIterator& r  ) const
{ 
	return (0 == r.m_RowNumber);
}

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \retval	true if the passed AccessorPropertyTable_YIterator represents one position after 
			the last iterator (end())
  ! \retval	otherwise false
//! \param  r is the AccessorPropertyTable_YIterator which internal cachController is asked for 
			the end()-method.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
bool SingleRowCaching::isEnd( const AccessorPropertyTable_YIterator& r  ) const
{ 
	return ( ITERATOR_ENDPOS == r.m_RowNumber );
}

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \retval	true if the internal row number is updated
  ! \retval	otherwise false
  ! \param  replicated is used in dewrived classes
  ! \param  r is the AccessorPropertyTable_YIterator which is controlled internally 
			if it is equivalent to an end()-iterator.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
bool SingleRowCaching::updateRowNumber( const bool replicated, AccessorPropertyTable_YIterator& r   )
{
	if( ! CachingPolicy::updateRowNumber(replicated, r) )
	{
		if( replicated )
			r.m_RowNumber = static_cast<Int32>(r.m_pCacheController->m_Resultset.getFetchedRows()) - 1;
		else 
		{
			++(r.m_PtYit);
			r.m_RowNumber = ITERATOR_ENDPOS;
		}
	}

	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	PropertyStateSet representing the internal filter (important for filtered iteration)
  ! \param  r is the AccessorPropertyTable_YIterator which is the getFilter()-method delegated to.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
const PropertyStateSet& SingleRowCaching::getFilter(const AccessorPropertyTable_YIterator & ) const
{
	static const char* const fun = "basar.db.ascpect.SingleRowCaching.getFilter()";

	if (true)
	{
		throw NoFilterAvailableException(ExceptInfo(fun,
													"with caching policy 'SINGLE_ROW_CACHING' "
													"it is not allowed to ask for filter "
													"(propertystateset)!",
													__FILE__, __LINE__ ) );
	}

    //for compiler warning only
    static const PropertyStateSet pss;

	return pss;
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	PropertyStateSet representing the internal state (important for changing states).
  ! \param  r is the AccessorPropertyTable_YIterator which is the getState()-method delegated to.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
const PropertyStateSet SingleRowCaching::getState( const AccessorPropertyTable_YIterator & ) const
{
	static const char* const fun = "basar.db.ascpect.SingleRowCaching.getState()";

	if (true)
	{
		throw NoStateAvailableException(ExceptInfo(	fun, 
													"with caching policy 'SINGLE_ROW_CACHING' "
													"it is not allowed to ask for state!",
													__FILE__, __LINE__ ) );	
	}

	return PropertyStateSet();
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \retval	true if the given attribut name has the passed datatytpe and has furthermore a state
			which occurs in the passed PropertyStateSet.
  ! \retval	otherwise false
  ! \param  rColName is the attributname
  ! \param  pt is the datatype
  ! \param  pss is the set of states which is looked up in 
  ! \param  r is the AccessorPropertyTable_YIterator which is the contains()-method is executed on.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
bool SingleRowCaching::contains(	const ColumnName &, 
									const PropertyType, 
									const PropertyStateSet, 
									const AccessorPropertyTable_YIterator & ) const
{
	static const char* const fun = "basar.db.ascpect.SingleRowCaching.contains()";

	if (true)
	{
		throw NoStateAvailableException(ExceptInfo( fun, 
													"with caching policy 'SINGLE_ROW_CACHING' "
													"it is not allowed to ask if a state is "
													"contained in propertystateset!",
													__FILE__, __LINE__ ) );
	}

	return false;
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \retval	true if the given attribut name has the passed datatytpe and has furthermore a state
			which occurs in the passed PropertyStateSet.
  ! \retval	otherwise false
  ! \param  p is a pair of an attribut name and the datatype
  ! \param  pss is the set of states which is looked up in 
  ! \param  r is the AccessorPropertyTable_YIterator which is the contains()-method is executed on.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
bool SingleRowCaching::contains(	const std::pair<ColumnName, PropertyType> &,
									const db::aspect::PropertyStateSet         , 
									const AccessorPropertyTable_YIterator     &) const
{
	static const char* const fun =	"basar.db.ascpect.SingleRowCaching.contains(pair<>)";
	
	if (true)
	{
		throw NoStateAvailableException(ExceptInfo( fun, 
													"with caching policy 'SINGLE_ROW_CACHING' "
													"it is not allowed to ask if a state is "
													"contained in propertystateset!",
													__FILE__, __LINE__ ) );	
	}

	return false;
}
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
void SingleRowCaching::checkIsRowLongerAvailable( const AccessorPropertyTable_YIterator& r ) const
{
	static const char* const fun =	"basar.db.ascpect.SingleRowCaching.checkIsRowLongerAvailable()";

	if( (static_cast<Int32>(r.m_pCacheController->m_Resultset.getFetchedRows())-1) != r.m_RowNumber )
	{
		VarString msg;
		msg.format("with caching policy 'SINGLE_ROW_CACHING' "
					"it is not allowed to get any values, if the current index of "
					"YIterator (line number <%d>) is not congruent "
					"to the number of datasets of resultset <%d>!",
					r.m_RowNumber, 
					static_cast<Int32>(r.m_pCacheController->m_Resultset.getFetchedRows())-1 );
		
		throw RowNoLongerAvailableException( ExceptInfo( fun, msg, __FILE__, __LINE__) );
	}
}
//-----------------------------------------------------------------------------------------------------------------------//
// cachecontroller specific methods
//-----------------------------------------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////
//! \return void 
/*! \param  toCreate determines if the internal snapshot will be created 
	(important in the context of transactions) */
//! \throw  no-throw
////////////////////////////////////////////////////////////////////////////////////////////////
void SingleRowCaching::switchCreateInternalSnapshot( bool)
{
	static ConstString fun = "basar.db.aspect.SingleRowCaching.switchCreateInternalSnapshot(bool)";
	
	throw InvalidValueException(ExceptInfo( fun, 
											"with caching policy 'SINGLE_ROW_CACHING' "
											"it is not allowed to switch off/on creation of internal "
											"snapshot!", 
											__FILE__, __LINE__) );
}

AccessorPropertyTable_YIterator SingleRowCaching::atRow( const basar::Int32 /*row*/ )
{

    static ConstString fun = "basar.db.aspect.SingleRowCaching.atRow()";

    throw InvalidMethodCallException( ExceptInfo ( fun, "with caching policy 'SINGLE_ROW_CACHING' it is not allowed to call method 'atRow()'", __FILE__, __LINE__ ) );
}

////////////////////////////////////////////////////////////////////////////////////////////////
//! \return property::PropertyTable_YIterator
//! \throw  no-throw
////////////////////////////////////////////////////////////////////////////////////////////////
property::PropertyTable_YIterator SingleRowCaching::prepareForReplicate()
{
    property::PropertyTable_YIterator yit = m_pCacheController->m_PropertyTableRef.begin();
    if ( true == yit.isEnd() )
    {
        return m_pCacheController->m_PropertyTableRef.insert( FOR_CLEAN );
    }
    else
    {
        yit.resetRowState();
        return yit;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////
//! \return 0 or ITERATOR_ENDPOS
//! \throw  throw RowNoLongerAvailableException */
////////////////////////////////////////////////////////////////////////////////////////////////
Int32 SingleRowCaching::getIndex4HitResult() const
{
	ConstString fun = "basar.db.aspect.SingleRowCaching.getIndex4HitResult()";
	
	switch (m_pCacheController->m_Resultset.getFetchedRows())
	{
	case 1:
		return 0;

	case 0:
		return ITERATOR_ENDPOS;

	default:
		if (true)
		{
			throw RowNoLongerAvailableException( ExceptInfo(fun, 
															"with caching policy 'SINGLE_ROW_CACHING' "
															"it is allowed to restart iteration only "
															"if max. one row has already been read. "
															"To check for equality to begin() "
															"use isBegin() instead!", 
															__FILE__, __LINE__) );
		}

		return 0;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
//! \return void
//! \throw  no-throw
////////////////////////////////////////////////////////////////////////////////////////////////
void SingleRowCaching::checkFilteredBeginAllowed() const
{ 
	ConstString fun = "basar.db.aspect.SingleRowCaching.checkFilteredBeginAllowed()";

	throw NoFilteredBeginAllowedException( ExceptInfo(	fun, 
														"with caching policy 'SINGLE_ROW_CACHING' "
														"it is not allowed to iterate in a filtered manner!", 
														__FILE__, __LINE__) );
}
////////////////////////////////////////////////////////////////////////////////////////////////
//! \throw  no-throw
//! \param  r is the AccessorPropertyTable_YIterator which is even important in successor classes.
////////////////////////////////////////////////////////////////////////////////////////////////
void SingleRowCaching::preIncrement( AccessorPropertyTable_YIterator& r )
{
	r.m_PtYit = r.m_pCacheController->m_PropertyTableRef.begin();
}

} // aspect
} // db
} // basar
