#ifndef GUARD_PROPERTYTABLESNAPSHOTREF_H
#define GUARD_PROPERTYTABLESNAPSHOTREF_H
//---------------------------------------------------------------------------------------//
/*! \file
 *  \brief  This class PropertyTableSnapshotRef is a special proxy (smart reference) 
			to the class PropertyTableSnapshot.
 *  \author Roland Kiefert 
 *  \date   25.10.2005
 */
//---------------------------------------------------------------------------------------//


//---------------------------------------------------------------------------------------//
// dll exports and imports
//---------------------------------------------------------------------------------------//
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

//---------------------------------------------------------------------------------------//
#include "libbasarproperty_definitions.h"

//---------------------------------------------------------------------------------------//
// namespaces
//---------------------------------------------------------------------------------------//
namespace basar
{
namespace property
{

//---------------------------------------------------------------------------------------//
// class declaration PropertyTableSnapshotRef
//---------------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////////////
/*! \brief  This class PropertyTableSnapshotRef is a special proxy (smart reference) 
			to the class PropertyTableSnapshot.

			Class PropertyTableSnapshotRef holds a smart (shared) pointer to its 
			implementation (class PropertyTableSnapshot). 
			So the reference-class can be copied as many times as necessary without any doubts about 
			memory allocation or leaks.
            Class throws all the exceptions the implementation throws. */
///////////////////////////////////////////////////////////////////////////////////////////
class PropertyTableSnapshotRef
{
	friend class PropertyTable;

public:
	
	//! \brief	default constructor with no special action \n no-throw
	LIBBASARPROPERTY_API PropertyTableSnapshotRef();

	//! overloaded constructor with a shared pointer to the implementation object \n no-throw
	LIBBASARPROPERTY_API PropertyTableSnapshotRef( const PropertyTableSnapshotSharedPtr pImpl );
	
	//! checks if the implementation is instantiated \n no-throw
	LIBBASARPROPERTY_API bool						isNull() const;

	/*! \brief	returns the time when the Snapshot is created \n no-throw */	
	LIBBASARPROPERTY_API Int32				        getSnapshotTime() const;
	
	//! resets the shared pointer \n no-throw	
	LIBBASARPROPERTY_API void						reset();

	//! clears the internal propertytable. \n no-throw	
	LIBBASARPROPERTY_API void						clear();

private:
	//---------------------------------------------------------------------------------------//
	// private methods
	//---------------------------------------------------------------------------------------//
    //! checks and returns shared pointer to implementation object \n throw NullReferenceException
	const PropertyTableSnapshotSharedPtr&			getImpl() const;
	
	//---------------------------------------------------------------------------------------//
	// private members
	//---------------------------------------------------------------------------------------//
	//! shared pointer to the implementation object. \n no-throw
	PropertyTableSnapshotSharedPtr					m_pImpl;
};

} // property
} // basar

#endif // GUARD_PROPERTYTABLESNAPSHOTREF_H
