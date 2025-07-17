//----------------------------------------------------------------------------
/*! \file
 *  \brief  internal interface BasarTimeEditBehaviour
 *  \author Michael Eichenlaub
 *  \date   24.08.2009
 */
//----------------------------------------------------------------------------

//---------------------------------------------------------------------------------------//
// includes
//---------------------------------------------------------------------------------------//
#include "basartimeeditbehaviour.h"

#pragma warning (push)
#pragma warning (disable: 4231 4244 4481 4512 4800)
#include "libbasarqtwidget_timeedit.h"
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
BasarTimeEditBehaviour::BasarTimeEditBehaviour ( PGuiPropTbl                   pPt,
												 std::list<QWidget*>::iterator it)
											   : TimeEditBehavour(pPt, it)
{
    m_pWidget = static_cast < BasarTimeEdit * > ( *m_it );
}

//---------------------------------------------------------------------------------------//
//!	\throw no-throw
//!
BasarTimeEditBehaviour::~BasarTimeEditBehaviour()
{
}

//---------------------------------------------------------------------------------------//
//!	\throw no-throw
//!
void BasarTimeEditBehaviour::setTime (const Time & rValue, Int32 )
{
	QTime val;

    if (!rValue.isValidTime())
		val = m_pWidget->minimumTime();
	else
		val.setHMS(rValue.getHour(), rValue.getMinute(), rValue.getSec(), rValue.getMSec());

    m_pWidget->setTime(val);	
}

//---------------------------------------------------------------------------------------//
//!	\throw no-throw
//!	\return Time value of current line in current widget
//!
Time BasarTimeEditBehaviour::getTime(Int32) const
{
	QTime qVal = m_pWidget->time();

	// doesn't make sense: check for BasarTimeEdit::Minimumtime() => 00:00:00.000 is valid time
    Time  bVal(qVal.hour(), qVal.minute(), qVal.second(), qVal.msec());

    return bVal;
}

//---------------------------------------------------------------------------------------//
//!	\throw  no-throw
//!
void BasarTimeEditBehaviour::clear()
{
	Time null;
	null.setInvalid();

    setTime(null, 0);
}

//---------------------------------------------------------------------------------------//
//!	\throw  no-throw
//! \return type of given QObject
//!
WidgetTypes	BasarTimeEditBehaviour::getWidgetType() const
{
    return basarTimeEdit;
}

//---------------------------------------------------------------------------------------//
}	// namespace tie
}	// namespace gui
}	// namespace basar
