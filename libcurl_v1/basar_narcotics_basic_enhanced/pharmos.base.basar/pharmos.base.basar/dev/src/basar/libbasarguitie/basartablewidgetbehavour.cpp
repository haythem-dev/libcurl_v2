//----------------------------------------------------------------------------
/*! \file
 *  \brief  class implementation of BasarTableWidgetBehavour
 *  \author Roland Kiefert
 *  \date   08.09.2006
 */
//----------------------------------------------------------------------------


//---------------------------------------------------------------------------------------//
// includes
//---------------------------------------------------------------------------------------//
#include "basartablewidgetbehavour.h"

#include "loggerpool.h"
#include "helper.h"

#include "guipropertytable.h"

#pragma warning (push)
#pragma warning (disable: 4231 4244 4481 4512 4800)
#include "libbasarqtwidget_tablewidget.h"
#include <QtWidgets/QHeaderView>

#include <Qtwidgets/qapplication.h>
#include <QtGui/qevent.h>
#pragma warning (pop)

//---------------------------------------------------------------------------------------//
// namespaces
//---------------------------------------------------------------------------------------//
namespace basar
{
    namespace gui
    {
        namespace tie
        {

///////////////////////////////////////////////////////////////////////////////////////////////////////////
            /*! \throw no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
            BasarTableWidgetBehavour::BasarTableWidgetBehavour ( PGuiPropTbl                   pPt,
                    std::list<QWidget*>::iterator it )
                : TableWidgetBehavour( pPt, it )
            {
                m_basarTableWidget = static_cast<BasarTableWidget*>( ( *m_it ) );
            }
///////////////////////////////////////////////////////////////////////////////////////////////////////////
            /*! \throw no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
            BasarTableWidgetBehavour::~BasarTableWidgetBehavour()
            {
                m_pPt->unregisterForMatchDoneNotification(m_Self);
            }

///////////////////////////////////////////////////////////////////////////////////////////
//! \throw  no-throw
///////////////////////////////////////////////////////////////////////////////////////////
            void BasarTableWidgetBehavour::clear()
            {
                m_basarTableWidget->clearContext();
                /*
                for (Int32 i = 0; i < m_basarTableWidget->rowCount(); ++i)
                {
                    m_basarTableWidget->hideRow(i);
                }*/
            }

///////////////////////////////////////////////////////////////////////////////////////////////
//! \throw no-throw
///////////////////////////////////////////////////////////////////////////////////////////
            void BasarTableWidgetBehavour::checkResize( QTableWidget* tableWidget )
            {
                BasarTableWidget* basarTableWidget = dynamic_cast<BasarTableWidget*>( tableWidget );

                if ( nullptr != basarTableWidget )
                {
                    //Important: Observe order of opertations!
                    //Resize columns is required to attain expected results from resize rows
                    //if text in a column would lead to a line break in the field, row size would be larger than necessary
                    //if column size weren't first resized

                    if ( basarTableWidget->isResizeColumnsToContents() )
                    {
                        basarTableWidget->resizeColumnsToContents();
                    }

                    if ( basarTableWidget->isResizeRowsToContents() )
                    {
                        basarTableWidget->resizeRowsToContents();
                    }

                    if ( basarTableWidget->isStretchLastHorizontalSection() )
                    {
                        basarTableWidget->horizontalHeader()->setStretchLastSection(true);
                    }
                }
                else
                {
                    BLOG_TRACE( LoggerPool::eventHandler(),
                                "BasarTableWidgetBehavour::checkResize used on a "
                                "TableWidget that is no Basar TableWidget" );
                }
            }
///////////////////////////////////////////////////////////////////////////////////////////////
//! \throw no-throw
//! \return type of given QObject
///////////////////////////////////////////////////////////////////////////////////////////
            WidgetTypes BasarTableWidgetBehavour::getWidgetType() const
            {
                return basarTableWidget;
            }

            void BasarTableWidgetBehavour::registerForNotification()
            {
                m_pPt->registerForMatchDoneNotification(m_Self);
            }

///////////////////////////////////////////////////////////////////////////////////////////////////////////
            /*! neccessary as the method setItem is not virtual in  QTableWidget
                *         or in BasarTableWidget
            *          overwritten in BasarTableWidget
            * @param row Row
            * @param column column
            * @param item item
            */
///////////////////////////////////////////////////////////////////////////////////////////////////////////

            void  BasarTableWidgetBehavour::setItem( int row, int column, QTableWidgetItem* item )
            {
                m_basarTableWidget->setItem( row, column, item );
            }

            void BasarTableWidgetBehavour::notifyMatchDone()
            {
                checkResize( m_basarTableWidget );
            }

///////////////////////////////////////////////////////////////////////////////////////////////////////////

        }// tie
    } // gui
} // basar
