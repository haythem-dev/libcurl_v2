#ifndef GUARD_PROPERTYTABLEREF_H
#define GUARD_PROPERTYTABLEREF_H
//-----------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  This class PropertyTableRef is a special proxy (smart reference) 
			to the class PropertyTable.
 *  \author Bischof Björn 
 *  \date   25.07.2005
 */
//-----------------------------------------------------------------------------------------------//

//-----------------------------------------------------------------------------------------------//
// dll imports and exports
//-----------------------------------------------------------------------------------------------//
#ifndef LIBBASARPROPERTY_API
	#ifdef _WIN32
		#ifdef LIBBASARPROPERTY_EXPORTS
			#define LIBBASARPROPERTY_API __declspec(dllexport)	//!< dll exported
		#else
			#define LIBBASARPROPERTY_API __declspec(dllimport)	//!< dll imported
		#endif 
	#else 
		#define LIBBASARPROPERTY_API
	#endif 
#endif 

//-----------------------------------------------------------------------------------------------//
#include "libbasarproperty_definitions.h"

//-----------------------------------------------------------------------------------------------//
// namespaces
//-----------------------------------------------------------------------------------------------//
namespace basar
{
namespace property
{

//-----------------------------------------------------------------------------------------------//
// forward declaration
//-----------------------------------------------------------------------------------------------//
class PropertyDescriptionListRef;
class PropertyStateSet;
class PropertyTable_YIterator;
class PropertyTableSnapshotRef;

///////////////////////////////////////////////////////////////////////////////////
/*! \brief  This class PropertyTableRef is a special proxy (smart reference) 
			to the class PropertyTable.

			Class PropertyTableRef holds a smart (shared) pointer to its 
			implementation (class PropertyTable). 
			So the reference-class can be copied as many times as necessary without any doubts about 
			memory allocation or leaks.
            Class throws all the exceptions the implementation throws. */
//////////////////////////////////////////////////////////////////////////////
// implementation
//////////////////////////////////////////////////////////////////////////////
class PropertyTableRef
{
public:
	//! overloaded constructor with a shared pointer to the implementation object /n no-throw
	LIBBASARPROPERTY_API PropertyTableRef( const PropertyTableSharedPtr& pImpl );
	
	//! checks if the implementation is instantiated /n no-throw
	LIBBASARPROPERTY_API bool						isNull() const;
	
	/*! \brief	the aggregated PropertyDescriptionListRef (member) is returned. /n no-throw*/	
	LIBBASARPROPERTY_API const PropertyDescriptionListRef getPropertyDescriptionList() const;
	
	/*! \brief	delegates the operation to the matching method of the 
				aggregated container (shared pointer) /n no-throw*/
	LIBBASARPROPERTY_API bool						empty() const;
	
	/*! \brief	delegates the operation to the matching method of the 
				aggregated container (shared pointer) /n no-throw*/
	LIBBASARPROPERTY_API void						clear();
	
	/*! \brief	converts one state to the other and only deletes if the state of a row is DELETE. /n no-throw */
	LIBBASARPROPERTY_API PropertyTable_YIterator	executePerformed( PropertyTable_YIterator& yit );
		
	/*! \brief	erases physically a row of the container 
				\n WrongOperationException */
	LIBBASARPROPERTY_API PropertyTable_YIterator	erase( PropertyTable_YIterator& yit );	
	
	/*! \brief	removes physically all rows in the range [first,  predecessor(beyondLast)] of container
				and returns the position of the next element
				\n WrongOperationException */
	LIBBASARPROPERTY_API PropertyTable_YIterator	erase( PropertyTable_YIterator& first, 
														   PropertyTable_YIterator& beyondLast );	
	
		/*! \brief	calls double parameterized constructor with the this-pointer 
				(as shared pointer) and the begin()-method of the container
				member attribut. \n no-throw */
	LIBBASARPROPERTY_API PropertyTable_YIterator	begin();
	
	/*! \brief	calls double parameterized constructor with the this-pointer 
				(as shared pointer) and the end()-method of the container
				member attribut. \n no-throw */
	LIBBASARPROPERTY_API PropertyTable_YIterator	end();
	
    /*! \brief	calls double parameterized constructor with the this-pointer 
				(as shared pointer) and the begin()-method of the container
				member attribut in which the iterator point to an object with a state 
				contained in the PropertyStateSet-parameter. \n no-throw */
	LIBBASARPROPERTY_API PropertyTable_YIterator	begin( const PropertyStateSet& pss );

	/*! \brief	inserts a new row in the container (list) with the passed InsertType. /n no-throw*/
	LIBBASARPROPERTY_API PropertyTable_YIterator	insert( const InsertTypeEnum eInsertTypeEnum );
	
    //! delegates the operation to the shared pointer	/n no-throw
	LIBBASARPROPERTY_API void						reset();
	/*! \brief	delegates the operation to the matching method of the 
				aggregated container (shared pointer) /n no-throw*/
	LIBBASARPROPERTY_API MaxSizeRows			    max_size() const;
	/*! \brief	delegates the operation to the matching method of the 
				aggregated container (shared pointer) /n no-throw*/
	LIBBASARPROPERTY_API NumberRows			        size() const;
	
	/*! \brief	iterates over a whole row and sets the state of every single 
				column to SS_DELETE. \n WrongOperationException */
	LIBBASARPROPERTY_API PropertyTable_YIterator	markForDelete( PropertyTable_YIterator& yit );
	
	/*! \brief	iterates over a whole row and sets the state of every single 
				column on SS_UNKNOWN. \n WrongOperationException */
	LIBBASARPROPERTY_API PropertyTable_YIterator	setStateUnknown( PropertyTable_YIterator& yit );

	/*! \brief	returns a shared pointer to a snapshot \n no-throw */
	LIBBASARPROPERTY_API 
	const PropertyTableSnapshotRef					createSnapshot() const;

	/*! \brief	returns the property table values to the snapshot values \n no-throw */
	LIBBASARPROPERTY_API void						resetToSnapshot( const PropertyTableSnapshotRef& snapShot );

    LIBBASARPROPERTY_API PropertyTable_YIterator at( basar::Int32 row ) const;
private:
	//--------------------------------------------------------------------------------------------//
	// private methods
	//--------------------------------------------------------------------------------------------//
	/*! \brief is privatized for later use in the public interface perhaps /n no-throw */
	PropertyTable_YIterator							setClean( PropertyTable_YIterator& yit );
    //! checks and returns shared pointer to implementation object \n NullReferenceException
	const PropertyTableSharedPtr&					getImpl() const;
	
	//--------------------------------------------------------------------------------------------//
	// private members
	//--------------------------------------------------------------------------------------------//
	//! shared pointer to the implementation object.
	PropertyTableSharedPtr							m_pImpl;


}; // end class PropertyTableRef

} // End namespace property
} // End namespace basar

#endif // GUARD_PROPERTYTABLEREF_H
