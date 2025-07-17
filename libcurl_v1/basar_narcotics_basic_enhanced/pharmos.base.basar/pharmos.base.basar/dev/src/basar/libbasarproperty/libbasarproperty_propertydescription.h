/*! $Id$                                          
*                                                  
* @file 
* @author Anke Klink                      
* @date 2008                            
*/    

#ifndef  __GUARD_LIBBASARPROPERTY_PROPERTYDESCRIPTION_H__ 
#define  __GUARD_LIBBASARPROPERTY_PROPERTYDESCRIPTION_H__ 

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
#include "libbasarproperty_propertytype.h"
#include "libbasarcmnutil_bstring.h"
#include "libbasarcmnutil_parameterlist.h"

//--------------------------------------------------------------------------------------------//
namespace basar    {
namespace property {
        
//--------------------------------------------------------------------------------------------//
/*!
* PropertyDescription, descripes one Property Name and Type
*/ 
class PropertyDescription
{      
public: 
    //! Default constructor
    LIBBASARPROPERTY_API  PropertyDescription();

    //!Constructor with name and type
    LIBBASARPROPERTY_API  PropertyDescription(ConstString name, const SupportedTypeEnum  type);
    //!Constructor with name and type
    LIBBASARPROPERTY_API  PropertyDescription(ConstString name, const PropertyType & type);
    //!Constructor with name and type
    LIBBASARPROPERTY_API  PropertyDescription(ConstString name, ConstString type_name);
    //!Constructor with name and defaulttype
    LIBBASARPROPERTY_API  PropertyDescription(ConstString name);

    //! operator
	LIBBASARPROPERTY_API bool operator==(const PropertyDescription & p) const;

    //!returns the name 
    LIBBASARPROPERTY_API  const VarString & getName() const;

    //!returns the type 
    LIBBASARPROPERTY_API  const PropertyType & getType() const;

    //! casts into a VarString, returns name
    LIBBASARPROPERTY_API  operator const VarString() const;

    //! returns a string with format "typename propname;" , f.ex. "int32 colint;"
    LIBBASARPROPERTY_API  VarString toDescriptionString() const;

    //! returns a string with propertyname and ';' , f.ex. "colint;"
    LIBBASARPROPERTY_API  VarString toPropertyString() const;

    //! returns a string with propertyname and enclosing '#' , f.ex. "#colint#"
    LIBBASARPROPERTY_API  VarString toSQLReplacementString() const;


	//! returns a string  preString+PropertyName+postString, f.ex. 
	LIBBASARPROPERTY_API VarString toName(
		ConstString pre, ConstString post) const;

	//! returns a string  preString+#+PropertyName+#+postString, f.ex. 
	LIBBASARPROPERTY_API VarString toSQLReplacementString(
		ConstString preString, 
		ConstString postString) const;

	//! returns a string  preString1+PropertyName+postString1 = preString2+#+PropertyName+#+postString2
    LIBBASARPROPERTY_API VarString toEqualString(
		ConstString preString1, 
		ConstString postString1,
		ConstString preString2, 
		ConstString postString2) const;

	//! returns a string  preString1+PropertyName+postString1 = preString2+#+PropertyName+#+postString2
    LIBBASARPROPERTY_API VarString toEqualString( 
		ConstString preString2, 
		ConstString postString2) const;


	//! returns a string  preString1+PropertyName+postString1 +sign+ preString2+#+PropertyName+#+postString2
	LIBBASARPROPERTY_API VarString toInEqualString(
		ConstString equilitySign,  
		ConstString preString1, 
		ConstString postString1,
		ConstString preString2, 
		ConstString postString2) const;

	//! returns a string   PropertyName  = preString2+#+PropertyName+#+postString2
    LIBBASARPROPERTY_API VarString toInEqualString(  
		ConstString equilitySign,  
		ConstString preString2, 
		ConstString postString2) const;

	//! returns a string with the Tablename"."PropertyName
	LIBBASARPROPERTY_API VarString toStringWithTableName(
		ConstString tablename, bool useAs = false) const;

	//! returns a string tablename.PropertyName AS PropertyName2
	LIBBASARPROPERTY_API VarString toAsStatment(
		ConstString tableName, 
		const PropertyDescription & prop2) const;

	//! returns a string  PropertyName AS PropertyName2
	LIBBASARPROPERTY_API VarString toAsStatment(  
		const PropertyDescription & prop2) const;
		
	//! returns a string NVL( preString1+PropertyName+postString1, value) 
	LIBBASARPROPERTY_API VarString toNVLString( 
		ConstString nvlValue,
		ConstString preString1 = "",
		ConstString postString1 = "")const;

	//! returns a string NVL( preString1+PropertyName+postString1, value) AS PropertyName2
	LIBBASARPROPERTY_API VarString toNVLStringAs( 
		ConstString nvlValue,
		ConstString preString1= "",
		ConstString postString1= "")const;

	//! returns a string NVL( preString1+PropertyName+postString1, value) AS PropertyName2
	LIBBASARPROPERTY_API VarString toNVLStringAs( 
		const PropertyDescription & prop2,
		ConstString nvlValue,
		ConstString preString1 ="",
		ConstString postString1="")const;

	
private:
    //! propertyname
    VarString m_name;

    //! propertytype
    PropertyType m_type;

}; 

//--------------------------------------------------------------------------------------------//
//! Array of Properties
typedef PropertyDescription PropertyDescriptionArray[];

//--------------------------------------------------------------------------------------------//
//! Vector of PropertyDescription Objects
struct PropertyDescriptionVector
{
    //! inner vector
    std::vector<PropertyDescription> m_vector;

    //! DefaultConstructor
    LIBBASARPROPERTY_API PropertyDescriptionVector();

    //! Constructor with an Array
    LIBBASARPROPERTY_API PropertyDescriptionVector(int length, const PropertyDescription *);

    //! returns a string with format "typename propname;" , f.ex. "int32 colint;string colstring;"
    LIBBASARPROPERTY_API  VarString toDescriptionString() const;

    //! returns a string with format "propname1;propname2;" , f.ex. "colint;colstring;"
    LIBBASARPROPERTY_API  VarString toPropertyString() const;

    //! returns all PropertyDescription names comma seperated, f.ex "colint,colstring"
    LIBBASARPROPERTY_API  VarString toCommaSeparatedNames() const;

		//! returns all PropertyDescription names comma seperated, f.ex "table.colint, table.colstring"
	LIBBASARPROPERTY_API  VarString toCommaSeparatedNames(ConstString tablename, bool useAs= false) const;

    //! casts the Object into a ParameterList
    LIBBASARPROPERTY_API operator cmnutil::ParameterList() const;


    //! casts the Object into a ParameterList
    LIBBASARPROPERTY_API  std::vector<VarString> toStringList() const;
};

//--------------------------------------------------------------------------------------------//
}
}

//--------------------------------------------------------------------------------------------//
#endif 
