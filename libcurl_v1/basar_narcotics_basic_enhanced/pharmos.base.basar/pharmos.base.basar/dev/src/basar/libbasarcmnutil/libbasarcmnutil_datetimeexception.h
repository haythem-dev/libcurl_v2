#ifndef GUARD_LIBBASARCMNUTIL_DATETIMEEXCEPTION_H
#define GUARD_LIBBASARCMNUTIL_DATETIMEEXCEPTION_H


//------------------------------------------------------------------------------------------------------//
// dll import and export macros
//------------------------------------------------------------------------------------------------------//
#ifndef LIBBASARCMNUTIL_API
	#ifdef _WIN32
		#ifdef LIBBASARCMNUTIL_EXPORTS
			#define LIBBASARCMNUTIL_API __declspec(dllexport)	//!< dll exported
		#else
			#define LIBBASARCMNUTIL_API __declspec(dllimport)	//!< dll imported
		#endif 
	#else 
		#define LIBBASARCMNUTIL_API
	#endif
#endif

//------------------------------------------------------------------------------------------------------//
// namespace section
//------------------------------------------------------------------------------------------------------//
namespace basar {
namespace cmnutil {

//------------------------------------------------------------------------------------------------------//
//! \brief special exception class for datetime
//! \n final class
//------------------------------------------------------------------------------------------------------//
class DateTimeException : public BasarException
{
public:
	//! constructs exception object and initialises text description string
	LIBBASARCMNUTIL_API DateTimeException	(const ExceptInfo & sInfo) 
											: basar::Exception(sInfo, "DateTimeException")
											{};
private:
						DateTimeException	();							//!< forbidden

};
//----------------------------------------------------------------------------
}	// namespace cmnutil
}	// namespace basar


#endif //GUARD_LIBBASARCMNUTIL_DATETIMEEXCEPTION_H
