//------------------------------------------------------------------------------
/*! \file
 *  \brief  public library interface: exceptions
 *  \author Thomas Hörath
 *  \date   14.12.2005
 */
//------------------------------------------------------------------------------

#ifndef GUARD_LIBBASARLOGIN_EXCEPTIONS_H
#define GUARD_LIBBASARLOGIN_EXCEPTIONS_H

//-----------------------------------------------------------------------------//
// dll export and import macros
//-----------------------------------------------------------------------------//
#ifndef LIBBASARLOGIN_API
	#ifdef _WIN32
		#ifdef LIBBASARLOGIN_EXPORTS
			#define LIBBASARLOGIN_API __declspec(dllexport)	//!< dll exported
		#else
			#define LIBBASARLOGIN_API __declspec(dllimport)	//!< dll imported
		#endif 
	#else 
		#define LIBBASARLOGIN_API
	#endif 
#endif 

//-----------------------------------------------------------------------------//
#include "libbasarcmnutil_exceptions.h"

//-----------------------------------------------------------------------------//
// namepsaces
//-----------------------------------------------------------------------------//
namespace basar  {
namespace login  {

//----------------------------------------------------------------------------//
// exception class definitions
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/*! \brief exception class, thrown if connect to backup login database failed

  This is a final class.*/
////////////////////////////////////////////////////////////////////////////////
class  BackupDBException : public Exception
{
public:
	//! constructs exception object and initialises text description string
	BackupDBException	(
		const ExceptInfo & sInfo	//!< output info
		) : basar::Exception(sInfo, "BackupDBException")
	{};
private:
	BackupDBException	();		//!< forbidden
};

////////////////////////////////////////////////////////////////////////////////
/*! \brief exception class, thrown if given application name is not valid

  This is a final class.*/
////////////////////////////////////////////////////////////////////////////////
class  InvalidApplNameException : public Exception
{
public:
	//! constructs exception object and initialises text description string
	LIBBASARLOGIN_API InvalidApplNameException	(
		const ExceptInfo & sInfo	//!< output-info
		) : basar::Exception(sInfo, "InvalidApplNameException")
	{};
private:
	InvalidApplNameException	();		//!< forbidden
};

////////////////////////////////////////////////////////////////////////////////
/*! \brief exception class, thrown if given area ID is not valid

  This is a final class.*/
////////////////////////////////////////////////////////////////////////////////
class  InvalidAreaException : public Exception
{
public:
	//! constructs exception object and initialises text description string
	LIBBASARLOGIN_API InvalidAreaException	(
		const ExceptInfo & sInfo	//!< output-info
		) : basar::Exception(sInfo, "InvalidAreaException")
	{};
private:
	InvalidAreaException	();		//!< forbidden
};

////////////////////////////////////////////////////////////////////////////////
/*! \brief exception class, thrown if no text (selected + std language) to reftext is found in pptext

  This is a final class.*/
////////////////////////////////////////////////////////////////////////////////
class  NoTextFoundException : public Exception
{
public:
	//! constructs exception object and initialises text description string
	LIBBASARLOGIN_API NoTextFoundException	(
		const ExceptInfo & sInfo	//!< output-info
		) : basar::Exception(sInfo, "NoTextFoundException")
	{};
private:
	NoTextFoundException	();		//!< forbidden
};

//------------------------------------------------------------------------------
} // login
} // basar

#endif // GUARD_LIBBASARLOGIN_EXCEPTIONS_H
