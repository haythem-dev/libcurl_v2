/** $Id$                                          
*                                                  
* @file                                            
* @author Anke Klink                      
* @date 2009                            
*/ 

#ifndef  GUARD_BASAR_LIBBASARGUITIE_LIBBASARGUITIE_EXCEPTIONHANDLERMACROS_H 
#define  GUARD_BASAR_LIBBASARGUITIE_LIBBASARGUITIE_EXCEPTIONHANDLERMACROS_H 

#include "libbasarguitie_exceptionhandlermanager.h" 
#include "libbasarguitie_manager.h"  

/*!
* Start of an enclosure to use the ExceptionHandlerManager 
*/
#define LIBBASARGUITIE_EXCEPTIONHANDLERMACRO_BEGIN \
	try { \

/*!
* Endstatatement for an enclosure to use the ExceptionHandlerManager.
* Uses 3 catch Statements and calls the ExceptionHandlerManager
* using default returnvalues listed in the ExceptionHandlerManager
* the value given in the macro is used as the context value
* for the found value of the handleException Functions and
* should represent a string value of the local function
* this macro is used in, f.ex.
* LIBBASARGUITIE_EXCEPTIONHANDLERMACRO_END("basar.gui.tie.BasarTableWidgetBehavour::eventFilterCtr")
* Examples can be found in the unitTest utguitie in the class
* using  basar::test::unit::guitie::TestExceptionThrower;
*  or in basar.gui.tie.BasarTableWidgetBehavour
*/
#define LIBBASARGUITIE_EXCEPTIONHANDLERMACRO_END(X1) \
	} \
	catch (basar::Exception & ex) \
	{ \
	   if (!basar::gui::tie::Manager::getInstance().getExceptionHandlerManager().handleException(ex, basar::ExceptInfo(X1,"LIBBASARGUITIE_MACRO", __FILE__, __LINE__))) \
		{ \
			throw; \
		} \
	} \
	catch (std::exception & ex) \
	{ \
		if (!basar::gui::tie::Manager::getInstance().getExceptionHandlerManager().handleException(ex, basar::ExceptInfo(X1,"LIBBASARGUITIE_MACRO",  __FILE__, __LINE__))) \
		{ \
			throw; \
		} \
	} \
	catch (...) \
	{ \
    	if (!basar::gui::tie::Manager::getInstance().getExceptionHandlerManager().handleUnknownException(basar::ExceptInfo(X1,"LIBBASARGUITIE_MACRO",  __FILE__, __LINE__))) \
		{ \
			throw; \
		} \
	} \

/*!
* Endstatatement for an enclosure to use the ExceptionHandlerManager.
* Uses 3 catch Statements and calls the ExceptionHandlerManager
* using the values X1, X2, X3 as default returnvalues for 
* the ExceptionHandlerManager functions.
* the first value (X0) given in the macro is used as the context value
* for the found value of the handleException Functions and
* should represent a string value of the local function
* this macro is used in, f.ex.
* LIBBASARGUITIE_EXCEPTIONHANDLERMACRO_END("basar::test::unit::guitie::TestExceptionThrower::function", true, true, true)
* X1 specifies the default returnvalue for all basar::Exceptions,
* X2 specifies the default returnvalue for all std::exceptions,
* X3 specifies the default returnvalue for all other exceptions
* Examples can be found in the unitTest utguitie in the class
* using  basar::test::unit::guitie::TestExceptionThrower;
*/
#define LIBBASARGUITIE_EXCEPTIONHANDLERMACRO_END_WITH_VALUE(X0, X1, X2, X3) \
	} \
	catch (basar::Exception & ex) \
	{ \
		if (!basar::gui::tie::Manager::getInstance().getExceptionHandlerManager().handleException(ex, basar::ExceptInfo(X0,"LIBBASARGUITIE_MACRO_VAL",  __FILE__, __LINE__), X1)) \
		{ \
			throw; \
		} \
	} \
	catch (std::exception & ex) \
	{ \
		if (!basar::gui::tie::Manager::getInstance().getExceptionHandlerManager().handleException(ex, basar::ExceptInfo(X0,"LIBBASARGUITIE_MACRO_VAL",   __FILE__, __LINE__), X2)) \
		{ \
			throw; \
		} \
	} \
	catch (...) \
	{ \
	   if (!basar::gui::tie::Manager::getInstance().getExceptionHandlerManager().handleUnknownException(basar::ExceptInfo(X0,"LIBBASARGUITIE_MACRO_VAL",  __FILE__, __LINE__), X3)) \
		{ \
			throw; \
		} \
	}\

#endif 

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
