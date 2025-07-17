//----------------------------------------------------------------------------
/*! \file
 *  \brief  tie of message boxes
 *  \author Thomas Hörath
 *  \date   28.12.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARGUITIE_MSGBOXES_H
#define GUARD_LIBBASARGUITIE_MSGBOXES_H


//------------------------------------------------------------------------------
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
#include "libbasar_definitions.h"

//------------------------------------------------------------------------------
class QWidget;

//------------------------------------------------------------------------------
namespace basar {
namespace gui {
namespace tie {

/*! enum MessageBoxButtonEnum defines all possible buttons for message boxes\n
    (identical to QMessageBox::StandardButton) */
enum MessageBoxButtonEnum 
{	
	ButtonNoButton 			= 0x00000000, //!< An invalid button.
	ButtonOk 				= 0x00000400, //!< An "OK" button defined with the AcceptRole. 
	ButtonOpen 				= 0x00002000, //!< A "Open" button defined with the AcceptRole. 
	ButtonSave 				= 0x00000800, //!< A "Save" button defined with the AcceptRole. 
	ButtonCancel 			= 0x00400000, //!< A "Cancel" button defined with the RejectRole. 
	ButtonClose 			= 0x00200000, //!< A "Close" button defined with the RejectRole. 
	ButtonDiscard 			= 0x00800000, //!< A "Discard" or "Don't Save" button, depending on the platform, defined with the DestructiveRole. 
	ButtonApply 			= 0x02000000, //!< An "Apply" button defined with the ApplyRole. 
	ButtonReset				= 0x04000000, //!< A "Reset" button defined with the ResetRole. 
	ButtonRestoreDefaults	= 0x08000000, //!< A "Restore Defaults" button defined with the ResetRole. 
	ButtonHelp 				= 0x01000000, //!< A "Help" button defined with the HelpRole. 
	ButtonSaveAll 			= 0x00001000, //!< A "Save All" button defined with the AcceptRole. 
	ButtonYes 				= 0x00004000, //!< A "Yes" button defined with the YesRole. 
	ButtonYesToAll 			= 0x00008000, //!< A "Yes to All" button defined with the YesRole. 
	ButtonNo 				= 0x00010000, //!< A "No" button defined with the NoRole. 
	ButtonNoToAll 			= 0x00020000, //!< A "No to All" button defined with the NoRole. 
	ButtonAbort 			= 0x00040000, //!< An "Abort" button defined with the RejectRole. 
	ButtonRetry 			= 0x00080000, //!< A "Retry" button defined with the AcceptRole. 
	ButtonIgnore 			= 0x00100000  //!< An "Ignore" button defined with the AcceptRole.
};

typedef basar::Int32 MessageBoxButtons;	  //!< bitwise OR of MessageBoxButtonEnum

//------------------------------------------------------------------------------
/*! \brief	create a modal message box with exclamation mark as icon
	\n		no-throw
	\throw  no-throw 
	\param  parent			qt-widget the msgboxes belong to, if none -> 0
	\param  message			output message to gui
	\param  buttons			bitwise OR of MessageBoxButtonEnum (default ButtonOk = "OK")
	\param  defaultButton	button which has focus for default (default ButtonOk = "OK")
	\param  windowTitle		title of msgbox (default empty -> title = applicationname)
	\return MessageBoxButtonEnum (button pressed on message box) */
LIBBASARGUITIE_API const MessageBoxButtonEnum warningMsgBox( QWidget				* parent,
															 ConstString			message,			
															 MessageBoxButtons		buttons       = ButtonOk,
															 MessageBoxButtonEnum	defaultButton = ButtonOk,
															 ConstString			windowTitle   = ""  
															);

/*! \brief	create a modal message box with exclamation mark as icon and an "OK" button (for convenience)
	\n		no-throw 
	\throw  no-throw 
	\param  parent			qt-widget the msgboxes belong to, if none -> 0
	\param  message			output message to gui
	\param  windowTitle		title of msgbox
	\return MessageBoxButtonEnum (button pressed on message box) */
LIBBASARGUITIE_API const MessageBoxButtonEnum warningMsgBox( QWidget				* parent,
															 ConstString			message,			
															 ConstString			windowTitle  
															);

/*! \brief	create a modal message box with "i" (information) as icon
	\n		no-throw
	\throw  no-throw 
	\param  parent			qt-widget the msgboxes belong to, if none -> 0
	\param  message			output message to gui
	\param  buttons			bitwise OR of MessageBoxButtonEnum (default ButtonOk = "OK")
	\param  defaultButton	button which has focus for default (default ButtonOk = "OK")
	\param  windowTitle		title of msgbox (default empty -> title = applicationname)
	\return MessageBoxButtonEnum (button pressed on message box) */
LIBBASARGUITIE_API const MessageBoxButtonEnum infoMsgBox(	QWidget					* parent,						
															ConstString				message,			
															MessageBoxButtons		buttons       = ButtonOk,
															MessageBoxButtonEnum	defaultButton = ButtonOk,
															ConstString				windowTitle   = ""  
														);

/*! \brief	create a modal message box with "i" (information) as icon and an "OK" button (for convenience)
	\n		no-throw
	\throw  no-throw 
	\param  parent			qt-widget the msgboxes belong to, if none -> 0
	\param  message			output message to gui
	\param  windowTitle		title of msgbox
	\return MessageBoxButtonEnum (button pressed on message box) */
LIBBASARGUITIE_API const MessageBoxButtonEnum infoMsgBox(	QWidget					* parent,						
															ConstString				message,			
															ConstString				windowTitle 
														);

/*! \brief	create a modal message box with question mark as icon
	\n		no-throw 
	\throw  no-throw  
	\param  parent			qt-widget the msgboxes belong to, if none -> 0
	\param  message			output message to gui
	\param  buttons			bitwise OR of MessageBoxButtonEnum (default ButtonYes | ButtonNo = "Yes" and "No")
	\param  defaultButton	button which has focus for default (default ButtonNoButton = no button)
	\param  windowTitle		title of msgbox (default empty -> title = applicationname)
	\return MessageBoxButtonEnum (button pressed on message box) */
LIBBASARGUITIE_API const MessageBoxButtonEnum questionMsgBox(	QWidget					* parent,						
																ConstString				message,			
																MessageBoxButtons		buttons       = ButtonYes | ButtonNo,
																MessageBoxButtonEnum	defaultButton = ButtonNoButton,
																ConstString				windowTitle   = "" 
															);

/*! \brief	create a modal message box with question mark as icon and "YES" and "NO" buttons (for convenience)
	\n		no-throw 
	\throw  no-throw  
	\param  parent			qt-widget the msgboxes belong to, if none -> 0
	\param  message			output message to gui
	\param  windowTitle		title of msgbox
	\return MessageBoxButtonEnum (button pressed on message box) */
LIBBASARGUITIE_API const MessageBoxButtonEnum questionMsgBox(	QWidget					* parent,						
																ConstString				message,			
																ConstString				windowTitle 
															);

/*! \brief	create a modal message box with red "X" (error) as icon
	\n		no-throw
	\throw  no-throw  
	\param  parent			qt-widget the msgboxes belong to, if none -> 0
	\param  message			output message to gui
	\param  buttons			bitwise OR of MessageBoxButtonEnum (default ButtonOk = "OK")
	\param  defaultButton	button which has focus for default (default ButtonOk = "OK")
	\param  windowTitle		title of msgbox (default empty -> title = applicationname)
	\return MessageBoxButtonEnum (button pressed on message box) */
LIBBASARGUITIE_API const MessageBoxButtonEnum criticalMsgBox(	QWidget					* parent,						
																ConstString				message,			
																MessageBoxButtons		buttons       = ButtonOk,
																MessageBoxButtonEnum	defaultButton = ButtonOk,
																ConstString				windowTitle   = ""  
															);	

/*! \brief	create a modal message box with red "X" (error) as icon and an "OK" button (for convenience)
	\n		no-throw
	\throw  no-throw  
	\param  parent			qt-widget the msgboxes belong to, if none -> 0
	\param  message			output message to gui
	\param  windowTitle		title of msgbox
	\return MessageBoxButtonEnum (button pressed on message box) */
LIBBASARGUITIE_API const MessageBoxButtonEnum criticalMsgBox(	QWidget					* parent,						
																ConstString				message,			
																ConstString				windowTitle  
															);	

//----------------------------------------------------------------------------
} // tie
} // gui
} // basar

//----------------------------------------------------------------------------
#endif	// GUARD
