//---------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  class implementation of cacheController
 *  \author Roland Kiefert
 *  \date   18.08.2005
 */
//---------------------------------------------------------------------------------------------//

//---------------------------------------------------------------------------------------------//
// includes
//---------------------------------------------------------------------------------------------//

#include "cachecontroller.h"

#include "nocaching.h"
#include "singlerowcaching.h"
#include "fullcaching.h"
#include "ondemandcaching.h"
#include "../libbasarproperty/propertytable.h"
#include "libbasardbaspect_accessorpropertytable_yiterator.h"

//---------------------------------------------------------------------------------------------//
// namespaces
//---------------------------------------------------------------------------------------------//
namespace basar  {
namespace db	 {
namespace aspect {

//---------------------------------------------------------------------------------------------//
// static section
//---------------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	shared pointer to CacheController implementation object
//! \param  propDescListRef property-description-list to set to cachecontroller
//! \param  eCachePolicy caching policy, default ON_DEMAND_CACHING
//! \throw  no-throw
////////////////////////////////////////////////////////////////////////////////////////////////
CacheController::CacheControllerSharedPtr CacheController::create( 
									const property::PropertyDescriptionListRef & propDescListRef, 
									const CachingPolicyEnum                      eCachePolicy /*= ON_DEMAND_CACHING*/ )
{
	CacheControllerSharedPtr pShared(new CacheController(propDescListRef, eCachePolicy));
	pShared->setWeakPtr(pShared); // save in weak pointer

	switch (eCachePolicy)
	{
	case NO_CACHING:
		pShared->m_Caching = boost::shared_ptr<CachingPolicy>( new NoCaching(pShared.get()) );		
		break;

	case SINGLE_ROW_CACHING:
		pShared->m_Caching = boost::shared_ptr<CachingPolicy>( new SingleRowCaching(pShared.get()) );
		break;

	case FULL_CACHING:
		pShared->m_Caching = boost::shared_ptr<CachingPolicy>( new FullCaching(pShared.get()) );
		break;

	case ON_DEMAND_CACHING:
	default:
		pShared->m_Caching = boost::shared_ptr<CachingPolicy>( new OnDemandCaching(pShared.get()) );
		break;
	}

	return pShared;
}

//---------------------------------------------------------------------------------------------//
// object section
//---------------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////
//! \throw  no-throw
//! \param  propDescListRef property-description-list to set to cachecontroller
//! \param  eCachePolicy caching policy, default ON_DEMAND_CACHING
////////////////////////////////////////////////////////////////////////////////////////////////
CacheController::CacheController( const property::PropertyDescriptionListRef & propDescListRef, 
								  const CachingPolicyEnum                      eCachePolicy /* = ON_DEMAND_CACHING */)
	: m_PropertyTableRef      (property::PropertyTableRef(property::PropertyTable::create(propDescListRef))),
	  m_CachePolicy           (eCachePolicy ), 
	  m_createInternalSnapshot( ((m_CachePolicy == NO_CACHING        ) || 
	                             (m_CachePolicy == SINGLE_ROW_CACHING)    ) ? false : true )
{
	m_Resultset.reset();
	m_Statement.reset();
	m_Index.clear();
}
////////////////////////////////////////////////////////////////////////////////////////////////
//! \throw  no-throw
//! \return shared_ptr to CachingPolicy instance
////////////////////////////////////////////////////////////////////////////////////////////////
boost::shared_ptr<CachingPolicy> CacheController::giveCachingPolicy() const
{
	return m_Caching;
}

//---------------------------------------------------------------------------------------------//
// iterator section
//---------------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////
//! \return yiterator to first row
//! \throw  no-throw
////////////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTable_YIterator CacheController::begin()
{
	return m_Caching->beginRow();
}

////////////////////////////////////////////////////////////////////////////////////////////////
//! \return yiterator to first row with given state in propertystate
//! \throw  InvalidValueException
//! \param  state propertystate to search for
////////////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTable_YIterator CacheController::begin( const PropertyStateSet& state )
{
	return m_Caching->beginRow( state );
}

////////////////////////////////////////////////////////////////////////////////////////////////
//! \return yiterator to last row
//! \throw  no-throw
////////////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTable_YIterator CacheController::end()
{
	return AccessorPropertyTable_YIterator(getThis(), m_PropertyTableRef.end(), ITERATOR_ENDPOS); 
}

AccessorPropertyTable_YIterator CacheController::at( const basar::Int32 row )
{
    return m_Caching->atRow( row );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \return empty yiterator to new row
//! \throw  InvalidStateException
//! \param  eInsertTypeEnum type which new row should have
///////////////////////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTable_YIterator CacheController::insert( InsertTypeEnum eInsertTypeEnum )
{
	return m_Caching->insert( eInsertTypeEnum );
}

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \throw  OutOfRangeIteratorException
	\param	aptYIt iterator to line which is deleted at next execute
	\return	AccessorPropertyTable_YIterator is the iterator pointing to the successor 
			element of the erased one. */
////////////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTable_YIterator CacheController::erase( AccessorPropertyTable_YIterator& aptYIt )
{
	return m_Caching->erase( aptYIt );
}

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \throw  OutOfRangeIteratorException
	\param	yItBegin iterator to line where delete starts
	\param	yItEnd iterator to line where delete ends
	\return	AccessorPropertyTable_YIterator is the iterator pointing to the successor 
			element of the erased ones. */
////////////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTable_YIterator CacheController::erase( AccessorPropertyTable_YIterator & yItBegin, 
													    AccessorPropertyTable_YIterator & yItEnd )
{
	return m_Caching->erase( yItBegin, yItEnd );
}

void CacheController::erase ( const PropertyStateSet& /*state*/ )
{
    //return m_Caching->erase( state );
}

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \throw no-throw
	\param yit iterator whose propertystates are set to SS_CLEAN 
	\return AccessorPropertyTable_YIterator removed of entries in state SS_DELETE, other are set to SS_CLEAN
*/
////////////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTable_YIterator CacheController::executePerformed( AccessorPropertyTable_YIterator yit )
{
	yit.m_PtYit = m_PropertyTableRef.executePerformed( yit.m_PtYit );
	return yit;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//! \throw  no-throw
//! \param  yit line which should be deleted at next execute
////////////////////////////////////////////////////////////////////////////////////////////////
void CacheController::markForDelete( AccessorPropertyTable_YIterator & yit )
{
	m_Caching->markForDelete( yit );
}

////////////////////////////////////////////////////////////////////////////////////////////////
//! \throw  no-throw
//! \param  yit iterator pointing to a row whose states are set to SS_UNKNOWN
////////////////////////////////////////////////////////////////////////////////////////////////
void CacheController::setStateUnknown( AccessorPropertyTable_YIterator & yit )
{
	m_Caching->setStateUnknown( yit );
}

//////////////////////////////////////////////////////////////////////////////////
//! \throw  no-throw
//! \return PropertyTableSnapshotRef referecing snapshot of property table
//////////////////////////////////////////////////////////////////////////////////
const PropertyTableSnapshotRef CacheController::createSnapshot() const
{
	return m_Caching->createSnapshot();
}

//////////////////////////////////////////////////////////////////////////////////
//! \throw  no-throw
//! \param  snapShot snapshot of propertytable to which should be resetted
//////////////////////////////////////////////////////////////////////////////////
void CacheController::resetToSnapshot( const PropertyTableSnapshotRef snapShot )
{
	m_PropertyTableRef.resetToSnapshot( snapShot );
}

////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	reference to property description list
//! \throw  no-throw
////////////////////////////////////////////////////////////////////////////////////////////////
const PropertyDescriptionListRef CacheController::getPropertyDescriptionList() const
{
	return m_PropertyTableRef.getPropertyDescriptionList();
}

////////////////////////////////////////////////////////////////////////////////////////////////
//! \return (only one group of) CacheController shared pointer 
//! \throw  no-throw
////////////////////////////////////////////////////////////////////////////////////////////////
CacheController::CacheControllerSharedPtr CacheController::getThis() const
{
	return CacheControllerSharedPtr(m_WeakPtr);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//! \throw  no-throw
//! \param weakPtr pointer to set to member
////////////////////////////////////////////////////////////////////////////////////////////////
void CacheController::setWeakPtr( CacheControllerWeakPtr weakPtr )
{
	m_WeakPtr = weakPtr;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//! \retval true if propertytable is empty
//! \return otherwise returns false (in case of NO_CACHING resultset is checked if results are present)
//! \throw  no-throw
////////////////////////////////////////////////////////////////////////////////////////////////
bool CacheController::empty() const
{
    if ( NO_CACHING == getCachingPolicy() )
    {
        //query resultset as we have no entries in property table
	    if ( !m_Resultset.isNull() && (0 < m_Resultset.getFetchedRows()) )
        {
		    return false;
	    }
    }

    return m_PropertyTableRef.empty(); 
}

////////////////////////////////////////////////////////////////////////////////////////////////
//! \return size of completely read resultset / propertytable
//! \throw  InvalidValueException
////////////////////////////////////////////////////////////////////////////////////////////////
NumberRows CacheController::size() const
{
	return m_Caching->size();
}

////////////////////////////////////////////////////////////////////////////////////////////////
NumberRows CacheController::getNumberOfFetchedRows() const
{
    NumberRows retval = 0;
	
	if (!m_Resultset.isNull() )
		retval = m_Resultset.getFetchedRows();

	return retval;
}

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return maximum size, limited by propertytable
	\throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
MaxSizeRows CacheController::max_size() const
{
	return m_PropertyTableRef.max_size(); // resultset not limited
}

////////////////////////////////////////////////////////////////////////////////////////////////
//! \throw  no-throw
////////////////////////////////////////////////////////////////////////////////////////////////
void CacheController::clear()
{
	m_PropertyTableRef.clear();
	m_Resultset       .reset();
	m_Statement       .reset();
	m_Index           .clear();
}
////////////////////////////////////////////////////////////////////////////////////////////////
//! \throw  no-throw
//! \param  Statement sql-statement to set to member
////////////////////////////////////////////////////////////////////////////////////////////////
void CacheController::setStatement( const db::sql::StatementRef Statement )
{
	m_Statement = Statement;
}
////////////////////////////////////////////////////////////////////////////////////////////////
//! \throw  no-throw
//! \param  Resultset resultset to set to CacheController
////////////////////////////////////////////////////////////////////////////////////////////////
void CacheController::setResultset( const db::sql::ResultsetRef Resultset )
{
	m_Resultset = Resultset;
	m_Index.clear();
}
////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	the current caching policy
//! \throw  no-throw
////////////////////////////////////////////////////////////////////////////////////////////////
CachingPolicyEnum CacheController::getCachingPolicy() const
{
	return m_CachePolicy;
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \retval	true if the internal snapshot should be created
	\return otherwise returns false
	\throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
bool CacheController::createInternalSnapshot()
{
	if ( m_createInternalSnapshot )
		m_SnapshotPTRef = createSnapshot();

	return m_createInternalSnapshot;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//! \throw  no-throw
////////////////////////////////////////////////////////////////////////////////////////////////
void CacheController::resetInternalSnapshot()
{
	if ( m_createInternalSnapshot )
		resetToSnapshot( m_SnapshotPTRef );
}

////////////////////////////////////////////////////////////////////////////////////////////////
//! \throw  no-throw
////////////////////////////////////////////////////////////////////////////////////////////////
void CacheController::clearInternalSnapshot()
{
	m_SnapshotPTRef.clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	toCreate indicates if the internal snapshot should be created if an sql execute 
			was called. It is not allowed to switch while the caching policies 'NO_CACHING' 
			and 'SINGLE_ROW_CACHING' are chosen. In these cases the InvalidValueException
			is thrown.
	\throw	InvalidValueException */
////////////////////////////////////////////////////////////////////////////////////////////////
void CacheController::switchCreateInternalSnapshot( bool toCreate )
{
	m_Caching->switchCreateInternalSnapshot( toCreate );
}

//--------------------------------------------------------------------------------------------//
} // aspect
} // db
} // basar
