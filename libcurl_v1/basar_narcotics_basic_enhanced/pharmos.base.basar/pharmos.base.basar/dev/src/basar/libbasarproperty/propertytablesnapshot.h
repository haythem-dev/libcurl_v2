#ifndef GUARD_PROPERTYTABLESNAPSHOT_H
#define GUARD_PROPERTYTABLESNAPSHOT_H
//---------------------------------------------------------------------------------------//
/*! \file
 *  \brief  class PropertyTableSnapshot creates a snapshot of the class PropertyTable
 *  \author Roland Kiefert 
 *  \date   26.10.2005
*/
//---------------------------------------------------------------------------------------//

#include "libbasarproperty_propertydescriptionlistref.h"

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
class PropertyTable;

//---------------------------------------------------------------------------------------//
// class declaration
//---------------------------------------------------------------------------------------//
/*! \brief  class PropertyTableSnapshot holds a container of rows (database resultsets).

    This is a final class.     
    Class throws the follwing exceptions: 
	class WrongOperationException */
//---------------------------------------------------------------------------------------//
class PropertyTableSnapshot
{
public:
	//----------------------------------------------------------------------------------//
	// static section
	//----------------------------------------------------------------------------------//
	//! \brief simple factory method (static method) to create a new object of this class
    static PropertyTableSnapshotSharedPtr	create( const PropertyTable& r, const PropertyTableWeakPtr sr );

	//----------------------------------------------------------------------------------//
	// object section
	//----------------------------------------------------------------------------------//
	//! \brief	destructor which cleans up the list \n no-throw
    ~PropertyTableSnapshot();
	
	//! \brief	returns the time when the snapshot is created \n no-throw
    Int32   getSnapshotTime() const;
    
	//! \brief	sets the propertytable to this snapshot \n no-throw 
	bool	setSnapshot( PropertyTable& r );

	//! \brief	clears the internal . \n no-throw 
	void	clear();

	

private:
	//---------------------------------------------------------------------------------------//
	// private methods
	//---------------------------------------------------------------------------------------//
	//! \brief	returns a shared pointer of this \n no-throw 
	//PropertyTableSnapshotSharedPtr			thisPtr();

	/*! \brief	registers a new pointer of this class to weak pointer of this class \n no-throw */
    void									setThisPtr( const PropertyTableSnapshotWeakPtr p );

	/*! \brief	privatize the default constructor with no special action.
				It is not allowed to construct a simple default object. 
				Always a PropertyDescriptionListRef-object has to be passed.
				\n no-throw */
    PropertyTableSnapshot( const PropertyTable& pptRef, const PropertyTableWeakPtr ppt);


	//---------------------------------------------------------------------------------------//
	// private members
	//---------------------------------------------------------------------------------------//
	//! stores a weak pointer of itself 
	PropertyTableSnapshotWeakPtr			m_this_as_weak_ptr;
    
	//! stores the PropertyDescriptionListRef passed via overloaded constructor
	PropertyDescriptionListRef              m_pdlRef;
	
	//! member attribute representing the container of rows (database resultsets)
	CollPropertyInternalRow					m_CollPIRow;
	
	//! stores a weak pointer to the propertytable
	PropertyTableWeakPtr					m_PropertyTable;

	//! stores the time when this is created
	Long32									m_Time;
};


} // END namespace property
} // END namespace basar


#endif // GUARD_PROPERTYTABLESNAPSHOT_H
