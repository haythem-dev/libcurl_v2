//--------------------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  public library interface: exceptions
 *  \author Michael Eichenlaub
 *  \date   14.08.2005
 */
//--------------------------------------------------------------------------------------------------------------//

#ifndef GUARD_LIBBASARGUITIE_EXCEPTIONS_H
#define GUARD_LIBBASARGUITIE_EXCEPTIONS_H

//----------------------------------------------------------------------------
#ifndef LIBBASARGUITIE_API
	#ifdef _WIN32
		#ifdef LIBBASARGUITIE_EXPORTS
			#define LIBBASARGUITIE_API __declspec(dllexport)        //!< dll exported
		#else
			#define LIBBASARGUITIE_API __declspec(dllimport)        //!< dll imported
		#endif
	#else
		#define LIBBASARGUITIE_API
	#endif
#endif	// LIBBASARGUITIE_API

//------------------------------------------------------------------------------
#include "libbasarcmnutil_exceptions.h"

//------------------------------------------------------------------------------
namespace basar {
namespace gui {
namespace tie {

//------------------------------------------------------------------------------
/*! \brief exception class, thrown if typecast failed
 *
 *  This is a final class. */
class  TypecastFailedException : public Exception
{
public:
	//! constructs exception object and initialises text description string
		LIBBASARGUITIE_API TypecastFailedException		(const ExceptInfo& sInfo	//!< struct with output-info
			) : basar::Exception(sInfo, "TypecastFailedException")
		{
		}
private:
		TypecastFailedException		();							//!< forbidden
};

//------------------------------------------------------------------------------
/*! \brief exception class, thrown if there is a column in one iterator which is unknown in the other one
 *
 *  This is a final class. */
class  UnknownColumnException : public Exception
{
public:
	//! constructs exception object and initialises text description string
		LIBBASARGUITIE_API UnknownColumnException		(const ExceptInfo& sInfo	//!< struct with output-info
			) : basar::Exception(sInfo, "UnknownColumnException")
		{
		}
private:
		UnknownColumnException		();							//!< forbidden
};

//------------------------------------------------------------------------------
/*! \brief exception class, thrown if the index is unknown in the property table
 *
 * This is a final class. */
class  UnknownIndexException : public Exception
{
public:
	//! constructs exception object and initialises text description string
		LIBBASARGUITIE_API UnknownIndexException		(const ExceptInfo& sInfo	//!< struct with output-info
			) : basar::Exception(sInfo, "UnknownIndexException")
		{
		}
private:
		UnknownIndexException		();							//!< forbidden
};

//------------------------------------------------------------------------------
/*! \brief	exception class GuiControlNotSupported for the class GuiPropertyTable_YIterator.
 *			It will be thrown if a gui ctrl occurs that is not regarded 
 *			in the GuiPropertyTable.
 *   This is a final class. */
class GuiControlNotSupportedException : public basar::Exception
{
public:
	//! constructs exception object and initialises text description string
	GuiControlNotSupportedException( const ExceptInfo& info //!< info struct comprehending place and reason of occurrence and line number
		):basar::Exception( info, "basar::property::GuiControlNotSupportedException")
	{
	};
private:
	GuiControlNotSupportedException		();							//!< forbidden
};

//------------------------------------------------------------------------------
/*! \brief	exception class UnableToReadWriteInLineOfControlException for the class 
 *			GuiPropertyTable_YIterator.
 *			Thrown if user wants to write in or read from a row of a multi line ctrl 
 *			that does not exist.
 * This is a final class. 
 */
class UnableToReadWriteInLineOfControlException : public basar::Exception
{
public:
	//! constructs exception object and initialises text description string
	UnableToReadWriteInLineOfControlException( const ExceptInfo& info //!< info struct comprehending place and reason of occurrence and line number
		):basar::Exception( info, "basar::property::UnableToReadWriteInLineOfControlException")
	{
	};
private:
	UnableToReadWriteInLineOfControlException		();							//!< forbidden
};

/*!
 * for  downward compatibility, identical to UnableToReadWriteInLineOfControlException
 */
typedef UnableToReadWriteInLineOfControlException UnableToWriteInLineOfControlException;

/*!
 * identical to UnableToReadWriteInLineOfControlException
 */
typedef UnableToReadWriteInLineOfControlException UnableToReadInLineOfControlException;

//------------------------------------------------------------------------------
/*! \brief	exception class NoValidMemoryPolicyException for the class 
 *			GuiPropertyTable_XIterator.
 *			Thrown if a policy is set that is not being taken into consideration.
 *
 *  This is a final class. */
class NoValidMemoryPolicyException : public basar::Exception
{
public:
	//! constructs exception object and initialises text description string
	//! param info struct comprehending place and reason of occurrence and line number
	NoValidMemoryPolicyException( const ExceptInfo& info //!< info struct comprehending place and reason of occurrence and line number
		):basar::Exception( info, "basar::property::NoValidMemoryPolicyException")
	{
	};
private:
	NoValidMemoryPolicyException		();							//!< forbidden
};

//--------------------------------------------------------------------------------------------------------------//
/*! \brief	exception NoValidWidgetPolicyException for the class GuiPropertyTable_XIterator.
 *			Thrown if a policy is set that is not being taken into consideration.
 *
 *  This is a final class. */
class NoValidWidgetPolicyException : public basar::Exception
{
public:
	//! constructs exception object and initialises text description string
	NoValidWidgetPolicyException( const ExceptInfo& info //!< info struct comprehending place and reason of occurrence and line number
		):basar::Exception( info, "basar::property::NoValidWidgetPolicyException")
	{
	};
private:
	NoValidWidgetPolicyException		();							//!< forbidden
};


//--------------------------------------------------------------------------------------------------------------//
} // tie
} // gui
} // basar

#endif	// GUARD
