/** $Id$              
*                  
* @file                
* @author Anke Klink        
* @date 2009          
*/  

#ifndef  GUARD_BASAR_LIBBASARGUITIE_LIBBASARGUITIE_DEFAULTEXCEPTIONHANDLER_H 
#define  GUARD_BASAR_LIBBASARGUITIE_LIBBASARGUITIE_DEFAULTEXCEPTIONHANDLER_H 

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
#include "libbasarguitie_exceptionhandler.h" 

//----------------------------------------------------------------------------
class QString;

//----------------------------------------------------------------------------
namespace basar
{
namespace gui
{
namespace tie
{
/*!
* This is the default way to show Exceptions, the default view can be 
* changed, by Reimplementation of ExceptionHandler an calling:
* Manager::getExceptionHandlerManager().setExceptionHandlerRef(ExceptionHandlerRef(boost::shared_ptr<ExceptionHandler>(myexhandlerptr));
* 
* The DefaultExceptionHandler::handleException and DefaultExceptionHandler::handleUnknownException
* shows a simple QMessagebox in english, with 
* Information about the Exception (ex.what()) and information about the 
* location where it was catched (values: context, file and line).
* If the given returnvalue is "true" a messagebox with the 
* type waring is shown and the returnvalue (true) is returned.
* If the given returnvalue is "false" a messagebox with the 
* type critical is shown and the returnvalue (false) is returned.
*/ 
class DefaultExceptionHandler : public ExceptionHandler 
{  
public:

	/*!
	* Constructor for DefaultExceptionHandler
	*/
	LIBBASARGUITIE_API  DefaultExceptionHandler();

	/*!
	* virtual Destructor for DefaultExceptionHandler
	*/
	LIBBASARGUITIE_API  virtual ~DefaultExceptionHandler();

	/*!
	* handles basar::Exception and returns if the Exception should be rethrown
	* or if it is handled satisfyingly.
	* The function shows a simple QMessagebox in english, with 
	* Information about the Exception (ex.what()) and information about the 
	* location where it was catched (values: context, file and line).
	* If the given returnvalue is "true" a messagebox with the 
	* type waring is shown and the returnvalue (true) is returned.
	* If the given returnvalue is "false" a messagebox with the 
	* type critical is shown and the returnvalue (false) is returned.
	*
	* @param ex The Exception that is to be handled
	* @param found Information about the location the Exception was catched
	* @param defretval default return value
	* @return true the exception was  handled satisfyingly 
	* @return false the exception is to be rethrown 
	*/
	LIBBASARGUITIE_API  bool handleException(const basar::Exception & ex, const ExceptionFoundInfo & found, bool defretval) ;

	/*!
	* handles std::Exception and returns if the Exception should be rethrown
	* or if it is handled satisfyingly.
	* The function shows a simple QMessagebox in english, with 
	* Information about the Exception (ex.what()) and information about the 
	* location where it was catched (values: context, file and line).
	* If the given returnvalue is "true" a messagebox with the 
	* type waring is shown and the returnvalue (true) is returned.
	* If the given returnvalue is "false" a messagebox with the 
	* type critical is shown and the returnvalue (false) is returned.
	*
	* @param ex The Exception that is to be handled
	* @param found Information about the location the Exception was catched
	* @param defretval default return value
	* @return true the exception was  handled satisfyingly 
	* @return false the exception is to be rethrown 
	*/
	LIBBASARGUITIE_API  bool handleException(const std::exception & ex, const ExceptionFoundInfo & found, bool defretval) ;

	/*!
	* handles a not defined Exception and returns if the Exception should be rethrown
	* or if it is handled satisfyingly  
	* The function shows a simple QMessagebox in english, 
	* with the text "Unknown Exception :" and 
	* information about the 
	* location where it was catched (values: context, file and line).
	* If the given returnvalue is "true" a messagebox with the 
	* type waring is shown and the returnvalue (true) is returned.
	* If the given returnvalue is "false" a messagebox with the 
	* type critical is shown and the returnvalue (false) is returned.
	*
	* @param found Information about the location the Exception was catched
	* @param defretval default return value
	* @return true the exception was  handled satisfyingly 
	* @return false the exception is to be rethrown 
	*/
	LIBBASARGUITIE_API  bool handleUnknownException(const ExceptionFoundInfo & found, bool defretval)  ;

protected:

	/*!
	* internal function, that writes the Information about the 
	* location where the exception was catched and about the 
	* returnvalue of a handleException/handleUnknownException
	* memberfunction
	* @param found Information about the location the Exception is catched at
	* @param ok defaines if it is to be a warning or an error
	* @retval Text to be shown
	*/
	QString getInfo( const ExceptionFoundInfo & found , bool ok);

	/*!
	* internal function, that shows the MessageBox with the Text s
	* as Text. If the value ok is true, the messagebox has the type
	* warning, if the value ok is false an error (messagebox type
	* critical) is shown. The Caption of the messagebox is
	* "Exception - Warning" or "Exception - Error". 
	* @param ok defaines if it is to be a warning or an error
	* @param s Text to be shown
	*/
	void show(const QString & s, bool ok);
}; 

//------------------------------------------------------------------------------
}
}
}
#endif 

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
