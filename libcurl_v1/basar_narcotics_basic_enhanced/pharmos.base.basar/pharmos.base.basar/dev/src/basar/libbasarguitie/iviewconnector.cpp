//----------------------------------------------------------------------------
/*! \file
 *  \brief  interface for view connectors
 *  \author Thomas Hörath
 *  \date   06.10.2006
 */
//----------------------------------------------------------------------------

#include "libbasarguitie_iviewconnector.h"

//----------------------------------------------------------------------------
namespace basar		{
namespace gui		{
namespace tie		{

//----------------------------------------------------------------------------
/*!	\throw  no-throw 
	\param parent			qt-widget the msgboxes belong to, if none -> 0
	\param message			output message to gui
	\param buttons			bitwise OR of MessageBoxButtonEnum (default ButtonOk = "OK")
	\param defaultButton	button which has focus for default (default ButtonOk = "OK")
	\param windowTitle		title of msgbox (default empty -> title = applicationname)
	\return MessageBoxButtonEnum (button pressed on message box) */
const MessageBoxButtonEnum IViewConnector::warning(	QWidget					* parent,
													ConstString				message,			
													MessageBoxButtons		buttons /* = ButtonOk */,
													MessageBoxButtonEnum	defaultButton /* = ButtonOk */,
													ConstString				windowTitle /* = "" */  
												  )
{
	return warningMsgBox( parent, message, buttons, defaultButton, windowTitle );
}

//----------------------------------------------------------------------------
/*!	\throw  no-throw  
	\param parent			qt-widget the msgboxes belong to, if none -> 0
	\param message			output message to gui
	\param buttons			bitwise OR of MessageBoxButtonEnum (default ButtonOk = "OK")
	\param defaultButton	button which has focus for default (default ButtonOk = "OK")
	\param windowTitle		title of msgbox (default empty -> title = applicationname)
	\return MessageBoxButtonEnum (button pressed on message box) */
const MessageBoxButtonEnum IViewConnector::error(	QWidget					* parent,						
													basar::ConstString		message,			
													MessageBoxButtons		buttons /* = ButtonOk */,
													MessageBoxButtonEnum	defaultButton /* = ButtonOk */,
													basar::ConstString		windowTitle /* = "" */ 
												)
{
	return criticalMsgBox( parent, message, buttons, defaultButton, windowTitle );
}

//----------------------------------------------------------------------------
/*!	\throw  no-throw 
	\param parent			qt-widget the msgboxes belong to, if none -> 0
	\param message			output message to gui
	\param buttons			bitwise OR of MessageBoxButtonEnum (default ButtonOk = "OK")
	\param defaultButton	button which has focus for default (default ButtonOk = "OK")
	\param windowTitle		title of msgbox (default empty -> title = applicationname)
	\return MessageBoxButtonEnum (button pressed on message box) */
const MessageBoxButtonEnum IViewConnector::info(	QWidget					* parent,						
													ConstString				message,			
													MessageBoxButtons		buttons /* = ButtonOk */,
													MessageBoxButtonEnum	defaultButton /* = ButtonOk */,
													ConstString				windowTitle /* = "" */  
												)
{
	return infoMsgBox( parent, message, buttons, defaultButton, windowTitle );
}

//----------------------------------------------------------------------------
/*!	\throw  no-throw  
	\param parent			qt-widget the msgboxes belong to, if none -> 0
	\param message			output message to gui
	\param buttons			bitwise OR of MessageBoxButtonEnum (default ButtonYes | ButtonNo = "Yes" and "No")
	\param defaultButton	button which has focus for default (default ButtonNoButton = no button)
	\param windowTitle		title of msgbox (default empty -> title = applicationname)
	\return MessageBoxButtonEnum (button pressed on message box) */
const MessageBoxButtonEnum IViewConnector::question(	QWidget					* parent,						
														ConstString				message,			
														MessageBoxButtons		buttons /* = ButtonYes | ButtonNo */,
														MessageBoxButtonEnum	defaultButton /* = ButtonNoButton */,
														ConstString				windowTitle /* = "" */ 
													)
{
	return questionMsgBox( parent, message, buttons, defaultButton, windowTitle );
}

//----------------------------------------------------------------------------
}	// namespace tie
}	// namespace gui
}	// namespace basar
