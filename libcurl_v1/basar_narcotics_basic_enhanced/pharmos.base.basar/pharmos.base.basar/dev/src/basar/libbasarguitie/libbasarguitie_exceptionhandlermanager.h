/*
* @file
* @author Anke Klink
* @date 2009
*/

#ifndef  GUARD_BASAR_LIBBASARGUITIE_EXCEPTIONHANDLERMANAGER_H 
#define  GUARD_BASAR_LIBBASARGUITIE_EXCEPTIONHANDLERMANAGER_H 

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
#include "libbasarguitie_exceptionhandlerref.h" 

//----------------------------------------------------------------------------
namespace basar
{
namespace gui
{
namespace tie
{

//----------------------------------------------------------------------------
/*!
* ExceptionManager handles basar::Exceptions, std::exceptions and unknown
* Exceptions.
*
* The ExceptionManager consists of 4 Values, an ExceptionHandlerRef 
* object that handles the Exceptions and 3 boolean default Values
* that set the default returnvalue for the ExceptionHandler functions.
*
* The default exceptionhandling (DefaultExceptionHandler) shows a 
* warning or error QMessageBox, detailing the source of the exception 
* and where it was catched. This default view can be changed, by
* overwriting the ExceptionHandler Interface and calling:
* Manager::getInstance().getExceptionManager().setExceptionHandlerRef(myhandler)
* An Example can be found in unitTest utguitie file exceptionhandler.cpp Test 
* GuiTie_TestExceptionHandler_testExceptionHandlerManager1 with 
* utguitie::TestExceptionHandler.
*
* The returnvalues for the handle Functions define, if the exception is to be rethrown.
* The returnvalues for the handle Functions work as the returnvalues of an
* event routine, if the returnvalue is true, the exception is seen as handled,
* if the returnvalue is false, the exception will be rethrown by the caller.
*                 
* Defaultreturnvalues can be defined for the three Types of exceptions:
* basar::Exception, std::exception, all else 
* Those are prefefined as true for all three Types, as the Exceptionhandler
* is to be used mostly in eventFilter routines. You may change the values by
* calling:
* <pre>
*   basar::gui::tie::Manager::getInstance().getExceptionHandlerManager().setDefaultRetvalBasarException(false);
*   basar::gui::tie::Manager::getInstance().getExceptionHandlerManager().setDefaultRetvalSTDException(false);
*   basar::gui::tie::Manager::getInstance().getExceptionHandlerManager().setDefaultRetvalUnknownException(false);
* or
*   basar::gui::tie::Manager::getInstance().getExceptionHandlerManager().setDefaultRetval(false, false, false);
* </pre>
*
* For each handle Function that uses the default returnvalue, there is one that
* lets you define an other defaultvalue, that is only used in this function call
* <pre>
*    bool handleException(const basar::Exception & ex, const ExceptionFoundInfo & found)  ;
*	 bool handleException(const std::exception & ex, const ExceptionFoundInfo & found)  ;
*	 bool handleUnknownException(const ExceptionFoundInfo & found)  ;
* </pre>
* use the default Values, set by:
* <pre>
* void setDefaultRetval(bool , bool , bool);
* void setDefaultRetvalBasarException(bool  );
* void setDefaultRetvalSTDException(bool  );
* void setDefaultRetvalUnknownException(bool  );
* </pre>
* The functions:
* <pre>
*    bool handleException(const basar::Exception & ex, const ExceptionFoundInfo & found, bool devretval)  ;
*	 bool handleException(const std::exception & ex, const ExceptionFoundInfo & found, bool devretval)  ;
*	 bool handleUnknownException(const ExceptionFoundInfo & found, bool devretval)  ;
* </pre>
* use the in the function given defaultvalues instead.
*
* To enclose your code with statements that use the ExceptionHandlerManager
* to handle your Exceptions, you can use the macros:
* <pre>
* LIBBASARGUITIE_EXCEPTIONHANDLERMACRO_BEGIN
*    ... (your code) ...
* LIBBASARGUITIE_EXCEPTIONHANDLERMACRO_END("Functionname to show where the Exception was catched")
* </pre>
* or the more detailed., that uses the second set of handle Functions.
* <pre>
* LIBBASARGUITIE_EXCEPTIONHANDLERMACRO_BEGIN
*    ... (your code) ...
* LIBBASARGUITIE_EXCEPTIONHANDLERMACRO_END_WITH_VALUE("Functionname to show where the Exception was catched", returnvalue for basar::Exception,returnvalue for std::exception , returnvalue for unknown Exception)
* </pre>
*     
* All calls to the handle Routines are logged with the 
* exceptionHandlerManager() Warn Level
* 
*/
class ExceptionHandlerManager
{
public:
	/**
	* Default constructor, uses DefaultExceptionHandler as
	* ExceptionHandler
	*/
	ExceptionHandlerManager();

	/**
	* Destructor
	*/
	~ExceptionHandlerManager();

	/** 
	* sets the ExceptionHandlerRef
	* @param exceptionhandlerref ExceptionHandlerRef used 
	*/
	LIBBASARGUITIE_API void  setExceptionHandlerRef(const ExceptionHandlerRef & exceptionhandlerref);

	/**
	* returns the ExceptionHandlerRef used
	* \return ExceptionHandlerRef used
	*/
	LIBBASARGUITIE_API const ExceptionHandlerRef & getExceptionHandlerRef()  const;


	/*!
	* handles basar::Exception and returns if the Exception should be rethrown
	* or if it is handled satisfyingly. The default retval is set by 
	* setDefaultRetvalBasarException(bool).
	*
	* @param ex The Exception that is to be handled
	* @param found Information about the location the Exception was catched
	* @return true the exception was  handled satisfyingly 
	* @return false the exception is to be rethrown 
	*/
	LIBBASARGUITIE_API bool handleException(const basar::Exception & ex, const ExceptionFoundInfo & found)  ;

	/*!
	* handles std::exception and returns if the Exception should be rethrown
	* or if it is handled satisfyingly. The default retval is set by 
	* setDefaultRetvalSTDException(bool).
	*
	* @param ex The Exception that is to be handled
	* @param found Information about the location the Exception was catched
	* @return true the exception was  handled satisfyingly 
	* @return false the exception is to be rethrown 
	*/
	LIBBASARGUITIE_API bool handleException(const std::exception & ex, const ExceptionFoundInfo & found)  ;

	/*!
	* handles a not defined Exception and returns if the Exception should be rethrown
	* or if it is handled satisfyingly. The default retval is set by 
	* setDefaultRetvalUnknownException(bool).
	*
	* @param found Information about the location the Exception was catched
	* @return true the exception was  handled satisfyingly 
	* @return false the exception is to be rethrown 
	*/
	LIBBASARGUITIE_API bool handleUnknownException(const ExceptionFoundInfo & found);

	/*!
	* handles basar::Exception and returns if the Exception should be rethrown
	* or if it is handled satisfyingly
	* @param ex The Exception that is to be handled
	* @param found Information about the location the Exception was catched
	* @param defaultRetval expected default return value
	* @return true the exception was  handled satisfyingly 
	* @return false the exception is to be rethrown 
	*/
	LIBBASARGUITIE_API bool handleException(const basar::Exception & ex, const ExceptionFoundInfo & found, bool defaultRetval);

	/*!
	* handles std::Exception and returns if the Exception should be rethrown
	* or if it is handled satisfyingly  
	* @param ex The Exception that is to be handled
	* @param found Information about the location the Exception was catched
	* @param defaultRetval expected default return value
	* @return true the exception was  handled satisfyingly 
	* @return false the exception is to be rethrown 
	*/
	LIBBASARGUITIE_API bool handleException(const std::exception & ex, const ExceptionFoundInfo & found, bool defaultRetval) ;

	/*!
	* handles a not defined Exception and returns if the Exception should be rethrown
	* or if it is handled satisfyingly  
	* @param found Information about the location the Exception was catched
	* @param defaultRetval expected default return value
	* @return true the exception was  handled satisfyingly 
	* @return false the exception is to be rethrown 
	*/
	LIBBASARGUITIE_API bool handleUnknownException(const ExceptionFoundInfo & found, bool defaultRetval) ;

	/*!
	* sets the default return Values  
	* <pre>
	* x1 is used in bool handleException(const basar::Exception & ex, const ExceptionFoundInfo & found) ;
	* x2 is used in bool handleException(const std::exception & ex, const ExceptionFoundInfo & found );
	* x3 is used in bool handleUnknownException(const ExceptionFoundInfo & found );
	* </pre>
	* @param x1 expected default return value for function handleException(const basar::Exception & ex, const ExceptionFoundInfo & found);
	* @param x2 expected default return value for function handleException(const std::exception & ex, const ExceptionFoundInfo & found);
	* @param x3 expected default return value for function handleUnknownException(const ExceptionFoundInfo & found);
	*/
	LIBBASARGUITIE_API void setDefaultRetval(bool x1, bool x2, bool x3);

	/*!
	* sets the default return Value for the member function
	* handleException(const basar::Exception & ex, const ExceptionFoundInfo & found);
	*  
	* @param v expected default return value for function handleException(const basar::Exception & ex, const ExceptionFoundInfo & found);
	* @return returnvalue of the inner ExceptionHandlerRef::handleException(const basar::Exception & ex, const ExceptionFoundInfo & found, bool v);
	*/
	LIBBASARGUITIE_API void setDefaultRetvalBasarException(bool  v);

	/*!
	* sets the default return Value for the member function
	* handleException(const std::exception & ex, const ExceptionFoundInfo & found);
	*  
	* @param v expected default return value for function handleException(const std::exception & ex, const ExceptionFoundInfo & found);
	* @return returnvalue of the inner ExceptionHandlerRef::handleException(const std::exception & ex, const ExceptionFoundInfo & found, bool v);
	*/
	LIBBASARGUITIE_API void setDefaultRetvalSTDException(bool v );

	/*!
	* sets the default return Value for the member function
	* handleUnknownException( const ExceptionFoundInfo & found);
	*  
	* @param v expected default return value for function handleUnknownException( const ExceptionFoundInfo & found);
	* @return returnvalue of the inner ExceptionHandlerRef::handleUnknownException( const ExceptionFoundInfo & found, bool v);
	*/
	LIBBASARGUITIE_API void setDefaultRetvalUnknownException(bool v );

	/*!
	* returns the default return Value used in  the member function
	* handleException(const basar::Exception & ex, const ExceptionFoundInfo & found);
	* 
	* @return default returnvalue of the inner ExceptionHandlerRef::handleException(const basar::Exception & ex, const ExceptionFoundInfo & found, bool v);
	*/
	LIBBASARGUITIE_API bool getDefaultRetvalBasarException( );

	/*!
	* returns the default return Value used in  the member function
	* handleException(const std::exception & ex, const ExceptionFoundInfo & found);
	* 
	* @return default returnvalue of the inner ExceptionHandlerRef::handleException(const std::exception & ex, const ExceptionFoundInfo & found, bool v);
	*/
	LIBBASARGUITIE_API bool getDefaultRetvalSTDException( );

	/*!
	* returns the default return Value used in the member function
	* handleUnknownException( const ExceptionFoundInfo & found);
	* @return default returnvalue of the inner ExceptionHandlerRef::handleUnknownException( const ExceptionFoundInfo & found, bool v);
	*/
	LIBBASARGUITIE_API bool getDefaultRetvalUnknownException( );

	/*!
	* returns the number of Exceptions handled
	* \return number of Exceptions handled
	*/
	LIBBASARGUITIE_API basar::Int32 getNumberOfExceptionHandled() const;

	/*!
	* resets the number of Exceptions handled
	*/
	LIBBASARGUITIE_API void resetNumberOfExceptionHandled();

	/*!
	* returns, if no exception was handled, 
	* (0 == getNumberOfExceptionHandled())
	* \return true if no exception was handled, false otherwise
	*/
	LIBBASARGUITIE_API bool checkNoExceptionHandled() const;


private:

	/*!
	* encloses the used ExceptionHandler
	*/
	ExceptionHandlerRef m_exceptionHandler;

	/*!
	* the default return Value used in  the member function
	* handleException(const basar::Exception & ex, const ExceptionFoundInfo & found); 
	*/
	bool m_defaultRetvalBasarException;

	/*!
	* the default return Value used in  the member function
	* handleException(const std::exception & ex, const ExceptionFoundInfo & found); 
	*/
	bool m_defaultRetvalSTDException;

	/*!
	* the default return Value used in  the member function
	* handleUnkownException( const ExceptionFoundInfo & found); 
	*/
	bool m_defaultRetvalUnknownException;

	/*!
	* number of ExceptionsHandled;
	*/
	basar::Int32 m_exceptionsHandled;

	/*!
	* writes the information from the ExceptionFoundInfo value
	* to a string, to be written to the logfile.
	*
 	* @param found Information about the location the Exception was catched
 	* @return VarString containing the information from the ExceptionFoundInfo value 
	*/
	basar::VarString writeFoundInfo(const ExceptionFoundInfo & found) const;
};

}
}
}
#endif 

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
