//----------------------------------------------------------------------------------------//
/*! \file
 *  \brief  This class PropertyTableSnapshotRef is a special proxy (smart reference) 
			to the class PropertyTableSnapshot.
 *  \author Roland Kiefert 
 *  \date   26.10.2005
 */
//----------------------------------------------------------------------------------------//

//----------------------------------------------------------------------------------------//
// includes
//----------------------------------------------------------------------------------------//

#include "libbasarproperty_propertytablesnapshotref.h"
#include "propertytablesnapshot.h"

#include "../macro.h"

//----------------------------------------------------------------------------------------//
// namespaces
//----------------------------------------------------------------------------------------//
namespace basar
{
namespace property
{
//////////////////////////////////////////////////////////////////////////////////////////
//! \throw	no-throw 
///////////////////////////////////////////////////////////////////////////////////////////
PropertyTableSnapshotRef::PropertyTableSnapshotRef()
{
}
///////////////////////////////////////////////////////////////////////////////////////////
/*! \param	pImpl is the shared pointer to the implementation object
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////
PropertyTableSnapshotRef::PropertyTableSnapshotRef( const PropertyTableSnapshotSharedPtr pImpl )
:m_pImpl( pImpl )
{
}
///////////////////////////////////////////////////////////////////////////////////////////
/*! \retval	true if the implementation object hasn't been instantiated yet
	\return otherwise returns false
	\throw no-throw */
///////////////////////////////////////////////////////////////////////////////////////////
bool PropertyTableSnapshotRef::isNull() const
{
	return( m_pImpl.get() == 0 );
}
///////////////////////////////////////////////////////////////////////////////////////////
//!	\return const shared pointer to implementation object
//! \throw  NullReferenceException 
///////////////////////////////////////////////////////////////////////////////////////////
const PropertyTableSnapshotSharedPtr& PropertyTableSnapshotRef::getImpl() const
{
	FUNCTION_NAME( basar.property.PropertyTableSnapshotRef.getImpl )
	if( isNull() ){
		throw NullReferenceException( 
			    ExceptInfo( fun, 
				            "Object of type PropertyTableSnapshot is not initialized!", 
							__FILE__, __LINE__) );
    }
	return m_pImpl;
}
///////////////////////////////////////////////////////////////////////////////////////////
/*! \return Long32 is the time when the snapshot is created.
	\throw no-throw */
///////////////////////////////////////////////////////////////////////////////////////////
Int32 PropertyTableSnapshotRef::getSnapshotTime() const
{
	return getImpl()->getSnapshotTime();
}
///////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////
void PropertyTableSnapshotRef::reset()
{
	m_pImpl.reset();
}
///////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////
void PropertyTableSnapshotRef::clear()
{
	return getImpl()->clear();
}

} // End namespace property
} // End namespace basar
