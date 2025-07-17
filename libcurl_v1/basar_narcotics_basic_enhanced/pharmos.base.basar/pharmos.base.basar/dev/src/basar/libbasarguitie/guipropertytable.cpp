//----------------------------------------------------------------------------
/*! \file
 *  \brief  class implementation of GuiPropertyTable
 *  \author Roland Kiefert
 *  \date   21.09.2005
 */
//----------------------------------------------------------------------------

//---------------------------------------------------------------------------------------//
// includes
//---------------------------------------------------------------------------------------//

#include "guipropertytable.h"

#pragma warning (push)
#pragma warning (disable: 4481 4512)
#include <QtWidgets/QWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QHeaderView>
#pragma warning(pop)


#include "libbasarguitie_exceptions.h"
#include "helper.h"
#include "loggerpool.h"
#include "../libbasarproperty/propertydescriptionlist.h"
#include "libbasarqtwidget_multicombobox.h"
#include "eventhandlercomposite.h"
#include "controlbehavour.h"
#include "../macro.h"

//---------------------------------------------------------------------------------------//
// namespaces
//---------------------------------------------------------------------------------------//
namespace basar {
namespace gui {
namespace tie {

//---------------------------------------------------------------------------------------//
// STATIC SECTION
//---------------------------------------------------------------------------------------//
//! \return	shared pointer to the implementation object of the class GuiPropertyTable
//! \param  qWidget Qt-container for the gui property table
//! \param  eh event handler for the GuiPropertyTable
//! \param  eWidgetPolicy policy that determines which kind of controls will serve as properties
//! \throw  no-throw
///////////////////////////////////////////////////////////////////////////////////////////
GuiPropertyTable::GuiPropertyTableSharedPtr GuiPropertyTable::create( const QWidget         * qWidget, 
																	  EventHandlerComposite * eh, 
																	  const WidgetPolicyEnum  eWidgetPolicy )
{
	// create implementation object from heap
	GuiPropertyTableSharedPtr pShared( new GuiPropertyTable(qWidget, eh, eWidgetPolicy) );	
	pShared->setWeakPtr( pShared );	// save in weak pointer 

	return pShared;
}
//---------------------------------------------------------------------------------------//
// IMPLEMENTATION
//---------------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////////////
//! \param  pWidget Qt-container for the gui property table
//! \param  eh event handler for the gui property table
//! \param  eWidgetPolicy policy that determines which kind of controls will serve as properties
//! \throw  no-throw
///////////////////////////////////////////////////////////////////////////////////////////
GuiPropertyTable::GuiPropertyTable( const QWidget         * pWidget       , 
								    EventHandlerComposite * eh            , 
								    const WidgetPolicyEnum  eWidgetPolicy )
								  : m_MemoryPolicy       (WITHOUT_MEMORY), 
								    m_WidgetPolicy       (eWidgetPolicy), 
									m_AbsoluteIndex      (0), 
									m_EventHandler       (eh), 
									m_MultiLineCtrlsExist(false)
{
    if (LoggerPool::guiPropTab().isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
	{
		VarString msg;
        if (pWidget != 0)
			msg.format("GuiPropertyTable::GuiPropertyTable for widgetName = <%s>", 
			           pWidget->objectName().toStdString().c_str()   );
        else
			msg.format("GuiPropertyTable::GuiPropertyTable for 0-Pointer widget");
        LoggerPool::guiPropTab().forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	// creates the propertydescriptionlist implementation object 
	m_PdlRef = basar::property::PropertyDescriptionList::create();

    if (0 != pWidget)
    { 
		/*! \todo	check the pointer 'pWidget' and the method 'findChildren()' on 0 (NULL).
					NULL -> exception; Allowed ctrls-check */
		// read in all ctrls of the passed Widget pointer 'pWidget'
		m_AllCtrls = pWidget->findChildren<QWidget*>().toStdList();

		// also insert widget itself (not in children-list), so that a frame is no must!
		m_AllCtrls.push_front(const_cast<QWidget*>(pWidget));
    }

	WidgetTypes                   wtype;
	bool                          mustKill;
	bool                          installFilter;
	std::list<QWidget*>::iterator itBeforeKilledOne; //for later use

	std::list<QWidget*>::iterator it = m_AllCtrls.begin();

	std::list<QWidget*>::const_iterator itAllCtrlsEnd = m_AllCtrls.end();

	while(it != itAllCtrlsEnd)
    {
		wtype         = Helper::getWidgetType(*(*it));
		mustKill      = false;
		installFilter = false;

		switch (wtype)
		{
		case qLabel:
			if ((m_WidgetPolicy & WITHOUT_LABEL) == WITHOUT_LABEL )
				mustKill = true;
			break;

		case qLineEdit:
			if ((m_WidgetPolicy & WITHOUT_LINEEDIT) == WITHOUT_LINEEDIT )
				mustKill = true;
			break;

		case qTextEdit:
			if ((m_WidgetPolicy & WITHOUT_TEXTEDIT) == WITHOUT_TEXTEDIT )
				mustKill = true;
			break;

		case qPushButton:
			if ((m_WidgetPolicy & WITHOUT_PUSHBUTTON) == WITHOUT_PUSHBUTTON )
				mustKill = true;
			break;

		case qListWidget:
			if ((m_WidgetPolicy & WITHOUT_LISTWIDGET) == WITHOUT_LISTWIDGET )
				mustKill = true;
			else
				m_MultiLineCtrlsExist = true;
			break;

		case qComboBox:
			if ((m_WidgetPolicy & WITHOUT_COMBOBOX) == WITHOUT_COMBOBOX )
				mustKill = true;
			else
				m_MultiLineCtrlsExist = true;
			break;

		case multiComboBox:
			if ((m_WidgetPolicy & WITHOUT_MULTICOMBOBOX) == WITHOUT_MULTICOMBOBOX )
				mustKill = true;
			else
				m_MultiLineCtrlsExist = true;
			break;

		case qTableWidget:
			if ((m_WidgetPolicy & WITHOUT_TABLEWIDGET) == WITHOUT_TABLEWIDGET )
				mustKill = true;
			else
				m_MultiLineCtrlsExist = true;
			break;

		case basarTableWidget:
			if ((m_WidgetPolicy & WITHOUT_BASARTABLEWIDGET) == WITHOUT_BASARTABLEWIDGET )
				mustKill = true;
			else
				m_MultiLineCtrlsExist = true;
			break;

		case qCheckBox:
			if ((m_WidgetPolicy & WITHOUT_CHECKBOX) == WITHOUT_CHECKBOX )
				mustKill = true;
			//! \todo if filter is installed, an assertion occurs at db::sql::managerimpl::shutdown(), because there
			//			is still a shared ptr to a connection although there is none open
			//installFilter = true;
			break;

		case qRadioButton:
			if ((m_WidgetPolicy & WITHOUT_RADIOBUTTON) == WITHOUT_RADIOBUTTON )
				mustKill = true;
			//! \todo if filter is installed, an assertion occurs at db::sql::managerimpl::shutdown(), because there
			//			is still a shared ptr to a connection although there is none open
			//installFilter = true;
			break;

		case qGroupBox:
			if ((m_WidgetPolicy & WITHOUT_GROUPBOX) == WITHOUT_GROUPBOX )
				mustKill = true;
			//! \todo if filter is installed, an assertion occurs at db::sql::managerimpl::shutdown(), because there
			//			is still a shared ptr to a connection although there is none open
			//installFilter = true;
			break;

		case qDateTimeEdit:
			if ((m_WidgetPolicy & WITHOUT_DATETIMEEDIT) == WITHOUT_DATETIMEEDIT )
				mustKill = true;
			//! \todo if filter is installed, an assertion occurs at db::sql::managerimpl::shutdown(), because there
			//			is still a shared ptr to a connection although there is none open
			//installFilter = true;
			break;

		case qTimeEdit:
			if ((m_WidgetPolicy & WITHOUT_TIMEEDIT) == WITHOUT_TIMEEDIT )
				mustKill = true;
			//! \todo if filter is installed, an assertion occurs at db::sql::managerimpl::shutdown(), because there
			//			is still a shared ptr to a connection although there is none open
			//installFilter = true;
			break;

		case qDateEdit:
			if ((m_WidgetPolicy & WITHOUT_DATEEDIT) == WITHOUT_DATEEDIT )
				mustKill = true;
			//! \todo if filter is installed, an assertion occurs at db::sql::managerimpl::shutdown(), because there
			//			is still a shared ptr to a connection although there is none open
			//installFilter = true;
			break;

		case basarDateTimeEdit:
			if ((m_WidgetPolicy & WITHOUT_BASARDATETIMEEDIT) == WITHOUT_BASARDATETIMEEDIT )
				mustKill = true;
			//! \todo if filter is installed, an assertion occurs at db::sql::managerimpl::shutdown(), because there
			//			is still a shared ptr to a connection although there is none open
			//installFilter = true;
			break;

		case basarTimeEdit:
			if ((m_WidgetPolicy & WITHOUT_BASARTIMEEDIT) == WITHOUT_BASARTIMEEDIT )
				mustKill = true;
			//! \todo if filter is installed, an assertion occurs at db::sql::managerimpl::shutdown(), because there
			//			is still a shared ptr to a connection although there is none open
			//installFilter = true;
			break;

		case basarDateEdit:
			if ((m_WidgetPolicy & WITHOUT_BASARDATEEDIT) == WITHOUT_BASARDATEEDIT )
				mustKill = true;
			//! \todo if filter is installed, an assertion occurs at db::sql::managerimpl::shutdown(), because there
			//			is still a shared ptr to a connection although there is none open
			//installFilter = true;
			break;

		default:
			mustKill = true;
		}	// end switch

		if (mustKill)
		{
			itBeforeKilledOne = m_AllCtrls.erase( it );
			
			if (m_AllCtrls.empty())
			{
				break;
			}

			it = itBeforeKilledOne;

			continue; //use next element in loop
		}

		// only one Container
		ControlBehavourSharedPtr controlbehavoursharedptr;

		//	every column of a multi columns widgets has to be treated 
		//	as it would be a single gui ctrl. 
		//	So it (the column) will be put separately to the PropertyTableDescriptionList.
		// RK doppelte Name nicht möglich

		bool exitLoop = false;

		wtype         = Helper::getWidgetType(*(*it));
		mustKill      = false;
		installFilter = false;

		switch (wtype)
		{
		case qTableWidget:
		case basarTableWidget:
			{
				controlbehavoursharedptr = Helper::getControlBehavour(*(*it),this,it);
				m_Ctrls.push_back(controlbehavoursharedptr);
				QTableWidget* pQTableWidget = static_cast<QTableWidget*>((*it));
				for( Int32 i = 0; i < pQTableWidget->columnCount(); ++i )
				{
                    QTableWidgetItem * item = pQTableWidget->horizontalHeaderItem (i);
                    if (0 != item)
                    {
				    	m_PdlRef.push_back(	item->text().toLocal8Bit().constData(), 
											basar::STRING );	
                    }
				}
				installFilter = true;
			}
			break;

		case multiComboBox:
			{
				controlbehavoursharedptr = Helper::getControlBehavour(*(*it),this,it);
				m_Ctrls.push_back(controlbehavoursharedptr);
				MultiComboBox* pMultiComboBox = static_cast<MultiComboBox*>((*it));
				if (!pMultiComboBox->isHorizontalHeaderHidden())
				{
					for( Int32 i = 0; i < pMultiComboBox->columnCount(); ++i )
					{
						m_PdlRef.push_back( pMultiComboBox->getHorizontalHeaderItem(i).toLocal8Bit().constData(), 
											basar::STRING );			
					}
				}

				installFilter = true;
			}
			break;

		case qTimeEdit:
		case basarTimeEdit:
			m_PdlRef.push_back( Helper::getWidgetName(*(*it)), basar::TIME );
			controlbehavoursharedptr = Helper::getControlBehavour(*(*it), this, it);
			m_Ctrls.push_back(controlbehavoursharedptr);
			break;		

		case qDateTimeEdit:
		case basarDateTimeEdit:
			m_PdlRef.push_back( Helper::getWidgetName(*(*it)), basar::DATETIME );
			controlbehavoursharedptr = Helper::getControlBehavour(*(*it), this, it);
			m_Ctrls.push_back(controlbehavoursharedptr);
			break;

		case qDateEdit: 
		case basarDateEdit:
			m_PdlRef.push_back( Helper::getWidgetName(*(*it)), basar::DATE );
			controlbehavoursharedptr = Helper::getControlBehavour(*(*it), this, it);
			m_Ctrls.push_back(controlbehavoursharedptr);
			break;

		default:
			{	
				VarString name = Helper::getWidgetName(*(*it));

				if (m_PdlRef.contains(name))
				{
					itBeforeKilledOne = m_AllCtrls.erase( it );
			
					if (m_AllCtrls.empty())
					{
						exitLoop = true;
						break;
					}

					it = itBeforeKilledOne;

					continue; //use next element in loop
				}
				else 
				{
					m_PdlRef.push_back( name, basar::STRING );
					controlbehavoursharedptr = Helper::getControlBehavour(*(*it),this,it);
					m_Ctrls.push_back(controlbehavoursharedptr);
				}
			}
		} //end switch(wtype), 2nd one

		if (exitLoop)
		{
			break;
		}

		if (installFilter && eh != 0)
		{
			// eventObject = Helper::getQObject(*(*it),this,it);
			if (controlbehavoursharedptr.get() == 0)
				controlbehavoursharedptr = Helper::getControlBehavour(*(*it),this,it);

			ControlBehavour* eventObject = controlbehavoursharedptr.get();
			(*it)->installEventFilter( eventObject );
			m_eventFilterMapByQObject[*it] = controlbehavoursharedptr;
			m_eventFilterMapByBehaviour[controlbehavoursharedptr] = *it;
		}

        if (controlbehavoursharedptr.get() != 0)
		{
			BLOG_TRACE(LoggerPool::guiPropTab(), 
					   "GuiPropertyTable::GuiPropertyTable added ControlBehavour" );
        }

		if (it != m_AllCtrls.end())
		{
			++it;
		}

	} // end WHILE

    if ( LoggerPool::guiPropTab().isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
    {
        VarString msg;
        msg.format("GuiPropertyTable::GuiPropertyTable has absoluteIndex = <%d> and size = <%d>", 
			       m_AbsoluteIndex, 
				   size() );
        LoggerPool::guiPropTab().forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);

        msg.format("GuiPropertyTable::GuiPropertyTable entries: <%s>", m_PdlRef.toString().c_str() );
        LoggerPool::guiPropTab().forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
    }

}

//----------------------------------------------------------------------------
/*!	\throw	no-throw
	\param	pMatcher matcher the event handler belongs to. */
void GuiPropertyTable::setMatcher(boost::shared_ptr<Matcher> pMatcher)
{
	removeMatcher();
	std::map<boost::shared_ptr< ControlBehavour > , QWidget * >::iterator iter;
	for (iter = m_eventFilterMapByBehaviour.begin(); iter != m_eventFilterMapByBehaviour.end(); ++iter)
	{
        boost::shared_ptr< ControlBehavour > controlbehavoursharedptr = iter->first;
		controlbehavoursharedptr->setMatcher(pMatcher);
		iter->second->installEventFilter(controlbehavoursharedptr.get());
	}
}	

//----------------------------------------------------------------------------
void GuiPropertyTable::removeMatcher()
{
	std::map<boost::shared_ptr< ControlBehavour > , QWidget * >::iterator iter;
	for (iter = m_eventFilterMapByBehaviour.begin(); iter != m_eventFilterMapByBehaviour.end(); ++iter)
	{
        boost::shared_ptr< ControlBehavour > controlbehavoursharedptr = iter->first;
		controlbehavoursharedptr->removeMatcher(/*pMatcher*/);
		iter->second->removeEventFilter(controlbehavoursharedptr.get());
	}
}

void GuiPropertyTable::registerForMatchDoneNotification(boost::weak_ptr<ControlBehavour> behaviour)
{
    auto insertResult = m_BehavioursToNotify.insert(behaviour);
}

void GuiPropertyTable::unregisterForMatchDoneNotification(boost::weak_ptr<ControlBehavour> behaviour)
{
    if (false == m_BehavioursToNotify.empty())
    {
        m_BehavioursToNotify.erase(behaviour);
    }
}
void GuiPropertyTable::notifyMatchDone()
{
    for (auto it = m_BehavioursToNotify.begin(); it != m_BehavioursToNotify.end(); ++it)
    {
        if (false == it->expired())
        {
            it->lock()->notifyMatchDone();
        }
    }
}
///////////////////////////////////////////////////////////////////////////////////////////
//! \throw  no-throw
///////////////////////////////////////////////////////////////////////////////////////////
GuiPropertyTable::~GuiPropertyTable()
{
    m_BehavioursToNotify.clear();

	if( 0 != m_EventHandler ){
		delete m_EventHandler;
		m_EventHandler = 0;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////
//! \throw  GuiControlNotSupportedException
///////////////////////////////////////////////////////////////////////////////////////////
void GuiPropertyTable::clear()
{
	FUNCTION_NAME( basar.gui.tie.GuiPropertyTable.clear )

	std::list<ControlBehavourSharedPtr>::iterator it;
	for( it = m_Ctrls.begin(); it != m_Ctrls.end(); ++it )
	{
		(*it)->clear();
	} // end FOR
}
///////////////////////////////////////////////////////////////////////////////////////////
//! \param  yit points to the line (item/row) which should be erased
//! \throw	NoValidMemoryPolicyException, GuiControlNotSupportedException
///////////////////////////////////////////////////////////////////////////////////////////
void GuiPropertyTable::erase( const GuiPropertyTable_YIterator yit )
{
	FUNCTION_NAME( basar.gui.tie.GuiPropertyTable.erase(GuiPropertyTable_YIterator) )

	//! \todo check with page
	std::list<ControlBehavourSharedPtr>::const_iterator it;

	for( it = m_Ctrls.begin(); it != m_Ctrls.end(); ++it ){		
		if( 0 == yit.m_CurrentRow ){
			if( 1 >= maxCountOfControls() ){
				clear();
				break; // after clearing the complete frame it isn't necessary to check further ctrls.
			}
		}
		else {
			(*it)->erase(yit.m_CurrentRow);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////
/*! \return Int32:	number of hits that the ctrl with the most hits (shown rows/items) has.
	\throw	GuiControlNotSupportedException */
///////////////////////////////////////////////////////////////////////////////////////////
Int32 GuiPropertyTable::maxCountOfControls() const
{
	FUNCTION_NAME( basar.gui.tie.GuiPropertyTable.maxCountOfControls )

	//	iterate over all ctrls to find out the ctrl with the most hits (shown items) in it.
	Int32 count = 0;
	std::list<ControlBehavourSharedPtr>::const_iterator it;
	
	for( it = m_Ctrls.begin(); it != m_Ctrls.end(); ++it )
    {
		if (count < (*it)->maxCountOfControls())
        {
            count = (*it)->maxCountOfControls();
        }
	}
	return count;
}
///////////////////////////////////////////////////////////////////////////////////////////
/*! \return GuiPropertyTable_YIterator pointing to the first row 
			of the GuiPropertyTable
	\throw  no-throw */
///////////////////////////////////////////////////////////////////////////////////////////
GuiPropertyTable_YIterator GuiPropertyTable::begin()  const
{
    GuiPropertyTable_YIterator retval;
    if (empty())
    {
        retval = end();
		BLOG_TRACE(LoggerPool::guiPropTab(), "GuiPropertyTable::begin(): Table is empty !" );
    }
    else
    {
        // first row:
         retval = GuiPropertyTable_YIterator( thisPtr(), 0 );
    }

    return retval;
}
///////////////////////////////////////////////////////////////////////////////////////////
/*! \return GuiPropertyTable_YIterator pointing to iterator beyond 
			last row of the GuiPropertyTable
	\throw  no-throw */
///////////////////////////////////////////////////////////////////////////////////////////
GuiPropertyTable_YIterator GuiPropertyTable::end()  const
{
	return GuiPropertyTable_YIterator( thisPtr(), ITERATOR_ENDPOS );
}
///////////////////////////////////////////////////////////////////////////////////////////    
/*! \return UInt32 indicating all hits which have been shown in the dialog so far
			regardless of the paging mechanism. 
	\throw  GuiControlNotSupportedException */
///////////////////////////////////////////////////////////////////////////////////////////
UInt32 GuiPropertyTable::size() const
{
  UInt32 maxControls = maxCountOfControls();
  UInt32 retval = maxControls += m_AbsoluteIndex; // + 1;
   if ( LoggerPool::guiPropTab().isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
    {
        VarString msg;
        msg.format("GuiPropertyTable::size = <%d> = ((absoluteIndex<%d> ) + maxCountOfControls<%d>) ",
			       retval, 
				   m_AbsoluteIndex,
				   maxControls);
        LoggerPool::guiPropTab().forcedLog(log4cplus::TRACE_LOG_LEVEL, 
			                                   msg, 
											   __FILE__, __LINE__);
    }
  
	return retval;
}
///////////////////////////////////////////////////////////////////////////////////////////
//! \return GuiPropertyTable_YIterator pointing to the new, empty row
//! \throw  no-throw
///////////////////////////////////////////////////////////////////////////////////////////
GuiPropertyTable_YIterator GuiPropertyTable::insert()
{
	if (!(m_MultiLineCtrlsExist))
	{
		m_AbsoluteIndex++;
	}
	return GuiPropertyTable_YIterator( thisPtr(), m_AbsoluteIndex + maxCountOfControls() );
}
///////////////////////////////////////////////////////////////////////////////////////////
/*! \return true if GuiPropertyTable is empty, false otherwise
	\throw  no-throw */
///////////////////////////////////////////////////////////////////////////////////////////
bool GuiPropertyTable::empty() const
{
	return (size() == 0);
}
//---------------------------------------------------------------------------------------//
// PROPERTIES
//---------------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	policy for the global event handler.
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////
void GuiPropertyTable::setEventPolicy( UInt32 policy )
{
	/*! \todo	the valid values of the policy are stored in an enum. 
				But here they are passed as an UInt32. So it would be necessary to check the 
				valid policies! */
	m_EventHandler->setEventPolicy(policy); 
}
///////////////////////////////////////////////////////////////////////////////////////////
/*!	\return event handler's ID
	\param	rEventHandler delegated to the member variable 'm_EventHandler'.
	\param	objectTyp determines for which type the event handler works.
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////
IdEventHandler GuiPropertyTable::setEventHandler( boost::shared_ptr<QObject> rEventHandler, WidgetTypes objectTyp /*= qUnset*/ )
{
	return m_EventHandler->setEventHandler(rEventHandler, objectTyp); 
}
///////////////////////////////////////////////////////////////////////////////////////////
/*!	\param	id event handler's ID
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////
void GuiPropertyTable::deleteEventHandler(IdEventHandler id )
{
	//! \todo perhaps check the index if it is not out of range?
	m_EventHandler->deleteEventHandler( id ); 
}
///////////////////////////////////////////////////////////////////////////////////////////
/*!	\return Int32 represents the absolute (regardless of the page shown), 0 indexed index 
			of the first visible row of a multi line ctrl. E.g. if the multi line ctrl has
			4 visible rows and the first page is shown, than 0 is returned. If the second
			page is presented, then 4 (the fifth row is shown --> 0 indexed --> 4) is returned.
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////
Int32 GuiPropertyTable::getAbsoluteIndex()
{
	return m_AbsoluteIndex;
}
///////////////////////////////////////////////////////////////////////////////////////////
/*!	\return Int32 represents the absolute (regardless of the page shown), 0 indexed index 
			of the selected row or item of a multi line ctrl, negative value is error. 
	\throw	no-throw*/
///////////////////////////////////////////////////////////////////////////////////////////
Int32 GuiPropertyTable::getCurrentIndex()
{
	Int32 index = -1;
	std::list<ControlBehavourSharedPtr>::const_iterator it;
	for( it = m_Ctrls.begin(); it != m_Ctrls.end(); ++it )
	{
		index = (*it)->getCurrentIndex();
		if (index > -1) break;
	}
	
	if (LoggerPool::guiPropTab().isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
	{
		VarString msg;
		msg.format("current index of gui property table is <%d>, absolute index is <%d>.",
					index, index +  m_AbsoluteIndex);
		LoggerPool::guiPropTab().forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}

	return (index + m_AbsoluteIndex);
}
///////////////////////////////////////////////////////////////////////////////////////////
/*!	\return vector of Int32 represents the absolute (regardless of the page shown), 0 indexed index 
			of the selected rows or items of a multi line ctrl. 
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////
std::vector<Int32> GuiPropertyTable::getSelectedIndices()
{
	std::vector<Int32> SelectedVector;
	SelectedVector.reserve(10); //maximale größe
	std::list<ControlBehavourSharedPtr>::const_iterator it;
	for( it = m_Ctrls.begin(); it != m_Ctrls.end(); ++it )
	{
		(*it)->getSelectedIndices(SelectedVector);
	}
	return SelectedVector;
}
///////////////////////////////////////////////////////////////////////////////////////////
//! \return	reference to the implementation of the PropertyDescriptionList
//! \throw  no-throw
///////////////////////////////////////////////////////////////////////////////////////////
basar::property::PropertyDescriptionListRef GuiPropertyTable::getPropertyDescriptionList() const
{
	return m_PdlRef;
}
///////////////////////////////////////////////////////////////////////////////////////////
//! \return (only one group of) GuiPropertyTable shared pointer 
//! \throw  no-throw
///////////////////////////////////////////////////////////////////////////////////////////
GuiPropertyTable::GuiPropertyTableSharedPtr GuiPropertyTable::thisPtr() const
{
	return GuiPropertyTableSharedPtr( m_WeakPtr );
}
///////////////////////////////////////////////////////////////////////////////////////////
//! \param weakPtr is a weak pointer to this. It is assigned to the member 'm_WeakPtr'.
//! \throw  no-throw
///////////////////////////////////////////////////////////////////////////////////////////
void GuiPropertyTable::setWeakPtr( GuiPropertyTableWeakPtr weakPtr )
{
	m_WeakPtr = weakPtr;
}
///////////////////////////////////////////////////////////////////////////////////////////
/*! \param  eMemoryPolicy is passed to be stored in the member 'm_MemoryPolicy'.
			It will affect the paging mechanism.
    \throw	NoValidMemoryPolicyException */
///////////////////////////////////////////////////////////////////////////////////////////
void GuiPropertyTable::setMemoryPolicy( MemoryPolicyEnum eMemoryPolicy )
{
	FUNCTION_NAME( basar.gui.tie.GuiPropertyTable.setMemoryPolicy(MemoryPolicyEnum) )

	if( WITHOUT_MEMORY != eMemoryPolicy && WITH_MEMORY != eMemoryPolicy ){
		basar::VarString msg;
		msg.format("memory policy to set <%d> is not valid.", eMemoryPolicy);
		throw NoValidMemoryPolicyException(	ExceptInfo( fun, msg.c_str(), __FILE__, __LINE__) );					
	}

	m_MemoryPolicy = eMemoryPolicy;
}
///////////////////////////////////////////////////////////////////////////////////////////
/*! \return if any multi line controls exist in the property table 
    \throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////
bool GuiPropertyTable::isMultiLinePropertyTable()
{
	return m_MultiLineCtrlsExist;
}

} // tie
} // gui
} // basar

