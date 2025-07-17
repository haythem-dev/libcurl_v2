//----------------------------------------------------------------------------
/*! \file
 *  \brief  class implementation of system event manager
 *  \author Thomas Hörath
 *  \date   14.08.2006
 */
//----------------------------------------------------------------------------

#include "libbasarappl_systemeventmanager.h"

#include "loggerpool.h"
#include "libbasardbaspect_accessorpropertytable_yiterator.h"
#include "libbasardbaspect_manager.h"
#include "libbasarappl_functions.h"

//----------------------------------------------------------------------------
namespace basar {

//------------------------------------------------------------------------------
namespace cmnutil {

//------------------------------------------------------------------------------
using appl::SystemEventManagerImpl;

/*!	specialised method 
* \return pointer to instance
*/
template <>
typename Singleton<SystemEventManagerImpl>::InstancePtr & 
	Singleton<SystemEventManagerImpl>::inst()
{
    // avoid init order fiasco
    appl::init();

    static InstancePtr s_Inst;
    return s_Inst;
}

//------------------------------------------------------------------------------
using appl::NullEventSourceImpl;

/*!	specialised method
* \return pointer to instance
*/
template <>
typename Singleton<NullEventSourceImpl>::InstancePtr & 
	Singleton<NullEventSourceImpl>::inst()
{
    // avoid init order fiasco
    appl::init();

    static InstancePtr s_Inst;
    return s_Inst;
}

//------------------------------------------------------------------------------
}	// namespace

//------------------------------------------------------------------------------
namespace appl  {

//----------------------------------------------------------------------------
/*!	\throw no-throw */
FuncExecuter::FuncExecuter(ISysEventExecuter* executer)
	: m_Executer(executer)
{}

//----------------------------------------------------------------------------
/*!	\throw no-throw
	\retval true if objects are equal
	\retval false if objects are not equal */
bool FuncExecuter::operator == (const FuncExecuter& rhp) const
{
	return *(m_Executer.get()) == *(rhp.m_Executer.get());
}

//----------------------------------------------------------------------------
/*!	\throw no-throw
	\return shared pointer to object */
boost::shared_ptr<ISysEventExecuter> FuncExecuter::operator -> ()
{
	return m_Executer;
}

//----------------------------------------------------------------------------
/*!	\throw no-throw 
	\retval true if objects are equal
	\retval false if objects are not equal */
bool NullEventSourceImpl::operator == (const IEventSource& rhs) const
{
	// because of singleton: same class / type --> same object
	// compare vtable address
	return (*(reinterpret_cast<const int *>(&(rhs)))) == *(reinterpret_cast<const int *>(this));
}

//----------------------------------------------------------------------------
/*!	\throw no-throw */
SystemEventManagerImpl::SystemEventManagerImpl()
{
}

//----------------------------------------------------------------------------
/*!	\throw no-throw */
SystemEventManagerImpl::~SystemEventManagerImpl()
{
}

//----------------------------------------------------------------------------
/*!	\throw no-throw */
void SystemEventManagerImpl::registerExecuter(EventName event, FuncExecuter executer)
{
	// insert eventName, construct an empty std::list, if not there, insert executer in that list
	m_SysEvents[event].push_front(executer);

	if (LoggerPool::eventManager().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("executer for event <%s> registrated.", 
					event.c_str());
		LoggerPool::eventManager().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}

//----------------------------------------------------------------------------
/*!	\throw no-throw */
void SystemEventManagerImpl::unregisterExecuter(EventName event, FuncExecuter executer)
{
	CollEvent::iterator it = m_SysEvents.find(event);

	if ( it == m_SysEvents.end() ) 
	{
		if (LoggerPool::eventManager().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
		{
			VarString msg;
			msg.format("no executer for event <%s> found for deregistrating.", 
						event.c_str());
			LoggerPool::eventManager().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
		}
		return; // no exception
	}

	(*it).second.remove(executer); // remove from list

	if ( (*it).second.empty() ) // list empty?
		m_SysEvents.erase(it); // erase whole entry in table, if list empty

	if (LoggerPool::eventManager().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("executer for event <%s> deregistrated.", 
					event.c_str());
		LoggerPool::eventManager().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}

//----------------------------------------------------------------------------
/*!	\throw no-throw */
void SystemEventManagerImpl::registerExecuter (EventName event,FuncExecuter executer, const bool forSave)
{
	if ( forSave )
		registerExecuter(event, executer);
	else
		unregisterExecuter(event, executer);
}

//----------------------------------------------------------------------------
/*!	\throw no-throw 
	\retval true if event name is registrated
	\retval false if event name is not yet registrated */
bool	SystemEventManagerImpl::isHandlerAvailable	(EventName event) const
{
	CollEvent::const_iterator cit = m_SysEvents.find(event);
	return cit != m_SysEvents.end();	
}

//----------------------------------------------------------------------------
/*!	\throw no-throw 
	\return return value of called handle (false, if no listener found for given event) */
EventReturnType SystemEventManagerImpl::fire(EventName                                   event, 
							                 db::aspect::AccessorPropertyTable_YIterator yit, 
											 IEventSource                              & rSource )
{
	CollEvent::iterator it = m_SysEvents.find(event);
	
	if ( it == m_SysEvents.end() )
	{
		if (LoggerPool::eventManager().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
		{
			VarString msg;
			msg.format("no event <%s> found for firing.", 
						event.c_str());
			LoggerPool::eventManager().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
		}

		EventReturnStruct retStruct;
		retStruct.ret = HANDLER_NOT_FOUND;
		retStruct.message = "No EventListener found for current context";
		return retStruct;
	}

	if (LoggerPool::eventManager().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("event <%s> fired.", 
					event.c_str());
		LoggerPool::eventManager().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	// Object dispatching to the object that is associated with the event
	// This object is a Executer and dispatches in turn to the associated method
	return (*it).second.front()->execute(rSource, yit);
}

//----------------------------------------------------------------------------
/*!	\throw no-throw 
	\return return value as AND-conjunction of all called handles 
					(if one handle returns false, whole function returns fase also) 
	for seldom use only */
EventReturnType SystemEventManagerImpl::fireMulti(EventName                                   event, 
								                  db::aspect::AccessorPropertyTable_YIterator yit, 
												  IEventSource                             & rSource)
{
	CollEvent::iterator it = m_SysEvents.find(event);
	
	if ( it == m_SysEvents.end() ) 
	{
		if (LoggerPool::eventManager().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
		{
			VarString msg;
			msg.format("no event <%s> found for firing.", 
						event.c_str());
			LoggerPool::eventManager().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
		}

		EventReturnStruct retStruct;
		retStruct.ret = HANDLER_NOT_FOUND;
		retStruct.message.format("No EventListener found for event <%s>.", event.c_str());
		return retStruct;
	}

	CollFuncExecuter::iterator lit = (*it).second.begin();
	
	EventReturnStruct retStruct;
    EventReturnStruct retStructTemp;
	retStruct.ret = HANDLER_NOT_FOUND;
	retStruct.message = "";

	// fire, begin with latest Handler
	for (; lit != (*it).second.end(); ++lit)
	{
		retStructTemp = (*lit)->execute(rSource, yit);

		if ( retStructTemp.ret > retStruct.ret )
			retStruct = retStructTemp;

		if (retStruct.ret >= HANDLER_ERROR)
			break;
	}

	if (LoggerPool::eventManager().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("fire multi for event <%s> called.", 
					event.c_str());
		LoggerPool::eventManager().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return retStruct;
}

//----------------------------------------------------------------------------
/*!	\throw no-throw
*   \return return value of called handle (false, if no listener found for given event)
*/
EventReturnType SystemEventManagerImpl::fireSpecified (EventName                                   event,
							                           db::aspect::AccessorPropertyTable_YIterator yit, 
													   IEventSource                              & rSource )
{
	CollEvent::iterator it = m_SysEvents.find(event);
	
	if ( it == m_SysEvents.end() ) 
	{
		if (LoggerPool::eventManager().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
		{
			VarString msg;
			msg.format("no event <%s> found for firing.", 
						event.c_str());
			LoggerPool::eventManager().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
		}
		
		EventReturnStruct retStruct;
		retStruct.ret = HANDLER_NOT_FOUND;
		retStruct.message.format("No EventListener found for event <%s>.", event.c_str());
		return retStruct;
	}

	CollFuncExecuter::iterator lit = (*it).second.begin();

	bool listenerFound = false;
	EventReturnStruct retStruct;
    EventReturnStruct retStructTemp;
	retStruct.ret = HANDLER_NOT_FOUND;
	retStruct.message = "";


	// fire, begin with latest Handler
	for (; lit != (*it).second.end(); ++lit)
	{
		if ( rSource == (*lit)->getEventSource() )
		{
			retStructTemp = (*lit)->execute(rSource, yit);

			if ( retStructTemp.ret > retStruct.ret )
				retStruct = retStructTemp;

			listenerFound = true;

			if (retStruct.ret >= HANDLER_ERROR)
				break;
		}
	}
	
	if ( listenerFound == false )
	{
		if (LoggerPool::eventManager().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
		{
			VarString msg;
			msg.format("no event <%s> found for firing for specific source.", 
						event.c_str());
			LoggerPool::eventManager().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
		}

		retStruct.ret = HANDLER_NOT_FOUND;
		retStruct.message.format("No EventListener found for event <%s>.", event.c_str());
		return retStruct;
	}
	else
	{
		if (LoggerPool::eventManager().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
		{
			VarString msg;
			msg.format("fire specific for event <%s> called.", 
						event.c_str());
			LoggerPool::eventManager().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
		}

		return retStruct;
	}
}

//----------------------------------------------------------------------------
/*!	\throw no-throw
*  	\return return value of called handle (false, if no listener found for given event)
*/
EventReturnType SystemEventManagerImpl::fire(EventName event, IEventSource & rSource)
{
	return fire(event, basar::db::aspect::AccessorPropertyTable_YIterator(), rSource);
}

//----------------------------------------------------------------------------
/*!	\throw no-throw
*	\return return value as AND-conjunction of all called handles 
					(if one handle returns false, whole function returns fase also) 
	for seldom use only */
EventReturnType SystemEventManagerImpl::fireMulti(EventName event, IEventSource & rSource)
{
	return fireMulti(event, basar::db::aspect::AccessorPropertyTable_YIterator(), rSource); 
}

//----------------------------------------------------------------------------
/*!	\throw no-throw
*   \return return value of called handle (false, if no listener found for given event)
*/
EventReturnType SystemEventManagerImpl::fireSpecified (EventName event, IEventSource & rSource)
{
	return fireSpecified(event, basar::db::aspect::AccessorPropertyTable_YIterator(), rSource);
}

//----------------------------------------------------------------------------
}//appl
}//basar
