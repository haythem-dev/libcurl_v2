/** $Id$                                          
*                                                  
* @file                                            
* @author Anke Klink                      
* @date 2009                            
*/    

#ifndef  GUARD_BASAR_LIBBASARGUITIE_EXCEPTIONHANDLER_H
#define  GUARD_BASAR_LIBBASARGUITIE_EXCEPTIONHANDLER_H

//----------------------------------------------------------------------------
#include <exception>
#include "libbasar_definitions.h"
#include "libbasarguitie_definitions.h"

//----------------------------------------------------------------------------
namespace basar
{
namespace gui
{
namespace tie
{

/*!
* Interfaceclass for exceptionhandling by the ExceptionHandlerManager.
* This class can be overwritten to define your own ExceptionHandler.
* As default ExceptionHandler DefaultExceptionHandler class is used.
*/
class ExceptionHandler
{
public:

	/*!
		* empty constructor for interface 
		* class  ExceptionHandler
		*/
	ExceptionHandler() {}

	/*!
		* empty virtual destructor for 
		* interface class  ExceptionHandler
		*/
	virtual ~ExceptionHandler() {}

	/*!
		* handles basar::Exception and returns if the Exception should be rethrown
		* or if it is handled satisfyingly
		* @param ex The Exception that is to be handled
		* @param found Information about the location the Exception was catched
		* @param defretval expected default return value
		* @return true the exception was  handled satisfyingly 
		* @return false the exception is to be rethrown 
		*/
	virtual  bool handleException(const basar::Exception & ex, const ExceptionFoundInfo & found, bool defretval) = 0;
	
	/*!
		* handles std::Exception and returns if the Exception should be rethrown
		* or if it is handled satisfyingly  
		* @param ex The Exception that is to be handled
		* @param found Information about the location the Exception was catched
		* @param defretval exspected default return value
		* @return true the exception was  handled satisfyingly 
		* @return false the exception is to be rethrown 
		*/
	virtual  bool handleException(const std::exception & ex, const ExceptionFoundInfo & found, bool defretval) = 0;
	
	/*!
		* handles a not defined Exception and returns if the Exception should be rethrown
		* or if it is handled satisfyingly  
		* @param found Information about the location the Exception was catched
		* @param defretval exspected default return value
		* @return true the exception was  handled satisfyingly 
		* @return false the exception is to be rethrown 
		*/
	virtual  bool handleUnknownException(const ExceptionFoundInfo & found, bool defretval) = 0;
};

}
}
}
#endif 

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
