#ifndef GUARD_PROPERTYTABLE_YITERATOR_SNAPSHOT_H
#define GUARD_PROPERTYTABLE_YITERATOR_SNAPSHOT_H
//---------------------------------------------------------------------------------------//
/*! \file
 *  \brief  class PropertyTableYIteratorSnapshot creates a snapshot of a yiterator.
 *  \author Bischof Bjoern
 *  \date   05.09.2006
*/
//---------------------------------------------------------------------------------------//

#include "libbasarproperty_propertytable_yiterator.h"

//FIXME: for compile only
#include "propertytable.h"
//---------------------------------------------------------------------------------------//
// namespaces section
//---------------------------------------------------------------------------------------//
namespace basar
{
namespace property
{

//--------------------------------------------------------------------------------------//
// forward declaration
//--------------------------------------------------------------------------------------//
class PropertyTable_YIterator;

//---------------------------------------------------------------------------------------//
// class declaration
//---------------------------------------------------------------------------------------//
/*! \brief	class PropertyTableYIteratorSnapshot holds a deep copy of the values of
			another yiterator. 

    This is a final class.     
*/
//---------------------------------------------------------------------------------------//
class PropertyTableYIteratorSnapshot
{
public:
	//----------------------------------------------------------------------------------//
	// static section
	//----------------------------------------------------------------------------------//
	/*! \brief	simple factory method (static method) to create a new object of this class
				with the values of the passed object of a yiterator. \n no-throw */
    static PropertyTableYIteratorSnapshotSharedPtr	create( const PropertyTable_YIterator& yit );

	//----------------------------------------------------------------------------------//
	// object section
	//----------------------------------------------------------------------------------//
	//! \brief	d'tor which cleans up the list of values. \n no-throw
    ~PropertyTableYIteratorSnapshot();
	
	//! \brief	set the propertytable to this snapshot. \n no-throw 
	bool											setSnapshot( PropertyTable_YIterator& yit );

	/*! \brief	clears the internal values of the snapshot. \n no-throw */
	void											clear();

	bool											isEqual();

private:
	//---------------------------------------------------------------------------------------//
	// private methods
	//---------------------------------------------------------------------------------------//
	/*! \brief	register a new pointer of this class to weak pointer opf this class \n no-throw */
    void											setThisPtr( const PropertyTableYIteratorSnapshotWeakPtr p );

	/*! \brief	to privatize the dflt c'tor with no special action.
				It is not allowed to construct a simple default object. 
				Always the a PropertyDescriptionListRef-object has to be passed.
				\n no-throw */
    PropertyTableYIteratorSnapshot( const PropertyTable_YIterator& yit );


	//---------------------------------------------------------------------------------------//
	// private members
	//---------------------------------------------------------------------------------------//
	//! \brief	stores a weak pointer of itself 
	PropertyTableYIteratorSnapshotWeakPtr		m_this_as_weak_ptr;

	//! \brief	original yiterator
	PropertyTable_YIterator						m_OriginalYIterator;

	//! \brief	stores a weak pointer of itself 
	PairInsertTypeCollPropertyInternalColumn	m_Snapshot;
	
};

} // END namespace property
} // END namespace basar


#endif // GUARD_PROPERTYTABLE_YITERATOR_SNAPSHOT_H
