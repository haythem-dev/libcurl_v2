//----------------------------------------------------------------------------
/*! \file
 *  \brief  internal interface BasarDateTimeEditBehaviour
 *  \author Michael Eichenlaub
 *  \date   24.08.2009
 */
//----------------------------------------------------------------------------

//---------------------------------------------------------------------------------------//
// includes
//---------------------------------------------------------------------------------------//
#include "basardatetimeeditbehaviour.h"

#pragma warning (push)
#pragma warning (disable: 4231 4244 4481 4512 4800)
#include "libbasarqtwidget_datetimeedit.h"
#pragma warning (pop)

//---------------------------------------------------------------------------------------//
// namespaces
//---------------------------------------------------------------------------------------//
namespace basar {
namespace gui	{
namespace tie	{

//---------------------------------------------------------------------------------------//
//!	\throw no-throw
//!
BasarDateTimeEditBehaviour::BasarDateTimeEditBehaviour ( PGuiPropTbl                   pPt,
														 std::list<QWidget*>::iterator it)
													   : DateTimeEditBehavour(pPt, it)
{
    m_pWidget = static_cast < BasarDateTimeEdit * > ( *m_it );
}

//---------------------------------------------------------------------------------------//
//!	\throw no-throw
//!
BasarDateTimeEditBehaviour::~BasarDateTimeEditBehaviour()
{
}

//---------------------------------------------------------------------------------------//
//!	\throw no-throw
//!
void BasarDateTimeEditBehaviour::setDateTime (const DateTime & rValue, Int32 )
{
	QDateTime val;

	if ( rValue.isValidDate() )
		val.setDate(QDate(rValue.getYear(), rValue.getMonth (), rValue.getDay()));
	else
		val.setDate(m_pWidget->minimumDate());

	if ( rValue.isValidTime() )
		val.setTime(QTime(rValue.getHour(), rValue.getMinute(), rValue.getSec(), rValue.getMSec()));
	else
		val.setTime(m_pWidget->minimumTime());

    m_pWidget->setDateTime(val);	
}

//---------------------------------------------------------------------------------------//
//!	\throw no-throw
//!	\return DateTime value of current line in current widget
//!
DateTime BasarDateTimeEditBehaviour::getDateTime(Int32) const
{
	QDateTime qVal = m_pWidget->dateTime();
    DateTime  bVal;

	if (m_pWidget->minimumDate() == qVal.date())
		bVal.setInvalidDate();
	else
	{
		QDate date = qVal.date();
		bVal.setDate(date.year(), date.month (), date.day   ());
	}

	// doesn't make sense: check for BasarDateTimeEdit::Minimumtime() => 00:00:00.000 is valid time
	QTime time = qVal.time();
	bVal.setTime(time.hour(), time.minute(), time.second(), time.msec());

    return bVal;
}

//---------------------------------------------------------------------------------------//
//!	\throw  no-throw
//!
void BasarDateTimeEditBehaviour::clear()
{
	DateTime null;
	null.setInvalid();

	setDateTime(null , 0);
}

//---------------------------------------------------------------------------------------//
//!	\throw  no-throw
//! \return type of given QObject
//!
WidgetTypes	BasarDateTimeEditBehaviour::getWidgetType() const
{
    return basarDateTimeEdit;
}

//---------------------------------------------------------------------------------------//
}	// namespace tie
}	// namespace gui
}	// namespace basar
