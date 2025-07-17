//-------------------------------------------------------------------------------------------//
/*! \file
*  \brief	class representing the possible datatypes 
 *  \author Bischof Björn 
 *  \date   25.07.2005
 */
//-------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------//

#include "libbasarproperty_propertytype.h"
#include "libbasarcmnutil_bstring.h"
#include "libbasarcmnutil_exceptions.h"

#include "../macro.h"

#include <map>

//-------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------//
namespace basar
{
namespace property
{

//-------------------------------------------------------------------------------------------//
// initialization of static variable
//-------------------------------------------------------------------------------------------//
/*! \brief  get supported property type name
            (functions avoids static init order fiasco)
    \return supported property type name  */
static PropertyType::SupportedTypeName suppTypeNameCol(Int32 idx //!< SupportedType enum
													  )
{
	static PropertyType::SupportedTypeName suppTypeNameColl[] = 
	{	
		"unknown", 
		"int16", 
		"int32",
        "int64",
		"float32", 
		"float64", 
		"decimal", 
		"string",
	    "datetime",
        "timespan",
		"date",
		"time",
		"intdate",
		"inttime",
        "binary"
	};
	
	return suppTypeNameColl[idx];
}

//-------------------------------------------------------------------------------------------//

//! map for finding type by string
typedef std::map <PropertyType::SupportedTypeName, SupportedTypeEnum>	CollLookupTypes;

//! init'ion of lookup map. PropertyType::s_SupportedTypeNameColl has to init'ed first!
static CollLookupTypes & prefillLookupColl()
{
	static CollLookupTypes coll;
	
	coll.clear();

	coll.insert (CollLookupTypes::value_type(suppTypeNameCol(basar::DATE    ), basar::DATE    ));
    coll.insert (CollLookupTypes::value_type(suppTypeNameCol(basar::DATETIME), basar::DATETIME));
    coll.insert (CollLookupTypes::value_type(suppTypeNameCol(basar::TIMESPAN), basar::TIMESPAN));
	coll.insert (CollLookupTypes::value_type(suppTypeNameCol(basar::DECIMAL ), basar::DECIMAL ));
	coll.insert (CollLookupTypes::value_type(suppTypeNameCol(basar::FLOAT32 ), basar::FLOAT32 )); 
	coll.insert (CollLookupTypes::value_type(suppTypeNameCol(basar::FLOAT64 ), basar::FLOAT64 )); 
	coll.insert (CollLookupTypes::value_type(suppTypeNameCol(basar::INT16   ), basar::INT16   )); 
	coll.insert (CollLookupTypes::value_type(suppTypeNameCol(basar::INT32   ), basar::INT32   ));
	coll.insert (CollLookupTypes::value_type(suppTypeNameCol(basar::INTDATE ), basar::INTDATE ));
	coll.insert (CollLookupTypes::value_type(suppTypeNameCol(basar::INTTIME ), basar::INTTIME ));
	coll.insert (CollLookupTypes::value_type(suppTypeNameCol(basar::STRING  ), basar::STRING  ));
	coll.insert (CollLookupTypes::value_type(suppTypeNameCol(basar::TIME    ), basar::TIME    ));
	coll.insert (CollLookupTypes::value_type(suppTypeNameCol(basar::UNKNOWN ), basar::UNKNOWN ));
    coll.insert(CollLookupTypes::value_type(suppTypeNameCol(basar::INT64    ), basar::INT64   ));
    coll.insert(CollLookupTypes::value_type(suppTypeNameCol(basar::BINARY   ), basar::BINARY  ));

	return coll;
}

//-------------------------------------------------------------------------------------------//
// implementation
//-------------------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw no-throw
PropertyType::PropertyType() : m_type( basar::UNKNOWN ) 
{
}				

///////////////////////////////////////////////////////////////////////////////////////////////
//!	\param eSuppType valid property type
//!	\throw no-throw
///////////////////////////////////////////////////////////////////////////////////////////////
PropertyType::PropertyType( const SupportedTypeEnum eSuppType ) : m_type (eSuppType)
{	
}	

///////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw UnknownPropertyTypeException
//!	\param rConstTypeName (string) has to be checked for a valid state (SupportedTypeEnum)
///////////////////////////////////////////////////////////////////////////////////////////////
PropertyType::PropertyType( const TypeName& rConstTypeName )
{
    FUNCTION_NAME( basar.property.PropertyType(const TypeName&) )
   
    TypeName rTypeName = rConstTypeName;
    rTypeName.trim(TypeName::FIRST_END);

	static const CollLookupTypes coll = prefillLookupColl(); // filled only once!
	
	CollLookupTypes::const_iterator itFound = coll.find(rTypeName);

	if ( itFound == coll.end() )
	{
		VarString msg;
		msg.format("An unknown/invalid typename <%s> was passed to initialize the object!", rTypeName.c_str());
        throw UnknownPropertyTypeException( ExceptInfo( fun, msg.c_str(), __FILE__, __LINE__) );
    }

	m_type = itFound->second;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//! \retval true if types are equal 
//! \return otherwise returns false
//! \param  type via const reference an object of PropertyType (type) is passed to check equality
//! \throw  no-throw
///////////////////////////////////////////////////////////////////////////////////////////////
bool PropertyType::operator==( const PropertyType& type ) const
{
    return type.m_type == m_type;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//! \retval true if types are not equal 
//! \return otherwise returns false
//! \param  type via const reference an object of PropertyType (type) is passed to check unequality
//! \throw  no-throw
///////////////////////////////////////////////////////////////////////////////////////////////
bool PropertyType::operator!=( const PropertyType& type ) const
{
    return !(type.m_type == m_type);
}

///////////////////////////////////////////////////////////////////////////////////////////////
//! \return PropertyType::SupportedTypeEnum representing the current state
//!	\throw no-throw
///////////////////////////////////////////////////////////////////////////////////////////////
basar::SupportedTypeEnum PropertyType::getType() const
{
    return m_type;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//! \return SupportedTypeName representing the current state as string
//!	\throw no-throw
///////////////////////////////////////////////////////////////////////////////////////////////
const PropertyType::SupportedTypeName PropertyType::toString() const
{
    return suppTypeNameCol(m_type);
}

} // End namespace property
} // End namespace basar
