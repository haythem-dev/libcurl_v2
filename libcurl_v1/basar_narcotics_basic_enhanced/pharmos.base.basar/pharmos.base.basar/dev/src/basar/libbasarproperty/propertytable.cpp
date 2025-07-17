////////////////////////////////////////////////////////////////////////////////////
/*! \file
 *  \brief  class PropertyTable holds a container of rows (database resultsets).
 *  \author Bischof Björn 
 *  \date   25.07.2005
 */
////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
// includes
////////////////////////////////////////////////////////////////////////////////////

#include "propertytable.h"

#include "libbasarproperty_propertytable_yiterator.h"
#include "libbasarproperty_propertytablesnapshotref.h"
#include "libbasarproperty_exceptions.h"
#include "propertytablesnapshot.h"
#include "propertyinternal.h"

#include "../macro.h"

////////////////////////////////////////////////////////////////////////////////////
// namespaces
////////////////////////////////////////////////////////////////////////////////////
namespace basar
{
namespace property
{

////////////////////////////////////////////////////////////////////////////////////
// static section
////////////////////////////////////////////////////////////////////////////////////
/*! \return	PropertyTableSharedPtr shared_ptr<PropertyDescriptionList> to the implementation 
	\param	r is the proxy of the class PropertyDescriptionList */
////////////////////////////////////////////////////////////////////////////////////
PropertyTableSharedPtr PropertyTable::create( const PropertyDescriptionListRef& r )
{
    PropertyTableSharedPtr p( new PropertyTable( r ) ); // RAII
    p->setThisPtr(p);
    return p;
}

////////////////////////////////////////////////////////////////////////////////////
// implementation
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
/*! \param pdlRef is the proxy of the class PropertyDescriptionList
	\throw no-throw */
////////////////////////////////////////////////////////////////////////////////////
PropertyTable::PropertyTable ( const PropertyDescriptionListRef& pdlRef )
							 : m_pdlRef( pdlRef )
{
}

////////////////////////////////////////////////////////////////////////////////////
/*! \throw no-throw */ 
////////////////////////////////////////////////////////////////////////////////////
PropertyTable::~PropertyTable()
{
	clear();	
}

////////////////////////////////////////////////////////////////////////////////////
/*! \return PropertyDescriptionListRef is a reference stored in a member.
	\throw no-throw */
////////////////////////////////////////////////////////////////////////////////////
const PropertyDescriptionListRef PropertyTable::getPropertyDescriptionList() const
{
    return m_pdlRef;
}

////////////////////////////////////////////////////////////////////////////////////
/*! \throw no-throw */ 
////////////////////////////////////////////////////////////////////////////////////
void PropertyTable::clear()
{
    m_CollPIRow.clear();
}

////////////////////////////////////////////////////////////////////////////////////
/*! \return ColumnSize is the number of the stored rows (database resultsets).
	\throw no-throw */ 
////////////////////////////////////////////////////////////////////////////////////
NumberRows PropertyTable::size() const
{
	return m_CollPIRow.size();
}

////////////////////////////////////////////////////////////////////////////////////
/*! \retval true if there are no rows in the container.
	\return otherwise returns false
	\throw no-throw */ 
////////////////////////////////////////////////////////////////////////////////////
bool PropertyTable::empty() const
{
	return m_CollPIRow.empty();
}

////////////////////////////////////////////////////////////////////////////////////
/*! \return ColumnSize is the max memory that can be allocated for the rows.
	\throw no-throw */ 
////////////////////////////////////////////////////////////////////////////////////
MaxSizeRows PropertyTable::max_size() const
{
	return m_CollPIRow.max_size();
}

////////////////////////////////////////////////////////////////////////////////////
/*! \return PropertyTableSharedPtr
	\throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////
PropertyTableSharedPtr PropertyTable::thisPtr() const
{
	return PropertyTableSharedPtr( m_this_as_weak_ptr );
}

////////////////////////////////////////////////////////////////////////////////////
/*! \param	p a simple pointer of this is converted to a weak-pointer
	\throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////
void PropertyTable::setThisPtr( const PropertyTableWeakPtr p )
{
	m_this_as_weak_ptr = p;
}

////////////////////////////////////////////////////////////////////////////////////
/*! \throw no-throw
    \return PropertyTable_YIterator pointing to first row of propertytable */
////////////////////////////////////////////////////////////////////////////////////
PropertyTable_YIterator PropertyTable::begin()
{
    return PropertyTable_YIterator( thisPtr(), m_CollPIRow.begin() );
}

////////////////////////////////////////////////////////////////////////////////////
/*! \return PropertyTable_YIterator points to the object with the given state contained 
			in the parameter
	\param	pss	is/are the state/s the row the returned iterator points to must be in.
			\n Row only has to be in one of the given states.
	\throw	no-throw */
////////////////////////////////////////////////////////////////////////////////////
PropertyTable_YIterator PropertyTable::begin( const PropertyStateSet& pss )
{
    // To determine the state of a row every single element (PropertyInternal) 
	// in the concerning row has to be asked for its state. 
	// The method 'PropertyStateSet PropertyTable_YIterator::getState() const'
	// will do the job.
	// The returned PropertyStateSet from the method 
	// 'PropertyTable_YIterator::getState() const' is checked
    // if it is contained in the passed pss parameter. 
    // If one of the returned states of the row is contained in the passed parameter
	// pss the row is considered as being in this state.
	
	PropertyTable_YIterator yit = begin(); // iterate over the complete list	
	yit.m_Pss = pss;
	
	while( yit != end() && !pss.contains(yit.getState()) )
	{
		++yit;	
	}
	
	return yit;
}

////////////////////////////////////////////////////////////////////////////////////
/*! \throw no-throw
    \return PropertyTable_YIterator pointing to position beyond the last row
			of propertytable */
////////////////////////////////////////////////////////////////////////////////////
PropertyTable_YIterator PropertyTable::end()
{
    return PropertyTable_YIterator( thisPtr(), m_CollPIRow.end() );
}

////////////////////////////////////////////////////////////////////////////////////
/*!	\return	PropertyTable_YIterator pointing to the inserted row.
	\param	eInsertTypeEnum will determine the type of the inserted row.
	\throw	no-throw */
////////////////////////////////////////////////////////////////////////////////////
PropertyTable_YIterator PropertyTable::insert( const InsertTypeEnum eInsertTypeEnum )
{
	CollPropertyInternalRowIter it = m_CollPIRow.insert( m_CollPIRow.end(), PairInsertTypeCollPropertyInternalColumn(eInsertTypeEnum, CollPropertyInternalColumn(m_pdlRef.size()) ) );

	return PropertyTable_YIterator ( thisPtr(), it );
}

////////////////////////////////////////////////////////////////////////////////////
/*!	\return	PropertyTable_YIterator is the iterator pointing to the successor 
			element of the erased one.
	\param	first is the position of the first element removed from the list. 
	\param	beyondLast is the position just beyond the last element removed from the list. 
	\throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////
PropertyTable_YIterator PropertyTable::erase( PropertyTable_YIterator & first,
											  PropertyTable_YIterator & beyondLast )
{
	if ( first.isBegin() && beyondLast.isEnd())
		clear();
	else
	{
		while (first != beyondLast)
			first = erase(first);
	}

	return beyondLast;
}

////////////////////////////////////////////////////////////////////////////////////
/*!	\return	PropertyTable_YIterator is the iterator pointing to the successor 
			element of the erased one.
	\param	yit is the iterator which has to be killed 
	\throw  OutOfRangeIteratorException */
////////////////////////////////////////////////////////////////////////////////////
PropertyTable_YIterator PropertyTable::erase( PropertyTable_YIterator & yit )
{
	FUNCTION_NAME( basar.property.PropertyTable.erase(yit) )

	if( end() == yit )
	{
		throw OutOfRangeIteratorException ( ExceptInfo( fun, 
														"passed iterator equals end()!", 
														__FILE__, __LINE__) );
	}
	
	PropertyTable_YIterator retyit = yit;
	++retyit;

	//! \todo unfiltered switching regarding the statement: "yit.m_It++"
	m_CollPIRow.erase( yit.m_It ); 

	return retyit;
}
 
////////////////////////////////////////////////////////////////////////////////////
/*!	\param	yit is the iterator whose state has to be changed  
			or it has to be killed if the row has the state SS_DELETE.
	\return PropertyTable_YIterator removed of entries in state SS_DELETE, other are set to SS_CLEAN
	\throw	no-throw */
////////////////////////////////////////////////////////////////////////////////////
PropertyTable_YIterator PropertyTable::executePerformed( PropertyTable_YIterator& yit )
{
	 
	//	UNSET / UNKNOWN --> stay untouched
	/*	the state of a line is considered as if only a single state is DELETE.
	then the method 'erase()' is executed;*/
	//	Rest --> Zustands-Diagramm: update/insert --> clean
	if (!yit.isNull())
	{
		basar::property::PropertyStateSet ps = yit.getState();
		SupportedStateEnum eState = yit.getState().getStates();
		
		if( (SS_UNSET == eState) || (SS_UNKNOWN == eState) )
		{
			// do nothing if the states are only SS_UNSET or only SS_UNKNOWN ?
		}
		else if (ps.contains(SS_DELETE ))
		{
			// remove the line for SS_DELETE, no matter, what other
			// lines there are
			yit = erase( yit );
		}
		else 
		{
			 
			basar::InsertTypeEnum insertTypeEnum = yit.getInsertType();
			if (FOR_UNKNOWN == insertTypeEnum )
			{
				// do nothing, as it is not possible to reset unknown ?
			}
			else if (FOR_DELETE == insertTypeEnum )
			{
				// do nothing, why ? 
			}
			else if (ps.contains(SS_INSERT) || ps.contains(SS_UPDATE))
			{
				yit.setState( SS_CLEAN );
                yit.setInsertType( FOR_CLEAN );
			}
		}
			 
	} 
	
	return yit;
}

////////////////////////////////////////////////////////////////////////////////////
/*!	\return	PropertyTable_YIterator is the same iterator as the passed one.
	\param	yit is the iterator whose state has to be changed to SS_DELETE. 
	\throw	WrongOperationException */
////////////////////////////////////////////////////////////////////////////////////
PropertyTable_YIterator PropertyTable::markForDelete( PropertyTable_YIterator& yit )
{
	FUNCTION_NAME( basar.property.PropertyTable.markForDelete(PropertyTable_YIterator) )

	// regardless of the insert type the state UNSET will throw the same exception 
	// 'WrongOperationException()';
	if( SS_UNSET == yit.getState().getStates() )
	{
		throw WrongOperationException( ExceptInfo( fun, 
												   "Operation was executed on an unset state!", 
												   __FILE__, __LINE__) );
	}

	yit.setState( SS_DELETE );
	
	return yit;
}

////////////////////////////////////////////////////////////////////////////////////
/*!	\return	PropertyTable_YIterator is the same iterator as the passed one.
	\param	yit is the iterator whose state has to be changed to SS_UNKNOWN. 
	\throw	WrongOperationException */
////////////////////////////////////////////////////////////////////////////////////
PropertyTable_YIterator PropertyTable::setStateUnknown( PropertyTable_YIterator& yit )
{
	FUNCTION_NAME( basar.property.PropertyTable.setStateUnknown(PropertyTable_YIterator) )

	// regardless of the insert type the state UNSET will throw the same exception 
	// 'WrongOperationException()';
	if( SS_UNSET == yit.getState().getStates() )
	{
		throw WrongOperationException( ExceptInfo( fun, 
												   "Operation was executed on an unset state!", 
												   __FILE__, __LINE__) );
	}

	yit.setState( SS_UNKNOWN );
	
	return yit;
	
}

////////////////////////////////////////////////////////////////////////////////////
/*! \todo	this method: 'PropertyTable_YIterator	PropertyTable::setClean( PropertyTable_YIterator yit )' 
	        is not implemented yet. 
            Therefore an simple string exception is thrown to indicate this fact only. */
/*!	\return	PropertyTable_YIterator is the same iterator as the passed one.
	\param	yit is the iterator whose state has to be changed to SS_CLEAN. 
	\throw	no-throw */
////////////////////////////////////////////////////////////////////////////////////
PropertyTable_YIterator	PropertyTable::setClean( PropertyTable_YIterator & yit )
{
	yit = yit;

	throw	"NOT IMPLEMENTED YET! - This exception has no logic reason.\n"
			"the method 'PropertyTable_YIterator PropertyTable::setClean(PropertyTable_YIterator yit) "
			"is not implemented yet!";

	// for later implementation		
	//yit.setState( SS_CLEAN );
	//return yit;
}

////////////////////////////////////////////////////////////////////////////////////
/*!	\return	Snapshot of the current property table.
    \throw	no-throw */
////////////////////////////////////////////////////////////////////////////////////
PropertyTableSnapshotRef PropertyTable::createSnapshot()
{
	return PropertyTableSnapshotRef( PropertyTableSnapshot::create( *this, m_this_as_weak_ptr ));
}

////////////////////////////////////////////////////////////////////////////////////
//!	\param	snapShot is a snapshot of a property table. 
//!	\throw	DifferentPropertyTableException
////////////////////////////////////////////////////////////////////////////////////
void PropertyTable::resetToSnapshot( const PropertyTableSnapshotRef& snapShot )
{
	FUNCTION_NAME( basar.property.PropertyTable.resetToSnapshot(SnapshotPropertyTableRef) )

	if( !(snapShot.getImpl()->setSnapshot(*this)) )
	{
		//! \todo	should throw the method 
		//			'bool SnapshotPropertyTable::setSnapshot( PropertyTable& r )'.
		throw DifferentPropertyTableException(ExceptInfo( fun, 
														  "The property table of the snapshot "
					                                      "is not equal to property table of this!", 
														  __FILE__, __LINE__) );
	}
}

PropertyTable_YIterator PropertyTable::at( basar::Int32 row )
{
    FUNCTION_NAME( basar.property.PropertyTable.at );

    if ( 0 > row )
    {
        basar::VarString msg;
        msg.format( "row <%d> is before begin, not allowed", row );

        throw OutOfRangeIteratorException( ExceptInfo( fun, msg, __FILE__, __LINE__ ) );
    }

    CollPropertyInternalRowIter it = m_CollPIRow.begin();

    const CollPropertyInternalRowIter::difference_type dist = std::distance( it, m_CollPIRow.end() );

    if ( row > dist )
    {
        basar::VarString msg;
        msg.format( "requested row <%d> exceeds available distance <%d>", row, dist );

        throw OutOfRangeIteratorException( ExceptInfo( fun, msg, __FILE__, __LINE__ ) );
    }

    std::advance( it, row );

    return PropertyTable_YIterator( thisPtr(), it );
}

//--------------------------------------------------------------------------------------------------
} // End namespace property
} // End namespace basar
