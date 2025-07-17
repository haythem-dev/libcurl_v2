#ifndef GUARD_LIBBASARDBSQL_CONNECTIONLIST_ITERATOR_H
#define GUARD_LIBBASARDBSQL_CONNECTIONLIST_ITERATOR_H
//----------------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  connection iterator class definition
 *  \author Bjoern Bischof
 *  \date   15.12.2005
 */
//----------------------------------------------------------------------------------------------------------//

//----------------------------------------------------------------------------------------------------------//
#include "libbasardbsql_api.h"
#include "libbasardbsql_definitions.h"

//----------------------------------------------------------------------------------------------------------//
namespace basar {
namespace db    {
namespace sql   {

//----------------------------------------------------------------------------------------------------------//
/*! \brief  class ConnectionList_Iterator represents an iterator pointing 
			to an element of collection of connections
                        
    This is a final class.     
    Class throws the follwing exceptions: 
	class	OutOfRangeIteratorException, 
			WrongPropertyTypeException,
			WrongOperationException,
			FatalErrorException(?). */
//----------------------------------------------------------------------------------------------------------//
// class declaration
//----------------------------------------------------------------------------------------------------------//
class ConnectionList_Iterator
{
	public:	
		/*! \brief overloaded constructor. \n no-throw */
		LIBBASARDBSQL_API ConnectionList_Iterator( CollConnectionSharedPtr coll, CollConnectionIt it );
		
		/*! \brief standard-constructor. \n no-throw */
		LIBBASARDBSQL_API ConnectionList_Iterator();

		/*! \brief checks if iterator points to begin() of the inner container
			\n     no-throw */
		LIBBASARDBSQL_API bool						isBegin() const;

		/*! \brief checks if iterator points to end() of the inner container
			\n     no-throw */
		LIBBASARDBSQL_API bool						isEnd() const;

		/*! \brief operator to check two iterators for equality  \n     no-throw */
		LIBBASARDBSQL_API bool						operator==( const ConnectionList_Iterator& rhs ) const;
		
		/*! \brief operator to check two iterators for unequality \n     no-throw */
		LIBBASARDBSQL_API bool						operator!=( const ConnectionList_Iterator& rhs ) const;

		/*! \brief operator to increase iterator (praecrement) \n     OutOfRangeIteratorException */
		LIBBASARDBSQL_API ConnectionList_Iterator	operator ++	();
		
		/*! \brief operator to increase iterator (postcrement) \n     OutOfRangeIteratorException */
		LIBBASARDBSQL_API ConnectionList_Iterator	operator ++	(Int32);

		/*! \brief operator ->\n     OutOfRangeIteratorException */
		LIBBASARDBSQL_API CollConnectionPair*		operator->();
		
		/*! \brief operator -> \n     OutOfRangeIteratorException */
		LIBBASARDBSQL_API const CollConnectionPair*	operator->() const;

        /*! \brief checking if  pointer to implementation class is null 
		\n no-throw */
		LIBBASARDBSQL_API bool isNull() const;

		/*! \brief setting the pointer to null.
		\n no-throw */
		LIBBASARDBSQL_API void reset();

	private:
		/*! \brief checks if the iterator is valid
 			\n     no-throw */
		void										checkValid() const;
		CollConnectionSharedPtr						m_CollConnectionSharedPtr;		//!< reference to collection with connections
		CollConnectionIt							m_CollConnectionIt;				//!< iterator to connection
};


//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

//----------------------------------------------------------------------------
#endif	// end GUARD_LIBBASARDBSQL_CONNECTIONLIST_ITERATOR_H
