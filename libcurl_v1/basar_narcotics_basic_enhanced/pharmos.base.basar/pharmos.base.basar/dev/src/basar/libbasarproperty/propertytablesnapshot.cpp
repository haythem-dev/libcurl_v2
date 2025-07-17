//---------------------------------------------------------------------------------//
/*! \file
 *  \brief  class PropertyTableSnapshot make a snapshot of the class PropertyTable
 *  \author Roland Kiefert 
 *  \date   26.10.2005
 */
//---------------------------------------------------------------------------------//


//----------------------------------------------------------------------------------//
// includes
//----------------------------------------------------------------------------------//

#include "propertytablesnapshot.h"
#include "propertytable.h"
#include "propertyinternal.h"

//----------------------------------------------------------------------------------//
// namespaces
//----------------------------------------------------------------------------------//
namespace basar
{
namespace property
{

//----------------------------------------------------------------------------------//
// static section
//----------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	PropertyTableSnapshotSharedPtr shared_ptr<PropertyTableSnapshot> 
			to the implementation 
	\param	r is the proxy of the class PropertyTable 
	\param  sr weak pointer to property table */
////////////////////////////////////////////////////////////////////////////////////////////////
PropertyTableSnapshotSharedPtr PropertyTableSnapshot::create( const PropertyTable      & r , 
															  const PropertyTableWeakPtr sr)
{
    PropertyTableSnapshotSharedPtr p( new PropertyTableSnapshot( r, sr ) );
    p->setThisPtr(p);
    return p;
}
//----------------------------------------------------------------------------------//
// class section
//----------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \param pptRef is the proxy of the class PropertyTable
	\param ppt weak pointer to property table
	\throw no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
PropertyTableSnapshot::PropertyTableSnapshot( const PropertyTable      & pptRef, 
											  const PropertyTableWeakPtr ppt   )
											: m_pdlRef       ( pptRef.m_pdlRef    ), 
											  m_CollPIRow    ( pptRef.m_CollPIRow ), 
											  m_PropertyTable( ppt )
{
	//m_pdlRef = pptRef.m_pdlRef;	//.deepCopy();
	m_Time = basar::cmnutil::DateTime::getCurrent().getTime();
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \throw no-throw */ 
////////////////////////////////////////////////////////////////////////////////////////////////
PropertyTableSnapshot::~PropertyTableSnapshot()
{
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return Long32 is the time when the snapshot is created.
	\throw no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
Int32 PropertyTableSnapshot::getSnapshotTime() const
{
    return m_Time;
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return PdlSharedPtr
	\throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
//PropertyTableSnapshotSharedPtr PropertyTableSnapshot::thisPtr()
//{
//	return PropertyTableSnapshotSharedPtr( m_this_as_weak_ptr );
//}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \param	p a simple pointer of this is converted to a weak-pointer
	\throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
void PropertyTableSnapshot::setThisPtr( const PropertyTableSnapshotWeakPtr p )
{
	m_this_as_weak_ptr = p;
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \retval true if the snapshot was created from this property table
	\return otherwise returns false
	\param	r propertytable for which the snapshot should be set
	\throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
bool PropertyTableSnapshot::setSnapshot( PropertyTable& r )
{
	if( r.m_this_as_weak_ptr.lock().get() != m_PropertyTable.lock().get() )
		return false;
	
	r.m_CollPIRow = m_CollPIRow;
	r.m_pdlRef    = m_pdlRef;

	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
void PropertyTableSnapshot::clear()
{
	m_CollPIRow.clear();
}

} // End namespace property
} // End namespace basar
