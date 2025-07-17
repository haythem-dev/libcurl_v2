//---------------------------------------------------------------------------------------------//
//! \file
//! \brief  internal implementation for the caching policy 'on demand caching'
//! \author Bjoern Bischof
//! \date   10.10.2006
//---------------------------------------------------------------------------------------------//

//---------------------------------------------------------------------------------------//
// includes
//---------------------------------------------------------------------------------------//

#include "ondemandcaching.h"

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
//! \param  cacheController is passed to the adequate overloaded c'tor of the basic class.
////////////////////////////////////////////////////////////////////////////////////////////////
OnDemandCaching::OnDemandCaching( CacheController* cacheController  )
:CachingPolicy( cacheController )
{
}
////////////////////////////////////////////////////////////////////////////////////////////////
//! \throw  no-throw
////////////////////////////////////////////////////////////////////////////////////////////////
OnDemandCaching::~OnDemandCaching()
{
}
////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	void
//! \param  r is the AccessorPropertyTable_YIterator which is the prae-++operator does on.
//! \throw  no-throw
////////////////////////////////////////////////////////////////////////////////////////////////
void OnDemandCaching::preIncrement(AccessorPropertyTable_YIterator& r)
{
	++(r.m_PtYit);
}
////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	void
//! \param  r is the AccessorPropertyTable_YIterator which is the post-++operator does on.
//! \throw  no-throw
////////////////////////////////////////////////////////////////////////////////////////////////
void OnDemandCaching::postIncrement(AccessorPropertyTable_YIterator& r)
{
	(r.m_PtYit)++;
}
////////////////////////////////////////////////////////////////////////////////////////////////
//! \retval	always true - only important to detect if the inner call to basic class has done the work
//! \param  replicated was neccessary for other derived classes
//! \param  r is the AccessorPropertyTable_YIterator which the internal row number is updated for.
//! \throw  no-throw
////////////////////////////////////////////////////////////////////////////////////////////////
bool OnDemandCaching::updateRowNumber( const bool                        replicated, 
									   AccessorPropertyTable_YIterator & r )
{
	if( ! CachingPolicy::updateRowNumber(replicated, r) )
		r.m_RowNumber += r.m_PtYit.getFilteredStep();

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//! \return PropertyTableSnapshotRef representing the newly created snapshot of the propertytable
//! \throw  no-throw
////////////////////////////////////////////////////////////////////////////////////////////////
const PropertyTableSnapshotRef OnDemandCaching::createSnapshot() const
{
	return m_pCacheController->m_PropertyTableRef.createSnapshot();
}

////////////////////////////////////////////////////////////////////////////////////////////////
//! \param	yit AccessorPropertyTable YIterator which setStateUnknown() is executed on.
//! \throw  no-throw
//!
////////////////////////////////////////////////////////////////////////////////////////////////
void OnDemandCaching::setStateUnknown( AccessorPropertyTable_YIterator & yit )
{
	m_pCacheController->m_PropertyTableRef.setStateUnknown( yit.m_PtYit );
}

////////////////////////////////////////////////////////////////////////////////////////////////
//! \return AccessorPropertyTable_YIterator represents one dataset after the erased one.
//! \param	yItBegin is the first dataset of the interval that has to be deleted
//! \param	yItEnd is the iterator one beyond the last erased one
//! \throw  no-throw
////////////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTable_YIterator OnDemandCaching::erase( AccessorPropertyTable_YIterator & yItBegin, 
													    AccessorPropertyTable_YIterator & yItEnd )
{
	property::PropertyTable_YIterator yIt = m_pCacheController->m_PropertyTableRef.erase(yItBegin.m_PtYit, 
																						 yItEnd.m_PtYit);

	Int32 row = (yIt == m_pCacheController->m_PropertyTableRef.end()) 
			  ? ITERATOR_ENDPOS 
			  : yItBegin.m_RowNumber;

	return AccessorPropertyTable_YIterator( m_pCacheController->getThis(), yIt, row );	
}

////////////////////////////////////////////////////////////////////////////////////////////////
//! \return AccessorPropertyTable_YIterator represents one dataset after the erased one.
//! \param	aptYIt is the dataset that has to be deleted from the propertytable
//! \throw  no-throw
////////////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTable_YIterator OnDemandCaching::erase( AccessorPropertyTable_YIterator& aptYIt )
{
	replicate(aptYIt.m_RowNumber + 1);

	property::PropertyTable_YIterator yIt = m_pCacheController->m_PropertyTableRef.erase( aptYIt.m_PtYit );
	
	Int32 row = (yIt == m_pCacheController->m_PropertyTableRef.end()) 
			  ? ITERATOR_ENDPOS 
			  : aptYIt.m_RowNumber;

	return AccessorPropertyTable_YIterator( m_pCacheController->getThis(), yIt, row );	
}

////////////////////////////////////////////////////////////////////////////////////////////////
//! \return AccessorPropertyTable_YIterator represents an iterator that points to newly inserted dataset.
//! \param	eInsertTypeEnum determines the kind of insert. Important for state transitions.
//! \throw  InsertNotAllowedException
////////////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTable_YIterator OnDemandCaching::insert( InsertTypeEnum eInsertTypeEnum )
{
	static ConstString fun = "basar.db.aspect.OnDemandCaching.insert()";

	// no valid insert type given
	if( !m_pCacheController->m_Resultset.isNull     () && 
		!m_pCacheController->m_Resultset.isAfterLast()   )
	{
		VarString msg;
		msg.format("with caching policy '%d' inserts of type '%d' are allowed only "
				   "if resultset is not used at all or it has been completely read.",
				   m_pCacheController->getCachingPolicy(),
				   eInsertTypeEnum);

		throw InsertNotAllowedException(ExceptInfo( fun, msg, __FILE__, __LINE__) );
	}

	property::PropertyTable_YIterator yItPropTab = m_pCacheController->m_PropertyTableRef.insert( eInsertTypeEnum );
	
	//	the method 'insert()' will always append (insert at the end) a new AccessorPropertyTable.
	//	Thus the third parameter (representing the current row number) of the AccessorPropertyTable_YIterator c'tor
	//	can be provided by the size of the PropertyTable
	return AccessorPropertyTable_YIterator( m_pCacheController->getThis(), 
											yItPropTab, 
											static_cast<Int32>( m_pCacheController->m_PropertyTableRef.size() - 1 )
										  );
}

AccessorPropertyTable_YIterator OnDemandCaching::atRow( const basar::Int32 row )
{
    static ConstString fun = "basar.db.aspect.OnDemandCaching.atRow()";

    const basar::property::NumberRows rows = m_pCacheController->m_PropertyTableRef.size();

    if ( static_cast<basar::property::NumberRows> (row) > rows )
    {
        //try replicating until row
        if ( false == replicate( row ) )
        {
            VarString msg;
            msg.format("requested row <%d> is larger than PropTab size", row );

            throw OutOfRangeIndexException( ExceptInfo( fun, msg, __FILE__, __LINE__ ) );
        }
    }

    property::PropertyTable_YIterator yItPropTab = m_pCacheController->m_PropertyTableRef.at( row );
    return AccessorPropertyTable_YIterator( m_pCacheController->getThis(), yItPropTab, row );
}

////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	Int32 
//! \throw  no-throw
////////////////////////////////////////////////////////////////////////////////////////////////
Int32 OnDemandCaching::getIndex4EmptyResult() const
{
	if( !m_pCacheController->m_PropertyTableRef.empty() )
		return 0;

	return CachingPolicy::getIndex4EmptyResult();
}
////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	ITERATOR_ENDPOS or 0
//! \throw  no-throw
////////////////////////////////////////////////////////////////////////////////////////////////
Int32 OnDemandCaching::getIndex4HitResult() const
{
	if( m_pCacheController->m_PropertyTableRef.empty() )
		return ITERATOR_ENDPOS;

	return CachingPolicy::getIndex4HitResult();
}
////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	ITERATOR_ENDPOS or number of steps caused by filtered iteration 
/*! \param  yit is the PropertyTable_YIterator which is consulted to decide 
			if the current position is the end of the propertyTable. */
//! \throw  no-throw
////////////////////////////////////////////////////////////////////////////////////////////////
Int32 OnDemandCaching::getIndex4FilterResult( property::PropertyTable_YIterator& yit ) const
{
	if( !m_pCacheController->m_PropertyTableRef.empty() )
	{ 
		if ( yit != m_pCacheController->m_PropertyTableRef.end() ){
			return static_cast<Int32>( yit.getFilteredStep() ); 
		}
	}

	return CachingPolicy::getIndex4FilterResult( yit );
}

//---------------------------------------------------------------------------------------------//
} // aspect
} // db
} // basar
