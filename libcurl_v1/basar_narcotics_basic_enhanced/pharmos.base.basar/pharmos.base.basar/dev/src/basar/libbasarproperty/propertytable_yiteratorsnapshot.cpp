//------------------------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  class PropertyTableYIteratorSnapshot creates a snapshot of a yiterator.
 *  \author Bischof Bjoern
 *  \date   05.09.2006
 */
//------------------------------------------------------------------------------------------------------------------//


//------------------------------------------------------------------------------------------------------------------//
// includes
//------------------------------------------------------------------------------------------------------------------//

#include "propertytable_yiteratorsnapshot.h"

#include "libbasarproperty_exceptions.h"
#include "propertyinternal.h"
#include "../macro.h"

//------------------------------------------------------------------------------------------------------------------//
// namespaces
//------------------------------------------------------------------------------------------------------------------//
namespace basar
{
namespace property
{

//------------------------------------------------------------------------------------------------------------------//
// static section
//------------------------------------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////
/*	! \return	PropertyTableYIteratorSnapshotSharedPtr shared_ptr<PropertyTableSnapshot> 
				to the implementation 
	! \param	yit
	! \throw	no-throw */ 
////////////////////////////////////////////////////////////////////////////////////////////////
PropertyTableYIteratorSnapshotSharedPtr	PropertyTableYIteratorSnapshot::create( const PropertyTable_YIterator& yit )
{
    PropertyTableYIteratorSnapshotSharedPtr p( new PropertyTableYIteratorSnapshot(yit) );
    p->setThisPtr( p );
    return p;
}
//------------------------------------------------------------------------------------------------------------------//
// class section
//------------------------------------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \param yit iterator to make a snapshot from
	\throw no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
PropertyTableYIteratorSnapshot::PropertyTableYIteratorSnapshot( const PropertyTable_YIterator& yit )
:m_OriginalYIterator( yit ), m_Snapshot( *(yit.m_It))
{
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \throw no-throw */ 
////////////////////////////////////////////////////////////////////////////////////////////////
PropertyTableYIteratorSnapshot::~PropertyTableYIteratorSnapshot()
{
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \param	p: a simple pointer of this is converted to a weak-pointer
	\throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
void PropertyTableYIteratorSnapshot::setThisPtr( const PropertyTableYIteratorSnapshotWeakPtr p )
{
	m_this_as_weak_ptr = p;
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \retval true if the snapshot create from this property table
	\param	yit: yiterator to set as snapshot
	\throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
bool PropertyTableYIteratorSnapshot::setSnapshot( PropertyTable_YIterator & yit )
{
	FUNCTION_NAME( basar.property.PropertyTable_XIterator.setSnapshot(PropertyTable_YIterator&) )
		
	if (yit == yit)
	{
		VarString msg(	"This method has not been implemeneted yet, because of the difficulty "
				        "with resetting the whole yiterator. "
					    "'Whole' means property values with the corresponding states. To be continued!" );
		throw NotImplementedYetException( ExceptInfo( fun, msg, __FILE__, __LINE__) );	
	}
	
	return false;
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return void
	\throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
void PropertyTableYIteratorSnapshot::clear()
{
	m_Snapshot.second.clear();
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \retval	true 
	\retval	otherwise false
	\throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
bool PropertyTableYIteratorSnapshot::isEqual()
{
	//!\todo
	/*bool equal = true;
	
	PropertyTable_XIterator xitOrig = m_OriginalYIterator.begin();
	CollPropertyInternalColumnIter itSnapshot = m_Snapshot.second.begin();
	basar::SupportedTypeEnum eType;
	for( ;  xitOrig != m_OriginalYIterator.end(); ++xitOrig, ++itSnapshot ){
		if( (xitOrig.getState().getState() != basar::SS_UNSET) && (itSnapshot->getState().getState() != basar::SS_UNSET) ){
			eType = xitOrig.getType().getType();
			if( basar::UNKNOWN == eType ){
				if( xitOrig.getSQLString() != itSnapshot->){
					
				}
			} else if( basar::INT16 == eType ){
			
			} else if( basar::INT32 == eType ){
			
			} else if( basar::INTDATE == eType ){
			
			} else if( basar::INTTIME == eType ){
			
			} else if( basar::FLOAT32 == eType ){
			
			} else if( basar::FLOAT64 == eType ){
			
			} else if( basar::DECIMAL == eType ){
			
			} else if( basar::STRING == eType ){
			
			} else if( basar::DATETIME == eType ){
			
			} else if( basar::DATE == eType ){
			
			} else if( basar::TIME == eType ){
			
			} else {
			
			}
		}
	}

	return equal;*/
	return true;
}

} // End namespace property
} // End namespace basar
