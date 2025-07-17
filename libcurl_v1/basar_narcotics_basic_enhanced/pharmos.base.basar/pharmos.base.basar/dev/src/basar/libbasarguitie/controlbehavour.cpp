//----------------------------------------------------------------------------
/*! \file
 *  \brief  class implementation of ControlBehavour
 *  \author Roland Kiefert
 *  \date   08.09.2006
 */
//----------------------------------------------------------------------------

//---------------------------------------------------------------------------------------//
// includes
//---------------------------------------------------------------------------------------//


#include "controlbehavour.h"

#include "../macro.h"
#include "helper.h"
#include "guipropertytable.h"
#include "libbasarguitie_exceptions.h"
#include "libbasarguitie_exceptionhandlermacros.h"
#include "eventhandlercomposite.h"
#include "loggerpool.h"

#pragma warning (push)
#pragma warning (disable: 4231 4244 4800)
#include <QtWidgets/QWidget>
#include <QtCore/QEvent>
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
ControlBehavour::ControlBehavour( PGuiPropTbl                   pPt,
								  std::list<QWidget*>::iterator it)
								: m_it   (it), 
								  m_pPt  (pPt)
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
ControlBehavour::~ControlBehavour()
{

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\retval true if it is possible to switch to control's next column
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ControlBehavour::isSwitchNextAllowed(Int32 & rCurrentColumn)
{
	rCurrentColumn = 0;
	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	rValue        string-value to set to row/column
	\param	currentColumn column to set string value to
	\param	currentRow    row to set string value to
	\throw	GuiControlNotSupportedException */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void ControlBehavour::setString( const I18nString & rValue, Int32 currentRow, Int32 currentColumn)
{
	FUNCTION_NAME( basar.gui.tie.ControlBehavour.setString() )
	basar::VarString msg;
	msg.format("occurred type <%d> of widget <%s> is not handled for "
		       "setting string value <%s> to row <%d>, column <%d>", 
				Helper::getWidgetType(*(*m_it)), 
				m_pPt->getPropertyDescriptionList().getName( getIndex(currentColumn)).c_str(),
				rValue.c_str(),
				currentRow,
				currentColumn);
	throw GuiControlNotSupportedException( ExceptInfo( fun, msg, __FILE__, __LINE__) );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	rValue        string-value to set to column
	\param	currentColumn column to set string value to
	\throw	GuiControlNotSupportedException */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void ControlBehavour::setCurrentString( const I18nString & rValue, Int32 currentColumn )
{
	FUNCTION_NAME( basar.gui.tie.ControlBehavour.setCurrentString() )
	basar::VarString msg;
	msg.format("occurred type <%d> of widget <%s> is not handled for "
		       "setting string value <%s> to row <current>, column <%d>", 
				Helper::getWidgetType(*(*m_it)), 
				m_pPt->getPropertyDescriptionList().getName( getIndex(currentColumn)).c_str(),
				rValue.c_str(),
				currentColumn);
	throw GuiControlNotSupportedException( ExceptInfo( fun, msg, __FILE__, __LINE__) );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	rValue        DateTime-value to set to column
	\param	currentColumn column to set datetime value to
	\throw	GuiControlNotSupportedException */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void ControlBehavour::setDateTime( const DateTime & rValue, Int32 currentColumn )
{
	FUNCTION_NAME( basar.gui.tie.ControlBehavour.setDateTime())
	basar::VarString msg;
	msg.format("occurred type <%d> of widget <%s> is not handled for "
		       "setting datetime value <%s> to column <%d>",
				Helper::getWidgetType(*(*m_it)), 
				m_pPt->getPropertyDescriptionList().getName( getIndex(currentColumn)).c_str(),
				rValue.toStrDate().c_str(),
				currentColumn);
	throw GuiControlNotSupportedException( ExceptInfo( fun, msg, __FILE__, __LINE__) );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	rValue        Date-value to set to column
	\param	currentColumn column to set datetime value to
	\throw	GuiControlNotSupportedException */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void ControlBehavour::setDate( const Date & rValue, Int32 currentColumn )
{
	FUNCTION_NAME( basar.gui.tie.ControlBehavour.setDate())
	basar::VarString msg;
	msg.format("occurred type <%d> of widget <%s> is not handled for "
		       "setting datetime value <%s> to column <%d>", 
				Helper::getWidgetType(*(*m_it)), 
				m_pPt->getPropertyDescriptionList().getName( getIndex(currentColumn)).c_str(),
				rValue.toStrDate().c_str(),
				currentColumn);
	throw GuiControlNotSupportedException( ExceptInfo( fun, msg, __FILE__, __LINE__) );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	rValue        Time-value to set to column
	\param	currentColumn column to set datetime value to
	\throw	GuiControlNotSupportedException */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void ControlBehavour::setTime( const Time & rValue, Int32 currentColumn )
{
	FUNCTION_NAME( basar.gui.tie.ControlBehavour.setTime())
	basar::VarString msg;
	msg.format("occurred type <%d> of widget <%s> is not handled for "
		       "setting datetime value <%s> to column <%d>", 
				Helper::getWidgetType(*(*m_it)), 
				m_pPt->getPropertyDescriptionList().getName( getIndex(currentColumn)).c_str(),
				rValue.toStrDate().c_str(),
				currentColumn);
	throw GuiControlNotSupportedException( ExceptInfo( fun, msg, __FILE__, __LINE__) );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return I18nString     value of the asked row + column in the current ctrl.
	\param	currentColumn column to get string value from
	\param	currentRow    row to get string value from
	\throw	GuiControlNotSupportedException */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
I18nString ControlBehavour::getString(Int32 currentRow, Int32 currentColumn) const
{
	FUNCTION_NAME( basar.gui.tie.ControlBehavour.getString() )
	basar::VarString msg;
	msg.format("occurred type <%d> of widget <%s> is not handled for "
		       "getting string from row <%d>, column <%d>",
				Helper::getWidgetType(*(*m_it)), 
				m_pPt->getPropertyDescriptionList().getName( getIndex(currentColumn) ).c_str(),
				currentRow,
				currentColumn);
	throw GuiControlNotSupportedException( ExceptInfo( fun, msg, __FILE__, __LINE__) );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return I18nString    value of the asked column and current row in the current ctrl.
	\param	currentColumn column to get string value from
	\throw	GuiControlNotSupportedException */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
I18nString ControlBehavour::getCurrentString(Int32 currentColumn) const
{
	FUNCTION_NAME( basar.gui.tie.ControlBehavour.getCurrentString() )
	basar::VarString msg;
	msg.format("occurred type <%d> of widget <%s> is not handled for "
		       "getting current string from row <current>, column <%d>", 
				Helper::getWidgetType(*(*m_it)), 
				m_pPt->getPropertyDescriptionList().getName( getIndex(currentColumn)).c_str(),
				currentColumn);
	throw GuiControlNotSupportedException( ExceptInfo( fun, msg, __FILE__, __LINE__) );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return DateTime      value of the asked column and current row in the current ctrl.
	\param	currentColumn column to get datetime value from
	\throw	GuiControlNotSupportedException */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
DateTime ControlBehavour::getDateTime(Int32 currentColumn) const
{
	FUNCTION_NAME( basar.gui.tie.ControlBehavour.getDateTime() )
	basar::VarString msg;
	msg.format("occurred type <%d> of widget <%s> is not handled for "
		       "getting datetime from row <current>, column <%d>",
				Helper::getWidgetType(*(*m_it)),
				m_pPt->getPropertyDescriptionList().getName( getIndex(currentColumn) ).c_str(),
				currentColumn);
	throw GuiControlNotSupportedException( ExceptInfo( fun, msg, __FILE__, __LINE__) );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return DateTime      value of the asked column and current row in the current ctrl.
	\param	currentColumn column to get datetime value from
	\throw	GuiControlNotSupportedException */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
Date ControlBehavour::getDate(Int32 currentColumn) const
{
	FUNCTION_NAME( basar.gui.tie.ControlBehavour.getDate() )
	basar::VarString msg;
	msg.format("occurred type <%d> of widget <%s> is not handled for "
		       "getting datetime from row <current>, column <%d>",
				Helper::getWidgetType(*(*m_it)),
				m_pPt->getPropertyDescriptionList().getName( getIndex(currentColumn) ).c_str(),
				currentColumn);
	throw GuiControlNotSupportedException( ExceptInfo( fun, msg, __FILE__, __LINE__) );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return DateTime      value of the asked column and current row in the current ctrl.
	\param	currentColumn column to get datetime value from
	\throw	GuiControlNotSupportedException */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
Time ControlBehavour::getTime(Int32 currentColumn) const
{
	FUNCTION_NAME( basar.gui.tie.ControlBehavour.getTime() )
	basar::VarString msg;
	msg.format("occurred type <%d> of widget <%s> is not handled for "
		       "getting datetime from row <current>, column <%d>",
				Helper::getWidgetType(*(*m_it)),
				m_pPt->getPropertyDescriptionList().getName( getIndex(currentColumn) ).c_str(),
				currentColumn);
	throw GuiControlNotSupportedException( ExceptInfo( fun, msg, __FILE__, __LINE__) );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return	ColumnSize representing the position (0 indexed) of the ctrl in the list of all ctrls.
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
ColumnSize ControlBehavour::getIndex(Int32) const
{
	return m_pPt->getPropertyDescriptionList().getIndex( Helper::getWidgetName(*(*m_it)));  
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\retval true if a value is found for given row / column
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ControlBehavour::isValueExist( Int32, Int32 ) const
{
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return current widget name
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
VarString ControlBehavour::getObjectName() const
{
	return Helper::getWidgetName(*(*m_it));
}

///////////////////////////////////////////////////////////////////////////////////////////
//! \throw  GuiControlNotSupportedException
///////////////////////////////////////////////////////////////////////////////////////////
void ControlBehavour::clear()
{
	FUNCTION_NAME( basar.gui.tie.ControlBehavour.clear )
	basar::VarString msg;
	msg.format("occurred type <%d> is not handled!", Helper::getWidgetType(*(*m_it)));
	throw GuiControlNotSupportedException( ExceptInfo( fun, msg, __FILE__, __LINE__) );
}
///////////////////////////////////////////////////////////////////////////////////////////
//! \throw	GuiControlNotSupportedException
///////////////////////////////////////////////////////////////////////////////////////////
void ControlBehavour::erase( Int32 )
{
	FUNCTION_NAME( basar.gui.tie.ControlBehavour.erase(GuiPropertyTable_YIterator) )
	basar::VarString msg;
	msg.format("occurred type <%d> is not handled!", Helper::getWidgetType(*(*m_it)));
	throw GuiControlNotSupportedException( ExceptInfo( fun, msg, __FILE__, __LINE__) );
}
///////////////////////////////////////////////////////////////////////////////////////////
/*! \return Int32:	number of hits that the ctrl with the most hits (shown rows/items) has.
	\throw	GuiControlNotSupportedException */
///////////////////////////////////////////////////////////////////////////////////////////
Int32 ControlBehavour::maxCountOfControls() const
{
	FUNCTION_NAME( basar.gui.tie.ControlBehavour.maxCountOfControls )
	basar::VarString msg;
	msg.format("occurred type <%d> is not handled!", Helper::getWidgetType(*(*m_it)));
	throw GuiControlNotSupportedException( ExceptInfo( fun, msg, __FILE__, __LINE__) );

}
///////////////////////////////////////////////////////////////////////////////////////////
/*!	\return represents the absolute (regardless of the page shown), 0 based index 
			of selected row or item of a multi line ctrl, negative value is error. 
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////
Int32 ControlBehavour::getCurrentIndex()
{
	return -1;
}

///////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////
void ControlBehavour::getSelectedIndices( std::vector<Int32>& )
{
}

//----------------------------------------------------------------------------
/*!	\throw no-throw
	\param	pObject   object the event belongs to.
	\param	pEvent    event to throw.
	\retval true      if the parent object can work with this event
	\return otherwise returns false */
bool ControlBehavour::eventFilter(QObject * pObject, QEvent * pEvent)
{
	bool retval = false;

	LIBBASARGUITIE_EXCEPTIONHANDLERMACRO_BEGIN
	{
		assert(0 != pObject);
		assert(0 != pEvent );

		if (m_pPt->m_EventHandler->isGlobalEventFilter())
		{
            if (m_pPt->m_EventHandler->eventFilterGlobal(pObject, pEvent))			// global filter
            {
                return true;
            }
            if (m_pPt->m_EventHandler->isPolicyActive(getWidgetType()))
            {
                if (eventFilterCtr(pObject, pEvent))
                {
                    return true;
                }
            }
		}

		retval = m_pPt->m_EventHandler->eventFilterLK(pObject, pEvent, getWidgetType());
	}

	LIBBASARGUITIE_EXCEPTIONHANDLERMACRO_END("basar.gui.tie.ControlBehavour::eventFilter")

    return retval;
}
//----------------------------------------------------------------------------
/*!	\throw no-throw
	\retval true      if the parent object can work with this event
	\return otherwise returns false */
bool ControlBehavour::eventFilterCtr(QObject *, QEvent *)
{
	return false;
}
//----------------------------------------------------------------------------
/*!	\throw	no-throw
	\param	pMatcher matcher the event handler belongs to. */
void ControlBehavour::setMatcher(boost::shared_ptr<Matcher> pMatcher)
{
	m_pMatcher = pMatcher;
	
    BLOG_TRACE(LoggerPool::eventHandler(), "matcher set to event handler.");
}
//----------------------------------------------------------------------------
/*!	\throw	no-throw */
void ControlBehavour::removeMatcher()
{
	m_pMatcher.reset();
}

void ControlBehavour::notifyMatchDone()
{
    //empty base implementation
}

void tie::ControlBehavour::setSelf(boost::shared_ptr<ControlBehavour> self)
{
    m_Self = ControlBehaviourWeakPtr(self);
}

///////////////////////////////////////////////////////////////////////////////////////////
//! \throw no-throw
//! \return type of given QObject
///////////////////////////////////////////////////////////////////////////////////////////
WidgetTypes	ControlBehavour::getWidgetType() const
{
	return qUnknown;
}

void ControlBehavour::registerForNotification()
{
    //empty base implementation
}

//---------------------------------------------------------------------------------------//
}// tie
} // gui
} // basar
