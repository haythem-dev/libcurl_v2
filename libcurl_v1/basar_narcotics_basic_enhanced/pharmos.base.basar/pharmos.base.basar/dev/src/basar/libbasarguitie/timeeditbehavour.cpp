//----------------------------------------------------------------------------
/*! \file
*  \brief  class implementation of TimeEditBehavour
*  \author Roland Kiefert
*  \date   08.09.2006
*/
//----------------------------------------------------------------------------


//---------------------------------------------------------------------------------------//
// includes
//---------------------------------------------------------------------------------------//
#include "timeeditbehavour.h"

#include "libbasarcmnutil_datetimetools.h"
#include "libbasarqtwidget_i18n.h"

#pragma warning (push)
#pragma warning (disable: 4231 4244 4481 4512 4800)
#include <QtWidgets/QTimeEdit>
#pragma warning (pop)

//---------------------------------------------------------------------------------------//
// namespaces
//---------------------------------------------------------------------------------------//
namespace basar {
namespace gui {
namespace tie {

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
TimeEditBehavour::TimeEditBehavour(PGuiPropTbl                   pPt,
									std::list<QWidget*>::iterator it )
									: ControlBehavour(pPt, it)
{
    m_TimeEditWidget = static_cast<QTimeEdit*>( (*m_it) );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
TimeEditBehavour::~TimeEditBehavour()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void TimeEditBehavour::setTime( const Time& rValue, Int32 )
{
    QTime time(rValue.getHour(), rValue.getMinute(), rValue.getSec(), rValue.getMSec());
    if (!rValue.isValidTime())
    {
        time = QTime(basar::cmnutil::DateTimeTools::getHour     (basar::cmnutil::NULLVAL_SQL_TIME),
						basar::cmnutil::DateTimeTools::getMinute(basar::cmnutil::NULLVAL_SQL_TIME),
						basar::cmnutil::DateTimeTools::getSec   (basar::cmnutil::NULLVAL_SQL_TIME),
						basar::cmnutil::DateTimeTools::getMSec  (basar::cmnutil::NULLVAL_SQL_TIME));
    }

	m_TimeEditWidget->setDisplayFormat( QString::fromLatin1(widget::I18n::getPhTimeFormat()) );
    m_TimeEditWidget->setTime(time);	
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return DateTime value of the current line in the current ctrl.
\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
Time TimeEditBehavour::getTime(Int32) const
{
    QTime time = m_TimeEditWidget->time();	
    Time  retVal(time.hour(), time.minute(), time.second(), time.msec());

    return retVal;
}

///////////////////////////////////////////////////////////////////////////////////////////
//! \throw  no-throw
///////////////////////////////////////////////////////////////////////////////////////////
void TimeEditBehavour::clear()
{
    m_TimeEditWidget->setTime(m_TimeEditWidget->minimumTime());
}
///////////////////////////////////////////////////////////////////////////////////////////
//! \throw	no-throw
///////////////////////////////////////////////////////////////////////////////////////////
void TimeEditBehavour::erase( Int32 )
{
}
///////////////////////////////////////////////////////////////////////////////////////////
/*! \return Int32:	number of hits that the ctrl with the most hits (shown rows/items) has.
\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////
Int32 TimeEditBehavour::maxCountOfControls() const
{
    return 1;
}
///////////////////////////////////////////////////////////////////////////////////////////
//! \throw no-throw
//! \return type of given QObject
///////////////////////////////////////////////////////////////////////////////////////////
WidgetTypes	TimeEditBehavour::getWidgetType() const
{
    return qTimeEdit;
}

///////////////////////////////////////////////////////////////////////////////////////////
}// tie
} // gui
} // basar
