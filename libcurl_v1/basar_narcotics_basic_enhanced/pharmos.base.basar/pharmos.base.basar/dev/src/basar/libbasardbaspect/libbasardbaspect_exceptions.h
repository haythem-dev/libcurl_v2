#ifndef GUARD_LIBBASARDBASPECT_EXCEPTION_H
#define GUARD_LIBBASARDBASPECT_EXCEPTION_H
//------------------------------------------------------------------------------
/*! \file
 *  \brief  public library interface: exceptions
 *  \author Bjoern Bischof
 *  \date   17.11.2005
 */
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include "libbasarcmnutil_exceptions.h"

//-----------------------------------------------------------------------------//
// dll export and import macros
//-----------------------------------------------------------------------------//
#ifndef LIBBASARDBASPECT_API
	#ifdef _WIN32
		#ifdef LIBBASARDBASPECT_EXPORTS
			#define LIBBASARDBASPECT_API __declspec(dllexport)	//!< dll exported
		#else
			#define LIBBASARDBASPECT_API __declspec(dllimport)	//!< dll imported
		#endif 
	#else 
		#define LIBBASARDBASPECT_API
	#endif 
#endif 


//-----------------------------------------------------------------------------//
// namepsaces
//-----------------------------------------------------------------------------//
namespace basar  {
namespace db     {
namespace aspect {


//----------------------------------------------------------------------------//
// exception class definitions
//----------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////
/*! \brief exception class, thrown if called access method is unknown

  This is a final class.*/
////////////////////////////////////////////////////////////////////////////////
class  UnknownAccessMethodException : public Exception
{
public:
	//! constructs exception object and initialises text description string
		LIBBASARDBASPECT_API UnknownAccessMethodException		(const ExceptInfo& sInfo	//!< struct with output-info
			) : basar::Exception(sInfo, "UnknownAccessMethodException")
		{
		}
private:
		UnknownAccessMethodException		();							//!< forbidden
};

////////////////////////////////////////////////////////////////////////////////
/*! \brief exception class, thrown if accessor method already exists

  This is a final class.*/
////////////////////////////////////////////////////////////////////////////////
class  ExistAccessorMethodException : public Exception
{
public:
	//! constructs exception object and initialises text description string
		LIBBASARDBASPECT_API ExistAccessorMethodException		(const ExceptInfo& sInfo	//!< struct with output-info
			) : basar::Exception(sInfo, "ExistAccessorMethodException")
		{
		}
private:
		ExistAccessorMethodException		();							//!< forbidden
};
////////////////////////////////////////////////////////////////////////////////
/*! \brief exception class, thrown if called method name is unknown

  This is a final class.*/
////////////////////////////////////////////////////////////////////////////////
class  UnknownMethodNameException : public Exception
{
public:
	//! constructs exception object and initialises text description string
		LIBBASARDBASPECT_API UnknownMethodNameException		(const ExceptInfo& sInfo	//!< struct with output-info
			) : basar::Exception(sInfo, "UnknownMethodNameException")
		{
		}
private:
		UnknownMethodNameException		();							//!< forbidden
};
////////////////////////////////////////////////////////////////////////////////
/*! \brief exception class, thrown if accessor instance is unknown

  This is a final class.*/
////////////////////////////////////////////////////////////////////////////////
class  UnknownAccessorInstanceException : public Exception
{
public:
	//! constructs exception object and initialises text description string
		LIBBASARDBASPECT_API UnknownAccessorInstanceException		(const ExceptInfo& sInfo	//!< struct with output-info
			) : basar::Exception(sInfo, "UnknownAccessorInstanceException")
		{
		}
private:
		UnknownAccessorInstanceException		();							//!< forbidden
};
////////////////////////////////////////////////////////////////////////////////
/*! \brief exception class, thrown if accessor instance already exists

  This is a final class.*/
////////////////////////////////////////////////////////////////////////////////
class  ExistAccessorInstanceException : public Exception
{
public:
	//! constructs exception object and initialises text description string
		LIBBASARDBASPECT_API ExistAccessorInstanceException		(const ExceptInfo& sInfo	//!< struct with output-info
			) : basar::Exception(sInfo, "ExistAccessorInstanceException")
		{
		}
private:
		ExistAccessorInstanceException		();							//!< forbidden
};
////////////////////////////////////////////////////////////////////////////////
/*! \brief exception class, thrown if accessor definition is unknown

  This is a final class.*/
////////////////////////////////////////////////////////////////////////////////
class  UnknownAccessorDefinitionException : public Exception
{
public:
	//! constructs exception object and initialises text description string
		LIBBASARDBASPECT_API UnknownAccessorDefinitionException		(const ExceptInfo& sInfo	//!< struct with output-info
			) : basar::Exception(sInfo, "UnknownAccessorDefinitionException")
		{
		}
private:
		UnknownAccessorDefinitionException		();							//!< forbidden
};
////////////////////////////////////////////////////////////////////////////////
/*! \brief exception class, thrown if accessor definition already exists

  This is a final class.*/
////////////////////////////////////////////////////////////////////////////////
class  ExistAccessorDefinitionException : public Exception
{
public:
	//! constructs exception object and initialises text description string
		LIBBASARDBASPECT_API ExistAccessorDefinitionException		(const ExceptInfo& sInfo	//!< struct with output-info
			) : basar::Exception(sInfo, "ExistAccessorDefinitionException")
		{
		}
private:
		ExistAccessorDefinitionException		();							//!< forbidden
};
////////////////////////////////////////////////////////////////////////////////
/*! \brief exception class, thrown if an invalid value occurs

  This is a final class.*/
////////////////////////////////////////////////////////////////////////////////
class InvalidValueException : public Exception
{
public:
	//! constructs exception object and initialises text description string
		LIBBASARDBASPECT_API InvalidValueException		(const ExceptInfo& sInfo	//!< struct with output-info
			) : basar::Exception(sInfo, "InvalidValueException")
		{
		}
private:
		InvalidValueException		();							//!< forbidden
};
////////////////////////////////////////////////////////////////////////////////
/*! \brief exception class, thrown if a method is called in a forbidden context.

  This is a final class.*/
////////////////////////////////////////////////////////////////////////////////
class InvalidMethodCallException : public Exception
{
public:
	//! constructs exception object and initialises text description string
		LIBBASARDBASPECT_API InvalidMethodCallException( const ExceptInfo& sInfo	//!< struct with output-info
			) : basar::Exception(sInfo, "InvalidMethodCallException")
		{
		}
private:
		InvalidMethodCallException(); //!< forbidden
};
////////////////////////////////////////////////////////////////////////////////////
/*! \brief	exception class AlreadyInTransactionException for the class 
			basar::db::aspect::Connection.

    This is a final class.
	If a transaction is already in progress you cannot start a new one.
	The method call basar::db::aspect::Connection::transactionBegin() 
	is redundant. Review the program's logic to make sure the previous 
	transaction has not accidentally failed to finish.
	(The informix database error no. is '-535    Already in transaction.') */
////////////////////////////////////////////////////////////////////////////////////
class AlreadyInTransactionException : public Exception
{
public:

	//! constructs exception object and initialises text description string
	LIBBASARDBASPECT_API AlreadyInTransactionException( const ExceptInfo& info		//!< struct with output-info
		) :basar::Exception( info, "basar.db.aspect.AlreadyInTransactionException" )
	{
	}
private:
		AlreadyInTransactionException		();							//!< forbidden

};
////////////////////////////////////////////////////////////////////////////////////
/*! \brief	exception class NotInTransactionException for the class 
			basar::db::aspect::Connection.

    This is a final class.
	The informix database server cannot execute a COMMIT WORK or ROLLBACK WORK
	without previously having started a transaction with BEGIN WORK.
	(The informix database error no. is '-255    Not in transaction.')
	Before this will happen the class basar::db::aspect::Connection will catch 
	this state and points out this circumstance. */
////////////////////////////////////////////////////////////////////////////////////
class NotInTransactionException : public Exception
{
public:

	//! constructs exception object and initialises text description string
	LIBBASARDBASPECT_API NotInTransactionException( const ExceptInfo& info		//!< struct with output-info
		) :basar::Exception( info, "basar.db.aspect.NotInTransactionException" )
	{
	}
private:
		NotInTransactionException		();							//!< forbidden

};
////////////////////////////////////////////////////////////////////////////////////
/*! \brief	exception class SnapshotNotAllowedToSet for the class 
			basar::db::aspect::Accessor.

    This is a final class. */
////////////////////////////////////////////////////////////////////////////////////
class SnapshotNotAllowedToResetException : public Exception
{
public:

	//! constructs exception object and initialises text description string
	LIBBASARDBASPECT_API SnapshotNotAllowedToResetException( const ExceptInfo& info 	//!< struct with output-info
		) :basar::Exception( info, "basar.db.aspect.SnapshotNotAllowedToResetException" )
	{
	}
private:
		SnapshotNotAllowedToResetException(); //!< forbidden

};
////////////////////////////////////////////////////////////////////////////////////
/*! \brief	exception class SnapshotNotAllowedToCreateException for the class 
			basar::db::aspect::Accessor.

    This is a final class. */
////////////////////////////////////////////////////////////////////////////////////
class SnapshotNotAllowedToCreateException : public Exception
{
public:

	//! constructs exception object and initialises text description string
	LIBBASARDBASPECT_API SnapshotNotAllowedToCreateException( const ExceptInfo& info 	//!< struct with output-info
		) :basar::Exception( info, "basar.db.aspect.SnapshotNotAllowedToCreateException" )
	{
	}
private:
		SnapshotNotAllowedToCreateException(); //!< forbidden

};
////////////////////////////////////////////////////////////////////////////////////
/*! \brief	exception class RowNoLongerAvailableException for the class 
			basar::db::aspect::Accessor. 
			Thrown if the method 'AccessorPropertyTableRef::begin()' is called twice 
			in the caching mode 'SINGLE_ROW_CACHING'.

    This is a final class. */
////////////////////////////////////////////////////////////////////////////////////
class RowNoLongerAvailableException : public Exception
{
public:

	//! constructs exception object and initialises text description string
	LIBBASARDBASPECT_API RowNoLongerAvailableException( const ExceptInfo& info		//!< struct with output-info
		) :basar::Exception( info, "basar.db.aspect.RowNoLongerAvailableException" )
	{
	}
private:
		RowNoLongerAvailableException		();							//!< forbidden

};
//-----------------------------------------------------------------------------------------------------------//
/*! \brief	exception class XIteratorNotAvailableException for the class 
			basar::db::aspect::AccessorPropertyTable_YIterator. 
			Thrown if the method 'AccessorPropertyTable_YIterator::begin()' is called 
			in the caching mode 'NO_CACHING'.

    This is a final class. */
//-----------------------------------------------------------------------------------------------------------//
class XIteratorNotAvailableException : public Exception
{
public:

	//! constructs exception object and initialises text description string
	LIBBASARDBASPECT_API XIteratorNotAvailableException( const ExceptInfo& info //!< struct with output-info
	) 
	:basar::Exception( info, "basar.db.aspect.XIteratorNotAvailableException" )
	{
	}
private:
		XIteratorNotAvailableException(); //!< forbidden

};
//-----------------------------------------------------------------------------------------------------------//
/*! \brief	exception class InsertNotAllowedException for the class 
			basar::db::aspect::CacheController. 
			Thrown if the method 'CacheController::insert' is called in the caching mode 'NO_CACHING'.

    This is a final class. */
//-----------------------------------------------------------------------------------------------------------//
class InsertNotAllowedException : public Exception
{
public:

	//! constructs exception object and initialises text description string
	LIBBASARDBASPECT_API InsertNotAllowedException( const ExceptInfo& info //!< struct with output-info
	) 
	:basar::Exception( info, "basar.db.aspect.InsertNotAllowedException" )
	{
	}
private:
		InsertNotAllowedException(); //!< forbidden

};
//-----------------------------------------------------------------------------------------------------------//
/*! \brief	exception class NoSetAllowedException for the class 
			basar::db::aspect::AccessorPropertyTable_YIterator. 
			Thrown if the set-methods are called in the caching mode 'NO_CACHING'.

    This is a final class. */
//-----------------------------------------------------------------------------------------------------------//
class NoSetAllowedException : public Exception
{
public:

	//! constructs exception object and initialises text description string
	LIBBASARDBASPECT_API NoSetAllowedException( const ExceptInfo& info //!< struct with output-info
	) 
	:basar::Exception( info, "basar.db.aspect.NoSetAllowedException" )
	{
	}
private:
		NoSetAllowedException(); //!< forbidden

};

//-----------------------------------------------------------------------------------------------------------//
/*! \brief	exception class NoGetAllowedException for the class 
			basar::db::aspect::AccessorPropertyTable_YIterator. 
			Thrown if a get-method is called in the caching mode 'NO_CACHING' and 
			the current rowNumber is not congruent with the number of the datasets of the resultset.

    This is a final class. */
//-----------------------------------------------------------------------------------------------------------//
class NoGetAllowedException : public Exception
{
public:

	//! constructs exception object and initialises text description string
	LIBBASARDBASPECT_API NoGetAllowedException( const ExceptInfo& info //!< struct with output-info
	) 
	:basar::Exception( info, "basar.db.aspect.NoGetAllowedException" )
	{
	}
private:
		NoGetAllowedException(); //!< forbidden

};
//-----------------------------------------------------------------------------------------------------------//
/*! \brief	exception class NoFilteredBeginAllowedException for the class 
			basar::db::aspect::AccessorPropertyTable_YIterator. 
			Thrown if the begin-method is called with a filter (PropertyStateSet) and 
			the caching policy is 'NO_CACHING' or 'SINGLE_ROW_CACHING'.

    This is a final class. */
//-----------------------------------------------------------------------------------------------------------//
class NoFilteredBeginAllowedException : public Exception
{
public:

	//! constructs exception object and initialises text description string
	LIBBASARDBASPECT_API NoFilteredBeginAllowedException( const ExceptInfo& info //!< struct with output-info
	) 
	:basar::Exception( info, "basar.db.aspect.NoFilteredBeginAllowedException" )
	{
	}
private:
		NoFilteredBeginAllowedException(); //!< forbidden

};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \brief	exception class NoFilterAvailableException for the class 
			basar::db::aspect::AccessorPropertyTable_YIterator;
			Thrown if the getFilter()-method is called and 
			the caching policy is 'NO_CACHING' or 'SINGLE_ROW_CACHING'

    This is a final class.*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class NoFilterAvailableException : public basar::Exception
{
public:
	//! constructs exception object and initialises text description string
	NoFilterAvailableException( const ExceptInfo& info //!< info struct comprehending place and reason of occurrence and line number 
		):basar::Exception( info, "basar.property.NoFilterAvailableException")
	{
	};
private:
	NoFilterAvailableException();
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \brief	exception class NoStateAvailableException for the class 
			basar::db::aspect::AccessorPropertyTable_YIterator;
			Thrown if the getState()-method is called and 
			the caching policy is 'NO_CACHING' or 'SINGLE_ROW_CACHING'

    This is a final class.*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class NoStateAvailableException : public basar::Exception
{
public:
	//! constructs exception object and initialises text description string
	NoStateAvailableException( const ExceptInfo& info //!< info struct comprehending place and reason of occurrence and line number 
		):basar::Exception( info, "basar.db.aspect.NoStateAvailableException")
	{
	};
private:
	NoStateAvailableException();
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \brief	exception class UnknownTypeException for the class 
			basar::db::aspect::Accessor;
			Thrown if the setPreparedStatementValues()-method is called
			and an unkown basar type is encountered
			

    This is a final class.*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class UnknownTypeException : public basar::Exception
{
public:
	//! constructs exception object and initialises text description string
	UnknownTypeException( const ExceptInfo& info //!< info struct comprehending place and reason of occurrence and line number 
		):basar::Exception( info, "basar.db.aspect.UnknownTypeException")
	{
	};
private:
	UnknownTypeException();
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \brief	exception class StorePreparedStatementFailed for the class 
			basar::db::aspect::Accessor;
			Thrown if the getPreparedStatementRef()-method can't store a
			prepared statement in it's map
		

    This is a final class.*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class StorePreparedStatementFailedException : public basar::Exception
{
public:
	//! constructs exception object and initialises text description string
	StorePreparedStatementFailedException( const ExceptInfo& info //!< info struct comprehending place and reason of occurrence and line number 
		):basar::Exception( info, "basar.db.aspect.StorePreparedStatementFailedException")
	{
	};
private:
	StorePreparedStatementFailedException();
};
//------------------------------------------------------------------------------
} // aspect
} // db
} // basar

#endif // GUARD_LIBBASARDBASPECT_EXCEPTION_H
