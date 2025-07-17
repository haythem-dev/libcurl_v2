//----------------------------------------------------------------------------
/*! \file
 *  \brief  class implementation of ListWidgetBehavour
 *  \author Roland Kiefert
 *  \date   08.09.2006
 */
//----------------------------------------------------------------------------


//---------------------------------------------------------------------------------------//
// includes
//---------------------------------------------------------------------------------------//
#include "listwidgetbehavour.h"

#include "guipropertytable.h"
#include "../macro.h"
#include "libbasarguitie_exceptions.h"

#pragma warning (push)
#pragma warning (disable: 4231 4244 4481 4512 4800)
#include <QtWidgets/QListWidget>
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
ListWidgetBehavour::ListWidgetBehavour(PGuiPropTbl                   pPt,
									   std::list<QWidget*>::iterator it )
									  : ControlBehavour(pPt, it)
{
	m_ListWidget = static_cast<QListWidget*>( (*m_it) );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
ListWidgetBehavour::~ListWidgetBehavour()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	rValue string-value to set to column
	\param	currentColumn column to set string value to
	\param	currentRow row to set string value to
	\throw	UnableToReadWriteInLineOfControlException */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void ListWidgetBehavour::setString( const I18nString& rValue, Int32 currentRow, Int32 currentColumn )
{
	FUNCTION_NAME( basar.gui.tie.ListWidgetBehavour.setString(I18nString&) )

	VarString errMsg;
	if( m_ListWidget->count() < currentRow ){
		errMsg.format( "value <%s> can't be written in control 'QListWidget' <%s> at line %ld!", 
						rValue.c_str(), 
						m_pPt->getPropertyDescriptionList().getName( getIndex(currentColumn) ).c_str(), 
						currentRow );
		throw UnableToReadWriteInLineOfControlException( ExceptInfo( fun, errMsg.c_str(), __FILE__, __LINE__) );
	} else if( m_ListWidget->count() == currentRow ) {
		m_ListWidget->addItem( QString::fromLocal8Bit(rValue.c_str()) );
	} else {
		m_ListWidget->item( currentRow )->setText( QString::fromLocal8Bit(rValue.c_str()) );
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void ListWidgetBehavour::setCurrentString( const I18nString& rValue, Int32 )
{
	QList<QListWidgetItem *> li = m_ListWidget->findItems( QString::fromLocal8Bit(rValue.c_str()),Qt::MatchExactly);
	if (!(li.empty()))
		m_ListWidget->setCurrentItem(li.first());
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return I18nString value of the current line in the current ctrl.
	\param	currentColumn column to get string value from
	\param	currentRow row to get string value from
	\throw	UnableToReadWriteInLineOfControlException */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
I18nString ListWidgetBehavour::getString(Int32 currentRow, Int32 currentColumn) const
{
	FUNCTION_NAME( basar.gui.tie.ListWidgetBehavour.getString() )
		
	if( m_ListWidget->count() <= currentRow )
	{
		VarString errMsg;
		errMsg.format( "Can't be read from control 'QListWidget' <%s> at line %ld!", 
						m_pPt->getPropertyDescriptionList().getName( getIndex(currentColumn) ).c_str(), 
						currentRow );
		throw UnableToReadWriteInLineOfControlException( ExceptInfo( fun, errMsg, __FILE__, __LINE__) );
	} else {
		return m_ListWidget->item( currentRow )->text().toLocal8Bit().constData();
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return I18nString value of the current line in the current ctrl.
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
I18nString ListWidgetBehavour::getCurrentString(Int32) const
{
	return m_ListWidget->currentItem()->text().toLocal8Bit().constData();
}
///////////////////////////////////////////////////////////////////////////////////////////
//! \throw  no-throw
///////////////////////////////////////////////////////////////////////////////////////////
void ListWidgetBehavour::clear()
{
	m_ListWidget->clear();
}
///////////////////////////////////////////////////////////////////////////////////////////
//! \throw	no-throw
///////////////////////////////////////////////////////////////////////////////////////////
void ListWidgetBehavour::erase( Int32 row )
{
	if( m_ListWidget->count() > row ){
		delete m_ListWidget->takeItem( row );
	}
}
///////////////////////////////////////////////////////////////////////////////////////////
/*! \return Int32:	number of hits that the ctrl with the most hits (shown rows/items) has.
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////
Int32 ListWidgetBehavour::maxCountOfControls() const
{
	return m_ListWidget->count();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return	ColumnSize representing the position (0 indexed) of the ctrl in the list of all ctrls.
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
Int32 ListWidgetBehavour::getCurrentIndex()
{
	return m_ListWidget->currentRow();
}
///////////////////////////////////////////////////////////////////////////////////////////
/*!	\return vector of Int32 represents the absolute (regardless of the page shown), 0 indexed index 
			of the selected rows or items of a multi line ctrl. 
	\throw	no-throw*/
///////////////////////////////////////////////////////////////////////////////////////////
void ListWidgetBehavour::getSelectedIndices(std::vector<Int32>& vec)
{
	QList<QListWidgetItem*> itemList = m_ListWidget->selectedItems();
	for (Int32 i=0; i<itemList.size(); ++i)
	{
		if (std::find(vec.begin(),
					  vec.end(),
					  m_ListWidget->row(itemList.at(i))+m_pPt->getAbsoluteIndex()) == vec.end())
			vec.push_back(m_ListWidget->row(itemList.at(i))+m_pPt->getAbsoluteIndex());
	}
}
///////////////////////////////////////////////////////////////////////////////////////////
//! \throw no-throw
//! \return type of given QObject
///////////////////////////////////////////////////////////////////////////////////////////
WidgetTypes	ListWidgetBehavour::getWidgetType() const
{
	return qListWidget;
}

}// tie
} // gui
} // basar
