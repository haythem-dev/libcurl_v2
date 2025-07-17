//----------------------------------------------------------------------------
/*! \file
 *  \brief  all global statusline for a main window
 *  \author Roland Kiefert
 *  \date   22.12.2005
 */
//----------------------------------------------------------------------------

#include "aboutdlg.h"
#include "libbasarguiqt_about.h"

#include "libbasarguitie_functions.h"

//------------------------------------------------------------------------------
namespace basar {
namespace gui {
namespace qt {

//------------------------------------------------------------------------------
/*!	\throw no-throw */
About::About()
{
}
 
//------------------------------------------------------------------------------
/*!	\throw no-throw */
About::~About()
{
}

//------------------------------------------------------------------------------
/*!	\throw no-throw
 *  \return dialogs result
 */
tie::WidgetReturnEnum About::show(QWidget *parent)
{
   AboutDlg dlg(parent);
//   dlg.setAttribute(Qt::setSizeGripEnabled(false);
   return tie::getWidgetReturnType(dlg.exec());
}

//------------------------------------------------------------------------------
}	// namespace qt
}	// namespace gui
}	// namespace bassar
