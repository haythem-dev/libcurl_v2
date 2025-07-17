//----------------------------------------------------------------------------
/*! \file
 *  \brief  interface for view connectors
 *  \author Thomas Hörath
 *  \date   06.10.2006
 */
//----------------------------------------------------------------------------

#ifndef GUARD_IVIEWCONNECTOR_H
#define GUARD_IVIEWCONNECTOR_H

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

//------------------------------------------------------------------------------
#include "libbasarguitie_definitions.h"
#include "libbasarguitie_msgboxes.h"

//----------------------------------------------------------------------------
class QWidget;

//----------------------------------------------------------------------------
namespace basar		{
namespace gui		{
namespace tie		{

//----------------------------------------------------------------------------
/*! derive all view-connectors from this. show, hide, shutdown must be implemented. 
	overwrite message-functions, if info to user should not appear as message box.	
	\n interface class */
class LIBBASARGUITIE_API IViewConnector
{
public:
	//! virtual dummy destructor
	virtual										~IViewConnector	() {};

	/*! \brief show vc's dialog
		\n no-throw */
	virtual basar::gui::tie::WidgetReturnEnum	show		() = 0;

	/*! \brief hide vc's dialog
		\n no-throw */
	virtual void								hide		() = 0;

	/*! \brief close vc's dialog
		\n no-throw */
	virtual bool								shutdown	() = 0;

	/*! \brief message (error) from use case shown via message box
		\n no-throw */
	virtual const MessageBoxButtonEnum			error(		QWidget					* parent,						
															basar::ConstString		message,			
															MessageBoxButtons		buttons = ButtonOk,
															MessageBoxButtonEnum	defaultButton = ButtonOk,
															basar::ConstString		windowTitle = ""  
													 );	

	/*! \brief message (warning) from use case shown via message box
		\n no-throw */
	virtual const MessageBoxButtonEnum			warning(	QWidget					* parent,
															ConstString				message,			
															MessageBoxButtons		buttons = ButtonOk,
															MessageBoxButtonEnum	defaultButton = ButtonOk,
															ConstString				windowTitle = ""  
														);

	/*! \brief message (info) from use case shown via message box
		\n no-throw */
	virtual const MessageBoxButtonEnum			info(	QWidget					* parent,						
														ConstString				message,			
														MessageBoxButtons		buttons = ButtonOk,
														MessageBoxButtonEnum	defaultButton = ButtonOk,
														ConstString				windowTitle = ""  
													);

	/*! \brief message (question) from use case shown via message box
		\n no-throw */
	virtual const MessageBoxButtonEnum			question(	QWidget					* parent,						
															ConstString				message,			
															MessageBoxButtons		buttons = ButtonYes | ButtonNo,
															MessageBoxButtonEnum	defaultButton = ButtonNoButton,
															ConstString				windowTitle = "" 
														);
};


//----------------------------------------------------------------------------
}	// namespace tie
}	// namespace gui
}	// namespace basar

//------------------------------------------------------------------------------
#endif	// GUARD
