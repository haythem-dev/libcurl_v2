//////////////////////////////////////////////////////////////////////////////
/*! \file
 *  \brief  This class PropertyTableRef is a special proxy (smart reference) 
			to the class PropertyTable.
 *  \author Bischof Björn 
 *  \date   25.07.2005
 */
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// includes
//////////////////////////////////////////////////////////////////////////////

#include "libbasarproperty_propertytableref.h"

#include "libbasarproperty_propertydescriptionlistref.h"
#include "propertytable.h"
#include "libbasarproperty_propertytable_yiterator.h"
#include "libbasarproperty_propertytablesnapshotref.h"

#include "../macro.h"

//////////////////////////////////////////////////////////////////////////////
// namespaces
//////////////////////////////////////////////////////////////////////////////
namespace basar
{
namespace property
{

//////////////////////////////////////////////////////////////////////////////
/*! \param	pImpl is the shared pointer to the implementation object
	\throw	no-throw */
//////////////////////////////////////////////////////////////////////////////
PropertyTableRef::PropertyTableRef( const PropertyTableSharedPtr& pImpl )
:m_pImpl( pImpl )
{
}
//////////////////////////////////////////////////////////////////////////////
/*! \retval	true if the implementation object hasn't been instantiated yet 
	\return otherwise returns false
	\throw	no-throw  */
//////////////////////////////////////////////////////////////////////////////
bool PropertyTableRef::isNull() const
{
	return( m_pImpl.get() == 0 );
}
//////////////////////////////////////////////////////////////////////////////
//!	\return const shared pointer to implementation object
//! \throw  NullReferenceException 
//////////////////////////////////////////////////////////////////////////////
const PropertyTableSharedPtr& PropertyTableRef::getImpl() const
{
	FUNCTION_NAME( basar.property.PropertyTableRef.getImpl )
	if( isNull() ){
		throw NullReferenceException( 
			    ExceptInfo( fun, 
				            "object of type PropertyTable is not initialized", 
							__FILE__, __LINE__) );
    }
	return m_pImpl;
}
//////////////////////////////////////////////////////////////////////////////
/*! \return PropertyDescriptionListRef is a reference stored in a member.
	\throw no-throw */
//////////////////////////////////////////////////////////////////////////////
const PropertyDescriptionListRef PropertyTableRef::getPropertyDescriptionList() const
{
	return getImpl()->getPropertyDescriptionList();
}
//////////////////////////////////////////////////////////////////////////////
/*! \retval true if there are no rows in the container.
	\return otherwise returns false
	\throw no-throw */ 
//////////////////////////////////////////////////////////////////////////////
bool PropertyTableRef::empty() const
{
	return getImpl()->empty();
}
//////////////////////////////////////////////////////////////////////////////
/*! \throw no-throw */ 
//////////////////////////////////////////////////////////////////////////////
void PropertyTableRef::clear()
{
	return getImpl()->clear();
}
//////////////////////////////////////////////////////////////////////////////
/*!	\param	yit is the iterator whose state has to be changed 
			or it has to be killed if the row has the state DELETE.
	\return PropertyTable_YIterator removed of entries in state SS_DELETE, other are set to SS_CLEAN
	\throw	no-throw */
//////////////////////////////////////////////////////////////////////////////
PropertyTable_YIterator PropertyTableRef::executePerformed( PropertyTable_YIterator& yit )
{
	return getImpl()->executePerformed( yit );
}
//////////////////////////////////////////////////////////////////////////////
/*!	\return	PropertyTable_YIterator is the iterator pointing to the successor 
			element of the erased one.
	\param	yit is the iterator which has to be killed
	\throw	WrongOperationException */
//////////////////////////////////////////////////////////////////////////////
PropertyTable_YIterator	PropertyTableRef::erase( PropertyTable_YIterator& yit )
{	
	return getImpl()->erase( yit );
}
//////////////////////////////////////////////////////////////////////////////
/*!	\return	PropertyTable_YIterator is the iterator pointing to the successor 
			element of the erased one.
	\param	first is the iterator which has to be killed first
	\param	beyondLast is the successor iterator of the range of iterators 
			which have to be killed 
	\throw	WrongOperationException */
//////////////////////////////////////////////////////////////////////////////
PropertyTable_YIterator	PropertyTableRef::erase( PropertyTable_YIterator& first, 
												 PropertyTable_YIterator& beyondLast )
{
	return getImpl()->erase( first, beyondLast );
}
//////////////////////////////////////////////////////////////////////////////
/*! \throw no-throw
    \return PropertyTable_YIterator pointing to first row of PropertyTable */
//////////////////////////////////////////////////////////////////////////////
PropertyTable_YIterator	PropertyTableRef::begin()
{
	return getImpl()->begin();
}
//////////////////////////////////////////////////////////////////////////////
/*! \throw no-throw
    \return PropertyTable_YIterator pointing to one position beyond the last element */
//////////////////////////////////////////////////////////////////////////////
PropertyTable_YIterator	PropertyTableRef::end()
{
	return getImpl()->end();
}
//////////////////////////////////////////////////////////////////////////////
/*! \return PropertyTable_YIterator points to the object with the given state contained 
			in the parameter
	\param	pss	is/are the state/s the object the returned iterator points to must be in.
			\n Object only has to be in one of the given states.
	\throw	no-throw */
//////////////////////////////////////////////////////////////////////////////
PropertyTable_YIterator	PropertyTableRef::begin( const PropertyStateSet& pss )
{
	return getImpl()->begin( pss );
}
//////////////////////////////////////////////////////////////////////////////
/*!	\return	PropertyTable_YIterator pointing to the inserted row.
	\param	eInsertTypeEnum will determine the type of the inserted row.
	\throw no-throw */
//////////////////////////////////////////////////////////////////////////////
PropertyTable_YIterator PropertyTableRef::insert( const InsertTypeEnum eInsertTypeEnum )
{
	return getImpl()->insert( eInsertTypeEnum );
}
//////////////////////////////////////////////////////////////////////////////
/*! \throw	no-throw */
//////////////////////////////////////////////////////////////////////////////
void PropertyTableRef::reset()
{
	m_pImpl.reset();
}
//////////////////////////////////////////////////////////////////////////////
/*! \return ColumnSize is the max memory that can be allocated for the rows.
	\throw no-throw */ 
//////////////////////////////////////////////////////////////////////////////
MaxSizeRows PropertyTableRef::max_size() const
{
	return getImpl()->max_size();
}
//////////////////////////////////////////////////////////////////////////////
/*! \return ColumnSize is the number of the stored rows (database resultsets).
	\throw no-throw */ 
//////////////////////////////////////////////////////////////////////////////
NumberRows PropertyTableRef::size() const
{
	return getImpl()->size();
}
//////////////////////////////////////////////////////////////////////////////
/*!	\return	PropertyTable_YIterator is the same iterator as the passed one.
	\param	yit is the iterator whose state has to be changed to SS_DELETE. 
	\throw	WrongOperationException */
//////////////////////////////////////////////////////////////////////////////
PropertyTable_YIterator	PropertyTableRef::markForDelete( PropertyTable_YIterator& yit )
{
	return getImpl()->markForDelete( yit );
}
//////////////////////////////////////////////////////////////////////////////
/*!	\return	PropertyTable_YIterator is the same iterator as the passed one.
	\param	yit is the iterator whose state has to be changed to SS_UNKNOWN. 
	\throw	WrongOperationException */
//////////////////////////////////////////////////////////////////////////////
PropertyTable_YIterator	PropertyTableRef::setStateUnknown( PropertyTable_YIterator& yit )
{
	return getImpl()->setStateUnknown( yit );
}
//////////////////////////////////////////////////////////////////////////////
/*!	\return	PropertyTable_YIterator is the same iterator as the passed one.
	\param	yit is the iterator whose state has to be changed to SS_CLEAN. 
	\throw	no-throw */
// BB: NOT REALLY IMPLEMENTED.
//////////////////////////////////////////////////////////////////////////////
PropertyTable_YIterator PropertyTableRef::setClean( PropertyTable_YIterator& yit )
{
	return yit;
}
//////////////////////////////////////////////////////////////////////////////
//! \return a shared pointer to a snapshot 
//! \throw no-throw
//////////////////////////////////////////////////////////////////////////////
const PropertyTableSnapshotRef PropertyTableRef::createSnapshot() const
{
	return getImpl()->createSnapshot();
}
//////////////////////////////////////////////////////////////////////////////
//! \param snapShot	snapshot to which a property table is set back 
//! \throw no-throw 
//////////////////////////////////////////////////////////////////////////////
void PropertyTableRef::resetToSnapshot( const PropertyTableSnapshotRef& snapShot )
{
	getImpl()->resetToSnapshot( snapShot );
}

PropertyTable_YIterator PropertyTableRef::at( basar::Int32 row ) const
{
    return getImpl()->at( row );
}

} // End namespace property
} // End namespace basar
