//----------------------------------------------------------------------------
/*! \file
 *  \brief  ...
 *  \author Bischof Björn 
 *  \date   03.09.2005
 */
//----------------------------------------------------------------------------
#ifndef GUARD_SAVEACCESSCOLLECTION_H
#define GUARD_SAVEACCESSCOLLECTION_H

//----------------------------------------------------------------------------------------------------------//
// includes
//----------------------------------------------------------------------------------------------------------//
#include "libbasar_definitions.h"

//----------------------------------------------------------------------------------------------------------//
// namespaces
//----------------------------------------------------------------------------------------------------------//
namespace basar
{
//----------------------------------------------------------------------------------------------------------//
// template class definition
//----------------------------------------------------------------------------------------------------------//
/*! \brief	Class SaveAccessCollection enables a protected access to a collection class incl. iterator.
			Both have to be passed as template parameters at instantiation.
			The collection class (Proxy) has to provide a method 'get()' for accessing the implementation 
			pointer object to test if the implementation object has already been instantiated.

	This is a final class.
	Class throws InvalidIteratorException.*/
template<typename Collection, typename Iterator>
class SaveAccessCollection
{
private:
	Collection m_Coll;	//!< member of the collection aggregating its implementation.
	Iterator   m_Iter;	//!< member of an iterator belonging to the collection.
	
	/*! \brief	private method to check if the implementation object has been instantiated.
				\n InvalidIteratorException */	
	void checkCollection() const
	{
		const char* fun = "basar.property.SaveAccessCollection.checkCollection";
		if( m_Coll.get() == 0 ){
			throw InvalidIteratorException
					( 
						ExceptInfo( fun, "Iterator is not initialized/valid!", __FILE__, __LINE__ )
					);
		}
	}
public:
	
	/*! \brief	default constructor. \n no-throw */	
	SaveAccessCollection()
	:m_Coll( Collection() ), m_Iter( Iterator() )
	{}
	
	/*! \brief	copy constructor. n no-throw */	
	SaveAccessCollection( const Collection& coll, const Iterator& iter )
	:m_Coll( coll ), m_Iter( iter )
	{}
	
	/*! \brief	returns the collection class as reference. 
				\n throws InvalidIteratorException */	
	Collection& Coll()
	{
		checkCollection();
		return m_Coll;
	}
	/*! \brief	returns the collection class as const reference. 
				\n throws InvalidIteratorException */	
	const Collection& Coll() const 
	{
		checkCollection();
		return m_Coll;
	}
	/*! \brief	returns the iterator of the collection class as reference. 
				\n throws InvalidIteratorException */	
	Iterator& Iter()
	{
		checkCollection();
		return m_Iter;
	}
	/*! \brief	returns the iterator of the collection class as const reference. 
				\n throws InvalidIteratorException */	
	const Iterator& Iter() const
	{
		checkCollection();
		return m_Iter;
	}

}; // END class PropertyTable_YIterator

} // END namespace basar


#endif // GUARD_SAVEACCESSCOLLECTION_H
