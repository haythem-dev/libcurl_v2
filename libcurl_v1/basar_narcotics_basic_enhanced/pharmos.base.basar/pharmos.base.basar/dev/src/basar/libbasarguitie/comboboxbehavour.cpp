//----------------------------------------------------------------------------
/*! \file
 *  \brief  class implementation of ComboBoxBehavour
 *  \author Roland Kiefert
 *  \date   08.09.2006
 */
//----------------------------------------------------------------------------


//---------------------------------------------------------------------------------------//
// includes
//---------------------------------------------------------------------------------------//
#include "comboboxbehavour.h"

#include "../macro.h"
#include "guipropertytable.h"
#include "libbasarguitie_exceptions.h"

#pragma warning (push)
#pragma warning (disable: 4231 4244 4481 4512 4800)
#include <QtWidgets/QComboBox>
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
ComboBoxBehavour::ComboBoxBehavour ( PGuiPropTbl                   pPt,
								     std::list<QWidget*>::iterator it)
								   : ControlBehavour(pPt, it)
{
	m_ComboBoxWidget = static_cast<QComboBox*>( (*m_it) );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
ComboBoxBehavour::~ComboBoxBehavour()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	rValue        string-value to set to column
	\param	currentColumn column to set string value to
	\param	currentRow    row to set string value to
	\throw	UnableToReadWriteInLineOfControlException */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void ComboBoxBehavour::setString( const I18nString& rValue, Int32 currentRow, Int32 currentColumn )
{
	FUNCTION_NAME( basar.gui.tie.ComboBoxBehavour.setString())

	if( m_ComboBoxWidget->count() < currentRow )
	{
		VarString errMsg;
		errMsg.format( "value <%s> can't be written in widget 'QComboBox' <%s> at line %ld!", 
						rValue.c_str(), 
						m_pPt->getPropertyDescriptionList().getName( getIndex(currentColumn) ).c_str(), 
						currentRow );
		throw UnableToReadWriteInLineOfControlException( ExceptInfo( fun, errMsg, __FILE__, __LINE__) );
	} 

	// no feature for selecting/set current item! => use transformer for correct match
	if ( m_ComboBoxWidget->count() == currentRow )
	{
		m_ComboBoxWidget->addItem( QString::fromLocal8Bit(rValue.c_str()) );					// add item
	}
	else 
	{
		m_ComboBoxWidget->setItemText( currentRow, QString::fromLocal8Bit(rValue.c_str()) );	// overwrite existing item
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void ComboBoxBehavour::setCurrentString( const I18nString& rValue, Int32 )
{
	Int32 index = m_ComboBoxWidget->findText( QString::fromLocal8Bit(rValue.c_str()) );

	if (index != -1)
		m_ComboBoxWidget->setCurrentIndex(index);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return I18nString    value of the current line in the current ctrl.
	\throw	UnableToReadWriteInLineOfControlException */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
I18nString ComboBoxBehavour::getString(Int32 currentRow, Int32 currentColumn) const
{
	FUNCTION_NAME( basar.gui.tie.ComboBoxBehavour.getString() )

	if( m_ComboBoxWidget->count() <= currentRow )
	{
		VarString errMsg;
		errMsg.format( "value can't be read from widget 'QComboBox' <%s> at line %ld!", 
						m_pPt->getPropertyDescriptionList().getName( getIndex(currentColumn) ).c_str(), 
						currentRow );
		throw UnableToReadWriteInLineOfControlException( ExceptInfo( fun, errMsg, __FILE__, __LINE__) );
	} 
	
	return m_ComboBoxWidget->itemText( currentRow ).toLocal8Bit().constData();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return I18nString value of the current line in the current ctrl.
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
I18nString ComboBoxBehavour::getCurrentString(Int32) const
{
	return m_ComboBoxWidget->currentText().toLocal8Bit().constData();
}

///////////////////////////////////////////////////////////////////////////////////////////
//! \throw  no-throw
///////////////////////////////////////////////////////////////////////////////////////////
void ComboBoxBehavour::clear()
{
	m_ComboBoxWidget->clear();
}

///////////////////////////////////////////////////////////////////////////////////////////
//! \throw	no-throw
///////////////////////////////////////////////////////////////////////////////////////////
void ComboBoxBehavour::erase( Int32 row )
{
	if ( m_ComboBoxWidget->count() > row )
		m_ComboBoxWidget->removeItem( row );
}

///////////////////////////////////////////////////////////////////////////////////////////
/*! \return Int32:	number of hits that the ctrl with the most hits (shown rows/items) has.
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////
Int32 ComboBoxBehavour::maxCountOfControls() const
{
	return m_ComboBoxWidget->count();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return	ColumnSize representing the position (0 indexed) of the ctrl in the list of all ctrls.
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
Int32 ComboBoxBehavour::getCurrentIndex() 
{
	return m_ComboBoxWidget->currentIndex();
}

///////////////////////////////////////////////////////////////////////////////////////////
//! \throw no-throw
//! \return type of given QObject
///////////////////////////////////////////////////////////////////////////////////////////
WidgetTypes	ComboBoxBehavour::getWidgetType() const
{
	return qComboBox;
}

//----------------------------------------------------------------------------
}// tie
} // gui
} // basar
