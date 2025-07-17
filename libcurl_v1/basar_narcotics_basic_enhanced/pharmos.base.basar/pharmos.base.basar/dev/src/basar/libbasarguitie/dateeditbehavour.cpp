//----------------------------------------------------------------------------
/*! \file
*  \brief  class implementation of DateEditBehavour
*  \author Roland Kiefert
*  \date   08.09.2006
*/
//----------------------------------------------------------------------------


//---------------------------------------------------------------------------------------//
// includes
//---------------------------------------------------------------------------------------//
#include "dateeditbehavour.h"

#include "libbasarqtwidget_i18n.h"
#include "libbasarcmnutil_datetimetools.h"

#pragma warning (push)
#pragma warning (disable: 4231 4244 4481 4512 4800)
#include <QtWidgets/QDateEdit>
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
            DateEditBehavour::DateEditBehavour( PGuiPropTbl                   pPt,
												std::list<QWidget*>::iterator it )
											  : ControlBehavour(pPt, it)
            {
                m_DateEditWidget = static_cast<QDateEdit*>( (*m_it) );
            }

            ///////////////////////////////////////////////////////////////////////////////////////////////////////////
            /*!	\throw no-throw */
            ///////////////////////////////////////////////////////////////////////////////////////////////////////////
            DateEditBehavour::~DateEditBehavour()
            {
            }

            ///////////////////////////////////////////////////////////////////////////////////////////////////////////
            /*!	\throw	no-throw */
            ///////////////////////////////////////////////////////////////////////////////////////////////////////////
            void DateEditBehavour::setDate( const Date& rValue, Int32 )
            {
                QDate date(rValue.getYear(), rValue.getMonth(), rValue.getDay());

                if (!rValue.isValid())
                {
                    date = QDate (  basar::cmnutil::DateTimeTools::getYear (basar::cmnutil::NULLVAL_SQL_DATE),
									basar::cmnutil::DateTimeTools::getMonth(basar::cmnutil::NULLVAL_SQL_DATE), 
									basar::cmnutil::DateTimeTools::getDay  (basar::cmnutil::NULLVAL_SQL_DATE));
                }

				m_DateEditWidget->setDisplayFormat( QString::fromLatin1(widget::I18n::getPhDateFormat()) );
                m_DateEditWidget->setDate(date);	
            }
            ///////////////////////////////////////////////////////////////////////////////////////////////////////////
            /*!	\return Date value of the current line in the current ctrl.
            \throw	no-throw */
            ///////////////////////////////////////////////////////////////////////////////////////////////////////////
            Date DateEditBehavour::getDate(Int32) const
            {
                QDate date = m_DateEditWidget->date();
				Date  retVal;

				if (m_DateEditWidget->maximumDate() == date)
					retVal.setInvalid();
				else
					retVal.setDate(date.year(), date.month(), date.day());

				return retVal;
            }

            ///////////////////////////////////////////////////////////////////////////////////////////
            //! \throw  no-throw
            ///////////////////////////////////////////////////////////////////////////////////////////
            void DateEditBehavour::clear()
            {
                setDate(Date(Date::NULL_DATE), 0);
            }
            ///////////////////////////////////////////////////////////////////////////////////////////
            //! \throw	no-throw
            ///////////////////////////////////////////////////////////////////////////////////////////
            void DateEditBehavour::erase( Int32 )
            {
            }
            ///////////////////////////////////////////////////////////////////////////////////////////
            /*! \return Int32:	number of hits that the ctrl with the most hits (shown rows/items) has.
            \throw	no-throw */
            ///////////////////////////////////////////////////////////////////////////////////////////
            Int32 DateEditBehavour::maxCountOfControls() const
            {
                return 1;
            }
            ///////////////////////////////////////////////////////////////////////////////////////////
            //! \throw no-throw
            //! \return type of given QObject
            ///////////////////////////////////////////////////////////////////////////////////////////
            WidgetTypes	DateEditBehavour::getWidgetType() const
            {
                return qDateEdit;
            }

        }// tie
    } // gui
} // basar
