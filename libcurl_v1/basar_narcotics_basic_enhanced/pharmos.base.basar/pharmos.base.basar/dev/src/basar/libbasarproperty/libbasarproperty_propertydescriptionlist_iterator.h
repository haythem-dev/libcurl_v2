#ifndef GUARD_PROPERTYDESCRIPTIONLIST_ITERATOR_H
#define GUARD_PROPERTYDESCRIPTIONLIST_ITERATOR_H
//--------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  class PropertyDescriptionList_Iterator is an robust wrapper class around a stl iterator to travers
			the PropertyDescriptionList
 *  \author Bischof Björn 
 *  \date   25.07.2005
 */
//--------------------------------------------------------------------------------------------//


//--------------------------------------------------------------------------------------------//
// dll imports and exports
//--------------------------------------------------------------------------------------------//
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

//--------------------------------------------------------------------------------------------//
#include "libbasarproperty_definitions.h"

//--------------------------------------------------------------------------------------------//
// namespaces
//--------------------------------------------------------------------------------------------//
namespace basar
{
namespace property
{

//--------------------------------------------------------------------------------------------//
// class declaration
//--------------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \brief  class PropertyDescriptionList_Iterator is a robust wrapper class around a stl iterator to traverse
			the PropertyDescriptionList

    This is a final class.     
    Class throws the follwing exceptions: class UnknownColumnNameException and class OutOfRangeException. */
class PropertyDescriptionList_Iterator
{	
public: 
	/*! \brief	constructor registers the concerned PropertyDescriptionlList and 
				the original std::vector<>::iterator \n no-throw */
	LIBBASARPROPERTY_API 
	PropertyDescriptionList_Iterator(	const basar::property::PdlSharedPtr rPdl, 
										const basar::property::ColNamePropTypeCollIter rIt );

	/*! \brief	checks if the member attribute (original std::vector<>::iterator) resembles 
				the one of the passed iterator-object  \n no-throw */
	LIBBASARPROPERTY_API bool			operator==( const PropertyDescriptionList_Iterator rhp ) const;
	
	/*! \brief	negates the return value of the operator==() \n no-throw */
	LIBBASARPROPERTY_API bool			operator!=( const PropertyDescriptionList_Iterator rhp ) const;

	/*! \brief checks if iterator points to begin() of the inner container
		\n     no-throw */
	LIBBASARPROPERTY_API bool			isBegin() const;

	/*! \brief checks if iterator points to end() of the inner container
		\n     no-throw */
	LIBBASARPROPERTY_API bool			isEnd() const;

	/*! \brief  prefix-operator delegates the call to the concerned function of 
				the original std::vector<>::iterator
				\n OutOfRangeException */
	LIBBASARPROPERTY_API 
	PropertyDescriptionList_Iterator	operator++(); // prefix
	/*! \brief  postfix-operator delegates the call to the concerned function of the 
				original std::vector<>::iterator
				\n OutOfRangeException */
	LIBBASARPROPERTY_API 
	PropertyDescriptionList_Iterator	operator++( int i );
	
	/*! \brief  dereferencing-operator delegates the call to the concerned function 
				of the original std::vector<>::iterator to have direct access to the 
				object the iterator is pointing to.
				\n OutOfRangeException */
	LIBBASARPROPERTY_API 
	ColNamePropTypePair&				operator*();  
	
	/*! \brief  dereferencing-operator delegates the call to the concerned function 
				of the original std::vector<>::iterator to have direct access to the 
				object the iterator is pointing to.
				\n OutOfRangeException */
	LIBBASARPROPERTY_API 
	const ColNamePropTypePair&			operator*() const;  

	/*! \brief  returns the original std::vector<>::iterator
				\n OutOfRangeException */
    LIBBASARPROPERTY_API
    ColNamePropTypeCollIter          operator->();

    /*! \brief  returns the original std::vector<>::iterator
			\n OutOfRangeException */
    LIBBASARPROPERTY_API
    const ColNamePropTypeCollIter          operator->() const;


	/*! \brief is the value Null */
	LIBBASARPROPERTY_API bool isNull() const;

	/*! \brief resets the value */
	LIBBASARPROPERTY_API void reset();
	
private:
	//--------------------------------------------------------------------------------------------//
	// private methods
	//--------------------------------------------------------------------------------------------//
	/*! \brief	checks if the shared pointer member holding the propertytable is valid / initialized.
				This will occur if the iterator is constructed with the deafult constructor and used.
				\n InvalidIteratorException */
	void								checkInitialization() const;

	//--------------------------------------------------------------------------------------------//
	// private members
	//--------------------------------------------------------------------------------------------//
	//! shared pointer to the actual container
	PdlSharedPtr						m_Pdl;
	//! iterator of the aggregated container
	ColNamePropTypeCollIter				m_It;

}; // END class PropertyDescriptionList_Iterator


} // END namespace property
} // END namespace basar

#endif // GUARD_PROPERTYDESCRIPTIONLIST_ITERATOR_H
