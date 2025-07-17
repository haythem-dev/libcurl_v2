//----------------------------------------------------------------------------
/*! \file
*  \brief  class implementation of TableWidgetBehavour
*  \author Roland Kiefert
*  \date   08.09.2006
*/
//----------------------------------------------------------------------------


//---------------------------------------------------------------------------------------//
// includes
//---------------------------------------------------------------------------------------//
#include "tablewidgetbehavour.h"

#include "../macro.h"
#include "guipropertytable.h"
#include "libbasarguitie_exceptions.h"
#include "libbasarguitie_exceptionhandlermacros.h"
#include "loggerpool.h"
#include "matcher.h"
#include "libbasardbaspect_accessorpropertytable_yiterator.h"

#include <QtCore/QEvent>

#pragma warning (push)
#pragma warning (disable: 4231 4244 4512)
#include <QtGui/QKeyEvent>
#pragma warning (pop)

#pragma warning (push)
#pragma warning (disable: 4231 4244 4481 4800)
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QApplication>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QScrollbar>
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
	TableWidgetBehavour::TableWidgetBehavour(PGuiPropTbl                   pPt,
											 std::list<QWidget*>::iterator it )
											: ControlBehavour(pPt, it), m_tableWidget(nullptr)
	{
		m_tableWidget = static_cast<QTableWidget*>( (*m_it) );
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*!	\throw no-throw */
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	TableWidgetBehavour::~TableWidgetBehavour()
	{
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*!	\return	ControlBehavour switched to next one (prae) 
	\param  rCurrentColumn index of current column
	\throw	no-throw */
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool TableWidgetBehavour::isSwitchNextAllowed(Int32 & rCurrentColumn)
	{
		bool retval = true;
		if( rCurrentColumn < m_tableWidget->columnCount() -1 )
		{
			++rCurrentColumn;
			retval = false;
		} 
		else 
		{
			rCurrentColumn = 0;
			retval = true;
		}
		return retval;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*!	\throw	NoValidMemoryPolicyException */
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	void TableWidgetBehavour::setString( const I18nString& rValue, Int32 currentRow, Int32 currentColumn )
	{
		FUNCTION_NAME( basar.gui.tie.TableWidgetBehavour.setString(I18nString&) )
			VarString errMsg;
		//	WITHOUT_MEMORY: means that the QTableWidget will display 
		//					the data without any vertical scroll bar. 
		//					The ctrl. is cleared, row or page specific,
		//					and the data will be displayed in a number 
		//					of visible lines only.
		if( WITHOUT_MEMORY == m_pPt->m_MemoryPolicy ){
			if( (m_pPt->getAbsoluteIndex() <= currentRow) && 
				(currentRow < m_pPt->getAbsoluteIndex() + m_tableWidget->rowCount() ) ){
					// EVENT:	being within the qtablewidget
					//			'm_CurrentRow' is located between the first 
					//			(included) visible row of the qtablewidget 
					//			and the last (included) visible row of the qtablewidget.
					//			--> nothing to do.

				} else if( currentRow == (m_pPt->m_AbsoluteIndex - 1) ){
					// EVENT:	browse one line back
					if( 0 == m_pPt->getAbsoluteIndex() ){
						// if the current line is 0 then nothing has to be done.
						return;
					}
					// method 'removeRow' is 0 indicated;
					m_tableWidget->removeRow( m_tableWidget->rowCount() - 1 );
					m_tableWidget->insertRow(0);
					--m_pPt->m_AbsoluteIndex;

				} else if( currentRow == (m_pPt->m_AbsoluteIndex + m_tableWidget->rowCount() ) ){ 
					// EVENT:	browse one line forward
					m_tableWidget->removeRow(0);
					m_tableWidget->insertRow( m_tableWidget->rowCount() );
					++m_pPt->m_AbsoluteIndex;

				} else {
					//!  \todo check to browse one page forward and one page backwards
					// EVENT:	browse one page
					clear();
					m_pPt->m_AbsoluteIndex = currentRow;
				}
		} else if( WITH_MEMORY == m_pPt->m_MemoryPolicy ){
			// WITH_MEMORY: means that the QTableWidget will display 
			//				the data with the use of the vertical scroll bar. 
			//				I.e. the ctrl. will always push new data (rows) to the back
			//				and the vertical scroll bar is shown if necessary.			
			if( 0 <= currentRow && currentRow < m_tableWidget->rowCount() ){
				//	The current row is between the first (included) and 
				//	the last (included) row of the QTableWidget.
				//	--> nothing to do.
			} else if( m_tableWidget->rowCount() == currentRow ){
				//	the current row is one row after the last one of the QTableWidget.
				//	--> a new row has to be inserted.
				m_tableWidget->insertRow( m_tableWidget->rowCount() );
			}
		} else {
			basar::VarString msg;
			msg.format("memory policy <%d> of control <%s> is not valid for setting value <%s>.", 
				m_pPt->m_MemoryPolicy, 
				m_pPt->getPropertyDescriptionList().getName( getIndex(currentColumn) ).c_str(), 
				rValue.c_str());
			throw NoValidMemoryPolicyException(	ExceptInfo( fun, msg.c_str(), __FILE__, __LINE__) );
		}
		QTableWidgetItem* item = new QTableWidgetItem( QString::fromLocal8Bit( rValue.c_str() ) );
		setItem( currentRow - m_pPt->m_AbsoluteIndex, currentColumn, item );
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*!	neccessary as the method setItem is not virtual in  QTableWidget
	 *         or in BasarTableWidget
	*		   overwritten in BasarTableWidget
	* @param row Row
	* @param column column
	* @param item item 
	*/
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	void  TableWidgetBehavour::setItem(int row, int column, QTableWidgetItem * item)
	{
		m_tableWidget->setItem( row, column, item );
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*!	\return I18nString value of the current line in the current ctrl.
	\throw	UnableToReadWriteInLineOfControlException */
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	I18nString TableWidgetBehavour::getString(Int32 currentRow, Int32 currentColumn) const
	{
		I18nString retval;
		FUNCTION_NAME( basar.gui.tie.TableWidgetBehavour.getString() )
		VarString errMsg;
		Int32 rowCount = m_tableWidget->rowCount();
		Int32 absIndex = m_pPt->getAbsoluteIndex();
		Int32 realRowIndex = currentRow - absIndex;
		if( realRowIndex < 0  || realRowIndex >= rowCount)
		{
			errMsg.format( "Can't be read from control 'QTableWidget' <%s> at line %ld!", 
				m_pPt->getPropertyDescriptionList().getName( getIndex(currentColumn) ).c_str(), 
				realRowIndex );
			throw UnableToReadWriteInLineOfControlException( ExceptInfo( fun, errMsg.c_str(), __FILE__, __LINE__) );
		} 
		else 
		{
			QTableWidgetItem * item = m_tableWidget->item( realRowIndex, currentColumn );
			if (item != 0)
			{
				retval = item->text().toLocal8Bit().constData();
			}
		}
		return retval;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*!	\return	ColumnSize representing the position (0 indexed) of the ctrl in the list of all ctrls.
	\param	currentColumn column to get index for
	\throw	no-throw */
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	ColumnSize TableWidgetBehavour::getIndex(Int32 currentColumn) const
	{
		return m_pPt->getPropertyDescriptionList().getIndex( 
			m_tableWidget->horizontalHeaderItem( currentColumn )->text().toLocal8Bit().constData()
		);  
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*!	\retval true if a value is found
	\param	currentColumn column to check for value
	\param	currentRow row to check for value
	\throw	no-throw */
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool TableWidgetBehavour::isValueExist(Int32 currentRow, Int32 currentColumn) const
	{
		return (m_tableWidget->item(currentRow - m_pPt->getAbsoluteIndex(),currentColumn) != 0);
	}

	///////////////////////////////////////////////////////////////////////////////////////////
	//! \throw  no-throw
	///////////////////////////////////////////////////////////////////////////////////////////
	void TableWidgetBehavour::clear()
	{
		std::list<basar::VarString> horizontalList;
		std::list<basar::VarString> verticalList;
		if (! m_tableWidget->horizontalHeader()->isHidden()) {
			for( int column = 0; column < m_tableWidget->columnCount(); ++column )
			{
				QTableWidgetItem * item = m_tableWidget->horizontalHeaderItem(column);
				if (item != 0)
				{ 
				    horizontalList.push_back( item->text().toLocal8Bit().data() );
				}
			}
		}
		if (! m_tableWidget->verticalHeader()->isHidden()) 
		{
			for( int row = 0; row < m_tableWidget->rowCount(); ++row )
			{
				QTableWidgetItem * item = m_tableWidget->verticalHeaderItem(row);
				if (item != 0)
				{ 
				   verticalList.push_back( item->text().toLocal8Bit().data() );
				}
			}
		}
		QStringList list;
		std::list<basar::VarString>::iterator it;
		m_tableWidget->clear();
		it = horizontalList.begin();
		for( ; it != horizontalList.end(); ++it )
		{
			list << QString::fromLocal8Bit( (*it).c_str() );
		}
		if (list.size() > 0)
		{
			m_tableWidget->setHorizontalHeaderLabels(list);
		}
		it = verticalList.begin();
		list.clear();
		for( ; it != verticalList.end(); ++it )
		{
			list << QString::fromLocal8Bit( (*it).c_str() );
		}
		if (list.size() > 0)
		{
			m_tableWidget->setVerticalHeaderLabels(list);
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////////
	//! \throw	NoValidMemoryPolicyException
	///////////////////////////////////////////////////////////////////////////////////////////
	void TableWidgetBehavour::erase( Int32 row )
	{
		FUNCTION_NAME( basar.gui.tie.TableWidgetBehavour.erase(GuiPropertyTable_YIterator) )
		if( m_tableWidget->rowCount() > row ){
			m_tableWidget->removeRow( row );
			if( WITHOUT_MEMORY == m_pPt->m_MemoryPolicy ){			
				m_tableWidget->setCurrentItem( m_tableWidget->item(m_tableWidget->rowCount()-1, 0) ); 
				m_tableWidget->setFocus(); // automatically the focus is set to the current item.
				m_tableWidget->insertRow( m_tableWidget->rowCount() ); 
				QKeyEvent ke( QEvent::KeyPress, Qt::Key_Down, Qt::NoModifier ); 
				QApplication::sendEvent( m_tableWidget, &ke );					
			} else if( WITH_MEMORY == m_pPt->m_MemoryPolicy ){
				// nothing has to be done!
			} else {
				basar::VarString msg;
				msg.format("No valid Memory-Policy <%d> is set!", m_pPt->m_MemoryPolicy);
				throw NoValidMemoryPolicyException(	ExceptInfo( fun, msg.c_str(), __FILE__, __LINE__) );
			}
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////////
	/*! \return Int32:	number of hits that the ctrl with the most hits (shown rows/items) has.
	\throw	no-throw */
	///////////////////////////////////////////////////////////////////////////////////////////
	Int32 TableWidgetBehavour::maxCountOfControls() const
	{
		Int32 count = 0;

		for( Int32 i = m_tableWidget->rowCount()-1; 0 <= i ;--i )
		{
			for (Int32 j = 0; j <= m_tableWidget->columnCount()-1; ++j) 
			{
				if( 0 != m_tableWidget->item(i, j) )
				{
					if( count < i+1 )
					{
						count = i+1;
						break;
					}
				}
			}
		} 

		return count;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*!	\return	ColumnSize representing the position (0 indexed) of the ctrl in the list of all ctrls.
	\throw	no-throw */
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	Int32 TableWidgetBehavour::getCurrentIndex() 
	{
		return m_tableWidget->currentRow();
	}
	///////////////////////////////////////////////////////////////////////////////////////////
	/*!	\param	vec vector of Int32 represents the absolute (regardless of the page shown), 0 indexed index 
	of the selected rows or items of a multi line ctrl. 
	\throw	no-throw*/
	///////////////////////////////////////////////////////////////////////////////////////////
	void TableWidgetBehavour::getSelectedIndices(std::vector<Int32>& vec)
	{
		QList<QTableWidgetSelectionRange> rangeList;
		rangeList = m_tableWidget->selectedRanges();
		for (Int32 i=0; i<rangeList.size(); ++i)
		{
			for (Int32 j=rangeList.at(i).topRow(); j<=rangeList.at(i).bottomRow(); ++j)
			{
				if (std::find(vec.begin(),vec.end(),j+m_pPt->getAbsoluteIndex()) == vec.end())
					vec.push_back(j+m_pPt->getAbsoluteIndex());
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	//! \throw no-throw
	// is empty and will be overwritten in BasarTableWidgetBehavour
	///////////////////////////////////////////////////////////////////////////////////////////
	void TableWidgetBehavour::checkResize(QTableWidget * )
	{
		// do nothing, used in childclass BasarTableWidgetBehavour
	}

	///////////////////////////////////////////////////////////////////////////////////////////
	/*!	\throw no-throw
	\param	pObject object the event belongs to.
	\param	pEvent event to throw.
	\retval true if the parent object can work with this event */
	///////////////////////////////////////////////////////////////////////////////////////////
	bool TableWidgetBehavour::eventFilterCtr(QObject *pObject, QEvent * pEvent)
	{
		bool retval = false;
		LIBBASARGUITIE_EXCEPTIONHANDLERMACRO_BEGIN
		{
			if ( pEvent->type() == QEvent::KeyPress )
			{ 
				QKeyEvent* keyEvent = dynamic_cast<QKeyEvent* > (pEvent);
				if (keyEvent == 0)
				{
					// ?!? not realy possible.
					BLOG_TRACE(LoggerPool::eventHandler(), "strange QKeyEvent Found");
				}
				else if (keyEvent->isAutoRepeat() )
				{
					retval = false;
				}
				else if (keyEvent->modifiers() != Qt::NoModifier)
				{
					retval = false;
				}
				else
				{ 
					QTableWidget * tableWidget = dynamic_cast<QTableWidget*>(pObject);
					if (tableWidget == 0)
					{
						retval = false;
						BLOG_TRACE(LoggerPool::eventHandler(), "no basar event handling for no table widget.");
					}
					else
					{
						switch (keyEvent->key())
						{
						case Qt::Key_PageDown: 
						     retval = doPageDown(tableWidget); 
							break;
						case Qt::Key_PageUp:
							retval = doPageUp(tableWidget);
							break;
						case Qt::Key_Down:
							retval = doRowDown(tableWidget);
							break;
						case Qt::Key_Up:
							retval = doRowUp(tableWidget);
							break;
						default:
							BLOG_TRACE(LoggerPool::eventHandler(), "no basar event handling for table widget - key pressed, excpet than key up / down + page up / down.");
							retval = false;
							break;
						}
					}
				}
			}
		}
		LIBBASARGUITIE_EXCEPTIONHANDLERMACRO_END("basar.gui.tie.TableWidgetBehavour::eventFilterCtr")
		return retval;
	}

	///////////////////////////////////////////////////////////////////////////////////////////
	//! \throw no-throw
	//! \return type of given QObject
	///////////////////////////////////////////////////////////////////////////////////////////
	WidgetTypes	TableWidgetBehavour::getWidgetType() const
	{
		return qTableWidget;
	}

	///////////////////////////////////////////////////////////////////////////////////////////
	//! helperfunctions for one PageUp (no signals), part of eventFilterCtr
	//! \return false (always)
	///////////////////////////////////////////////////////////////////////////////////////////
	bool TableWidgetBehavour::doPageUp(QTableWidget * tableWidget) 
	{
		BLOG_TRACE(LoggerPool::eventHandler(), "event handling for table widget - page up.");
		int column = tableWidget->currentColumn();
		tableWidget->blockSignals(true);
		if (tableWidget->currentRow() == 0)
		{
			if ( !m_pMatcher.expired() )
			{
				basar::Int32 absIndex = m_pMatcher.lock()->getLeft().getAbsoluteIndex();
				int rowCount = tableWidget->rowCount();
				if (absIndex > rowCount)
				{
					m_pMatcher.lock()->RightToLeft(absIndex-rowCount, rowCount);
				}
				else if (absIndex > 0)
				{
					m_pMatcher.lock()->RightToLeft(0, rowCount);
				}
				checkResize(tableWidget);
				
				QTableWidgetItem * item = tableWidget->item(0, column);
				tableWidget->setCurrentItem( item );
			}		
		}
		tableWidget->blockSignals(false);
		
		return false;
	}

	///////////////////////////////////////////////////////////////////////////////////////////
	//! helperfunctions for one PageDown (no signals), part of eventFilterCtr
	//! \return false (always)
	///////////////////////////////////////////////////////////////////////////////////////////
	bool TableWidgetBehavour::doPageDown(QTableWidget * tableWidget) 
	{
		BLOG_TRACE(LoggerPool::eventHandler(), "event handling for table widget - page down.");
		// only if we are already in the last line:
		if (tableWidget->currentRow() == tableWidget->rowCount()-1)
		{
			if ( !m_pMatcher.expired() )
			{
				basar::db::aspect::AccessorPropertyTable_YIterator yiter = m_pMatcher.lock()->getCurrentRight();
				if (!yiter.isEnd())
				{
					// if the next one is the last, then don't have to do anything.
					++yiter;
				}
				if (!yiter.isEnd())
				{
					int column = tableWidget->currentColumn();
					tableWidget->blockSignals(true);
					m_pMatcher.lock()->RightToLeft(m_pMatcher.lock()->getLeft().getAbsoluteIndex() + tableWidget->rowCount(),
						                           tableWidget->rowCount());
					QTableWidgetItem * item = tableWidget->item(tableWidget->rowCount()-1, column);
					tableWidget->setCurrentItem( item );
					checkResize(tableWidget);
					tableWidget->blockSignals(false);	
				}
			}
		}
		return false;
	}

	///////////////////////////////////////////////////////////////////////////////////////////
	//! helperfunctions for one KeyUp (no signals), part of eventFilterCtr
	//! \return false (always)
	///////////////////////////////////////////////////////////////////////////////////////////
	bool TableWidgetBehavour::doRowUp(QTableWidget * tableWidget) 
	{
		BLOG_TRACE(LoggerPool::eventHandler(), "event handling for table widget - key up.");

		if (tableWidget->currentRow() == 0)
		{
			if ( !m_pMatcher.expired() )
			{
				if (m_pMatcher.lock()->getLeft().getAbsoluteIndex() > 0)
				{
					tableWidget->blockSignals(true);
					m_pMatcher.lock()->RightToLeft(m_pMatcher.lock()->getLeft().getAbsoluteIndex() - 1, 
						                           tableWidget->rowCount());
					checkResize(tableWidget);
					tableWidget->blockSignals(false);
				}
			}
		}
		return false;
	}

	///////////////////////////////////////////////////////////////////////////////////////////
	//! helperfunctions for one KeyDown(no signals), part of eventFilterCtr
	//! \return false (always)
	///////////////////////////////////////////////////////////////////////////////////////////
	bool TableWidgetBehavour::doRowDown(QTableWidget * tableWidget) 
	{
		BLOG_TRACE(LoggerPool::eventHandler(), "event handling for table widget - key down.");

		if (tableWidget->currentRow() == tableWidget->rowCount()-1)
		{
			if ( !m_pMatcher.expired() )
			{
				basar::db::aspect::AccessorPropertyTable_YIterator yiter = m_pMatcher.lock()->getCurrentRight();
				if (!yiter.isEnd())
				{
					// if the next one is the last, then don't have to do anything.
					++yiter;
				}
				if (!yiter.isEnd())
				{
					tableWidget->blockSignals(true);
					m_pMatcher.lock()->RightToLeft(m_pMatcher.lock()->getLeft().getAbsoluteIndex() + 1 /*tableWidget->rowCount()*/, 
						                           tableWidget->rowCount());
					checkResize(tableWidget);
					tableWidget->blockSignals(false);
				}
			}
		}
		return false;
	}

}// tie
} // gui
} // basar
