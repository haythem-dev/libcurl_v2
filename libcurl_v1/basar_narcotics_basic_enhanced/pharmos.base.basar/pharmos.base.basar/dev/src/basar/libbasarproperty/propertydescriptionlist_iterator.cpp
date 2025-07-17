//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \file
 *  \brief  class PropertyDescriptionList_Iterator is an robust wrapper class around a stl iterator to travers
			the PropertyDescriptionList
 *  \author Bischof Björn 
 *  \date   23.08.2005
 */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// includes
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "libbasarproperty_propertydescriptionlist_iterator.h"
#include "propertydescriptionlist.h"
#include "libbasarproperty_propertytype.h"
#include "../macro.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// namespaces
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace basar
{
namespace property
{
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\param rPdl is the shared pointer to the current container.
	\param rIt is the current iterator to the first parameter (container).
	\throw no-throw */ 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
PropertyDescriptionList_Iterator::PropertyDescriptionList_Iterator(  const basar::property::PdlSharedPtr rPdl, 
																     const basar::property::ColNamePropTypeCollIter rIt )
:m_Pdl( rPdl ), m_It( rIt )
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \retval	true if the iterator (this) points to the iterator the method begin() returns.
	\return	otherwise returns false
	\throw	InvalidIteratorException */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool PropertyDescriptionList_Iterator::isBegin() const
{
	return m_Pdl->begin() == *this;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \retval	true if the iterator (this) points to the iterator the method end() returns.
	\retval	otherwise false
	\throw	InvalidIteratorException */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool PropertyDescriptionList_Iterator::isEnd() const
{
	return m_Pdl->end() == *this;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \retval true if the container and its belonging iterator are the same compared with rhp one.
	\return otherwise returns false
	\param	rhp is the passed iterator compared with the container's iterator
	\throw	InvalidIteratorException, DifferentContainersIteratorException */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool PropertyDescriptionList_Iterator::operator==( const PropertyDescriptionList_Iterator rhp ) const
{
	FUNCTION_NAME( basar.property.PropertyDescriptionList_Iterator.operator==() const )	

	// checks if the iterator is valid/initialised
	checkInitialization();

	// comparison: same container and same iterator
	if( rhp.m_Pdl != m_Pdl ){
		throw DifferentContainersIteratorException
			  (
				ExceptInfo( fun, "Iterator of different container.", __FILE__, __LINE__ )
			  );
	}
	return rhp.m_It == m_It;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \retval true if the container and its belonging iterator are not the same compared with rhp one.
	\return otherwise returns false
	\param	rhp is the iterator which is compared with the container's iterator. 
	\throw	InvalidIteratorException */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool PropertyDescriptionList_Iterator::operator !=( const PropertyDescriptionList_Iterator rhp ) const
{
	return !(this->operator==(rhp) );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return PropertyDescriptionList_Iterator is the new forward switched (precrement) iterator as a copy
	\throw	InvalidIteratorException, OutOfRangeIteratorException */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
PropertyDescriptionList_Iterator PropertyDescriptionList_Iterator::operator++()
{
	FUNCTION_NAME( basar.property.PropertyDescriptionList_Iterator.operator++() )
	
	// checks if the iterator is valid
	checkInitialization();

	if( m_Pdl->m_ColNamePropTypeColl.end() == m_It ){
		VarString msg = "current position of iterator is end()!";
		throw OutOfRangeIteratorException( ExceptInfo( fun, msg.c_str(), __FILE__, __LINE__) );
	}
	++m_It;
	return *this;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return PropertyDescriptionList_Iterator is the new forward switched (postcrement) iterator as a copy
	\param	i post-crement
	\throw	InvalidIteratorException, OutOfRangeIteratorException */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
PropertyDescriptionList_Iterator PropertyDescriptionList_Iterator::operator++( int i )
{
	FUNCTION_NAME( basar.property.PropertyDescriptionList_Iterator.operator++(int) )
	i = i;

	// checks if the iterator is valid
	checkInitialization();

	if( m_Pdl->m_ColNamePropTypeColl.end() == m_It ){
		VarString msg = "current position of iterator is end()!";
		throw OutOfRangeIteratorException( ExceptInfo( fun, msg.c_str(), __FILE__, __LINE__) );
	}
	PropertyDescriptionList_Iterator it( *this );
	m_It++;                
	return it;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	ColNamePropTypePair is returned as reference relating to the object the iterator is pointing to 
	\throw InvalidIteratorException, OutOfRangeIteratorException */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
ColNamePropTypePair& PropertyDescriptionList_Iterator::operator*()
{
	FUNCTION_NAME( basar.property.PropertyDescriptionList_Iterator.operator*() )

	// checks if the iterator is valid
	checkInitialization();

	if( m_Pdl->m_ColNamePropTypeColl.end() == m_It ){
		VarString msg = "Current position of iterator is end()!";
		throw OutOfRangeIteratorException( ExceptInfo( fun, msg.c_str(), __FILE__, __LINE__) );
	}
	return *m_It;
}  
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	ColNamePropTypePair is returned as reference relating to the object the iterator is pointing to 
	\throw InvalidIteratorException, OutOfRangeIteratorException */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
const ColNamePropTypePair& PropertyDescriptionList_Iterator::operator*() const
{
	FUNCTION_NAME( basar.property.PropertyDescriptionList_Iterator.operator*() const )

	// checks if the iterator is valid
	checkInitialization();

	if( m_Pdl->m_ColNamePropTypeColl.end() == m_It ){
		VarString msg = "Current position of iterator is end()!";
		throw OutOfRangeIteratorException( ExceptInfo( fun, msg.c_str(), __FILE__, __LINE__) );
	}
	return *m_It;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	ColNamePropTypeCollIter pointing to ColNamePropTypePair is returned
	\throw InvalidIteratorException, OutOfRangeIteratorException */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
ColNamePropTypeCollIter PropertyDescriptionList_Iterator::operator->()
{
	FUNCTION_NAME( basar.property.PropertyDescriptionList_Iterator.operator->() )

	// checks if the iterator is valid
	checkInitialization();

	if( m_Pdl->m_ColNamePropTypeColl.end() == m_It ){
		VarString msg = "Current position of iterator is end()!";
		throw OutOfRangeIteratorException( ExceptInfo( fun, msg.c_str(), __FILE__, __LINE__) );
	}

    return m_It;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	ColNamePropTypeCollIter pointing to ColNamePropTypePair is returned
	\throw InvalidIteratorException, OutOfRangeIteratorException */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
const ColNamePropTypeCollIter PropertyDescriptionList_Iterator::operator->() const
{
	FUNCTION_NAME( basar.property.PropertyDescriptionList_Iterator.operator->() const )

	// checks if the iterator is valid
	checkInitialization();

	if( m_Pdl->m_ColNamePropTypeColl.end() == m_It ){
		VarString msg = "Current position of iterator is end()!";
		throw OutOfRangeIteratorException( ExceptInfo( fun, msg.c_str(), __FILE__, __LINE__) );
	}

    return m_It;
}
//------------------------------------------------------------------------------------//
// uninitialized iterator - safe access methods to it
//------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////
/*! \throw	InvalidIteratorException */
////////////////////////////////////////////////////////////////////////////////////////
void PropertyDescriptionList_Iterator::checkInitialization() const
{
	FUNCTION_NAME( basar.property.PropertyDescriptionList_Iterator.checkInitialization )

	if( m_Pdl.get() == 0 ){
		VarString msg = "Iterator is not initialised!";
		throw InvalidIteratorException( ExceptInfo( fun, msg.c_str(), __FILE__, __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! checks the value 
 *  \return true if interal PropertyDescriptionList is null, false otherwise
 */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool PropertyDescriptionList_Iterator::isNull() const
{
	return ( m_Pdl.get() == 0 );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! resets the value */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PropertyDescriptionList_Iterator::reset()
{
	m_Pdl.reset();
}

} // End namespace property
} // End namespace basar
