//----------------------------------------------------------------------------
/*! \file
 *  \brief  basar datetime edit widget
 *  \author Michael Eichenlaub 
 *  \date   18.08.2009
 */
//----------------------------------------------------------------------------


#ifdef _WIN32
#pragma warning (push)
#pragma warning(disable: 4481) // C4481: nonstandard extension used: override specifier 'override'
#pragma warning(disable: 4512) // qobject_impl.h - C4512: 'QtPrivate::QSlotObjectBase' : assignment operator could not be generated
#endif
#include "libbasarqtwidget_datetimeedit.h"
#ifdef _WIN32
#pragma warning (pop)
#endif

#include "libbasarqtwidget_i18n.h"

//----------------------------------------------------------------------------
//! \throw no-throw
//!
BasarDateTimeEdit::BasarDateTimeEdit ( QWidget *parent ) 
							         : QDateTimeEdit(parent)
{
	setDisplayFormat(basar::gui::widget::I18n::getPhDateTimeFormat());
}

//----------------------------------------------------------------------------
//! \throw no-throw
//!
BasarDateTimeEdit::BasarDateTimeEdit ( const QDateTime & datetime,
									   QWidget         * parent  ) 
							         : QDateTimeEdit(datetime, parent)
{
	setDisplayFormat(basar::gui::widget::I18n::getPhDateTimeFormat());
}

//----------------------------------------------------------------------------
//! \throw no-throw
//!
BasarDateTimeEdit::BasarDateTimeEdit ( const QDate & date,
									   QWidget     * parent  ) 
							         : QDateTimeEdit(date, parent)
{
	setDisplayFormat(basar::gui::widget::I18n::getPhDateTimeFormat());
}

//----------------------------------------------------------------------------
//! \throw no-throw
//!
BasarDateTimeEdit::BasarDateTimeEdit ( const QTime & time,
									   QWidget     * parent  ) 
							         : QDateTimeEdit(time, parent)
{
	setDisplayFormat(basar::gui::widget::I18n::getPhDateTimeFormat());
}
