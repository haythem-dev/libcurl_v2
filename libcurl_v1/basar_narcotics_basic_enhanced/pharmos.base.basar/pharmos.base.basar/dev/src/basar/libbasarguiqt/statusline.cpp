//----------------------------------------------------------------------------
/*! \file
 *  \brief  all global statuslines for a main window
 *  \author Roland Kiefert
 *  \date   09.12.2005
 */
//----------------------------------------------------------------------------

#include "libbasarguiqt_statusline.h"

#include "libbasarcmnutil_bstring.h"

#pragma warning (push)
#pragma warning (disable: 4127 4481 4512)
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStatusBar>
#pragma warning (pop)

//------------------------------------------------------------------------------
namespace basar {
namespace gui {
namespace qt {

//------------------------------------------------------------------------------
/*!	\throw no-throw */
StatusLine::StatusLine() : m_PermanentConnection(0), m_PermanentUser(0)
{
}
 
//------------------------------------------------------------------------------
/*!	\throw no-throw */
StatusLine::~StatusLine()
{
	if (m_PermanentConnection != 0)
	{
		delete m_PermanentUser;
		m_PermanentUser = 0;
	}
	if (m_PermanentConnection != 0)
	{
		delete m_PermanentConnection;
		m_PermanentConnection = 0;
	}
}

//------------------------------------------------------------------------------
/*!	\throw no-throw */
void StatusLine::clear()
{
	if (m_PermanentConnection != 0)
	{
		m_Parent->statusBar()->removeWidget(m_PermanentConnection);
	}
	if (m_PermanentConnection != 0)
	{
		m_Parent->statusBar()->removeWidget(m_PermanentUser);
	}
}

//------------------------------------------------------------------------------
/*!	\throw no-throw */
void StatusLine::init(QMainWindow *parent)
{
	m_Parent = parent;
	m_PermanentConnection = new QLineEdit;
	m_PermanentUser = new QLineEdit;

	m_Parent->statusBar()->addPermanentWidget(m_PermanentConnection,0);
	m_Parent->statusBar()->addPermanentWidget(m_PermanentUser,0);
}

//------------------------------------------------------------------------------
/*!	\throw no-throw */
void StatusLine::setUser(const VarString & rName)
{
	m_PermanentUser->setAttribute(Qt::WA_Disabled);
	VarString value("USER: ");
	value += rName;
//	int t = value.size()*m_PermanentUser->width()/4.5;
	Int32 t = static_cast<Int32>(value.size()*7.5);
/*	Int32 i = m_Parant->font().stretch();
	i = m_PermanentUser->font().pixelSize();
	i = m_PermanentUser->font().pointSize();
	i = m_PermanentUser->font().weight();
	double r = m_PermanentUser->font().pointSizeF();
	HFONT lf = m_PermanentUser->font().handle();*/
	m_PermanentUser->setFixedWidth(t);
	m_PermanentUser->setText(value.c_str());
}

//------------------------------------------------------------------------------
/*!	\throw no-throw */
void StatusLine::setConnection(const VarString & rConn)
{
	m_PermanentConnection->setAttribute(Qt::WA_Disabled);
	VarString value("CONN: ");
	value += rConn;
	Int32 t = static_cast<Int32>(value.size()*7.5);
	m_PermanentConnection->setFixedWidth(t); 
	m_PermanentConnection->setText(value.c_str());
}

//------------------------------------------------------------------------------
}	// namespace qt
}	// namespace gui
}	// namespace bassar
