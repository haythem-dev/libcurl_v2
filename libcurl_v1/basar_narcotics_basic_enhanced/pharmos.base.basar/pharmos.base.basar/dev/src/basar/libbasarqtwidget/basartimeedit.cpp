//----------------------------------------------------------------------------
/*! \file
 *  \brief  basar time edit widget
 *  \author Michael Eichenlaub 
 *  \date   18.08.2009
 */
//----------------------------------------------------------------------------
#ifdef _WIN32
#pragma warning (push)
#pragma warning(disable: 4481) // C4481: nonstandard extension used: override specifier 'override'
#pragma warning(disable: 4512) // qobject_impl.h - C4512: 'QtPrivate::QSlotObjectBase' : assignment operator could not be generated
#endif
#include "libbasarqtwidget_timeedit.h"
#ifdef _WIN32
#pragma warning (pop)
#endif

#include "libbasarqtwidget_i18n.h"

//----------------------------------------------------------------------------
//! \throw no-throw
BasarTimeEdit::BasarTimeEdit ( QWidget *parent ) 
							 : QTimeEdit(parent)
{
	setDisplayFormat(basar::gui::widget::I18n::getPhTimeFormat());
}

//----------------------------------------------------------------------------
//! \throw no-throw
//!
BasarTimeEdit::BasarTimeEdit ( const QTime & time,
							   QWidget     * parent  ) 
							 : QTimeEdit(time, parent)
{
	setDisplayFormat(basar::gui::widget::I18n::getPhTimeFormat());
}
