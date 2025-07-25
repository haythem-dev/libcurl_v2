#ifndef GUARD_PROPERTYTYPE_H
#define GUARD_PROPERTYTYPE_H
//--------------------------------------------------------------------------------------------//
/*! \file
 *  \brief	class representing the possible datatypes 
 *  \author Bischof Bj�rn 
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
#include "libbasar_definitions.h"

//--------------------------------------------------------------------------------------------//
// namespaces
//--------------------------------------------------------------------------------------------//
namespace basar
{
namespace property
{

//--------------------------------------------------------------------------------------------//
// class PropertyType
//--------------------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////
/*! \brief  class PropertyType represents the possible (data-)types of the class 
			PropertyInternal (attribute/column of a resultset row)
            
    This is a final class.     
    It is copyable and assignable - these methods are generated by the compiler.
    Class throws the exception class UnknownPropertyTypeException.*/
///////////////////////////////////////////////////////////////////////////////////
class PropertyType
{
public:
	//-----------------------------------//
	// typedefs
	//-----------------------------------//
	typedef basar::VarString TypeName;			//!< TypeName as string
	typedef basar::VarString SupportedTypeName;	//!< SupportedTypeName as string

public:
	//-----------------//
    // object section
    //-----------------//
    //! default constructor assigns the value 'UNKNOWN' to the member attribute \n no-throw
	LIBBASARPROPERTY_API PropertyType();
    /*! \brief	overloaded constructor assigns the passed argument as SupportedType to the member attribute \n no-throw */
    LIBBASARPROPERTY_API PropertyType( const SupportedTypeEnum eSuppType );
    /*! \brief	overloaded constructor assigns the passed argument as TypeName (string) to the member attribute 
				\n UnknownPropertyTypeException */
    LIBBASARPROPERTY_API PropertyType( const TypeName& rTypeName );
    /*! \brief	checks equality with the member variable m_type (datatype: PropertyType::SupportedTypeEnum) 
				\n no-throw */
    LIBBASARPROPERTY_API bool						operator==  ( const PropertyType& ) const; 
	/*! \brief	negates the return value of the operator==() \n no-throw */
    LIBBASARPROPERTY_API bool						operator!=  ( const PropertyType& ) const; 
	//! returns the member attribute type as SupportedTypeName (string) \n no-throw
    LIBBASARPROPERTY_API const SupportedTypeName	toString    ()                      const; 
    //! returns the member attribute type as SupportedType (enum) \n no-throw    
    LIBBASARPROPERTY_API SupportedTypeEnum	        getType     ()                      const; 

private:
	//-----------------//
    // member section
    //-----------------//
	//! member attribute representing the current value (type SupportedTypeEnum)
	SupportedTypeEnum m_type;

}; // END class PropertyType

} // END namespace property
} // END namespace basar

#endif // END GUARD_PROPERTYTYPE_H
