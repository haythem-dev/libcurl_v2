//-------------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  This class PropertyDescriptionListRef is a special proxy (smart reference) 
			to the class PropertyDescriptionList.
 *  \author Bischof Björn 
 *  \date   25.07.2005
 */
//-------------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------------//

#include "libbasarproperty_propertydescriptionlistref.h"
#include "propertydescriptionlist.h"
#include "libbasarproperty_propertydescriptionlist_iterator.h"
#include "libbasarproperty_propertytype.h"

#include "libbasardebug.h"
#include "../macro.h"

//////////////////////////////////////////////////////////////////////////////
// namespaces
//////////////////////////////////////////////////////////////////////////////
namespace basar
{
namespace property
{

//----------------------------------------------------------------------------
void dumpPropDescList(size_t addr)
{
	if (0 == addr)
		return;

	PropertyDescriptionListRef obj = *(reinterpret_cast<PropertyDescriptionListRef * >(addr));

	if (obj.isNull())
		return;

	std::ostringstream outStr;

	obj.toStream(outStr);

	debug::printDbgMsg(outStr.str().c_str());
}

///////////////////////////////////////////////////////////////////////////////////////////////
// implementation
///////////////////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------//
// methods of the reference class only 
//-------------------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw no-throw
///////////////////////////////////////////////////////////////////////////////////////////////
PropertyDescriptionListRef::PropertyDescriptionListRef()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw no-throw
//!	\param pImpl implementation object in the form of a shared pointer
///////////////////////////////////////////////////////////////////////////////////////////////
PropertyDescriptionListRef::PropertyDescriptionListRef( const PdlSharedPtr pImpl )
:m_pImpl( pImpl )
{
}
///////////////////////////////////////////////////////////////////////////////////////////////
/*! \retval	true if the implementation object hasn't been instantiated yet 
	\return otherwise returns false
	\throw no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////
bool PropertyDescriptionListRef::isNull() const
{
	return( m_pImpl.get() == 0 );
}
///////////////////////////////////////////////////////////////////////////////////////////////
//!	\return const shared pointer to implementation object
//! \throw  NullReferenceException 
///////////////////////////////////////////////////////////////////////////////////////////////
const PdlSharedPtr& PropertyDescriptionListRef::getImpl() const
{
	FUNCTION_NAME( basar.property.PropertyDescriptionListRef.getImpl ) 
	if( isNull() ){
		VarString msg = "object of type PropertyDescriptionList is not initialized!";
		throw NullReferenceException( ExceptInfo( fun, msg.c_str(), __FILE__, __LINE__) );
    }
	return m_pImpl;
}
///////////////////////////////////////////////////////////////////////////////////////////////
/*! \throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////
void PropertyDescriptionListRef::reset()
{
	m_pImpl.reset();
}
//-------------------------------------------------------------------------------------------//
// methods of the implementation object (delegated)
//-------------------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	rName is the columnname appended to the already existing columns in combination 
			with the next parameter pType
	\param	pType is the type (SupportedTypeEnum) appended to the already existing columns
	\throw  PropertyNameAlreadyExistsException */
///////////////////////////////////////////////////////////////////////////////////////////////
void PropertyDescriptionListRef::push_back( const ColumnName& rName, const PropertyType pType )
{
    getImpl()->push_back( rName, pType );
}
///////////////////////////////////////////////////////////////////////////////////////////////
/*! \param	pair is the combination of a columnname and a type (SupportedTypeEnum) which 
			is added to the already existing columns
	\throw  PropertyNameAlreadyExistsException */
///////////////////////////////////////////////////////////////////////////////////////////////
void PropertyDescriptionListRef::push_back( const ColNamePropTypePair& pair )
{
    getImpl()->push_back( pair );
}

///////////////////////////////////////////////////////////////////////////////////////////////
/*! \param	pd is the combination of a columnname and a type (SupportedTypeEnum) which 
			is added to the already existing columns
	\throw  PropertyNameAlreadyExistsException */
///////////////////////////////////////////////////////////////////////////////////////////////
void PropertyDescriptionListRef::push_back( const PropertyDescription & pd)
    {
         getImpl()->push_back( pd );
    }
///////////////////////////////////////////////////////////////////////////////////////////////
//!	\return	size of the row
//! \throw  PropertyNameAlreadyExistsException
///////////////////////////////////////////////////////////////////////////////////////////////
NumberColumns PropertyDescriptionListRef::size() const
{
    return getImpl()->size();
}
///////////////////////////////////////////////////////////////////////////////////////////////
/*! \retval true if the passed columnname already exists
	\return otherwise returns false
	\param	rName is the columnname which existence is asked for
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////
bool PropertyDescriptionListRef::contains( const ColumnName& rName ) const
{
	return getImpl()->contains( rName );
}
///////////////////////////////////////////////////////////////////////////////////////////////
/*! \retval true if the passed columnname already exists.
	\return otherwise returns false
	\param	rName is the columnname whose existence is checked.
	\param	rPropType is the state whose existence is checked.
			in combination with the passed columnname.
	\throw	no-throw */
 ///////////////////////////////////////////////////////////////////////////////////////////////
bool PropertyDescriptionListRef::contains( const ColumnName& rName, 
										   const PropertyType& rPropType ) const
{
    return getImpl()->contains( rName, rPropType );
}
///////////////////////////////////////////////////////////////////////////////////////////////
/*! \retval true if the passed columnname already exists 
	\param	pair combination of columnname and state
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////
bool PropertyDescriptionListRef::contains( const ColNamePropTypePair& pair ) const
{
    return getImpl()->contains( pair );
}
///////////////////////////////////////////////////////////////////////////////////////////////
/*! \return index of given column
	\param	rColName string based columnname whose index is needed
	\throw	UnknownPropertyNameException */
///////////////////////////////////////////////////////////////////////////////////////////////
ColumnIndex PropertyDescriptionListRef::getIndex( const ColumnName& rColName ) const
{
    return getImpl()->getIndex( rColName );
}
///////////////////////////////////////////////////////////////////////////////////////////////
/*! \return ColumnName for given index
	\param	index for which the columnname is needed
	\throw	OutOfRangeIndexException */
///////////////////////////////////////////////////////////////////////////////////////////////
const ColumnName& PropertyDescriptionListRef::getName( const ColumnIndex index ) const
{
    return getImpl()->getName( index );
}
///////////////////////////////////////////////////////////////////////////////////////////////
/*! \return PropertyType
	\param	index for which the type (SupportedTypeEnum) is needed
	\throw	OutOfRangeIndexException */
///////////////////////////////////////////////////////////////////////////////////////////////
const PropertyType& PropertyDescriptionListRef::getType( const ColumnIndex index ) const
{
    return getImpl()->getType( index );
}
///////////////////////////////////////////////////////////////////////////////////////////////
/*! \return PropertyType
	\param	rName columnname for which type (SupportedTypeEnum) is needed
	\throw	UnknownPropertyNameException */
///////////////////////////////////////////////////////////////////////////////////////////////
const PropertyType& PropertyDescriptionListRef::getType( const ColumnName& rName ) const
{
    return getImpl()->getType( rName );
}
///////////////////////////////////////////////////////////////////////////////////////////////
/*! \return PropertyDescriptionList_Iterator pointing to the first element
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////
PropertyDescriptionList_Iterator PropertyDescriptionListRef::begin()
{
    return getImpl()->begin();
}
///////////////////////////////////////////////////////////////////////////////////////////////
/*! \return PropertyDescriptionList_Iterator pointing to the element one position beyond the last one
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////
PropertyDescriptionList_Iterator PropertyDescriptionListRef::end()
{
    return getImpl()->end();
}
///////////////////////////////////////////////////////////////////////////////////////////////
/*! \return Reference to PropertyDescriptionList
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////
PropertyDescriptionListRef PropertyDescriptionListRef::deepCopy()
{
    return PropertyDescriptionListRef( getImpl()->deepCopy() );
}
///////////////////////////////////////////////////////////////////////////////////////////////
/*! \param	rValuePairs pair of strings to set to PropertyDescriptionList
	\throw	string based exception indicates that no space was given to separate the 
			datatype from the columnname */
///////////////////////////////////////////////////////////////////////////////////////////////
void PropertyDescriptionListRef::construct( const VarString& rValuePairs )
{
	getImpl()->construct( rValuePairs );
}

///////////////////////////////////////////////////////////////////////////////////////////////
/*! \param	rValuePairs pair of strings to set to PropertyDescriptionList
	\throw	string based exception indicates that no space was given to separate the 
			datatype from the columnname */
///////////////////////////////////////////////////////////////////////////////////////////////
void PropertyDescriptionListRef::construct( const PropertyDescriptionVector& rValuePairs )
{
	getImpl()->construct( rValuePairs );
}

///////////////////////////////////////////////////////////////////////////////////////////////
/*! \param	strm parameter is default initialized and could be overwritten by any 
			std::ostream object, e.g. by the error channel or any object of an ofstream file.
  	\throw  no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////
void PropertyDescriptionListRef::toStream( std::ostream& strm ) const
{
	getImpl()->toStream( strm );
}
///////////////////////////////////////////////////////////////////////////////////////////////
/*! writes the data to a string (in the same format as the construction string)
    f.ex. "string col1;int col2;"
    \return string (in the same format as the construction string) f.ex. "string col1;int col2;"
  	\throw  no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////
VarString  PropertyDescriptionListRef::toString() const
{
   return getImpl()->toString();
}


} // End namespace property
} // End namespace basar
