//----------------------------------------------------------------------------
/*! \file
 *  \brief  class implementation of MultiComboBoxBehavour
 *  \author Roland Kiefert
 *  \date   08.09.2006
 */
//----------------------------------------------------------------------------


//---------------------------------------------------------------------------------------//
// includes
//---------------------------------------------------------------------------------------//
#include "multicomboboxbehavour.h"

#include "../macro.h"
#include "guipropertytable.h"
#include "libbasarguitie_exceptions.h"

#pragma warning (push)
#pragma warning (disable: 4231 4244 4481 4512 4800)
#include "libbasarqtwidget_multicombobox.h"
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
MultiComboBoxBehavour::MultiComboBoxBehavour(PGuiPropTbl                   pPt,
											 std::list<QWidget*>::iterator it )
											: ControlBehavour(pPt, it)
{
	m_MultiComboBoxWidget = static_cast<MultiComboBox*>( (*m_it) );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
MultiComboBoxBehavour::~MultiComboBoxBehavour()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return	ControlBehavour switched to next one (prae) 
	\param  rCurrentColumn index of current column
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MultiComboBoxBehavour::isSwitchNextAllowed(Int32 & rCurrentColumn)
{
	if( rCurrentColumn < m_MultiComboBoxWidget->columnCount() - 1 ){
		++rCurrentColumn;
		return false;
	} else {
		rCurrentColumn = 0;
		return true;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	rValue string-value to set to column
	\param	currentColumn column to set string value to
	\param	currentRow row to set string value to
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void MultiComboBoxBehavour::setString( const I18nString& rValue, Int32 currentRow, Int32 currentColumn )
{
	m_MultiComboBoxWidget->addItem(currentRow, currentColumn, QString::fromLocal8Bit(rValue.c_str()) );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return I18nString value of the current line in the current ctrl.
	\param	currentColumn column to get string value from
	\param	currentRow row to get string value from
	\throw	UnableToReadWriteInLineOfControlException */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
I18nString MultiComboBoxBehavour::getString(Int32 currentRow, Int32 currentColumn) const
{
	FUNCTION_NAME( basar.gui.tie.MultiComboBoxBehavour.getString() )
	if( m_MultiComboBoxWidget->count() <= currentRow ){
		VarString errMsg;
		errMsg.format( "Can't be read from control 'MultiComboBox' <%s> at line %ld!", 
						m_pPt->getPropertyDescriptionList().getName( getIndex(currentColumn) ).c_str(), 
						currentRow );
		throw UnableToReadWriteInLineOfControlException( ExceptInfo( fun, errMsg.c_str(), __FILE__, __LINE__) );
	} else {
		return m_MultiComboBoxWidget->itemText( currentRow, currentColumn).toLocal8Bit().constData();
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return	ColumnSize representing the position (0 indexed) of the ctrl in the list of all ctrls.
	\param	currentColumn column to ask for index
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
ColumnSize MultiComboBoxBehavour::getIndex(Int32 currentColumn) const
{
	return m_pPt->getPropertyDescriptionList().getIndex(
			m_MultiComboBoxWidget->getHorizontalHeaderItem( currentColumn ).toLocal8Bit().constData() );  
}

///////////////////////////////////////////////////////////////////////////////////////////
//! \throw  no-throw
///////////////////////////////////////////////////////////////////////////////////////////
void MultiComboBoxBehavour::clear()
{
	m_MultiComboBoxWidget->clearContext();
}
///////////////////////////////////////////////////////////////////////////////////////////
//! \throw	no-throw
///////////////////////////////////////////////////////////////////////////////////////////
void MultiComboBoxBehavour::erase( Int32 row )
{
	if( m_MultiComboBoxWidget->count() > row ){
		m_MultiComboBoxWidget->removeItem( row );
	}
}
///////////////////////////////////////////////////////////////////////////////////////////
/*! \return Int32:	number of hits that the ctrl with the most hits (shown rows/items) has.
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////
Int32 MultiComboBoxBehavour::maxCountOfControls() const
{
	return m_MultiComboBoxWidget->count();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return	ColumnSize representing the position (0 indexed) of the ctrl in the list of all ctrls.
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
Int32 MultiComboBoxBehavour::getCurrentIndex() 
{
	return m_MultiComboBoxWidget->currentIndex();
}
///////////////////////////////////////////////////////////////////////////////////////////
//! \throw no-throw
//! \return type of given QObject
///////////////////////////////////////////////////////////////////////////////////////////
WidgetTypes	MultiComboBoxBehavour::getWidgetType() const
{
	return multiComboBox;
}

}// tie
} // gui
} // basar
