//----------------------------------------------------------------------------
/*! \file
 *  \brief  message boxes implemtation
 *  \author	Thomas Hörath
 *  \date   28.12.2005
 */
//----------------------------------------------------------------------------

#include "libbasarguitie_msgboxes.h"

#pragma warning (push)
#pragma warning (disable: 4127 4512)
#include <QtWidgets/QMessageBox>
#pragma warning (pop)

//----------------------------------------------------------------------------
namespace basar {
namespace gui {
namespace tie {

//----------------------------------------------------------------------------
const MessageBoxButtonEnum warningMsgBox( QWidget				* parent,
  										  ConstString			message,			
										  MessageBoxButtons		buttons       /* = ButtonOk */,
										  MessageBoxButtonEnum	defaultButton /* = ButtonOk */,
										  ConstString			windowTitle   /* = ""       */ 
										)
{
	QMessageBox::StandardButtons bts = static_cast<QMessageBox::StandardButtons>( buttons );
	QMessageBox::StandardButton defBt = static_cast<QMessageBox::StandardButton>( defaultButton );
	return static_cast<MessageBoxButtonEnum>( QMessageBox::warning( parent, QString::fromLocal8Bit(windowTitle), QString::fromLocal8Bit(message), bts, defBt ) );
}

//----------------------------------------------------------------------------
const MessageBoxButtonEnum warningMsgBox( QWidget				* parent,
  										  ConstString			message,			
										  ConstString			windowTitle
										)
{
	return static_cast<MessageBoxButtonEnum>( QMessageBox::warning( parent, QString::fromLocal8Bit(windowTitle), QString::fromLocal8Bit(message), 
												QMessageBox::Ok, QMessageBox::Ok ) );
}

//----------------------------------------------------------------------------
const MessageBoxButtonEnum infoMsgBox(	QWidget					* parent,						
										ConstString				message,			
										MessageBoxButtons		buttons       /* = ButtonOk */,
										MessageBoxButtonEnum	defaultButton /* = ButtonOk */,
										ConstString				windowTitle   /* = ""       */  
									 )
{
	QMessageBox::StandardButtons bts = static_cast<QMessageBox::StandardButtons>( buttons );
	QMessageBox::StandardButton defBt = static_cast<QMessageBox::StandardButton>( defaultButton );
	return static_cast<MessageBoxButtonEnum>( QMessageBox::information( parent, QString::fromLocal8Bit(windowTitle), QString::fromLocal8Bit(message), bts, defBt ) );
}

//----------------------------------------------------------------------------
const MessageBoxButtonEnum infoMsgBox(	QWidget					* parent,						
										ConstString				message,			
										ConstString				windowTitle
									 )
{
	return static_cast<MessageBoxButtonEnum>( QMessageBox::information( parent, QString::fromLocal8Bit(windowTitle), QString::fromLocal8Bit(message), 
												QMessageBox::Ok, QMessageBox::Ok ) );
}

//----------------------------------------------------------------------------
const MessageBoxButtonEnum questionMsgBox(	QWidget					* parent,						
											ConstString				message,			
											MessageBoxButtons		buttons       /* = ButtonYes | ButtonNo */,
											MessageBoxButtonEnum	defaultButton /* = ButtonNoButton       */,
											ConstString				windowTitle   /* = ""                   */
										 )
{
	QMessageBox::StandardButtons bts = static_cast<QMessageBox::StandardButtons>( buttons );
	QMessageBox::StandardButton defBt = static_cast<QMessageBox::StandardButton>( defaultButton );
	return static_cast<MessageBoxButtonEnum>( QMessageBox::question( parent, QString::fromLocal8Bit(windowTitle), QString::fromLocal8Bit(message), bts, defBt ) );
}

//----------------------------------------------------------------------------
const MessageBoxButtonEnum questionMsgBox(	QWidget					* parent,						
											ConstString				message,			
											ConstString				windowTitle
										 )
{
	return static_cast<MessageBoxButtonEnum>( QMessageBox::question( parent, QString::fromLocal8Bit(windowTitle), QString::fromLocal8Bit(message), 
												QMessageBox::Yes | QMessageBox::No, QMessageBox::NoButton ) );
}

//----------------------------------------------------------------------------
const MessageBoxButtonEnum criticalMsgBox(	QWidget					* parent,						
											ConstString				message,			
											MessageBoxButtons		buttons       /* = ButtonOk */,
											MessageBoxButtonEnum	defaultButton /* = ButtonOk */,
											ConstString				windowTitle   /* = ""       */  
										 )
{
	QMessageBox::StandardButtons bts = static_cast<QMessageBox::StandardButtons>( buttons );
	QMessageBox::StandardButton defBt = static_cast<QMessageBox::StandardButton>( defaultButton );
	return static_cast<MessageBoxButtonEnum>( QMessageBox::critical( parent, QString::fromLocal8Bit(windowTitle), QString::fromLocal8Bit(message), bts, defBt ) );
}

//----------------------------------------------------------------------------
const MessageBoxButtonEnum criticalMsgBox(	QWidget					* parent,						
											ConstString				message,			
											ConstString				windowTitle
										 )
{
	return static_cast<MessageBoxButtonEnum>( QMessageBox::critical( parent, QString::fromLocal8Bit(windowTitle), QString::fromLocal8Bit(message), 
												QMessageBox::Ok, QMessageBox::Ok ) );
}

//----------------------------------------------------------------------------
}//tie
}//gui
}//basar
