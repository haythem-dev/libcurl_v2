//----------------------------------------------------------------------------
/*! \file
 *  \brief  basar date edit widget
 *  \author Michael Eichenlaub 
 *  \date   18.08.2009
 */
//----------------------------------------------------------------------------
#ifdef _WIN32
#pragma warning (push)
#pragma warning(disable: 4481) // C4481: nonstandard extension used: override specifier 'override'
#pragma warning(disable: 4512) // qobject_impl.h - C4512: 'QtPrivate::QSlotObjectBase' : assignment operator could not be generated
#endif
#include "libbasarqtwidget_dateedit.h"
#ifdef _WIN32
#pragma warning (pop)
#endif

#include "libbasarqtwidget_i18n.h"

//----------------------------------------------------------------------------
//! \throw no-throw
//!
BasarDateEdit::BasarDateEdit ( QWidget *parent ) 
							 : QDateEdit(parent)
{
	setDisplayFormat(basar::gui::widget::I18n::getPhDateFormat());
}

//----------------------------------------------------------------------------
//! \throw no-throw
//!
BasarDateEdit::BasarDateEdit ( const QDate & date,
							   QWidget     * parent  ) 
							 : QDateEdit(date, parent)
{
	setDisplayFormat(basar::gui::widget::I18n::getPhDateFormat());
}
