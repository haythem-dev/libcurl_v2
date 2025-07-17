//----------------------------------------------------------------------------------------------------------//
/*! \file
  *  \brief  class implementation of parameter list iterator
 *  \author Bjoern Bischof
 *  \date   15.12.2005
 */
//----------------------------------------------------------------------------------------------------------//

#include "libbasardbsql_connectionlist_iterator.h"
#include "libbasarcmnutil_exceptions.h"

//------------------------------------------------------------------------------------------------//
// namespaces
//------------------------------------------------------------------------------------------------//
namespace basar 
{
namespace db 
{
namespace sql
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\param col pointer to parameter-list
	\param it iterator to parameter-list */
////////////////////////////////////////////////////////////////////////////////////////////////////
ConnectionList_Iterator::ConnectionList_Iterator( CollConnectionSharedPtr col, CollConnectionIt it ) 
: m_CollConnectionSharedPtr( col ), m_CollConnectionIt( it )
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////////
ConnectionList_Iterator::ConnectionList_Iterator() 
{

}
////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \retval	true if the inner map contains at least one item and the iterator 
			is pointing to the first.
	\return	otherwise returns false
	\throw	InvalidIteratorException */
////////////////////////////////////////////////////////////////////////////////////////////////////
bool ConnectionList_Iterator::isBegin() const
{
	checkValid();
	return ( m_CollConnectionSharedPtr->begin() == m_CollConnectionIt );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \retval	true if the inner map contains at least one item and the iterator 
			is pointing to the position one beyond the last item.
	\return	otherwise returns false
	\throw	InvalidIteratorException */
////////////////////////////////////////////////////////////////////////////////////////////////////
bool ConnectionList_Iterator::isEnd() const
{
	checkValid();
	return ( m_CollConnectionSharedPtr->end() == m_CollConnectionIt );
}			
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\param rhs iterator on list to compare with m_ColParamListMapIt / m_ColParamListMap
	\retval true if equal
	\return otherwise returns false */
////////////////////////////////////////////////////////////////////////////////////////////////////
bool ConnectionList_Iterator::operator==( const ConnectionList_Iterator& rhs ) const
{
	checkValid();
	if( rhs.m_CollConnectionSharedPtr != m_CollConnectionSharedPtr )
	{		
		throw DifferentContainersIteratorException
			  (
				ExceptInfo( "basar.db.sql.ConnectionList_Iterator.operator==.", 
				            "Iterator of different container.", 
							__FILE__, __LINE__
						   )
			  );
	}
	return ( rhs.m_CollConnectionIt == m_CollConnectionIt );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\param rhs iterator on list to compare with m_ColParamListMapIt / m_ColParamListMap
	\retval true if not equal
	\return otherwise returns false */
////////////////////////////////////////////////////////////////////////////////////////////////////
bool ConnectionList_Iterator::operator!=( const ConnectionList_Iterator& rhs ) const
{
	if( rhs.m_CollConnectionSharedPtr != m_CollConnectionSharedPtr )
	{
		throw DifferentContainersIteratorException
			  (	
				ExceptInfo( "basar.cmnutil.ConnectionList_Iterator.operator!=.", 
						    "Iterator of different container.", 
							__FILE__, __LINE__
						   )
			  );
	}
	return !operator==( rhs ) ;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw OutOfRangeIteratorException 
	\return iterator, set to next element (prae) */
////////////////////////////////////////////////////////////////////////////////////////////////////
ConnectionList_Iterator ConnectionList_Iterator::operator ++()
{
	checkValid();
	/*! \todo	following situation: 
				the collection has no elements (size() == 0) and
				this logic here would allow to increment 
				--> consequence: segmentation fault (crash, abend) */
	if ( m_CollConnectionSharedPtr->end() == m_CollConnectionIt )
	{
		throw OutOfRangeIteratorException
			  (
				ExceptInfo( "basar.cmnutil.ConnectionList_Iterator.operator++.", 
							"Current position of iterator is end()!", 
							__FILE__, __LINE__
						  )
		      );
	}
	++m_CollConnectionIt;
	return *this;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw OutOfRangeIteratorException 
	\return iterator, set to next element (post) */
////////////////////////////////////////////////////////////////////////////////////////////////////
ConnectionList_Iterator ConnectionList_Iterator::operator ++(Int32)
{
	checkValid();
	if ( m_CollConnectionSharedPtr->end() == m_CollConnectionIt )
	{
		throw OutOfRangeIteratorException
			  (
				ExceptInfo( "basar.cmnutil.ConnectionList_Iterator.operator++.", 
							"Current position of iterator is end()!", 
							__FILE__, __LINE__
						  )
		      );
	}
	ConnectionList_Iterator temp = *this;
	m_CollConnectionIt++;
	return temp;
} 
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw OutOfRangeIteratorException 
	\return pointer to current iterator */
////////////////////////////////////////////////////////////////////////////////////////////////////
CollConnectionPair* ConnectionList_Iterator::operator->()
{
	checkValid();

	if( m_CollConnectionSharedPtr->end() == m_CollConnectionIt )
	{
		throw OutOfRangeIteratorException
			  (
				ExceptInfo( "basar.cmnutil.ConnectionList_Iterator.operator->", 
							"Current position of iterator is end()!", 
							__FILE__, __LINE__)
			  );
	}

	return &(*m_CollConnectionIt);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw OutOfRangeIteratorException 
	\return pointer to current iterator */
////////////////////////////////////////////////////////////////////////////////////////////////////
const CollConnectionPair* ConnectionList_Iterator::operator->() const
{
	checkValid();
	
	if( m_CollConnectionSharedPtr->end() == m_CollConnectionIt )
	{
		throw OutOfRangeIteratorException
			  (
				ExceptInfo( "basar.cmnutil.ConnectionList_Iterator.operator->() const", 
							"Current position of iterator is end()!", 
							__FILE__, __LINE__)
			  );
	}

	return &(*m_CollConnectionIt);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw InvalidIteratorException */
////////////////////////////////////////////////////////////////////////////////////////////////////
void ConnectionList_Iterator::checkValid() const
{
	if( m_CollConnectionSharedPtr.get() == 0 )
	{
		throw InvalidIteratorException(	ExceptInfo("basar.db.sql.ConnectionList_Iterator.checkValid", 
											       "Iterator of connection list isn't valid.", 
											       __FILE__, __LINE__)
									   );
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw checks pointer
	\return has Pointer */
////////////////////////////////////////////////////////////////////////////////////////////////////
bool ConnectionList_Iterator::isNull() const
{
	return  (m_CollConnectionSharedPtr.get() == 0 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw checks pointer
	\return has Pointer */
////////////////////////////////////////////////////////////////////////////////////////////////////
void ConnectionList_Iterator::reset()
{
	m_CollConnectionSharedPtr.reset();
}

}//sql
}//db
}//basar
