#ifndef GUARD_LIBBASARPROPERTY_EXCEPTION_H
#define GUARD_LIBBASARPROPERTY_EXCEPTION_H
//------------------------------------------------------------------------------
/*! \file
 *  \brief  public library interface: exceptions
 *  \author Bjoern Bischof
 *  \date   21.12.2005
 */
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include "libbasar_definitions.h"
#include "libbasarcmnutil_exceptions.h"

//-----------------------------------------------------------------------------------------------------------//
// namepsaces
//-----------------------------------------------------------------------------------------------------------//
namespace basar    {
namespace property {

//-----------------------------------------------------------------------------------------------------------//
// exception class definitions
//-----------------------------------------------------------------------------------------------------------//

/*! \brief exception class AlreadyExistsException for the class PropertyDescriptionList.

This is a final class.
This exception indicates that a passed value has already been existing in the
PropertyDescriptionList.
*/
//--------------------------------------------------------------------------------------------//
    class PropertyNameAlreadyExistsException : public basar::Exception
    {
    public:
        //! constructs exception object and initialises text description string
        PropertyNameAlreadyExistsException(const ExceptInfo& info //!< info struct comprehending place and reason of occurrence and line number
        )
            : basar::Exception(info,
                "basar::property::PropertyNameAlreadyExistsException")
        {
        };
    };

/*! \brief exception class WrongPropertyTypeException for the class PropertyTable.

This is a final class.
This class indicates that a datatype is set with a value, but the method
the value is asked for is not the right method.*/
    class WrongPropertyTypeException : public basar::Exception
    {
    public:
        //! constructs exception object and initialises text description string
        WrongPropertyTypeException(const ExceptInfo& info //!< info struct comprehending place and reason of occurrence and line number 
        ) :basar::Exception(info, "basar::property::WrongPropertyTypeException")
        {
        };
    };

    /*! \brief exception class WrongOperationException for the class PropertyTable.

    This is a final class.
    This class indicates that an operation was executed on an unset state.
    But an unset state means that no valid value has been set so far.
    And no value can't be ask for.*/
    class WrongOperationException : public basar::Exception
    {
    public:
        //! constructs exception object and initialises text description string
        WrongOperationException(const ExceptInfo& info //!< info struct comprehending place and reason of occurrence and line number 
        ) :basar::Exception(info, "basar::property::WrongOperationException")
        {
        };
    };

    /*! \brief common basar exception class; to throw if iterator's containers don't match

    This is a final class. */
    class DifferentPropertyTableException : public basar::Exception
    {
    public:
        //! constructs exception object and initialises text description string
        DifferentPropertyTableException(const ExceptInfo& info//!< info struct comprehending place and reason of occurrence and line number 
        ) :basar::Exception(info, "basar::property::DifferentPropertyTableException")
        {
        };
    private:
        DifferentPropertyTableException();							//!< forbidden
    };

/*! \brief exception class FatalErrorException for the class PropertyTable_XIterator.

    This is a final class.
    This class indicates that a valid state was reached but without a value being set before.*/
class FatalErrorException : public basar::Exception
{
public:
	//! constructs exception object and initialises text description string
	FatalErrorException( const ExceptInfo& info //!< info struct comprehending place and reason of occurrence and line number 
		):basar::Exception( info, "basar::property::FatalErrorException")
	{
	};
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \brief exception class BadPropertyTypeCastException for the class PropertyTable_XIterator.

    This is a final class.
    This class indicates that a bad_any_cast has occured by the try to cast the value of 
	a PropertyInternal to a special data type. */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class BadPropertyTypeCastException : public basar::Exception
{
public:
	//! constructs exception object and initialises text description string
	BadPropertyTypeCastException( const ExceptInfo& info //!< info struct comprehending place and reason of occurrence and line number 
		):basar::Exception( info, "basar::property::BadPropertyTypeCastException")
	{
	};
private:
	//BadPropertyTypeCastException( const BadPropertyTypeCastException& );
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \brief exception class NotImplementedYetException for the class PropertyTable_XIterator.

    This is a final class.
    This class indicates that a module/method that should be used is not implemented yet. */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class NotImplementedYetException : public basar::Exception
{
public:
	//! constructs exception object and initialises text description string
	NotImplementedYetException( const ExceptInfo& info //!< info struct comprehending place and reason of occurrence and line number 
		):basar::Exception( info, "basar::property::NotImplementedYetException")
	{
	};
private:
	//NotImplementedYetException( const NotImplementedYetException& );
};

//-----------------------------------------------------------------------------------------------------------//
} // property
} // basar

#endif // GUARD_LIBBASARPROPERTY_EXCEPTION_H
