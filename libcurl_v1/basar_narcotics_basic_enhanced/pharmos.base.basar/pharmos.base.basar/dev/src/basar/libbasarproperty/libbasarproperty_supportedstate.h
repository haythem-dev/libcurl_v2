#ifndef GUARD_LIBBASARPROPERTY_SUPPORTEDSTATE_H
#define GUARD_LIBBASARPROPERTY_SUPPORTEDSTATE_H
//--------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  enum SupportedStateEnum with all possible states, used 
			in two other files; therefore this single header-file
 *  \author Bischof Björn 
 *  \date   25.07.2005
 */
//--------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------//
#include "libbasarcmnutil_exceptions.h"

//--------------------------------------------------------------------------------------------//
// namespaces
//--------------------------------------------------------------------------------------------//
namespace basar
{
namespace property
{

//--------------------------------------------------------------------------------------------//
// exceptions
//--------------------------------------------------------------------------------------------//
/*! \brief exception class IllegalSupportedStateException for the class PropertyState.

    This is a final class.
    This class indicates that an unknown state (SupportedStateEnum) 
    is passed to the constructor of the class PropertyType */
class IllegalPropertyStateException : public Exception
{	
public:
	//! constructs exception object and initialises text description string	\throw	no-throw
	IllegalPropertyStateException( const ExceptInfo& info //!< info struct is passed with place and line of occurence and line number;
		                         )
								 : Exception( info, "basar::property::IllegalPropertyStateException" )
	{
	}

	/*! \todo BB define of exception messages */
};

//--------------------------------------------------------------------------------------------//
// typedefs
//--------------------------------------------------------------------------------------------//
typedef VarString SupportedStateName; //!< supported state as string

//--------------------------------------------------------------------------------------------//
// static members of basar::property
//--------------------------------------------------------------------------------------------//
//! names of supported states as strings
static SupportedStateName s_SupportedStateNameColl[] = 	
{
	"SS_UNKNOWN", 
	"SS_CLEAN", 
	"SS_UNSET", 
	"SS_INSERT", 
	"SS_UPDATE", 
	"SS_DELETE"
};

} // END namespace property
} // END namespace basar

#endif // GUARD_LIBBASARPROPERTY_SUPPORTEDSTATE_H
