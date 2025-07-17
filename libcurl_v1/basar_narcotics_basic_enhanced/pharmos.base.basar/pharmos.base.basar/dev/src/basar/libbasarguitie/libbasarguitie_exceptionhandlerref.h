/** $Id$                                          
*                                                  
* @file                                            
* @author Anke Klink                      
* @date 2009                            
*/  

#ifndef  GUARD_BASAR_LIBBASARGUITIE_EXCEPTIONHANDLERREF_H
#define  GUARD_BASAR_LIBBASARGUITIE_EXCEPTIONHANDLERREF_H 

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

//----------------------------------------------------------------------------

#include <boost/shared_ptr.hpp>
#include "libbasar_definitions.h"
#include "libbasarguitie_definitions.h"

//----------------------------------------------------------------------------
namespace basar
{
namespace gui
{
namespace tie
{

//----------------------------------------------------------------------------
class ExceptionHandler;

//----------------------------------------------------------------------------
/*!
* Adapterclass surrounding a shared_ptr<ExceptionHandler>
* If the boost::shared_ptr<ExceptionHandler> is empty
* the default retval will be returned. The class
* does not throw an Exception
* ExceptionHandlerManager uses an ExceptionHandlerRef object
* to handle Exceptions.
*/
class ExceptionHandlerRef
{
	public:

	/*!
	* constructor, that takes a boost::shared_ptr<ExceptionHandler>
	* This way you can define your own ExceptionHandler and pass it
	* as ExceptionHandlerRef to the ExceptionHandlerManager
	* @param handler ExceptionHandler to be used
	*/
	LIBBASARGUITIE_API ExceptionHandlerRef(boost::shared_ptr<ExceptionHandler> handler);

	/*!
	* default constructor, uses DefaultExceptionHandler as 
	* internal ExceptionHandler
	*/
	LIBBASARGUITIE_API ExceptionHandlerRef();

	/**
	* virtual destructor
	*/
	LIBBASARGUITIE_API ~ExceptionHandlerRef();

	/*!
	* handles basar::Exception and returns if the Exception should be rethrown
	* or if it is handled satisfyingly. 
	*
	* The function calls the function
	* handleException(const basar::Exception & ex, const ExceptionFoundInfo & found, bool defretval)
	* on the internal boost::shared_ptr<ExceptionHandler> given at construct time,
	* if the boost::shared_ptr<ExceptionHandler> contains no value, the defretval
	* is returned, no exception is thrown.
	*
	* @param ex The Exception that is to be handled
	* @param found Information about the location the Exception was catched
	* @param defretval exspected default return value
	* @return true the exception was  handled satisfyingly 
	* @return false the exception is to be rethrown 
	*/
	LIBBASARGUITIE_API bool handleException(const basar::Exception   & ex, 
											const ExceptionFoundInfo & found, 
											bool defretval                  ) const;

	/*!
	* handles std::exception and returns if the Exception should be rethrown
	* or if it is handled satisfyingly. 
	*
	* The function calls the function
	* handleException(const std::exception & ex, const ExceptionFoundInfo & found, bool defretval)
	* on the internal boost::shared_ptr<ExceptionHandler> given at construct time,
	* if the boost::shared_ptr<ExceptionHandler> contains no value, the defretval
	* is returned, no exception is thrown.
	*
	* @param ex The Exception that is to be handled
	* @param found Information about the location the Exception was catched
	* @param defretval exspected default return value
	* @return true the exception was  handled satisfyingly 
	* @return false the exception is to be rethrown 
	*/
	LIBBASARGUITIE_API bool handleException(const std::exception & ex, const ExceptionFoundInfo & found, bool defretval) const;

	/*!
	* handles a not defined Exception and returns if the Exception should be rethrown
	* or if it is handled satisfyingly.
	*
	* The function calls the function
	* handleUnknownException(const ExceptionFoundInfo & found, bool defretval)
	* on the internal boost::shared_ptr<ExceptionHandler> given at construct time,
	* if the boost::shared_ptr<ExceptionHandler> contains no value, the defretval
	* is returned, no exception is thrown.
	*
	* @param found Information about the location the Exception was catched
	* @param defretval exspected default return value
	* @return true the exception was  handled satisfyingly 
	* @return false the exception is to be rethrown 
	*/
	LIBBASARGUITIE_API bool handleUnknownException(const ExceptionFoundInfo & found, bool defretval) const;

private:

	/*!
	* The internal ExceptionHandler, set by constructor
	*/
	boost::shared_ptr<ExceptionHandler> m_exceptionHandler;
};

//----------------------------------------------------------------------------
}
}
}

//----------------------------------------------------------------------------
#endif 
