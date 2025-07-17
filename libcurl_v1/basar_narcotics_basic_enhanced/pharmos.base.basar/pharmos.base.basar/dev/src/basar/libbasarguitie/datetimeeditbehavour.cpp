//----------------------------------------------------------------------------
/*! \file
*  \brief  class implementation of DateTimeEditBehavour
*  \author Roland Kiefert
*  \date   08.09.2006
*/
//----------------------------------------------------------------------------

//---------------------------------------------------------------------------------------//
// includes
//---------------------------------------------------------------------------------------//
#include "datetimeeditbehavour.h"

#include "libbasarqtwidget_i18n.h"
#include "libbasarcmnutil_datetimetools.h"

#pragma warning (push)
#pragma warning (disable: 4231 4244 4481 4512 4800)
#include <QtWidgets/QDateTimeEdit>
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
            DateTimeEditBehavour::DateTimeEditBehavour (PGuiPropTbl                   pPt,
														std::list<QWidget*>::iterator it )
													   : ControlBehavour(pPt, it)
            {
                m_DateTimeEditWidget = static_cast<QDateTimeEdit*>( (*m_it) );
            }
            ///////////////////////////////////////////////////////////////////////////////////////////////////////////
            /*!	\throw no-throw */
            ///////////////////////////////////////////////////////////////////////////////////////////////////////////
            DateTimeEditBehavour::~DateTimeEditBehavour()
            {
            }
            ///////////////////////////////////////////////////////////////////////////////////////////////////////////
            /*!	\throw	no-throw */
            ///////////////////////////////////////////////////////////////////////////////////////////////////////////
            void DateTimeEditBehavour::setDateTime( const DateTime & rValue, Int32 )
            {
				using basar::cmnutil::DateTimeTools;

                QDate date(rValue.getYear(), rValue.getMonth(), rValue.getDay());
                QTime time(rValue.getHour(), rValue.getMinute(), rValue.getSec(), rValue.getMSec());

                if (!rValue.isValidDate())
                    date = QDate(DateTimeTools::getYear (basar::cmnutil::NULLVAL_SQL_DATE),
								 DateTimeTools::getMonth(basar::cmnutil::NULLVAL_SQL_DATE), 
								 DateTimeTools::getDay  (basar::cmnutil::NULLVAL_SQL_DATE));

				if (!rValue.isValidTime())
                    time = QTime(DateTimeTools::getHour  (basar::cmnutil::NULLVAL_SQL_TIME),
								 DateTimeTools::getMinute(basar::cmnutil::NULLVAL_SQL_TIME),
								 DateTimeTools::getSec   (basar::cmnutil::NULLVAL_SQL_TIME),
								 DateTimeTools::getMSec  (basar::cmnutil::NULLVAL_SQL_TIME));

				m_DateTimeEditWidget->setDisplayFormat( QString::fromLatin1(widget::I18n::getPhDateTimeFormat()) );
                m_DateTimeEditWidget->setDate(date);	
                m_DateTimeEditWidget->setTime(time);	
            }

            ///////////////////////////////////////////////////////////////////////////////////////////////////////////
            /*!	\return DateTime value of the current line in the current ctrl.
            \throw	no-throw */
            ///////////////////////////////////////////////////////////////////////////////////////////////////////////
            DateTime DateTimeEditBehavour::getDateTime(Int32) const
            {
				QDate    date = m_DateTimeEditWidget->date();
                QTime    time = m_DateTimeEditWidget->time();	
                DateTime retVal;

				if (m_DateTimeEditWidget->maximumDate() == date)
					retVal.setInvalidDate();
				else
	                retVal.setDate(date.year(), date.month (), date.day   ());

				retVal.setTime(time.hour(), time.minute(), time.second(), time.msec());

                return retVal;
            }

            ///////////////////////////////////////////////////////////////////////////////////////////
            //! \throw  no-throw
            ///////////////////////////////////////////////////////////////////////////////////////////
            void DateTimeEditBehavour::clear()
            {
                setDateTime(DateTime(DateTime::NULL_DATE), 0);
            }
            ///////////////////////////////////////////////////////////////////////////////////////////
            //! \throw	no-throw
            ///////////////////////////////////////////////////////////////////////////////////////////
            void DateTimeEditBehavour::erase( Int32 )
            {
            }
            ///////////////////////////////////////////////////////////////////////////////////////////
            /*! \return Int32:	number of hits that the ctrl with the most hits (shown rows/items) has.
            \throw	no-throw */
            ///////////////////////////////////////////////////////////////////////////////////////////
            Int32 DateTimeEditBehavour::maxCountOfControls() const
            {
                return 1;
            }
            ///////////////////////////////////////////////////////////////////////////////////////////
            //! \throw no-throw
            //! \return type of given QObject
            ///////////////////////////////////////////////////////////////////////////////////////////
            WidgetTypes	DateTimeEditBehavour::getWidgetType() const
            {
                return qDateTimeEdit;
            }
        }// tie
    } // gui
} // basar
