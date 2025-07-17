#ifndef GUARD_PROPERTYTABLE_H
#define GUARD_PROPERTYTABLE_H
//--------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  class PropertyTable holds a container of rows (database resultsets).
 *  \author Bischof Björn 
 *  \date   25.07.2005
 */
//--------------------------------------------------------------------------------------------//


//--------------------------------------------------------------------------------------------//
// dll imports and exports
//--------------------------------------------------------------------------------------------//
#ifdef _WIN32
    #ifdef LIBBASARPROPERTY_EXPORTS
        #define LIBBASARPROPERTY_API __declspec(dllexport)	//!< dll exported
	#else
        #define LIBBASARPROPERTY_API __declspec(dllimport)	//!< dll imported
    #endif 
#else 
    #define LIBBASARPROPERTY_API
#endif 

//--------------------------------------------------------------------------------------------//
#include "libbasarproperty_propertydescriptionlistref.h"

//--------------------------------------------------------------------------------------------//
// namespaces
//--------------------------------------------------------------------------------------------//
namespace basar
{
namespace property
{

//--------------------------------------------------------------------------------------------//
// exceptions
//--------------------------------------------------------------------------------------------//


//--------------------------------------------------------------------------------------------//
// forward declarations
//--------------------------------------------------------------------------------------------//
class PropertyTable_YIterator;
class PropertyTableSnapshotRef;
class PropertyStateSet;

//--------------------------------------------------------------------------------------------//
// class declaration PropertyTable
//--------------------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////
/*! \brief  class PropertyTable holds a container of rows (database resultsets).

    This is a final class.     
    Class throws the follwing exceptions: 
	class WrongOperationException */
///////////////////////////////////////////////////////////////////////////////////
class PropertyTable
{
	friend class PropertyTableSnapshot;
	friend class PropertyTable_YIterator;

public:
    //---------------------------------------------------------------------------//
    // static section
    //---------------------------------------------------------------------------//
	//! \brief simple factory method (static method) to create a new object of this class /n no-throw
    LIBBASARPROPERTY_API static PropertyTableSharedPtr create( const PropertyDescriptionListRef& r);

	//---------------------------------------------------------------------------//
	// object section
	//---------------------------------------------------------------------------//
public:
	//! \brief	destructor which cleans up the list \n no-throw
    LIBBASARPROPERTY_API					~PropertyTable();
	
	/*! \brief	the aggregated PropertyDescriptionListRef (member) is returned.  /n no-throw */
	const PropertyDescriptionListRef		getPropertyDescriptionList() const;
	
	/*! \brief	delegates the operation to the matching method of the 
				aggregated container (shared pointer) /n no-throw */
	MaxSizeRows						        max_size() const;
	
	/*! \brief	delegates the operation to the matching method of the 
				aggregated container (shared pointer) /n no-throw */
	NumberRows						        size() const;
	
	/*! \brief	delegates the operation to the matching method of the 
				aggregated container (shared pointer) /n no-throw */
	bool									empty() const;
	
	/*! \brief	delegates the operation to the matching method of the 
				aggregated container (shared pointer) /n no-throw */
	void									clear();
	
	/*! \brief	calls double parameterized constructor with the this-pointer 
				(as shared pointer) and the begin()-method of the container
				member attribut. \n no-throw */
	PropertyTable_YIterator				    begin();
	
	/*! \brief	calls double parameterized constructor with the this-pointer 
				(as shared pointer) and the end()-method of the container
				member attribut. \n no-throw */
	PropertyTable_YIterator					end();
    
	/*! \brief	calls double parameterized constructor with the this-pointer 
				(as shared pointer) and the begin()-method of the container
				member attribut in which the iterator points to an object with a state 
				contained in the PropertyStateSet-parameter. \n no-throw */
	PropertyTable_YIterator					begin( const PropertyStateSet& pss );    
    
	/*! \brief	inserts a new row in the container (list) with the passed InsertType.  /n no-throw */
	PropertyTable_YIterator					insert( const InsertTypeEnum eInsertTypeEnum );
	
	/*! \brief	really erases a row from the container (list) \n OutOfRangeIteratorException */
	PropertyTable_YIterator					erase( PropertyTable_YIterator& yit );
	
	/*! \brief	erases physically a range of the container (list) \n no-throw */
	PropertyTable_YIterator					erase( PropertyTable_YIterator& first,
												   PropertyTable_YIterator& beyondLast);

	/*! \brief	iterates over a whole row and sets the state of every single 
				column to SS_DELETE. \n WrongOperationException */
	PropertyTable_YIterator					markForDelete( PropertyTable_YIterator& yit );
	
	/*! \brief	iterates over a whole row and sets the state of every single 
				column to SS_UNKNOWN. \n WrongOperationException */
	PropertyTable_YIterator					setStateUnknown( PropertyTable_YIterator& yit );

	/*! \brief	converts one state to the other and only deletes if the state of a row is SS_DELETE.  /n no-throw */
	PropertyTable_YIterator					executePerformed( PropertyTable_YIterator& yit );

	/*! \brief	returns a shared pointer to a snapshot \n no-throw */
	PropertyTableSnapshotRef				createSnapshot();
	/*! \brief	sets a property table back to the snapshot \n DifferentPropertyTableException */
	void									resetToSnapshot( const PropertyTableSnapshotRef& snapShot );

    PropertyTable_YIterator                 at( basar::Int32 row );

private:
	//--------------------------------------------------------------------------------------------//
	// private methods
	//--------------------------------------------------------------------------------------------//

	/*! \brief	returns a shared pointer of this \n no-throw */
	PropertyTableSharedPtr					thisPtr() const;

	/*! \brief	registers a new pointer of this class to weak pointer opf this class \n no-throw */
    void									setThisPtr( const PropertyTableWeakPtr p );
	/*! \brief is privatized for later use in the public interface perhaps /n no-throw */
	PropertyTable_YIterator					setClean( PropertyTable_YIterator& yit );

	/*! \brief	tp privatize the dflt constructor with no special action.
				It is not allowed to construct a simple default object. 
				Always a PropertyDescriptionListRef-object has to be passed.
				\n no-throw */
    PropertyTable( const PropertyDescriptionListRef& pdlRef );
	
	//--------------------------------------------------------------------------------------------//
	// private members
	//--------------------------------------------------------------------------------------------//
	//! stores a weak pointer of itself 
	PropertyTableWeakPtr                    m_this_as_weak_ptr;
    //! stores the PropertyDescriptionListRef passed via overloaded constructor 
	PropertyDescriptionListRef              m_pdlRef;
	//! member attribute representing the container of rows (database resultsets)
	CollPropertyInternalRow					m_CollPIRow;

}; // END class PropertyTable


} // END namespace property
} // END namespace basar


#endif // GUARD_PROPERTYTABLE_H
