//----------------------------------------------------------------------------
/*! \file
 *  \brief  class implementation of x-iterator to guipropertytable
 *  \author Roland Kiefert
 *  \date   24.08.2005
 */
//----------------------------------------------------------------------------
//---------------------------------------------------------------------------------------//
// includes
//---------------------------------------------------------------------------------------//

#include "libbasarguitie_propertytable.h"

#include "../macro.h"
#include "guipropertytable.h"
#include "controlbehavour.h"
#include "loggerpool.h"
#include "libbasarproperty_propertytype.h"

//---------------------------------------------------------------------------------------//
// namespace
//---------------------------------------------------------------------------------------//
namespace basar {
namespace gui {
namespace tie {

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
GuiPropertyTable_XIterator::GuiPropertyTable_XIterator () : m_it           (), 
															m_CurrentRow   ( -1),  
															m_CurrentColumn( -1)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\param pt pointer to GuiPropertyTable
	\param it iterator to QWidget-list
	\param currentRow number of current row */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
GuiPropertyTable_XIterator::GuiPropertyTable_XIterator ( boost::shared_ptr<GuiPropertyTable>                         pt,
													     std::list< boost::shared_ptr< ControlBehavour > >::iterator it, 
													     Int32                                                       currentRow )
													   : m_Pt           (pt), 
													     m_it           (it), 
														 m_CurrentRow   (currentRow),  
														 m_CurrentColumn(0)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\retval	true if equal
	\return otherwise returns false
	\param  rhs xiterator to check for equality
	\throw	DifferentContainersIteratorException */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
bool GuiPropertyTable_XIterator::operator == ( const GuiPropertyTable_XIterator & rhs ) const
{
	ConstString fun = "basar.gui.tie.GuiPropertyTable_XIterator.operator==()";	

	checkValid();

	// comparison: same container and same iterator
	if( m_Pt != rhs.m_Pt ){
		throw DifferentContainersIteratorException(ExceptInfo(  fun, 
																"Iterator of different container.", 
																__FILE__, __LINE__  ));		  
	}
	
	return ( rhs.m_CurrentRow    == m_CurrentRow    ) && 
		   ( rhs.m_CurrentColumn == m_CurrentColumn ) ;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\retval true if not equal 
	\return otherwise returns false
	\param  rhs xiterator that is checked for unequality
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
bool GuiPropertyTable_XIterator::operator != ( const GuiPropertyTable_XIterator & rhs ) const
{
	return !( operator == (rhs) );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return	GuiPropertyTable_XIterator switched to next one (prae) 
	\throw	OutOfRangeIteratorException */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
GuiPropertyTable_XIterator GuiPropertyTable_XIterator::operator++()
{
	FUNCTION_NAME( basar.gui.tie.GuiPropertyTable_XIterator::operator++ )

	checkValid();

	// check if the iterator stands at the end
	if( m_CurrentRow == ITERATOR_ENDPOS || m_it == m_Pt->m_Ctrls.end() )
	{
        VarString msg;
		msg.format( "Current position of iterator is end() (property: '%s')!", getName().c_str() ); 
		throw OutOfRangeIteratorException( ExceptInfo( fun, msg, __FILE__, __LINE__) );
    }
	
	if ((*m_it)->isSwitchNextAllowed(m_CurrentColumn)) 
		m_it++;

	if (m_it == m_Pt->m_Ctrls.end())
		m_CurrentRow = ITERATOR_ENDPOS;

	return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return GuiPropertyTable_XIterator switched to next one (post) 
	\param	i only for postfix
	\throw	OutOfRangeIteratorException */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
GuiPropertyTable_XIterator GuiPropertyTable_XIterator::operator++( int )
{
	FUNCTION_NAME( basar.gui.tie.GuiPropertyTable_XIterator.operator++(int) )

	checkValid();

	// check if the iterator stands at the end
	if( m_CurrentRow == ITERATOR_ENDPOS || m_it == m_Pt->m_Ctrls.end() ) {
        VarString msg;
		msg.format( "Current position of iterator is end() (property: '%s')!", getName().c_str() ); 
		throw OutOfRangeIteratorException( ExceptInfo( fun, msg, __FILE__, __LINE__) );
	}

	GuiPropertyTable_XIterator xit( *this );

	if ((*m_it)->isSwitchNextAllowed(m_CurrentColumn)) 
		m_it++;

	if (m_it == m_Pt->m_Ctrls.end())
		m_CurrentRow = ITERATOR_ENDPOS;

	return xit;
}
//------------------------------------------------------------------------------------------------------//
// properties methods
//------------------------------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	value string-value to set to column
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void GuiPropertyTable_XIterator::setString( const I18nString & value )
{
	//! \todo check local before writing in gui
	FUNCTION_NAME( basar.gui.tie.GuiPropertyTable_XIterator.setString(I18nString&) )
	checkValid();

	(*m_it)->setString(value, m_CurrentRow, m_CurrentColumn);

	if (LoggerPool::guiPropTab().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("string <%s> set to gui property <%s>.",
					value.c_str(), 
					getName().c_str());
		LoggerPool::guiPropTab().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	value string-value to set to column
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void GuiPropertyTable_XIterator::setCurrentString( const I18nString& value )
{
	//RK
	//! \todo check local before writing in gui
	FUNCTION_NAME( basar.gui.tie.GuiPropertyTable_XIterator.setCurrentString(I18nString&) )
	checkValid();

	(*m_it)->setCurrentString(value, m_CurrentColumn);

	if (LoggerPool::guiPropTab().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("current string <%s> set to gui property <%s>.",
					value.c_str(), 
					getName().c_str());
		LoggerPool::guiPropTab().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}

//------------------------------------------------------------------------------------------------------//
// properties methods
//------------------------------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	value DateTime-value to set to column
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void GuiPropertyTable_XIterator::setDateTime( const DateTime& value )
{
	FUNCTION_NAME( basar.gui.tie.GuiPropertyTable_XIterator.setDateTime(DateTime&) )
	checkValid();

	(*m_it)->setDateTime(value, m_CurrentColumn);

	if (LoggerPool::guiPropTab().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("datetime <%s, %s> set to gui property <%s>.",
					value.toStrDate().c_str(), 
					value.toStrTime().c_str(), 
					getName().c_str());
		LoggerPool::guiPropTab().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	value DateTime-value to set to column
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void GuiPropertyTable_XIterator::setTime( const Time& value )
{
	FUNCTION_NAME( basar.gui.tie.GuiPropertyTable_XIterator.setTime(Time&) )
	checkValid();

	(*m_it)->setTime(value, m_CurrentColumn);

	if (LoggerPool::guiPropTab().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("time <%s> set to gui property <%s>.",
					value.toStrTime().c_str(), 
					getName().c_str());
		LoggerPool::guiPropTab().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	value DateTime-value to set to column
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void GuiPropertyTable_XIterator::setDate( const Date& value )
{
	FUNCTION_NAME( basar.gui.tie.GuiPropertyTable_XIterator.setDate(Date&) )
	checkValid();

	(*m_it)->setDate(value, m_CurrentColumn);

	if (LoggerPool::guiPropTab().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("date <%s> set to gui property <%s>.",
					value.toStrDate().c_str(),  
					getName().c_str());
		LoggerPool::guiPropTab().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return I18nString value of the current line in the current ctrl.
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
I18nString GuiPropertyTable_XIterator::getString() const
{
	//! \todo check local before writing from gui
	FUNCTION_NAME( basar.gui.tie.GuiPropertyTable_XIterator.getString() )
	checkValid();
	
	I18nString retVal = (*m_it)->getString(m_CurrentRow, m_CurrentColumn);

	if (LoggerPool::guiPropTab().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("string <%s> read from gui property <%s>.",
					retVal.c_str(), 
					getName().c_str());
		LoggerPool::guiPropTab().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return retVal;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return I18nString value of the current line in the current ctrl.
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
I18nString GuiPropertyTable_XIterator::getCurrentString() const
{
	//! \todo check local before writing from gui
	FUNCTION_NAME( basar.gui.tie.GuiPropertyTable_XIterator.getCurrentString() )
	checkValid();
	
	I18nString retVal = (*m_it)->getCurrentString(m_CurrentColumn);

	if (LoggerPool::guiPropTab().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("current string <%s> read from gui property <%s>.",
					retVal.c_str(), 
					getName().c_str());
		LoggerPool::guiPropTab().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return retVal;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return DateTime value of the current line in the current ctrl.
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
DateTime GuiPropertyTable_XIterator::getDateTime() const
{
	FUNCTION_NAME( basar.gui.tie.GuiPropertyTable_XIterator.getDateTime() )
	checkValid();
	
	DateTime retVal = (*m_it)->getDateTime(m_CurrentColumn);

	if (LoggerPool::guiPropTab().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("datetime <%s, %s> read from gui property <%s>.",
					retVal.toStrDate().c_str(), 
					retVal.toStrTime().c_str(), 
					getName().c_str());
		LoggerPool::guiPropTab().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return retVal;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return DateTime value of the current line in the current ctrl.
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
Time GuiPropertyTable_XIterator::getTime() const
{
	FUNCTION_NAME( basar.gui.tie.GuiPropertyTable_XIterator.getTime() )
	checkValid();
	
	Time retVal = (*m_it)->getTime(m_CurrentColumn);

	if (LoggerPool::guiPropTab().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("time <%s> read from gui property <%s>.",
					retVal.toStrTime().c_str(), 
					getName().c_str());
		LoggerPool::guiPropTab().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return retVal;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return DateTime value of the current line in the current ctrl.
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
Date GuiPropertyTable_XIterator::getDate() const
{
	FUNCTION_NAME( basar.gui.tie.GuiPropertyTable_XIterator.getDate() )
	checkValid();
	
	Date retVal = (*m_it)->getDate(m_CurrentColumn);

	if (LoggerPool::guiPropTab().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("date <%s> read from gui property <%s>.",
					retVal.toStrDate().c_str(), 
					getName().c_str());
		LoggerPool::guiPropTab().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return retVal;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return	ColumnSize representing the position (0 indexed) of the ctrl in the list of all ctrls.
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
ColumnSize GuiPropertyTable_XIterator::getIndex() const
{
	checkValid();
	return (*m_it)->getIndex(m_CurrentColumn);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\retval true if a value is found
	\return otherwise returns false
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
bool GuiPropertyTable_XIterator::isValueExist() const
{
	checkValid();
	return (*m_it)->isValueExist(m_CurrentRow, m_CurrentColumn);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return ColumnName representing the name of the current ctrl
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
ColumnName GuiPropertyTable_XIterator::getName() const
{
	checkValid();
	//! \todo don't know if it is advisable to go via the propertydescriptionlist?
	return m_Pt->getPropertyDescriptionList().getName( getIndex() );  
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\return property::PropertyType representing the type of the current ctrl.
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
property::PropertyType GuiPropertyTable_XIterator::getType() const
{
	checkValid();
	return m_Pt->getPropertyDescriptionList().getType( (*m_it)->getObjectName() );  
}

//------------------------------------------------------------------------------------------------------//
// initialization check of a object of this class. Important in the case of dflt construction.
//------------------------------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw InvalidIteratorException */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void GuiPropertyTable_XIterator::checkValid() const
{
	FUNCTION_NAME( basar.gui.tie.GuiPropertyTable_XIterator.checkValid )	

	if ( m_Pt.get() == 0 ){
		throw InvalidIteratorException(ExceptInfo ( fun, 
													"pointer to GuiPropertyTable isn't valid.", 
													__FILE__, __LINE__) );
	}
}

//------------------------------------------------------------------------------------------------------//
//! \throw no-throw
//! \return bool indicating if GuiPropertyTable is null
bool GuiPropertyTable_XIterator::isNull() const
{
	return ( m_Pt.get() == 0 );
}

//------------------------------------------------------------------------------------------------------//
//! \throw no-throw
//!
void GuiPropertyTable_XIterator::reset()
{
	m_Pt.reset();
	m_CurrentRow    = -1;
	m_CurrentColumn = -1;
}

//------------------------------------------------------------------------------------------------------//
//! \throw no-throw
//! \return bool indication if Iterator is pointing at begin of GuiPropertyTable
bool GuiPropertyTable_XIterator::isBegin() const
{
	checkValid();
	return (0 == m_CurrentRow);
}

//------------------------------------------------------------------------------------------------------//
//! \throw no-throw
//! \return bool indication if Iterator is pointing at end of GuiPropertyTable
bool GuiPropertyTable_XIterator::isEnd() const
{
	checkValid();
	return (m_CurrentRow == ITERATOR_ENDPOS);
}

void GuiPropertyTable_XIterator::notifyMatchDone()
{
    checkValid();

    (*m_it)->notifyMatchDone();

}

//------------------------------------------------------------------------------------------------------//
} // tie
} // gui
} // basar
