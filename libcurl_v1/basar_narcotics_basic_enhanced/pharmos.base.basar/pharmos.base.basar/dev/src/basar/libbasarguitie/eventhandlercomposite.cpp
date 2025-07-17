//----------------------------------------------------------------------------
/*! \file
 *  \brief  all global event handlings for a typ.
 *  \author Roland Kiefert
 *  \date   20.09.2005
 */
//----------------------------------------------------------------------------


#include "eventhandlercomposite.h"
#include "loggerpool.h"
#include "libbasarcmnutil_bstring.h"
#include "libbasarcmnutil_exceptions.h"
#include "libbasarcmnutil_logging.h"

#pragma warning (push)
#pragma warning (disable: 4512)
#include <QtCore/QObject>
#pragma warning (pop)

//----------------------------------------------------------------------------
namespace basar {
namespace gui {
namespace tie {

//----------------------------------------------------------------------------
/*!	\throw no-throw */
EventHandlerComposite::EventHandlerComposite() : m_Policy ( LABEL_ON			| 
														    LINEEDIT_ON			| 
															TEXTEDIT_ON			|
															COMBOBOX_ON			| 
															PUSHBUTTON_ON		|
															LISTWIDGET_ON		| 
															TABLEWIDGET_ON		|
															BASARTABLEWIDGET_ON |
															MULTICOMBOBOX_ON    |
															LOCAL_ON			|
															GLOBAL_ON			
														  )
{
}

//----------------------------------------------------------------------------
/*!	\throw	no-throw
	\param	policy policy for the global event handler. */
void EventHandlerComposite::setEventPolicy(EventPolicyType policy)
{
	m_Policy = policy;
	
	if (LoggerPool::eventHandler().isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
	{
		basar::VarString msg;
		msg.format("event policy set to <%d>.", m_Policy);
		LoggerPool::eventHandler().forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}
}

//----------------------------------------------------------------------------
/*!	\throw InvalidParameterException
	\param	rEventHandler event handler which will be set.
	\param	wtype         widget type the event handler works for.
	\return event handler's ID */
IdEventHandler EventHandlerComposite::setEventHandler(boost::shared_ptr<QObject> rEventHandler, 
													  WidgetTypes                wtype /* = qUnset */)
{
	if ( (wtype < qFirstWidget) || (wtype > qUnset) )
	{
		VarString msg;
		msg.format("invalid wtype == %d", wtype);
		ExceptInfo sInfo("basar.gui.tie.EventHandler.setEventHandler", msg, __FILE__, __LINE__);
		throw InvalidParameterException(sInfo); 
	}

	IdEventHandler id;
	id.type          = wtype;
	id.pEventHandler = rEventHandler;

	m_CollEvHndl[wtype].push_back(id);

	BLOG_TRACE(LoggerPool::eventHandler(), "event handler stored in event handler collection.");

	return id;
}

//----------------------------------------------------------------------------
/*!	\throw InvalidParameterException
 *	\param id event handler's ID */
void EventHandlerComposite::deleteEventHandler(IdEventHandler id)
{
	if ( (id.type < qFirstWidget) || (id.type > qUnset) )
	{
		VarString msg;
		msg.format("invalid id.type == %d", id.type);
		ExceptInfo sInfo("basar.gui.tie.EventHandler.deleteEventHandler", msg, __FILE__, __LINE__);
		throw InvalidParameterException(sInfo); 
	}

	CollEventHandler               * pColl = &(m_CollEvHndl[id.type]);
	CollEventHandler::iterator       itCur = pColl->begin();
	CollEventHandler::const_iterator itEnd = pColl->end  ();

	for (; itCur != itEnd; ++itCur)
	{
		if ((*itCur).pEventHandler == id.pEventHandler)
		{
/*! \ingroup DIFF_DBG_REL */ 
#ifdef NDEBUG
			pColl->erase(itCur);
#else
			// debug purpose: check if multiple collection items of handler exist
			CollEventHandler::const_iterator itRemain = pColl->erase(itCur);

			for (; itRemain != itEnd; ++itCur)
			{
				assert((*itRemain).pEventHandler != id.pEventHandler);
			}
#endif
			break;
		}
	}

	BLOG_TRACE(LoggerPool::eventHandler(), "event handler erased from event handler collection.");
}

//----------------------------------------------------------------------------
/*!	\throw no-throw
	\retval true if the global filter is active
	\return otherwise returns false */
bool EventHandlerComposite::isGlobalEventFilter() 
{
	return (GLOBAL_ON == (m_Policy & GLOBAL_ON));
}
//----------------------------------------------------------------------------
/*!	\throw no-throw
	\param	pObject object the event belongs to.
	\param	pEvent event to throw.
	\param	wtype         widget type the event handler works for.
	\retval true if the parent object can work with this event
	\return otherwise returns false */
bool EventHandlerComposite::eventFilterLK(QObject * pObject, QEvent * pEvent, WidgetTypes wtype)
{
	if (LOCAL_ON == (m_Policy & LOCAL_ON))
	{
		BLOG_TRACE(LoggerPool::eventHandler(), "local event handling.");

		// --- qUnset
		CollEventHandler               * pColl = &(m_CollEvHndl[qUnset]);
		CollEventHandler::const_iterator itCur = pColl->begin(); 
		CollEventHandler::const_iterator itEnd = pColl->end  ();

		for (; itCur != itEnd; ++itCur)
		{
			if ((*itCur).pEventHandler->eventFilter(pObject, pEvent))
				return true;
		}


		// --- special local event filter 

		// now: itCur == m_CollEvHndlUnset.end  ();
		//      itEnd == m_CollEvHndlUnset.end  ();
		//      itCur == itEnd --> no for loop is executed

		if ( isPolicyActive( wtype ) )
		{
			pColl = &(m_CollEvHndl[wtype]);
			itCur = pColl->begin(); 
			itEnd = pColl->end  (); 

			for (; itCur != itEnd; ++itCur)
			{
				if ((*itCur).pEventHandler->eventFilter(pObject, pEvent))
					return true;
			}
		}	// if policy bit set
	}	// if local

	return false;
}

//----------------------------------------------------------------------------
/*!	\throw no-throw
	\param	pObject object the event belongs to.
	\param	pEvent event to throw.
	\retval true if the parent object can work with this event
	\return otherwise returns false */
bool EventHandlerComposite::eventFilterGlobal(QObject *pObject, QEvent * pEvent)
{
	pObject = pObject;
	pEvent  = pEvent;

	//if ( pEvent->type() == QEvent::KeyPress )
	{
	}
	return false;
}

//----------------------------------------------------------------------------
//! \throw no-throw
//! \retval true if policy bit is set for given widget type
//! \return otherwise returns false
bool EventHandlerComposite::isPolicyActive (WidgetTypes  type) const
{
	static const EventPolicyType POLICYBIT = 0x1;

	if ( (type < qFirstWidget) || (type > qLastWidget) )	
		return false;

	// means e.g.:		
	//        (m_Policy          & LABEL_ON ) == LABEL_ON
	return ( ((m_Policy >> type) & POLICYBIT) == POLICYBIT );
}

//----------------------------------------------------------------------------
} // tie
} // gui
} // basar
